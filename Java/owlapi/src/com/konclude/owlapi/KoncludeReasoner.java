/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.konclude.owlapi;

import com.konclude.jnibridge.AxiomExpressionBuildingBridge;
import com.konclude.jnibridge.KoncludeReasonerBridge;
import com.konclude.jnibridge.ObjectSetCallbackListener;
import com.konclude.jnibridge.QueryingBridge;
import com.konclude.jnibridge.SetOfObjectSetCallbackListener;

import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.reasoner.*;
import org.semanticweb.owlapi.reasoner.impl.OWLClassNode;
import org.semanticweb.owlapi.reasoner.impl.OWLClassNodeSet;
import org.semanticweb.owlapi.reasoner.impl.OWLNamedIndividualNode;
import org.semanticweb.owlapi.reasoner.impl.OWLNamedIndividualNodeSet;
import org.semanticweb.owlapi.reasoner.impl.OWLObjectPropertyNode;
import org.semanticweb.owlapi.reasoner.impl.OWLObjectPropertyNodeSet;
import org.semanticweb.owlapi.util.Version;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.Collections;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

/**
 * An OWL API OWLReasoner on top of the JNI bridge of Konclude.
 *
 * The ontology is translated into the native side once, when the reasoner is created, and
 * again by flush() after the ontology has changed, since the bridge cannot update an
 * installed ontology. The reasoner therefore buffers, see getBufferingMode.
 *
 * The reasoner holds a library instance of Konclude and has to be closed with dispose().
 *
 *
 * WHAT THE BRIDGE CANNOT ANSWER
 *
 * The querying bridge provides 12 queries about a named entity and 5 about a class expression,
 * which do not cover the OWLReasoner interface. The methods that it cannot answer throw an
 * UnsupportedOperationException instead of answering with an empty node set, because an
 * empty node set is a valid answer and a caller cannot tell it from 'not supported'.
 *
 * The native side has no query for these:
 *
 *   getDisjointClasses, getDisjointObjectProperties, getDisjointDataProperties
 *   getInverseObjectProperties
 *   getObjectPropertyDomains, getObjectPropertyRanges, getDataPropertyDomains
 *   getSubDataProperties, getSuperDataProperties, getEquivalentDataProperties
 *   getTopDataPropertyNode, getBottomDataPropertyNode
 *   getDataPropertyValues
 *   getDifferentIndividuals
 *   isEntailed
 *
 * The object property hierarchy and the same individuals used to be unsupported here as well,
 * because four of the 12 queries of the bridge reported nothing. That was a defect of the
 * native side, not of the translation, and it has been fixed, see Java/Readme.md.
 *
 * A query about an anonymous class expression, getSubClasses of an ObjectSomeValuesFrom for
 * instance, is answered as well, for isSatisfiable, the sub, super and equivalent classes and
 * the instances. The expression is built through a second building bridge that the querying
 * bridge binds to a revision of the installed ontology, so that the ontology is not changed,
 * and the native side answers it with the complex answering queries of its OWLlink interface.
 * The first such query pays for the preparation of that machinery. A named class keeps
 * being answered from the class hierarchy directly.
 *
 * Every native call of a reasoner runs on a thread of its own, see guard: CJNIHandler keeps
 * the JNIEnv and the method ids of the thread that initialised the library instance, so all
 * calls of one instance have to happen on that thread, and a caller such as Protege, which
 * classifies on a worker thread and asks from the event thread, must not have to know that.
 * A time out of the configuration is applied to the wait for that thread.
 *
 * An entity that the ontology does not mention is a fresh entity, see FreshEntityPolicy. The
 * native side is never asked about one: with DISALLOW a FreshEntitiesException is thrown, with
 * ALLOW the answer is the trivial one, an empty node set, and this applies to a class
 * expression as soon as one of its entities is fresh.
 *
 * isEntailmentCheckingSupported answers false for every axiom type, which is what the
 * interface asks for when isEntailed is not implemented.
 */
public class KoncludeReasoner implements OWLReasoner {

	/**
	 * What to do about an ABox that forces two individuals to be merged.
	 *
	 * Konclude used to stay in its precomputation for such an ontology unless the completion
	 * graph was built, see Java/Readme.md. That is fixed in
	 * CTotallyPrecomputationThread::createIndividualPrecomputationCheck, so OFF is the
	 * default. The other two remain for a shared library that predates the fix.
	 */
	public enum MergeSafety {
		/** leave the loading configuration alone, the default */
		OFF,
		/**
		 * Look at the ontology and build the completion graph if a forced merge is found, see
		 * KoncludeIndividualMergeInspector. For a library that predates the fix.
		 */
		DETECT,
		/** always build the completion graph, which is safe and slower */
		ALWAYS
	}

	/** the name of the shared library, without the platform specific prefix and suffix */
	public static final String NATIVE_LIBRARY_NAME = "Konclude";

	/**
	 * The loading configuration that a reasoner is created with unless a caller hands in one of
	 * its own, which replaces these arguments instead of extending them, so an own configuration
	 * has to start from this. An empty configuration means this one; it is not handed to the
	 * library as it is, because the library's own default lacks the last line.
	 *
	 * 'Konclude.Calculation.ProcessorCount' is PROCESSOR_COUNT, AUTO unless it is overridden,
	 * because the default of the library is a single processing unit, as it is for the command
	 * line without '-w AUTO'. Classifying SNOMED CT takes 156 s with one unit against 34.9 s with
	 * AUTO on a machine with 16 cores.
	 */
	public static final String DEFAULT_LOADING_CONFIGURATION;

	/**
	 * The system property, and the environment variable, that set the number of processing
	 * units, AUTO or a positive number; the property takes precedence. AUTO is one per logical
	 * processor that the system reports, which inside a container limited to fewer processors,
	 * or beside other work on the same machine, can be more than is useful. In Protege it is
	 * set with a line append=-Dkonclude.processorCount=8 in ~/.Protege/conf/jvm.conf, which run.sh
	 * reads in place of conf/jvm.conf if it exists.
	 */
	public static final String PROCESSOR_COUNT_PROPERTY = "konclude.processorCount";
	public static final String PROCESSOR_COUNT_ENVIRONMENT_VARIABLE = "KONCLUDE_PROCESSOR_COUNT";

	/** The number of processing units the loading configurations ask for, AUTO or a number. */
	public static final String PROCESSOR_COUNT;

	/**
	 * The system property, and the environment variable, whose settings are appended to
	 * DEFAULT_LOADING_CONFIGURATION and FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION, for tuning
	 * the reasoner on a machine without rebuilding, e.g.
	 * -Dkonclude.configuration="+=Konclude.Calculation.Memory.RecycledPoolLimit=200000".
	 * The property takes precedence. They come after the processor count, so a
	 * Konclude.Calculation.ProcessorCount among them wins. Only settings, tokens of the form
	 * +=Konclude.Key=Value separated by spaces or commas, are taken; commas are for Protege's
	 * jvm.conf, whose options run.sh splits at spaces. The rest is dropped, since other arguments start loaders,
	 * such as a server that opens a port, with a warning. A configuration
	 * handed to a constructor replaces the default ones and is not extended.
	 */
	public static final String CONFIGURATION_PROPERTY = "konclude.configuration";
	public static final String CONFIGURATION_ENVIRONMENT_VARIABLE = "KONCLUDE_CONFIGURATION";

	/** The settings taken from CONFIGURATION_PROPERTY, empty or ending in a space. */
	public static final String ADDITIONAL_CONFIGURATION;

	/**
	 * The default loading arguments plus
	 * 'Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction', which makes
	 * Konclude build the completion graph instead of taking its saturation based short cuts.
	 *
	 * This was the way around the defect described in Java/Readme.md, where Konclude stayed in
	 * its precomputation for an ontology whose ABox forces two named individuals to be merged.
	 * That is fixed, so this is only needed against a shared library that predates the fix,
	 * see MergeSafety. Building the completion graph is the expensive path that the short cuts
	 * exist to avoid, which matters for a large ABox.
	 */
	public static final String FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION;

	static {
		String property = System.getProperty(PROCESSOR_COUNT_PROPERTY);
		PROCESSOR_COUNT = processorCountSetting(property != null ? property
				: System.getenv(PROCESSOR_COUNT_ENVIRONMENT_VARIABLE));
		String configuration = System.getProperty(CONFIGURATION_PROPERTY);
		ADDITIONAL_CONFIGURATION = additionalConfigurationSettings(configuration != null ? configuration
				: System.getenv(CONFIGURATION_ENVIRONMENT_VARIABLE));
		DEFAULT_LOADING_CONFIGURATION =
				"-DefaultReasonerLoader "
				+ "+=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager "
				+ "-JNICommandProcessorLoader "
				+ "+=Konclude.Calculation.ProcessorCount=" + PROCESSOR_COUNT + " "
				+ ADDITIONAL_CONFIGURATION;
		FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION =
				"-DefaultReasonerLoader "
				+ "+=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager "
				+ "+=Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction=true "
				+ "-JNICommandProcessorLoader "
				+ "+=Konclude.Calculation.ProcessorCount=" + PROCESSOR_COUNT + " "
				+ ADDITIONAL_CONFIGURATION;
	}

	/**
	 * The settings of the value of the property or the environment variable, each followed by a
	 * space: the tokens of the form +=Konclude.Key=Value, the others dropped with a warning.
	 */
	public static String additionalConfigurationSettings(String value) {
		if (value == null) {
			return "";
		}
		StringBuilder settings = new StringBuilder();
		for (String token : value.trim().split("[\\s,]+")) {
			if (token.isEmpty()) {
				continue;
			}
			if (token.startsWith("+=Konclude.") && token.indexOf('=', 2) > 0) {
				settings.append(token).append(' ');
			} else {
				// LOGGER is initialised after this runs from the static initialiser above
				LoggerFactory.getLogger(KoncludeReasoner.class).warn(
						"Konclude: '{}' in the configuration is not a setting of the form +=Konclude.Key=Value and is dropped", token);
			}
		}
		return settings.toString();
	}

	/**
	 * The processor count for the loading configuration from the value of the property or the
	 * environment variable: AUTO for no value, for AUTO in any case and, with a warning, for
	 * anything that is not a positive number, since the library would take that for 1.
	 */
	public static String processorCountSetting(String value) {
		if (value == null || value.trim().isEmpty() || value.trim().equalsIgnoreCase("AUTO")) {
			return "AUTO";
		}
		try {
			int count = Integer.parseInt(value.trim());
			if (count > 0) {
				return Integer.toString(count);
			}
		} catch (NumberFormatException e) {
			// reported below
		}
		// LOGGER is initialised after this runs from the static initialiser above
		LoggerFactory.getLogger(KoncludeReasoner.class).warn(
				"Konclude: the processor count '{}' is neither AUTO nor a positive number, AUTO is used", value);
		return "AUTO";
	}

	private static boolean sNativeLibraryLoaded = false;

	private final OWLOntology mRootOntology;
	private final OWLDataFactory mDataFactory;
	private final OWLReasonerConfiguration mConfiguration;
	private final BufferingMode mBufferingMode;
	private final String mLoadingConfiguration;

	/** the changes of the ontology since the last flush, the reasoner buffers them */
	private final List<OWLOntologyChange> mPendingChanges = new ArrayList<OWLOntologyChange>();
	/** the inference types that were computed for the ontology currently installed in the library */
	private final Set<InferenceType> mPrecomputed = EnumSet.noneOf(InferenceType.class);

	/**
	 * The class taxonomy, read from the native side at once after the classification, see
	 * ClassTaxonomy; null until it is read and after every reinstallation.
	 */
	private ClassTaxonomy mClassTaxonomy;
	/**
	 * whether the taxonomy may be asked for, false after a request found no complete classification;
	 * it is asked for again after the class hierarchy is precomputed, or once after the next hierarchy
	 * query about a named class, which classifies, so that an ontology that is never classified, an
	 * inconsistent one, does not pay a request per query
	 */
	private boolean mClassTaxonomyRequestable = true;
	private boolean mClassTaxonomyRetried = false;
	/** -Dkonclude.taxonomyCache=false answers every hierarchy query of a named class natively */
	private static final boolean TAXONOMY_CACHE = !"false".equalsIgnoreCase(System.getProperty("konclude.taxonomyCache"));

	private final OWLOntologyChangeListener mChangeListener = new OWLOntologyChangeListener() {
		@Override
		public void ontologiesChanged(List<? extends OWLOntologyChange> changes) {
			for (OWLOntologyChange change : changes) {
				if (mRootOntology.getImportsClosure().contains(change.getOntology())) {
					mPendingChanges.add(change);
				}
			}
			if (mBufferingMode == BufferingMode.NON_BUFFERING) {
				flush();
			}
		}
	};

	private final MergeSafety mMergeSafety;

	private KoncludeReasonerBridge mBridge;
	private AxiomExpressionBuildingBridge mBuilder;
	private QueryingBridge mQuerying;
	private KoncludeOWLAPITranslator mTranslator;
	/**
	 * Builds the class expressions that the queries are asked about, bound to the querying
	 * bridge, see QueryingBridge.initOWLClassExpressionBuilder. Its translator does not declare
	 * the entities it meets, a declaration would be an axiom.
	 */
	private AxiomExpressionBuildingBridge mExpressionBuilder;
	private KoncludeOWLAPITranslator mExpressionTranslator;
	private boolean mDisposed = false;

	/** the configuration that was handed to the library instance, after the merge check */
	private String mAppliedLoadingConfiguration;
	private KoncludeIndividualMergeInspector mMergeInspector;

	/**
	 * Runs every native call of this reasoner, so that all of them happen on the one thread
	 * that initialised the library instance, whichever thread asks, and so that a call that
	 * does not return can be reported through the time out instead of blocking the caller
	 * for ever.
	 */
	private final ExecutorService mReasonerThread;
	/** the calls that wait for the reasoner's thread, so that interrupt() can release them */
	private final Set<Future<?>> mInFlight = Collections.synchronizedSet(new HashSet<Future<?>>());

	/**
	 * Set once a native call ran into the time out. The call is still running on the reasoner
	 * thread, the bridge cannot cancel it, so the reasoner cannot be used any more.
	 */
	private boolean mTimedOut = false;

	/**
	 * Loads the shared library of Konclude. It is searched on java.library.path and is loaded
	 * once per virtual machine. The method is called by the constructor, it is public so that
	 * a caller can report a missing library before it builds an ontology.
	 */
	public static synchronized void loadNativeLibrary() {
		if (!sNativeLibraryLoaded) {
			System.loadLibrary(NATIVE_LIBRARY_NAME);
			sNativeLibraryLoaded = true;
		}
	}

	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode) {
		this(rootOntology, configuration, bufferingMode, "", MergeSafety.OFF);
	}

	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode, String loadingConfiguration) {
		this(rootOntology, configuration, bufferingMode, loadingConfiguration, MergeSafety.OFF);
	}

	/**
	 * The loading configuration is handed to initKoncludeLibraryInstance, an empty string
	 * stands for DEFAULT_LOADING_CONFIGURATION. A non-empty string replaces the default loading
	 * arguments of Konclude instead of extending them, so it also has to contain
	 * '-JNICommandProcessorLoader'.
	 */
	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode, String loadingConfiguration, MergeSafety mergeSafety) {
		if (rootOntology == null) {
			throw new IllegalArgumentException("the root ontology is required");
		}
		loadNativeLibrary();
		mRootOntology = rootOntology;
		mDataFactory = rootOntology.getOWLOntologyManager().getOWLDataFactory();
		mConfiguration = configuration != null ? configuration : new SimpleConfiguration();
		mBufferingMode = bufferingMode != null ? bufferingMode : BufferingMode.BUFFERING;
		mLoadingConfiguration = loadingConfiguration != null ? loadingConfiguration : "";
		mMergeSafety = mergeSafety != null ? mergeSafety : MergeSafety.OFF;
		mReasonerThread = Executors.newSingleThreadExecutor(new ThreadFactory() {
			@Override
			public Thread newThread(Runnable runnable) {
				Thread thread = new Thread(runnable, "Konclude reasoner");
				// the thread cannot be stopped if a call does not return, so it must not
				// keep the virtual machine alive
				thread.setDaemon(true);
				return thread;
			}
		});
		rootOntology.getOWLOntologyManager().addOntologyChangeListener(mChangeListener);
		install();
	}


	// ---------------------------------------------------------------- the ontology

	/**
	 * Translates the ontology into a fresh library instance and opens the querying bridge.
	 *
	 * ATTENTION: this runs on the reasoner thread, as every native call does, because
	 * CJNIHandler keeps the JNIEnv and the method ids of the thread that initialised the
	 * library instance, so every call of one instance has to happen on the same thread. Mixing
	 * threads ends in a crash of the virtual machine, not in an exception.
	 */
	private void install() {
		guard("install", new NativeCall<Void>() {
			@Override
			public Void call() {
				doInstall();
				return null;
			}
		});
	}

	private void doInstall() {
		mAppliedLoadingConfiguration = chooseLoadingConfiguration();
		mBridge = new KoncludeReasonerBridge();
		mBridge.initKoncludeLibraryInstance(mAppliedLoadingConfiguration);
		boolean installed = false;
		try {
			mBuilder = new AxiomExpressionBuildingBridge();
			mBridge.initAxiomExpressionVisitingBridge(mBuilder);
			mTranslator = new KoncludeOWLAPITranslator(mBridge, mBuilder);
			// an axiom that the bridge cannot express is collected instead of being reported,
			// so that a caller can load an ontology and ask what was left out
			mTranslator.setIgnoreUnsupportedAxioms(true);
			mTranslator.translateOntology(mRootOntology);
			mBridge.finalizeAxiomExpressionVisitingBridge(mBuilder);

			mQuerying = new QueryingBridge();
			mBridge.initQueryingBridge(mQuerying);

			mExpressionBuilder = new AxiomExpressionBuildingBridge();
			mQuerying.initOWLClassExpressionBuilder(mBridge, mExpressionBuilder);
			mExpressionTranslator = new KoncludeOWLAPITranslator(mBridge, mExpressionBuilder);
			mExpressionTranslator.setDeclareEntities(false);
			installed = true;
		} finally {
			if (!installed) {
				// do not leak the library instance if the translation failed
				mBridge.closeKoncludeLibraryInstance();
				mBridge = null;
				mBuilder = null;
				mQuerying = null;
				mTranslator = null;
				mExpressionBuilder = null;
				mExpressionTranslator = null;
			}
		}
	}

	/**
	 * The loading configuration to use, which is the configured one unless the ontology forces
	 * a merge of two individuals and nothing else was asked for, see MergeSafety.
	 */
	private String chooseLoadingConfiguration() {
		if (mMergeSafety == MergeSafety.ALWAYS) {
			return FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION;
		}
		mMergeInspector = null;
		String configuration = mLoadingConfiguration.isEmpty() ? DEFAULT_LOADING_CONFIGURATION
				: mLoadingConfiguration;
		if (mMergeSafety == MergeSafety.OFF) {
			return configuration;
		}
		KoncludeIndividualMergeInspector inspector = new KoncludeIndividualMergeInspector(mRootOntology);
		if (!inspector.isMergeForced()) {
			return configuration;
		}
		mMergeInspector = inspector;
		if (!mLoadingConfiguration.isEmpty()) {
			// the caller asked for a configuration of its own, which is not overruled here
			return mLoadingConfiguration;
		}
		return FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION;
	}

	/** the loading configuration that the library instance was initialised with */
	public String getAppliedLoadingConfiguration() {
		return mAppliedLoadingConfiguration;
	}

	/**
	 * Whether the ontology forces two individuals to be merged, which Konclude does not
	 * terminate on unless the completion graph is built. Null if no such merge was found or if
	 * the check was switched off with MergeSafety.
	 */
	public KoncludeIndividualMergeInspector getForcedIndividualMerge() {
		return mMergeInspector;
	}

	private void uninstall() {
		// what was computed belongs to the ontology that is being removed from the library
		mPrecomputed.clear();
		mClassTaxonomy = null;
		mClassTaxonomyRequestable = true;
		mClassTaxonomyRetried = false;
		guard("uninstall", new NativeCall<Void>() {
			@Override
			public Void call() {
				doUninstall();
				return null;
			}
		});
	}

	private void doUninstall() {
		if (mBridge != null) {
			if (mQuerying != null) {
				// closes the expression builder as well, which the querying bridge owns
				mBridge.finalizeQueryingBridge(mQuerying);
			}
			mBridge.closeKoncludeLibraryInstance();
		}
		mBridge = null;
		mBuilder = null;
		mQuerying = null;
		mTranslator = null;
		mExpressionBuilder = null;
		mExpressionTranslator = null;
	}

	/** the axioms of the ontology that the bridge cannot express, empty if there are none */
	public List<OWLAxiom> getUnsupportedAxioms() {
		checkOpen();
		return mTranslator.getUnsupportedAxioms();
	}

	@Override
	public void flush() {
		checkOpen();
		if (mPendingChanges.isEmpty()) {
			return;
		}
		mPendingChanges.clear();
		// the bridge cannot update an installed ontology, so it is translated again
		uninstall();
		install();
	}

	@Override
	public void dispose() {
		if (mDisposed) {
			return;
		}
		mDisposed = true;
		mRootOntology.getOWLOntologyManager().removeOntologyChangeListener(mChangeListener);
		if (!mTimedOut) {
			// the closing has to happen on the thread that initialised the instance as well
			uninstall();
		}
		// a stuck call cannot be cancelled, its thread is a daemon so that it does not keep the
		// virtual machine alive
		mReasonerThread.shutdownNow();
	}

	@Override
	public OWLOntology getRootOntology() {
		return mRootOntology;
	}

	@Override
	public BufferingMode getBufferingMode() {
		return mBufferingMode;
	}

	@Override
	public List<OWLOntologyChange> getPendingChanges() {
		return Collections.unmodifiableList(new ArrayList<OWLOntologyChange>(mPendingChanges));
	}

	@Override
	public Set<OWLAxiom> getPendingAxiomAdditions() {
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (OWLOntologyChange change : mPendingChanges) {
			if (change.isAddAxiom()) {
				axioms.add(change.getAxiom());
			}
		}
		return axioms;
	}

	@Override
	public Set<OWLAxiom> getPendingAxiomRemovals() {
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (OWLOntologyChange change : mPendingChanges) {
			if (change.isRemoveAxiom()) {
				axioms.add(change.getAxiom());
			}
		}
		return axioms;
	}


	// ------------------------------------------------------------------ the reasoner

	@Override
	public String getReasonerName() {
		return "Konclude";
	}

	@Override
	public Version getReasonerVersion() {
		return new Version(0, 7, 0, 0);
	}

	@Override
	public long getTimeOut() {
		return mConfiguration.getTimeOut();
	}

	@Override
	public FreshEntityPolicy getFreshEntityPolicy() {
		return mConfiguration.getFreshEntityPolicy();
	}

	/**
	 * The queries of the bridge report a set per set of equivalent individuals, so the
	 * individuals are grouped by their sameAs relation.
	 */
	@Override
	public IndividualNodeSetPolicy getIndividualNodeSetPolicy() {
		return IndividualNodeSetPolicy.BY_SAME_AS;
	}

	/** the bridge has no way to cancel a running calculation, so this does nothing */
	@Override
	public void interrupt() {
		// The native side has no way to abandon a calculation, so this releases the callers that
		// wait for one, each with a ReasonerInterruptedException, while the calculation itself runs
		// on to its end on the reasoner's thread. A call made afterwards queues behind it. For
		// Protege that is the difference between a cancel button that returns and one that waits
		// for the classification of a large ontology to finish.
		Future<?>[] pending;
		synchronized (mInFlight) {
			pending = mInFlight.toArray(new Future<?>[0]);
		}
		for (Future<?> future : pending) {
			future.cancel(true);
		}
	}

	/**
	 * Konclude computes on demand, so the work of an inference type is triggered with a query
	 * that cannot be answered without it. Without this the method would return at once and the
	 * caller would pay for the classification in whichever query happens to arrive first, which
	 * for an editor is a window that stops responding with nothing to indicate why.
	 *
	 * The progress monitor of the configuration is told about each task, which is what Protege
	 * shows in its progress window, and while the task runs a ProgressReporter feeds it the
	 * progress of the native calculations.
	 */
	@Override
	public void precomputeInferences(InferenceType... inferenceTypes) {
		checkOpen();
		OWLClass thing = mDataFactory.getOWLThing();
		ReasonerProgressMonitor monitor = mConfiguration.getProgressMonitor();
		for (InferenceType inferenceType : inferenceTypes) {
			if (mPrecomputed.contains(inferenceType)) {
				continue;
			}
			if (InferenceType.CLASS_HIERARCHY == inferenceType) {
				monitor.reasonerTaskStarted(PRECOMPUTING);
				ProgressReporter reporter = new ProgressReporter(monitor, false);
				reporter.start();
				try {
					getSubClasses(thing, true);
				} finally {
					reporter.finish();
					monitor.reasonerTaskStopped();
				}
				mPrecomputed.add(inferenceType);
				mClassTaxonomyRequestable = true;
			} else if (InferenceType.CLASS_ASSERTIONS == inferenceType) {
				monitor.reasonerTaskStarted(ReasonerProgressMonitor.REALIZING);
				ProgressReporter reporter = new ProgressReporter(monitor, true);
				reporter.start();
				try {
					getInstances(thing, true);
				} finally {
					reporter.finish();
					monitor.reasonerTaskStopped();
				}
				mPrecomputed.add(inferenceType);
			}
			// the other inference types have no query of their own in the bridge, and the OWL API
			// allows a reasoner to ignore what it does not precompute
		}
	}

	/** how often the progress of a running precomputation is read and reported */
	private static final long PROGRESS_INTERVAL_MILLISECONDS = 250;

	private static final Logger LOGGER = LoggerFactory.getLogger(KoncludeReasoner.class);

	/**
	 * A native call that takes at least this long is logged at INFO with its duration, so that
	 * a host such as Protege shows in its log which questions cost the time. The questions
	 * about named classes take a fraction of a millisecond and stay at DEBUG.
	 */
	private static final long SLOW_CALL_MILLISECONDS = 50;

	/**
	 * The task reported for the first phase of a classification, the consistency check and
	 * the saturation, which Konclude calls the precomputation; ReasonerProgressMonitor.CLASSIFYING
	 * follows it once the classifier tests. The two are reported as one task after the other,
	 * as ELK reports its stages, so a window shows which phase it is in.
	 */
	public static final String PRECOMPUTING = "Precomputing";

	/**
	 * Reports the progress of the native calculations to the progress monitor while a
	 * precomputation runs. It has to be a thread of its own, the reasoner's thread is inside
	 * the query that triggers the calculation; the progress query of the bridge only reads
	 * counters of the calculation managers, so it may run beside that query, and it is the
	 * one native call of a reasoner that does not go through guard().
	 *
	 * The counters are those the command line prints with '-a', plus one it does not. A
	 * classification starts with the precomputation, whose time is the saturation; the
	 * saturation algorithm counts the nodes it has initialised against the nodes that exist,
	 * which are reported under the task PRECOMPUTING, both growing while the saturation
	 * creates successors. Once the classifier tests, its satisfiability and subsumption
	 * tests done against those to do are reported under the task CLASSIFYING; that total
	 * grows a little as well, which a monitor takes as a new maximum. The monitor is told
	 * that the task is busy while there is no count, at the very start and in between. On
	 * SNOMED CT the precomputation takes 19 of 36 seconds. A realization has one count, its
	 * tested instantiations, which is zero without individuals.
	 */
	private final class ProgressReporter extends Thread {

		private final ReasonerProgressMonitor mMonitor;
		private final boolean mRealization;
		private boolean mClassifying = false;
		private volatile boolean mFinished = false;

		ProgressReporter(ReasonerProgressMonitor monitor, boolean realization) {
			super("Konclude progress");
			mMonitor = monitor;
			mRealization = realization;
			setDaemon(true);
		}

		/** stops the reporting; the last report may still be on its way, none follows it */
		void finish() {
			mFinished = true;
			interrupt();
			try {
				join();
			} catch (InterruptedException exception) {
				Thread.currentThread().interrupt();
			}
		}

		@Override
		public void run() {
			mMonitor.reasonerTaskBusy();
			while (!mFinished) {
				try {
					Thread.sleep(PROGRESS_INTERVAL_MILLISECONDS);
				} catch (InterruptedException exception) {
					break;
				}
				if (mFinished) {
					break;
				}
				double[] progress = mQuerying.queryOWLReasoningProgress(mBridge);
				if (mRealization) {
					report(progress[QueryingBridge.PROGRESS_REALIZATION_TESTED],
							progress[QueryingBridge.PROGRESS_REALIZATION_TOTAL]);
				} else {
					double tested = progress[QueryingBridge.PROGRESS_CLASSIFICATION_TESTED];
					double total = progress[QueryingBridge.PROGRESS_CLASSIFICATION_TOTAL];
					if (!mClassifying && total > 0) {
						// the classifier has started testing, the precomputation is over
						mClassifying = true;
						mMonitor.reasonerTaskStopped();
						mMonitor.reasonerTaskStarted(ReasonerProgressMonitor.CLASSIFYING);
					}
					if (mClassifying) {
						report(tested, total);
					} else {
						report(progress[QueryingBridge.PROGRESS_SATURATION_INITIALIZED_NODES],
								progress[QueryingBridge.PROGRESS_SATURATION_NODES]);
					}
				}
			}
		}

		private boolean isRunning(double done, double total) {
			return total > 0 && done < total;
		}

		private void report(double done, double total) {
			if (isRunning(done, total)) {
				mMonitor.reasonerTaskProgressChanged((int) Math.min(done, Integer.MAX_VALUE),
						(int) Math.min(total, Integer.MAX_VALUE));
			} else {
				mMonitor.reasonerTaskBusy();
			}
		}
	}

	@Override
	public boolean isPrecomputed(InferenceType inferenceType) {
		return mPrecomputed.contains(inferenceType);
	}

	@Override
	public Set<InferenceType> getPrecomputableInferenceTypes() {
		return Collections.unmodifiableSet(EnumSet.of(InferenceType.CLASS_HIERARCHY,
				InferenceType.CLASS_ASSERTIONS));
	}


	// ------------------------------------------------------------- consistency

	@Override
	public boolean isConsistent() {
		checkOpen();
		return guard("isConsistent", new NativeCall<Boolean>() {
			@Override
			public Boolean call() {
				return Boolean.valueOf(mQuerying.checkIsOWLOntologyConsistent(mBridge));
			}
		}).booleanValue();
	}

	@Override
	public Node<OWLClass> getUnsatisfiableClasses() {
		return getBottomClassNode();
	}

	/**
	 * A named class is unsatisfiable exactly if it is equivalent to owl:Nothing, which is what
	 * the bottom node collects. An anonymous expression is asked of the native side.
	 */
	@Override
	public boolean isSatisfiable(final OWLClassExpression classExpression) {
		requireClassExpression(classExpression);
		if (!classExpression.isAnonymous()) {
			return !getBottomClassNode().contains(classExpression.asOWLClass());
		}
		if (!isKnown(classExpression)) {
			return true;
		}
		return guard(describe("isSatisfiable", classExpression), new NativeCall<Boolean>() {
			@Override
			public Boolean call() {
				return Boolean.valueOf(mQuerying.checkIsOWLClassExpressionSatisfiable(mBridge,
						classExpressionPointer(classExpression)));
			}
		}).booleanValue();
	}


	// --------------------------------------------------------- the class hierarchy

	@Override
	public NodeSet<OWLClass> getSubClasses(final OWLClassExpression classExpression, final boolean direct) {
		requireClassExpression(classExpression);
		if (!isKnown(classExpression)) {
			return new OWLClassNodeSet();
		}
		if (direct && !classExpression.isAnonymous()) {
			ClassTaxonomy taxonomy = getClassTaxonomy();
			if (taxonomy != null && taxonomy.contains(classExpression.asOWLClass())) {
				return taxonomy.getDirectSubClasses(classExpression.asOWLClass());
			}
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard(describe("getSubClasses", classExpression), new NativeCall<Void>() {
			@Override
			public Void call() {
				if (classExpression.isAnonymous()) {
					mQuerying.queryOWLClassExpressionSubClasses(mBridge,
							classExpressionPointer(classExpression), callback, direct);
				} else {
					OWLClass named = classExpression.asOWLClass();
					mQuerying.queryOWLSubClasses(mBridge, iri(named), named, callback, direct);
				}
				return null;
			}
		});
		allowClassTaxonomyRetry(classExpression);
		return classNodeSet(callback);
	}

	@Override
	public NodeSet<OWLClass> getSuperClasses(final OWLClassExpression classExpression, final boolean direct) {
		requireClassExpression(classExpression);
		if (!isKnown(classExpression)) {
			return new OWLClassNodeSet();
		}
		if (direct && !classExpression.isAnonymous()) {
			ClassTaxonomy taxonomy = getClassTaxonomy();
			if (taxonomy != null && taxonomy.contains(classExpression.asOWLClass())) {
				return taxonomy.getDirectSuperClasses(classExpression.asOWLClass());
			}
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard(describe("getSuperClasses", classExpression), new NativeCall<Void>() {
			@Override
			public Void call() {
				if (classExpression.isAnonymous()) {
					mQuerying.queryOWLClassExpressionSuperClasses(mBridge,
							classExpressionPointer(classExpression), callback, direct);
				} else {
					OWLClass named = classExpression.asOWLClass();
					mQuerying.queryOWLSuperClasses(mBridge, iri(named), named, callback, direct);
				}
				return null;
			}
		});
		allowClassTaxonomyRetry(classExpression);
		return classNodeSet(callback);
	}

	@Override
	public Node<OWLClass> getEquivalentClasses(final OWLClassExpression classExpression) {
		requireClassExpression(classExpression);
		final boolean anonymous = classExpression.isAnonymous();
		if (!isKnown(classExpression)) {
			return anonymous ? new OWLClassNode() : new OWLClassNode(classExpression.asOWLClass());
		}
		if (!anonymous) {
			ClassTaxonomy taxonomy = getClassTaxonomy();
			if (taxonomy != null && taxonomy.contains(classExpression.asOWLClass())) {
				return taxonomy.getEquivalentClasses(classExpression.asOWLClass());
			}
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard(describe("getEquivalentClasses", classExpression), new NativeCall<Void>() {
			@Override
			public Void call() {
				if (anonymous) {
					mQuerying.queryOWLClassExpressionEquivalentClasses(mBridge,
							classExpressionPointer(classExpression), callback);
				} else {
					OWLClass named = classExpression.asOWLClass();
					mQuerying.queryOWLEquivalentClasses(mBridge, iri(named), named, callback);
				}
				return null;
			}
		});
		Set<OWLClass> classes = collect(callback.getObjects(), OWLClass.class);
		if (!anonymous) {
			// the interface asks for a node that contains the asked class itself
			classes.add(classExpression.asOWLClass());
		}
		return new OWLClassNode(classes);
	}

	@Override
	public Node<OWLClass> getTopClassNode() {
		return getEquivalentClasses(mDataFactory.getOWLThing());
	}

	@Override
	public Node<OWLClass> getBottomClassNode() {
		return getEquivalentClasses(mDataFactory.getOWLNothing());
	}

	@Override
	public NodeSet<OWLClass> getDisjointClasses(OWLClassExpression classExpression) {
		throw unsupported("getDisjointClasses");
	}


	// ----------------------------------------------- the object property hierarchy

	@Override
	public NodeSet<OWLObjectPropertyExpression> getSubObjectProperties(
			OWLObjectPropertyExpression property, boolean direct) {
		final OWLObjectProperty named = namedObjectProperty(property, "getSubObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		// the native side ignores the inverses flag, see QueryingBridge
		guard("getSubObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSubObjectProperties(mBridge, iri(named), named, callback, direct, false);
				return null;
			}
		});
		return objectPropertyNodeSet(callback);
	}

	@Override
	public NodeSet<OWLObjectPropertyExpression> getSuperObjectProperties(
			OWLObjectPropertyExpression property, boolean direct) {
		final OWLObjectProperty named = namedObjectProperty(property, "getSuperObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getSuperObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSuperObjectProperties(mBridge, iri(named), named, callback, direct, false);
				return null;
			}
		});
		return objectPropertyNodeSet(callback);
	}

	@Override
	public Node<OWLObjectPropertyExpression> getEquivalentObjectProperties(
			OWLObjectPropertyExpression property) {
		final OWLObjectProperty named = namedObjectProperty(property, "getEquivalentObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNode(named);
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard("getEquivalentObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLEquivalentObjectProperties(mBridge, iri(named), named, callback);
				return null;
			}
		});
		Set<OWLObjectPropertyExpression> properties =
				collect(callback.getObjects(), OWLObjectPropertyExpression.class);
		// the interface asks for a node that contains the asked property itself
		properties.add(named);
		return new OWLObjectPropertyNode(properties);
	}

	@Override
	public Node<OWLObjectPropertyExpression> getTopObjectPropertyNode() {
		return getEquivalentObjectProperties(mDataFactory.getOWLTopObjectProperty());
	}

	@Override
	public Node<OWLObjectPropertyExpression> getBottomObjectPropertyNode() {
		return getEquivalentObjectProperties(mDataFactory.getOWLBottomObjectProperty());
	}

	@Override
	public NodeSet<OWLObjectPropertyExpression> getDisjointObjectProperties(
			OWLObjectPropertyExpression property) {
		throw unsupported("getDisjointObjectProperties");
	}

	@Override
	public Node<OWLObjectPropertyExpression> getInverseObjectProperties(
			OWLObjectPropertyExpression property) {
		throw unsupported("getInverseObjectProperties");
	}

	@Override
	public NodeSet<OWLClass> getObjectPropertyDomains(OWLObjectPropertyExpression property, boolean direct) {
		throw unsupported("getObjectPropertyDomains");
	}

	@Override
	public NodeSet<OWLClass> getObjectPropertyRanges(OWLObjectPropertyExpression property, boolean direct) {
		throw unsupported("getObjectPropertyRanges");
	}


	// ------------------------------------------------- the data property hierarchy

	@Override
	public NodeSet<OWLDataProperty> getSubDataProperties(OWLDataProperty property, boolean direct) {
		throw unsupported("getSubDataProperties");
	}

	@Override
	public NodeSet<OWLDataProperty> getSuperDataProperties(OWLDataProperty property, boolean direct) {
		throw unsupported("getSuperDataProperties");
	}

	@Override
	public Node<OWLDataProperty> getEquivalentDataProperties(OWLDataProperty property) {
		throw unsupported("getEquivalentDataProperties");
	}

	@Override
	public NodeSet<OWLDataProperty> getDisjointDataProperties(OWLDataPropertyExpression property) {
		throw unsupported("getDisjointDataProperties");
	}

	@Override
	public Node<OWLDataProperty> getTopDataPropertyNode() {
		throw unsupported("getTopDataPropertyNode");
	}

	@Override
	public Node<OWLDataProperty> getBottomDataPropertyNode() {
		throw unsupported("getBottomDataPropertyNode");
	}

	@Override
	public NodeSet<OWLClass> getDataPropertyDomains(OWLDataProperty property, boolean direct) {
		throw unsupported("getDataPropertyDomains");
	}


	// ------------------------------------------------------------------- the ABox

	@Override
	public NodeSet<OWLClass> getTypes(OWLNamedIndividual individual, boolean direct) {
		checkOpen();
		if (!isKnown(individual)) {
			return new OWLClassNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getTypes", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLTypes(mBridge, iri(individual), individual, callback, direct);
				return null;
			}
		});
		return classNodeSet(callback);
	}

	@Override
	public NodeSet<OWLNamedIndividual> getInstances(final OWLClassExpression classExpression, final boolean direct) {
		requireClassExpression(classExpression);
		if (!isKnown(classExpression)) {
			return new OWLNamedIndividualNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getInstances", new NativeCall<Void>() {
			@Override
			public Void call() {
				if (classExpression.isAnonymous()) {
					mQuerying.queryOWLClassExpressionInstances(mBridge,
							classExpressionPointer(classExpression), callback, direct);
				} else {
					OWLClass named = classExpression.asOWLClass();
					mQuerying.queryOWLInstances(mBridge, iri(named), named, callback, direct);
				}
				return null;
			}
		});
		return individualNodeSet(callback);
	}

	@Override
	public NodeSet<OWLNamedIndividual> getObjectPropertyValues(OWLNamedIndividual individual,
			OWLObjectPropertyExpression property) {
		final OWLObjectProperty named = namedObjectProperty(property, "getObjectPropertyValues");
		if (!isKnown(individual) || !isKnown(named)) {
			return new OWLNamedIndividualNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getObjectPropertyValues", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLObjectPropertyTargets(mBridge, iri(individual), individual,
						iri(named), named, callback);
				return null;
			}
		});
		return individualNodeSet(callback);
	}

	@Override
	public Node<OWLNamedIndividual> getSameIndividuals(OWLNamedIndividual individual) {
		checkOpen();
		if (!isKnown(individual)) {
			return new OWLNamedIndividualNode(individual);
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard("getSameIndividuals", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSameIndividuals(mBridge, iri(individual), individual, callback);
				return null;
			}
		});
		Set<OWLNamedIndividual> individuals = collect(callback.getObjects(), OWLNamedIndividual.class);
		// the interface asks for a node that contains the asked individual itself
		individuals.add(individual);
		return new OWLNamedIndividualNode(individuals);
	}

	@Override
	public Set<OWLLiteral> getDataPropertyValues(OWLNamedIndividual individual, OWLDataProperty property) {
		throw unsupported("getDataPropertyValues");
	}

	@Override
	public NodeSet<OWLNamedIndividual> getDifferentIndividuals(OWLNamedIndividual individual) {
		throw unsupported("getDifferentIndividuals");
	}


	// ------------------------------------------------------------- the entailment

	/**
	 * The axiom types whose entailment can be decided with the queries the bridge offers: a class
	 * inclusion is entailed when the class and the complement of its super class have no common
	 * instance, which the satisfiability check of an anonymous expression decides, and the other
	 * types are reduced to that or to the instance, property and individual queries.
	 */
	private static final Set<AxiomType<?>> ENTAILMENT_CHECKING_TYPES = Collections.unmodifiableSet(
			new HashSet<AxiomType<?>>(java.util.Arrays.<AxiomType<?>>asList(
					AxiomType.SUBCLASS_OF, AxiomType.EQUIVALENT_CLASSES, AxiomType.DISJOINT_CLASSES,
					AxiomType.OBJECT_PROPERTY_DOMAIN, AxiomType.OBJECT_PROPERTY_RANGE,
					AxiomType.SUB_OBJECT_PROPERTY, AxiomType.EQUIVALENT_OBJECT_PROPERTIES,
					AxiomType.CLASS_ASSERTION, AxiomType.OBJECT_PROPERTY_ASSERTION,
					AxiomType.SAME_INDIVIDUAL, AxiomType.DIFFERENT_INDIVIDUALS,
					AxiomType.DECLARATION)));

	@Override
	public boolean isEntailed(OWLAxiom axiom) {
		checkOpen();
		if (axiom == null) {
			throw new IllegalArgumentException("an axiom is required");
		}
		if (!isEntailmentCheckingSupported(axiom.getAxiomType())) {
			throw new UnsupportedEntailmentTypeException(axiom);
		}
		if (!isConsistent()) {
			throw new InconsistentOntologyException();
		}
		OWLAxiom plain = axiom.getAxiomWithoutAnnotations();
		if (plain instanceof OWLSubClassOfAxiom) {
			OWLSubClassOfAxiom sub = (OWLSubClassOfAxiom) plain;
			return isSubClassOf(sub.getSubClass(), sub.getSuperClass());
		}
		if (plain instanceof OWLEquivalentClassesAxiom) {
			List<OWLClassExpression> classes = new ArrayList<OWLClassExpression>(
					((OWLEquivalentClassesAxiom) plain).getClassExpressions());
			for (int i = 1; i < classes.size(); i++) {
				if (!isSubClassOf(classes.get(i - 1), classes.get(i))
						|| !isSubClassOf(classes.get(i), classes.get(i - 1))) {
					return false;
				}
			}
			return true;
		}
		if (plain instanceof OWLDisjointClassesAxiom) {
			List<OWLClassExpression> classes = new ArrayList<OWLClassExpression>(
					((OWLDisjointClassesAxiom) plain).getClassExpressions());
			for (int i = 0; i < classes.size(); i++) {
				for (int j = i + 1; j < classes.size(); j++) {
					if (isSatisfiable(mDataFactory.getOWLObjectIntersectionOf(classes.get(i), classes.get(j)))) {
						return false;
					}
				}
			}
			return true;
		}
		if (plain instanceof OWLObjectPropertyDomainAxiom) {
			// the domain is C when everything with a successor is a C
			OWLObjectPropertyDomainAxiom domain = (OWLObjectPropertyDomainAxiom) plain;
			return isSubClassOf(mDataFactory.getOWLObjectSomeValuesFrom(domain.getProperty(), mDataFactory.getOWLThing()),
					domain.getDomain());
		}
		if (plain instanceof OWLObjectPropertyRangeAxiom) {
			// the range is C when nothing has a successor outside C
			OWLObjectPropertyRangeAxiom range = (OWLObjectPropertyRangeAxiom) plain;
			return !isSatisfiable(mDataFactory.getOWLObjectSomeValuesFrom(range.getProperty(),
					mDataFactory.getOWLObjectComplementOf(range.getRange())));
		}
		if (plain instanceof OWLSubObjectPropertyOfAxiom) {
			OWLSubObjectPropertyOfAxiom sub = (OWLSubObjectPropertyOfAxiom) plain;
			OWLObjectPropertyExpression subProperty = sub.getSubProperty();
			OWLObjectPropertyExpression superProperty = sub.getSuperProperty();
			if (subProperty.isAnonymous() || superProperty.isAnonymous()) {
				throw new UnsupportedEntailmentTypeException(axiom);
			}
			if (subProperty.equals(superProperty) || superProperty.isOWLTopObjectProperty()
					|| subProperty.isOWLBottomObjectProperty()) {
				return true;
			}
			return getSuperObjectProperties(subProperty, false).containsEntity(superProperty)
					|| getEquivalentObjectProperties(subProperty).contains(superProperty);
		}
		if (plain instanceof OWLEquivalentObjectPropertiesAxiom) {
			List<OWLObjectPropertyExpression> properties = new ArrayList<OWLObjectPropertyExpression>(
					((OWLEquivalentObjectPropertiesAxiom) plain).getProperties());
			for (OWLObjectPropertyExpression property : properties) {
				if (property.isAnonymous()) {
					throw new UnsupportedEntailmentTypeException(axiom);
				}
			}
			for (int i = 1; i < properties.size(); i++) {
				if (!properties.get(i).equals(properties.get(0))
						&& !getEquivalentObjectProperties(properties.get(0)).contains(properties.get(i))) {
					return false;
				}
			}
			return true;
		}
		if (plain instanceof OWLClassAssertionAxiom) {
			OWLClassAssertionAxiom assertion = (OWLClassAssertionAxiom) plain;
			if (assertion.getIndividual().isAnonymous()) {
				throw new UnsupportedEntailmentTypeException(axiom);
			}
			if (assertion.getClassExpression().isOWLThing()) {
				return true;
			}
			return getInstances(assertion.getClassExpression(), false)
					.containsEntity(assertion.getIndividual().asOWLNamedIndividual());
		}
		if (plain instanceof OWLObjectPropertyAssertionAxiom) {
			OWLObjectPropertyAssertionAxiom assertion = ((OWLObjectPropertyAssertionAxiom) plain).getSimplified();
			if (assertion.getProperty().isAnonymous() || assertion.getSubject().isAnonymous()
					|| assertion.getObject().isAnonymous()) {
				throw new UnsupportedEntailmentTypeException(axiom);
			}
			return getObjectPropertyValues(assertion.getSubject().asOWLNamedIndividual(), assertion.getProperty())
					.containsEntity(assertion.getObject().asOWLNamedIndividual());
		}
		if (plain instanceof OWLSameIndividualAxiom) {
			List<OWLIndividual> individuals = new ArrayList<OWLIndividual>(
					((OWLSameIndividualAxiom) plain).getIndividuals());
			for (OWLIndividual individual : individuals) {
				if (individual.isAnonymous()) {
					throw new UnsupportedEntailmentTypeException(axiom);
				}
			}
			Node<OWLNamedIndividual> same = getSameIndividuals(individuals.get(0).asOWLNamedIndividual());
			for (int i = 1; i < individuals.size(); i++) {
				if (!same.contains(individuals.get(i).asOWLNamedIndividual())) {
					return false;
				}
			}
			return true;
		}
		if (plain instanceof OWLDifferentIndividualsAxiom) {
			// two individuals are different when their nominals cannot share an instance
			List<OWLIndividual> individuals = new ArrayList<OWLIndividual>(
					((OWLDifferentIndividualsAxiom) plain).getIndividuals());
			for (OWLIndividual individual : individuals) {
				if (individual.isAnonymous()) {
					throw new UnsupportedEntailmentTypeException(axiom);
				}
			}
			for (int i = 0; i < individuals.size(); i++) {
				for (int j = i + 1; j < individuals.size(); j++) {
					if (isSatisfiable(mDataFactory.getOWLObjectIntersectionOf(
							mDataFactory.getOWLObjectOneOf(individuals.get(i)),
							mDataFactory.getOWLObjectOneOf(individuals.get(j))))) {
						return false;
					}
				}
			}
			return true;
		}
		if (plain instanceof OWLDeclarationAxiom) {
			// a declaration says nothing that could fail to hold
			return true;
		}
		throw new UnsupportedEntailmentTypeException(axiom);
	}

	/** C is a sub class of D when C and not D have no common instance */
	private boolean isSubClassOf(OWLClassExpression subClass, OWLClassExpression superClass) {
		if (superClass.isOWLThing() || subClass.isOWLNothing() || subClass.equals(superClass)) {
			return true;
		}
		return !isSatisfiable(mDataFactory.getOWLObjectIntersectionOf(subClass,
				mDataFactory.getOWLObjectComplementOf(superClass)));
	}

	@Override
	public boolean isEntailed(Set<? extends OWLAxiom> axioms) {
		for (OWLAxiom axiom : axioms) {
			if (!isEntailed(axiom)) {
				return false;
			}
		}
		return true;
	}

	@Override
	public boolean isEntailmentCheckingSupported(AxiomType<?> axiomType) {
		return ENTAILMENT_CHECKING_TYPES.contains(axiomType);
	}


	// ----------------------------------------------------------------- the helpers

	private void requireClassExpression(OWLClassExpression classExpression) {
		checkOpen();
		if (classExpression == null) {
			throw new IllegalArgumentException("a class expression is required");
		}
	}

	/**
	 * Builds the expression on the native side and returns its address, for the queries about
	 * a class expression. Has to run inside a native call, see install, since the building is
	 * a series of native calls itself. A construct that the bridge cannot express is reported
	 * by a KoncludeUnsupportedConstructException.
	 */
	private long classExpressionPointer(OWLClassExpression classExpression) {
		return mExpressionTranslator.translateClassExpression(classExpression);
	}

	/**
	 * Whether every entity of the expression reached the native side, see isKnown(OWLEntity).
	 * The datatypes are left out, the built-in ones are known without being mentioned by an
	 * axiom.
	 */
	private boolean isKnown(OWLClassExpression classExpression) {
		for (OWLEntity entity : classExpression.getSignature()) {
			if (!entity.isOWLDatatype() && !isKnown(entity)) {
				return false;
			}
		}
		return true;
	}

	private OWLObjectProperty namedObjectProperty(OWLObjectPropertyExpression property, String method) {
		checkOpen();
		if (property == null) {
			throw new IllegalArgumentException("an object property expression is required");
		}
		if (property.isAnonymous()) {
			throw new UnsupportedOperationException(method + " of Konclude can only be asked about a "
					+ "named object property, the queries of the JNI bridge identify a property by its "
					+ "IRI, but was asked about '" + property + "'");
		}
		return property.asOWLObjectProperty();
	}

	/**
	 * Whether the entity reached the native side. The native side answers a query about an
	 * entity that it does not know with an empty result, which would look like a computed
	 * answer, so a fresh entity is either reported or answered trivially here.
	 */
	private boolean isKnown(OWLEntity entity) {
		if (mTranslator.isTranslated(entity)) {
			return true;
		}
		if (getFreshEntityPolicy() == FreshEntityPolicy.DISALLOW) {
			throw new FreshEntitiesException(Collections.singleton(entity));
		}
		return false;
	}

	private static String iri(OWLEntity entity) {
		return entity.getIRI().toString();
	}

	private UnsupportedOperationException unsupported(String method) {
		return new UnsupportedOperationException(method + " is not supported by Konclude through the "
				+ "JNI bridge, the native side provides no query for it");
	}

	private void checkOpen() {
		if (mDisposed) {
			throw new IllegalStateException("the reasoner has been disposed");
		}
		if (mTimedOut) {
			throw new IllegalStateException("a call of the reasoner ran into the time out and is "
					+ "still running, the bridge cannot cancel it, so this reasoner cannot be used "
					+ "any more, see the comment of KoncludeReasoner");
		}
		if (mBridge == null) {
			throw new IllegalStateException("the reasoner has no library instance");
		}
	}

	/** a call into the native side, which is what the time out of the OWL API is applied to */
	private interface NativeCall<T> {
		T call();
	}

	/**
	 * Runs a native call on the reasoner thread and waits for it, under the time out of the
	 * configuration if one is set. Every native call goes through here, so that all of them
	 * happen on the thread that initialised the library instance, see the class comment.
	 *
	 * ATTENTION: a call that ran into the time out keeps running, the bridge has no way to
	 * cancel it, so its thread and its share of the machine are lost until the virtual machine
	 * ends. The reasoner refuses to be used afterwards rather than queueing further calls
	 * behind the one that is stuck.
	 */
	private <T> T guard(String method, NativeCall<T> call) {
		final long start = System.nanoTime();
		Future<T> future = mReasonerThread.submit(new Callable<T>() {
			@Override
			public T call() throws Exception {
				return call.call();
			}
		});
		mInFlight.add(future);
		try {
			long timeOut = getTimeOut();
			if (timeOut == Long.MAX_VALUE || timeOut <= 0) {
				return future.get();
			}
			return future.get(timeOut, TimeUnit.MILLISECONDS);
		} catch (CancellationException exception) {
			throw new ReasonerInterruptedException(method + " of Konclude was interrupted, the "
					+ "calculation runs on to its end on the reasoner's thread");
		} catch (TimeoutException exception) {
			mTimedOut = true;
			throw new TimeOutException(method + " of Konclude did not return within the configured "
					+ getTimeOut() + " ms. An ABox that forces two individuals to be merged is known "
					+ "to keep Konclude in its precomputation, see Java/Readme.md and "
					+ "KoncludeReasoner.MergeSafety.");
		} catch (InterruptedException exception) {
			Thread.currentThread().interrupt();
			throw new ReasonerInterruptedException(exception);
		} catch (ExecutionException exception) {
			Throwable cause = exception.getCause();
			if (cause instanceof RuntimeException) {
				throw (RuntimeException) cause;
			}
			if (cause instanceof Error) {
				throw (Error) cause;
			}
			throw new IllegalStateException(cause);
		} finally {
			mInFlight.remove(future);
			forwardKoncludeLog();
			long milliseconds = (System.nanoTime() - start) / 1000000L;
			if (milliseconds >= SLOW_CALL_MILLISECONDS) {
				LOGGER.info("Konclude: {} took {} ms", method, milliseconds);
			} else if (LOGGER.isDebugEnabled()) {
				LOGGER.debug("Konclude: {} took {} ms", method, milliseconds);
			}
		}
	}

	/**
	 * Hands the log messages of the library to SLF4J, which puts them into the log of Protege and
	 * of any other program that uses the wrapper. Without this the library's log reached nobody,
	 * a warning that the ontology is not in OWL 2 DL included (issue #32). The messages are those
	 * of every library instance of the process, logged under 'com.konclude.native' and Konclude's
	 * domain, 'com.konclude.native.Reasoner.Preprocess.NonSimpleRoleRestrictionCheck' for
	 * instance. Level 60 is logged as a warning and 70 and above as an error. Konclude's
	 * information, level 30, is logged as debug: a classification writes a few hundred such
	 * lines, most of them about its internal commands, which would fill the log of Protege,
	 * whose root logger is at info; setting 'com.konclude.native' to debug shows them.
	 */
	private static void forwardKoncludeLog() {
		synchronized (NATIVE_LOG_LOCK) {
			String[] entries;
			try {
				entries = KoncludeReasonerBridge.takeKoncludeLogMessages();
			} catch (UnsatisfiedLinkError error) {
				// a library without the method, the log stays where it was
				return;
			}
			if (entries == null) {
				return;
			}
			for (int i = 0; i + 2 < entries.length; i += 3) {
				double level;
				try {
					level = Double.parseDouble(entries[i]);
				} catch (NumberFormatException exception) {
					level = 30;
				}
				Logger logger = LoggerFactory.getLogger(nativeLoggerName(entries[i + 1]));
				String message = entries[i + 2];
				if (level >= 70) {
					logger.error(message);
				} else if (level >= 60) {
					logger.warn(message);
				} else {
					logger.debug(message);
				}
			}
		}
	}

	private static final Object NATIVE_LOG_LOCK = new Object();

	/** '::Konclude::Reasoner::Preprocess' becomes 'com.konclude.native.Reasoner.Preprocess' */
	private static String nativeLoggerName(String domain) {
		StringBuilder name = new StringBuilder("com.konclude.native");
		for (String part : domain.split("::")) {
			part = part.trim();
			if (!part.isEmpty() && !part.equals("Konclude")) {
				name.append('.').append(part.replace(' ', '_'));
			}
		}
		return name.toString();
	}

	/** the name of a query in the log: with the expression when it is anonymous */
	private static String describe(String method, OWLClassExpression classExpression) {
		if (!classExpression.isAnonymous()) {
			return method;
		}
		String expression = classExpression.toString();
		if (expression.length() > 200) {
			expression = expression.substring(0, 200) + "...";
		}
		return method + " of " + expression;
	}

	/** the objects that the native side reported, without the ones of another type */
	private static <T> Set<T> collect(List<Object> objects, Class<T> type) {
		Set<T> result = new LinkedHashSet<T>();
		for (Object object : objects) {
			if (type.isInstance(object)) {
				result.add(type.cast(object));
			}
		}
		return result;
	}

	/**
	 * The class taxonomy, read once the classification is complete, null before that or if the
	 * cache is switched off. A caller that walks the whole hierarchy, such as the classification
	 * service of the SNOMED CT toolkit with its ReasonerTaxonomyWalker, asks for the direct sub and
	 * super classes of every class, about two million calls for SNOMED CT, and each of them
	 * through the bridge cost about 60 microseconds, two minutes in all; the taxonomy answers them
	 * on the caller's thread.
	 */
	private ClassTaxonomy getClassTaxonomy() {
		checkOpen();
		if (!TAXONOMY_CACHE) {
			return null;
		}
		if (mClassTaxonomy == null && mClassTaxonomyRequestable) {
			final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
			int[] edges = guard("getClassTaxonomy", new NativeCall<int[]>() {
				@Override
				public int[] call() {
					return mQuerying.queryOWLClassHierarchy(mBridge, callback);
				}
			});
			if (edges == null) {
				// not classified yet: the hierarchy query that the caller makes natively classifies,
				// after that the taxonomy is asked for again
				mClassTaxonomyRequestable = false;
			} else {
				mClassTaxonomy = new ClassTaxonomy(callback.getObjectSets(), edges);
			}
		}
		return mClassTaxonomy;
	}

	private void allowClassTaxonomyRetry(OWLClassExpression classExpression) {
		if (mClassTaxonomy == null && !mClassTaxonomyRequestable && !mClassTaxonomyRetried && !classExpression.isAnonymous()) {
			mClassTaxonomyRetried = true;
			mClassTaxonomyRequestable = true;
		}
	}

	/**
	 * The class taxonomy on the Java side: the nodes in the order the native side reported them,
	 * with their direct children and parents as positions. A node without a class object is kept,
	 * since edges run through it, but left out of every answer, as classNodeSet leaves it out.
	 */
	private static final class ClassTaxonomy {
		private final OWLClassNode[] mNodes;
		private final int[][] mChildren;
		private final int[][] mParents;
		private final Map<OWLClass, Integer> mNodePositions = new HashMap<OWLClass, Integer>();

		ClassTaxonomy(List<List<Object>> nodeObjectSets, int[] edges) {
			int nodeCount = nodeObjectSets.size();
			mNodes = new OWLClassNode[nodeCount];
			for (int i = 0; i < nodeCount; ++i) {
				Set<OWLClass> classes = collect(nodeObjectSets.get(i), OWLClass.class);
				mNodes[i] = classes.isEmpty() ? null : new OWLClassNode(classes);
				for (OWLClass cls : classes) {
					mNodePositions.put(cls, Integer.valueOf(i));
				}
			}
			int[] childCounts = new int[nodeCount];
			int[] parentCounts = new int[nodeCount];
			for (int e = 0; e + 1 < edges.length; e += 2) {
				++childCounts[edges[e]];
				++parentCounts[edges[e + 1]];
			}
			mChildren = new int[nodeCount][];
			mParents = new int[nodeCount][];
			for (int i = 0; i < nodeCount; ++i) {
				mChildren[i] = new int[childCounts[i]];
				mParents[i] = new int[parentCounts[i]];
			}
			int[] childFill = new int[nodeCount];
			int[] parentFill = new int[nodeCount];
			for (int e = 0; e + 1 < edges.length; e += 2) {
				int parent = edges[e];
				int child = edges[e + 1];
				mChildren[parent][childFill[parent]++] = child;
				mParents[child][parentFill[child]++] = parent;
			}
		}

		boolean contains(OWLClass cls) {
			return mNodePositions.containsKey(cls);
		}

		NodeSet<OWLClass> getDirectSubClasses(OWLClass cls) {
			return nodeSet(mChildren[mNodePositions.get(cls).intValue()]);
		}

		NodeSet<OWLClass> getDirectSuperClasses(OWLClass cls) {
			return nodeSet(mParents[mNodePositions.get(cls).intValue()]);
		}

		Node<OWLClass> getEquivalentClasses(OWLClass cls) {
			return mNodes[mNodePositions.get(cls).intValue()];
		}

		private NodeSet<OWLClass> nodeSet(int[] positions) {
			Set<Node<OWLClass>> nodes = new LinkedHashSet<Node<OWLClass>>();
			for (int position : positions) {
				if (mNodes[position] != null) {
					nodes.add(mNodes[position]);
				}
			}
			return new OWLClassNodeSet(nodes);
		}
	}

	private static NodeSet<OWLClass> classNodeSet(SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLClass>> nodes = new LinkedHashSet<Node<OWLClass>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLClass> classes = collect(objectSet, OWLClass.class);
			// the native side reports an empty set for a node whose entities it has no object
			// for, which is not a node of the answer
			if (!classes.isEmpty()) {
				nodes.add(new OWLClassNode(classes));
			}
		}
		return new OWLClassNodeSet(nodes);
	}

	private static NodeSet<OWLObjectPropertyExpression> objectPropertyNodeSet(
			SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLObjectPropertyExpression>> nodes = new LinkedHashSet<Node<OWLObjectPropertyExpression>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLObjectPropertyExpression> properties =
					collect(objectSet, OWLObjectPropertyExpression.class);
			if (!properties.isEmpty()) {
				nodes.add(new OWLObjectPropertyNode(properties));
			}
		}
		return new OWLObjectPropertyNodeSet(nodes);
	}

	private static NodeSet<OWLNamedIndividual> individualNodeSet(SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLNamedIndividual>> nodes = new LinkedHashSet<Node<OWLNamedIndividual>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLNamedIndividual> individuals = collect(objectSet, OWLNamedIndividual.class);
			if (!individuals.isEmpty()) {
				nodes.add(new OWLNamedIndividualNode(individuals));
			}
		}
		return new OWLNamedIndividualNodeSet(nodes);
	}
}

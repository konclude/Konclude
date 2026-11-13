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

package com.konclude.protegetest;

import com.konclude.owlapi.KoncludeReasoner;
import com.konclude.owlapitest.KoncludeOWLAPITest;
import com.konclude.protege.KoncludeProtegeReasonerInfo;

import org.protege.editor.owl.model.inference.ProtegeOWLReasonerInfo;
import org.semanticweb.owlapi.apibinding.OWLManager;
import org.semanticweb.owlapi.model.IRI;
import org.semanticweb.owlapi.model.OWLClass;
import org.semanticweb.owlapi.model.OWLDataFactory;
import org.semanticweb.owlapi.model.OWLEntity;
import org.semanticweb.owlapi.model.OWLNamedIndividual;
import org.semanticweb.owlapi.model.OWLOntology;
import org.semanticweb.owlapi.model.OWLOntologyManager;
import org.semanticweb.owlapi.reasoner.BufferingMode;
import org.semanticweb.owlapi.reasoner.InferenceType;
import org.semanticweb.owlapi.reasoner.OWLReasoner;
import org.semanticweb.owlapi.reasoner.OWLReasonerConfiguration;
import org.semanticweb.owlapi.reasoner.ReasonerProgressMonitor;

import java.util.Arrays;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

/**
 * The scenarios of the plug-in test, run inside the OSGi framework by KoncludeProtegePluginTest.
 *
 * The 'plugin' scenario does what Protege does with a reasoner: it creates the reasoner info
 * of the plug-in, sets it up, takes the factory and the configuration from it, creates the
 * reasoner and precomputes on a classification thread, and then asks the reasoner from other
 * threads, since that is the arrangement the JNI bridge is sensitive to. Every other scenario
 * is one of KoncludeOWLAPITest, run through the bundles instead of the class path.
 */
public class KoncludeProtegePluginScenarios {

	private static final String NS = "http://konclude.com/test/protege#";

	private static int sFailures = 0;

	/** runs the scenario and returns the number of checks that did not hold */
	public static int run(String scenario) throws Exception {
		System.out.println("scenario '" + scenario + "'");
		if ("plugin".equals(scenario)) {
			runPlugin();
			return sFailures;
		}
		// exits the virtual machine if a check does not hold, which reports the failure as well
		KoncludeOWLAPITest.main(new String[] { scenario });
		return 0;
	}

	private static void check(String label, Object actual, Object expected) {
		boolean ok = actual == null ? expected == null : actual.equals(expected);
		System.out.println("  " + label + " = " + actual + (ok ? "" : ", EXPECTED " + expected));
		if (!ok) {
			++sFailures;
		}
	}

	private static Set<String> names(Set<? extends OWLEntity> entities) {
		Set<String> names = new TreeSet<String>();
		for (OWLEntity entity : entities) {
			names.add(entity.getIRI().getFragment());
		}
		return names;
	}

	private static Set<String> expected(String... names) {
		return new TreeSet<String>(Arrays.asList(names));
	}

	/** counts the tasks that the reasoner reports, as the progress window of Protege shows them */
	private static class CountingMonitor implements ReasonerProgressMonitor {
		final AtomicInteger started = new AtomicInteger();
		final AtomicInteger busy = new AtomicInteger();
		final AtomicInteger stopped = new AtomicInteger();
		@Override public void reasonerTaskStarted(String taskName) {
			System.out.println("  task started: " + taskName);
			started.incrementAndGet();
		}
		@Override public void reasonerTaskStopped() { stopped.incrementAndGet(); }
		@Override public void reasonerTaskProgressChanged(int value, int max) { }
		// Protege paints a task that is started but never reported busy as a bar that stays at zero
		@Override public void reasonerTaskBusy() { busy.incrementAndGet(); }
	}

	private static void runPlugin() throws Exception {
		// the class comes through the plug-in bundle, which is what binds the shared library
		check("the reasoner is loaded through a bundle, not from the class path",
				KoncludeReasoner.class.getClassLoader() != ClassLoader.getSystemClassLoader(), true);

		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLDataFactory df = manager.getOWLDataFactory();
		OWLClass a = df.getOWLClass(IRI.create(NS + "A"));
		OWLClass b = df.getOWLClass(IRI.create(NS + "B"));
		OWLClass c = df.getOWLClass(IRI.create(NS + "C"));
		OWLNamedIndividual x = df.getOWLNamedIndividual(IRI.create(NS + "x"));
		OWLOntology ontology = manager.createOntology(IRI.create(NS));
		manager.addAxiom(ontology, df.getOWLSubClassOfAxiom(a, b));
		manager.addAxiom(ontology, df.getOWLSubClassOfAxiom(b, c));
		manager.addAxiom(ontology, df.getOWLClassAssertionAxiom(a, x));

		// what ProtegeOWLReasonerPluginJPFImpl and OWLReasonerManagerImpl do with the extension
		ProtegeOWLReasonerInfo info = new KoncludeProtegeReasonerInfo();
		info.setup(manager, "konclude-factory", "Konclude");
		info.initialise();
		check("reasoner id", info.getReasonerId(), "konclude-factory");
		check("reasoner name", info.getReasonerName(), "Konclude");
		check("recommended buffering", info.getRecommendedBuffering(), BufferingMode.BUFFERING);
		check("factory name", info.getReasonerFactory().getReasonerName(), "Konclude");

		CountingMonitor monitor = new CountingMonitor();
		final OWLReasonerConfiguration configuration = info.getConfiguration(monitor);
		final org.semanticweb.owlapi.reasoner.OWLReasonerFactory factory = info.getReasonerFactory();

		// Protege creates the reasoner and precomputes on its classification thread
		final AtomicReference<OWLReasoner> created = new AtomicReference<OWLReasoner>();
		final AtomicReference<Throwable> failure = new AtomicReference<Throwable>();
		Thread classification = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					OWLReasoner reasoner = factory.createReasoner(ontology, configuration);
					reasoner.precomputeInferences(InferenceType.CLASS_HIERARCHY, InferenceType.CLASS_ASSERTIONS);
					created.set(reasoner);
				} catch (Throwable throwable) {
					failure.set(throwable);
				}
			}
		}, "Classification Thread");
		classification.start();
		classification.join();
		if (failure.get() != null) {
			throw new Exception("creating the reasoner failed", failure.get());
		}
		final OWLReasoner reasoner = created.get();
		check("class hierarchy precomputed", reasoner.isPrecomputed(InferenceType.CLASS_HIERARCHY), true);
		check("class assertions precomputed", reasoner.isPrecomputed(InferenceType.CLASS_ASSERTIONS), true);
		check("tasks reported to the progress monitor", monitor.started.get(), 2);
		check("tasks reported as busy", monitor.busy.get(), 2);
		check("tasks reported as stopped", monitor.stopped.get(), 2);

		// and asks from the event thread, which here is the caller's thread
		check("consistent", reasoner.isConsistent(), true);
		check("sub classes of C", names(reasoner.getSubClasses(c, false).getFlattened()), expected("A", "B", "Nothing"));
		check("instances of C", names(reasoner.getInstances(c, false).getFlattened()), expected("x"));
		check("sub classes of (B and not A)", names(reasoner.getSubClasses(
				df.getOWLObjectIntersectionOf(b, df.getOWLObjectComplementOf(a)), false).getFlattened()),
				expected("Nothing"));

		// and from yet another thread, as the views of Protege do
		final AtomicReference<Set<String>> fromAnotherThread = new AtomicReference<Set<String>>();
		Thread view = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					fromAnotherThread.set(names(reasoner.getSuperClasses(a, false).getFlattened()));
				} catch (Throwable throwable) {
					failure.set(throwable);
				}
			}
		}, "View Thread");
		view.start();
		view.join();
		if (failure.get() != null) {
			throw new Exception("asking from another thread failed", failure.get());
		}
		check("super classes of A from another thread", fromAnotherThread.get(), expected("B", "C", "Thing"));

		// a change is buffered until Protege synchronises
		manager.addAxiom(ontology, df.getOWLSubClassOfAxiom(c, df.getOWLClass(IRI.create(NS + "D"))));
		check("pending changes", reasoner.getPendingChanges().size(), 1);
		reasoner.flush();
		check("super classes of A after the change", names(reasoner.getSuperClasses(a, false).getFlattened()),
				expected("B", "C", "D", "Thing"));

		reasoner.dispose();
		info.dispose();
	}
}

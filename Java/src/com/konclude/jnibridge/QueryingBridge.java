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

package com.konclude.jnibridge;

/**
 * Asks the native reasoner, corresponds to
 * Source/Control/Interface/JNI/com_konclude_jnibridge_QueryingBridge.cpp.
 *
 * All 20 entry points that the native side declares are declared here: 12 queries about a
 * named entity, the builder and 5 queries for a class expression, the progress of the
 * calculations, and the whole class hierarchy at once.
 *
 * An entity is identified by its IRI, the object beside it is only handed over so that the
 * native side can remember it if the entity has not been built before. The results are not
 * returned, they are reported to the callback listener, which receives the objects that were
 * handed over while the entities were built.
 *
 * The hierarchy queries report a set per set of equivalent entities and therefore take a
 * SetOfObjectSetCallbackListener, the queries that answer with a flat set take an
 * ObjectSetCallbackListener. A reported set can be empty, for instance for the bottom node
 * if no object has been associated with owl:Nothing while the ontology was built.
 *
 *
 * QUERIES ABOUT A CLASS EXPRESSION
 *
 * A class expression that is not a named class, the ObjectSomeValuesFrom of a DL query for
 * instance, is built first, through an AxiomExpressionBuildingBridge that
 * initOWLClassExpressionBuilder binds to this querying bridge. That builder writes into a
 * revision of the installed ontology that is never installed, so the expression does not
 * change the ontology, and the expression stays valid as long as this querying bridge is
 * open. The address it returns is what the class expression queries take, like the arguments
 * of the building bridge. The results are reported like those of the named queries. A query
 * that the native side does not answer throws a KoncludeReasonerException, since an empty
 * answer is a valid one.
 *
 *
 * WHAT THE NATIVE SIDE DOES NOT ANSWER
 *
 * The queries do not cover the OWL API OWLReasoner interface. Missing are the hierarchy
 * of the data properties, the disjoint classes and properties, the domains and the ranges of
 * properties, the inverse properties, the different individuals, the data property values
 * and the entailment check, so the native side has to be extended for them.
 */
public class QueryingBridge {

	/** written by the native side, holds the CJNIQueryProcessor of this bridge */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public QueryingBridge() {
	}


	// ----------------------------------------------------------- consistency

	public native boolean checkIsOWLOntologyConsistent(KoncludeReasonerBridge bridge);


	// ------------------------------------------------------- class hierarchy

	public native void queryOWLSubClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLSuperClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLEquivalentClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			ObjectSetCallbackListener callback);


	// --------------------------------------------- object property hierarchy

	/**
	 * ATTENTION: the native side accepts the inverses flag but ignores it, it calls
	 * queryOntologySubObjectProperties with the IRI, the direct flag and the callback only.
	 */
	public native void queryOWLSubObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			SetOfObjectSetCallbackListener callback, boolean direct, boolean inverses);

	/** ATTENTION: the native side ignores the inverses flag, see queryOWLSubObjectProperties */
	public native void queryOWLSuperObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			SetOfObjectSetCallbackListener callback, boolean direct, boolean inverses);

	public native void queryOWLEquivalentObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			ObjectSetCallbackListener callback);


	// --------------------------------------------------------------- ABox

	public native void queryOWLInstances(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLTypes(KoncludeReasonerBridge bridge, String iri, Object individualObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	/**
	 * The individuals that are the same as the given one, a flat set, so this takes an
	 * ObjectSetCallbackListener like the other queries that answer with one set. The generated
	 * header declared a SetOfObjectSetCallbackListener here while the implementation has always
	 * looked up visitObject on ObjectSetCallbackListener, which terminated the virtual machine
	 * as soon as the query reported anything.
	 */
	public native void queryOWLSameIndividuals(KoncludeReasonerBridge bridge, String iri, Object individualObject,
			ObjectSetCallbackListener callback);

	/** the individuals that are related to the given one by the given property */
	public native void queryOWLObjectPropertySources(KoncludeReasonerBridge bridge,
			String individualIRI, Object individualObject, String propertyIRI, Object propertyObject,
			SetOfObjectSetCallbackListener callback);

	/** the individuals that the given one is related to by the given property */
	public native void queryOWLObjectPropertyTargets(KoncludeReasonerBridge bridge,
			String individualIRI, Object individualObject, String propertyIRI, Object propertyObject,
			SetOfObjectSetCallbackListener callback);


	// -------------------------------------------------------- class expressions

	/**
	 * Binds the builder to this querying bridge, so that its buildOWL... methods build the
	 * expressions that the queries below are asked about, see the class comment. The builder
	 * is owned by the querying bridge and is closed with it by finalizeQueryingBridge, it must
	 * not be handed to finalizeAxiomExpressionVisitingBridge.
	 */
	public native void initOWLClassExpressionBuilder(KoncludeReasonerBridge bridge,
			AxiomExpressionBuildingBridge builder);

	public native boolean checkIsOWLClassExpressionSatisfiable(KoncludeReasonerBridge bridge,
			long classExpression);

	public native void queryOWLClassExpressionSubClasses(KoncludeReasonerBridge bridge,
			long classExpression, SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLClassExpressionSuperClasses(KoncludeReasonerBridge bridge,
			long classExpression, SetOfObjectSetCallbackListener callback, boolean direct);

	/** the named classes equivalent to the expression, a flat set */
	public native void queryOWLClassExpressionEquivalentClasses(KoncludeReasonerBridge bridge,
			long classExpression, ObjectSetCallbackListener callback);

	public native void queryOWLClassExpressionInstances(KoncludeReasonerBridge bridge,
			long classExpression, SetOfObjectSetCallbackListener callback, boolean direct);


	// ------------------------------------------------------------- progress

	/** the number of values that queryOWLReasoningProgress reports */
	public static final int PROGRESS_VALUE_COUNT = 12;
	public static final int PROGRESS_CALCULATION_PROCESSED_TASKS = 0;
	public static final int PROGRESS_CALCULATION_REMAINING_TASKS = 1;
	public static final int PROGRESS_CLASSIFICATION_PERCENT = 2;
	public static final int PROGRESS_CLASSIFICATION_TESTED = 3;
	public static final int PROGRESS_CLASSIFICATION_TOTAL = 4;
	public static final int PROGRESS_CLASSIFICATION_REMAINING_MILLISECONDS = 5;
	public static final int PROGRESS_REALIZATION_PERCENT = 6;
	public static final int PROGRESS_REALIZATION_TESTED = 7;
	public static final int PROGRESS_REALIZATION_TOTAL = 8;
	public static final int PROGRESS_REALIZATION_REMAINING_MILLISECONDS = 9;
	public static final int PROGRESS_SATURATION_INITIALIZED_NODES = 10;
	public static final int PROGRESS_SATURATION_NODES = 11;

	/**
	 * The progress of the calculations of the native reasoner, the numbers the command line
	 * prints with '-a': PROGRESS_VALUE_COUNT values indexed by the PROGRESS_ constants, the
	 * processed and the approximated remaining tasks of the calculation, then the percentage,
	 * the tested and the total tests and the approximated remaining milliseconds of the
	 * classification, the same four of the realization, and, what '-a' does not print, the
	 * saturation nodes initialised so far and the saturation nodes that exist, counted by the
	 * saturation algorithm on the ontology. A value that is not known is 0.
	 *
	 * Unlike the queries this may be called from any thread while a query is being answered
	 * on another, it only reads counters; that is what it is for.
	 */
	public native double[] queryOWLReasoningProgress(KoncludeReasonerBridge bridge);

	/**
	 * The whole class taxonomy at once, for a caller that reads all of it, which would otherwise ask
	 * about every class on its own. The callback, a SetOfObjectSetCallbackListener, receives one set
	 * per hierarchy node, the top node first, with the class objects of the node's classes; a node
	 * may come as an empty set when the native side has no object for its classes. The result holds
	 * the edges between the nodes as pairs of their positions in that order, parent first, or is
	 * null when the classification is not complete, in which case the callback received nothing.
	 */
	public native int[] queryOWLClassHierarchy(KoncludeReasonerBridge bridge, Object callback);


	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}

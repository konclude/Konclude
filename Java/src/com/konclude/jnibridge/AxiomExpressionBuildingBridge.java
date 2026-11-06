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
 * Builds axioms and expressions in the native reasoner, corresponds to
 * Source/Control/Interface/JNI/com_konclude_jnibridge_AxiomExpressionBuildingBridge.cpp.
 *
 * All 66 entry points that the native side declares are declared here, in the order in which
 * the OWL 2 structural specification introduces the constructs.
 *
 * The returned long values are the addresses of the built expressions, they are passed back
 * to the bridge to compose larger expressions and axioms. Axioms are told to the ontology
 * while they are built, so the return value of an axiom building method is usually not
 * needed. A returned 0 means that nothing was built, which happens when the bridge has not
 * been initialised.
 *
 * The object that is handed over together with an entity is remembered by the native side
 * and returned by the queries, which is how an entity of a client, an OWL API OWLClass for
 * instance, finds its way back into the query results. Only the first object that is handed
 * over for an entity is kept, later ones are ignored.
 *
 *
 * ARGUMENT LISTS
 *
 * The n-ary constructs take their arguments from an argument list instead of from their
 * parameters:
 *
 *   initArumentList(bridge);
 *   for (long argument : arguments) { addArgument(bridge, argument); }
 *   long expression = buildOWLObjectIntersectionFromArgumentList(bridge);
 *
 * where the building method pops the list. The native side keeps the lists on a stack, so
 * nested lists are allowed, as long as every list is popped by exactly one building method.
 *
 * ATTENTION: the native side does not check the stack. addArgument and every
 * ...FromArgumentList method crash the virtual machine if no list has been pushed, so a
 * caller has to pair them itself. closeArumentList is a no-op that the native side keeps for
 * symmetry, calling it is not required. Note the misspelling of both list methods, it is the
 * name that the native side exports.
 */
public class AxiomExpressionBuildingBridge {

	/** written by the native side, holds the CJNIAxiomExpressionVisitingLoader of this builder */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public AxiomExpressionBuildingBridge() {
	}


	// ---------------------------------------------------------------- entities

	public native long buildOWLClass(KoncludeReasonerBridge bridge, String iri, Object classObject);

	public native long buildOWLObjectProperty(KoncludeReasonerBridge bridge, String iri, Object propertyObject);

	public native long buildOWLDataProperty(KoncludeReasonerBridge bridge, String iri, Object propertyObject);

	public native long buildOWLNamedIndividual(KoncludeReasonerBridge bridge, String iri, Object individualObject);

	/** the identifier is the node identifier of the individual, without a namespace */
	public native long buildOWLAnonymousIndividual(KoncludeReasonerBridge bridge, String identifier, Object individualObject);


	// ------------------------------------------------------------ argument list

	/** pushes a new argument list, note the misspelling of the native name */
	public native void initArumentList(KoncludeReasonerBridge bridge);

	/** a no-op on the native side, note the misspelling of the native name */
	public native void closeArumentList(KoncludeReasonerBridge bridge);

	/** appends to the argument list on top of the stack, crashes if there is none */
	public native void addArgument(KoncludeReasonerBridge bridge, long argument);


	// ------------------------------------------------------- class expressions

	public native long buildOWLObjectIntersectionFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectUnionFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectComplement(KoncludeReasonerBridge bridge, long classExpression);

	public native long buildOWLObjectOneOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectSomeValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	public native long buildOWLObjectAllValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	/** the filler is the individual of the value restriction */
	public native long buildOWLObjectHasValue(KoncludeReasonerBridge bridge, long property, long individual);

	public native long buildOWLObjectHasSelf(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLMinCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLMaxCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLExactCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);


	// ---------------------------------------------------- property expressions

	public native long buildOWLObjectInverseOf(KoncludeReasonerBridge bridge, long property);


	// ---------------------------------------- data ranges, literals and facets

	public native long buildOWLDatatype(KoncludeReasonerBridge bridge, String iri, Object datatypeObject);

	/**
	 * Builds a literal of the given lexical form and datatype. The native side associates the
	 * handed over object with the lexical value, not with the literal, so two literals of the
	 * same lexical form and different datatypes share their object.
	 */
	public native long buildOWLLiteral(KoncludeReasonerBridge bridge, long datatype, String lexicalForm, Object literalObject);

	public native long buildOWLDataOneOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataComplementOf(KoncludeReasonerBridge bridge, long dataRange);

	public native long buildOWLDataIntersectionOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataUnionOfFromArgumentList(KoncludeReasonerBridge bridge);

	/** the object is associated with the facet, not with the restriction */
	public native long buildOWLFacetRestriction(KoncludeReasonerBridge bridge, String facetIRI, Object facetObject, long literal);

	/** the argument list holds the facet restrictions */
	public native long buildOWLDatatypeRestrictionFromArgumentList(KoncludeReasonerBridge bridge, long datatype);


	// -------------------------------------------- data property restrictions

	public native long buildOWLDataSomeValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	public native long buildOWLDataAllValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	/** the filler is the literal of the value restriction */
	public native long buildOWLDataHasValue(KoncludeReasonerBridge bridge, long property, long literal);

	public native long buildOWLDataMinCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLDataMaxCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLDataExactCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);


	// ------------------------------------------------------------ class axioms

	/**
	 * Builds a batch of expressions and axioms encoded in one array, which saves a native call
	 * per expression and per list operand. Every op is an opcode (OP_...) followed by its
	 * arguments: a fixed number of expression addresses, a cardinality, or an operand count and
	 * that many addresses, see the translator. The ops are executed in order and every op yields
	 * one result; an argument that is negative refers to an earlier result of the same batch,
	 * -(k + 1) for the k-th op, a positive argument is an address built before (an entity, a
	 * literal, a facet restriction or a result of an earlier batch). An axiom op also tells its
	 * axiom. Returns the number of ops that were built, less than the number of ops in the
	 * batch if one failed, or -1 without a bridge.
	 */
	public native int buildBatch(KoncludeReasonerBridge bridge, long[] ops, int length);

	/**
	 * A hint of how many axioms are about to be built, so that the native side sizes its axiom
	 * sets and its expression hash once instead of growing them step by step. Optional.
	 */
	public native void expectAxioms(KoncludeReasonerBridge bridge, long axiomCount);

	public static final int OP_OBJECT_INTERSECTION_OF = 1;
	public static final int OP_OBJECT_UNION_OF = 2;
	public static final int OP_OBJECT_COMPLEMENT_OF = 3;
	public static final int OP_OBJECT_ONE_OF = 4;
	public static final int OP_OBJECT_SOME_VALUES_FROM = 5;
	public static final int OP_OBJECT_ALL_VALUES_FROM = 6;
	public static final int OP_OBJECT_HAS_VALUE = 7;
	public static final int OP_OBJECT_HAS_SELF = 8;
	public static final int OP_OBJECT_MIN_CARDINALITY = 9;
	public static final int OP_OBJECT_MAX_CARDINALITY = 10;
	public static final int OP_OBJECT_EXACT_CARDINALITY = 11;
	public static final int OP_OBJECT_INVERSE_OF = 12;
	public static final int OP_DATA_ONE_OF = 13;
	public static final int OP_DATA_COMPLEMENT_OF = 14;
	public static final int OP_DATA_INTERSECTION_OF = 15;
	public static final int OP_DATA_UNION_OF = 16;
	public static final int OP_DATATYPE_RESTRICTION = 17;
	public static final int OP_DATA_SOME_VALUES_FROM = 18;
	public static final int OP_DATA_ALL_VALUES_FROM = 19;
	public static final int OP_DATA_HAS_VALUE = 20;
	public static final int OP_DATA_MIN_CARDINALITY = 21;
	public static final int OP_DATA_MAX_CARDINALITY = 22;
	public static final int OP_DATA_EXACT_CARDINALITY = 23;
	public static final int OP_SUB_CLASS_OF = 30;
	public static final int OP_EQUIVALENT_CLASSES = 31;
	public static final int OP_DISJOINT_CLASSES = 32;
	public static final int OP_DISJOINT_UNION = 33;
	public static final int OP_SUB_OBJECT_PROPERTY_OF = 34;
	public static final int OP_SUB_PROPERTY_CHAIN_OF = 35;
	public static final int OP_EQUIVALENT_OBJECT_PROPERTIES = 36;
	public static final int OP_DISJOINT_OBJECT_PROPERTIES = 37;
	public static final int OP_INVERSE_OBJECT_PROPERTIES = 38;
	public static final int OP_OBJECT_PROPERTY_DOMAIN = 39;
	public static final int OP_OBJECT_PROPERTY_RANGE = 40;
	public static final int OP_FUNCTIONAL_OBJECT_PROPERTY = 41;
	public static final int OP_INVERSE_FUNCTIONAL_OBJECT_PROPERTY = 42;
	public static final int OP_REFLEXIVE_OBJECT_PROPERTY = 43;
	public static final int OP_IRREFLEXIVE_OBJECT_PROPERTY = 44;
	public static final int OP_SYMMETRIC_OBJECT_PROPERTY = 45;
	public static final int OP_ASYMMETRIC_OBJECT_PROPERTY = 46;
	public static final int OP_TRANSITIVE_OBJECT_PROPERTY = 47;
	public static final int OP_SUB_DATA_PROPERTY_OF = 48;
	public static final int OP_EQUIVALENT_DATA_PROPERTIES = 49;
	public static final int OP_DISJOINT_DATA_PROPERTIES = 50;
	public static final int OP_DATA_PROPERTY_DOMAIN = 51;
	public static final int OP_DATA_PROPERTY_RANGE = 52;
	public static final int OP_FUNCTIONAL_DATA_PROPERTY = 53;
	public static final int OP_DECLARATION = 54;
	public static final int OP_CLASS_ASSERTION = 55;
	public static final int OP_OBJECT_PROPERTY_ASSERTION = 56;
	public static final int OP_NEGATIVE_OBJECT_PROPERTY_ASSERTION = 57;
	public static final int OP_DATA_PROPERTY_ASSERTION = 58;
	public static final int OP_NEGATIVE_DATA_PROPERTY_ASSERTION = 59;
	public static final int OP_SAME_INDIVIDUAL = 60;
	public static final int OP_DIFFERENT_INDIVIDUALS = 61;

	public native long buildOWLSubClassOfAxiom(KoncludeReasonerBridge bridge, long subClass, long superClass);

	public native long buildOWLEquivalentClassesFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointClassesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	/** the argument list holds the classes of the union */
	public native long buildOWLDisjointUnionAxiomFromArgumentList(KoncludeReasonerBridge bridge, long classExpression);


	// -------------------------------------------------- object property axioms

	public native long buildOWLSubObjectPropertyOfAxiom(KoncludeReasonerBridge bridge, long subProperty, long superProperty);

	/** the argument list holds the properties of the chain */
	public native long buildOWLSubPropertyChainOfAxiomFromArgumentList(KoncludeReasonerBridge bridge, long superProperty);

	public native long buildOWLEquivalentObjectPropertiesFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointObjectPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLInverseObjectPropertiesAxiom(KoncludeReasonerBridge bridge, long property, long inverseProperty);

	public native long buildOWLObjectPropertyDomainAxiom(KoncludeReasonerBridge bridge, long property, long domain);

	public native long buildOWLObjectPropertyRangeAxiom(KoncludeReasonerBridge bridge, long property, long range);

	public native long buildOWLFunctionalObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLInverseFunctionalObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLReflexiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLIrreflexiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLSymmetricObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLAsymmetricObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLTransitiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);


	// ---------------------------------------------------- data property axioms

	public native long buildOWLSubDataPropertyOfAxiom(KoncludeReasonerBridge bridge, long subProperty, long superProperty);

	public native long buildOWLEquivalentDataPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointDataPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataPropertyDomainAxiom(KoncludeReasonerBridge bridge, long property, long domain);

	public native long buildOWLDataPropertyRangeAxiom(KoncludeReasonerBridge bridge, long property, long range);

	public native long buildOWLFunctionalDataPropertyAxiom(KoncludeReasonerBridge bridge, long property);


	// ------------------------------------------------------------- assertions

	public native long buildOWLDeclarationAxiom(KoncludeReasonerBridge bridge, long entity);

	public native long buildOWLClassAssertionAxiom(KoncludeReasonerBridge bridge, long classExpression, long individual);

	public native long buildOWLObjectPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long object);

	public native long buildOWLNegativeObjectPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long object);

	public native long buildOWLDataPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long literal);

	public native long buildOWLNegativeDataPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long literal);

	public native long buildOWLSameIndividualAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDifferentIndividualsAxiomFromArgumentList(KoncludeReasonerBridge bridge);


	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}

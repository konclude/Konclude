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

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE


#include <jni.h>


#include "Control/Interface/JNI/JNISettings.h"
#include "Control/Interface/JNI/com_konclude_jnibridge_AxiomExpressionBuildingBridge.h"
#include "Control/Interface/JNI/CJNIInstanceManager.h"
#include "Control/Interface/JNI/CJNIEntityMappingHash.h"
#include "Control/Interface/JNI/CJNIAxiomExpressionVisitingLoader.h"
#include "Control/Interface/JNI/CJNIHandler.h"
#include "Control/Interface/JNI/CJNIQueryProcessor.h"


#include "Parser/ParserSettings.h"

#include "Utilities/UtilitiesSettings.h"
#include "Utilities/Container/CQtList.h"
#include "Utilities/Container/ContainerSettings.h"


#include "Parser/Expressions/CBuildExpression.h"

#include <vector>

using namespace Konclude::Utilities;
using namespace Konclude::Utilities::Container;
using namespace Konclude::Parser;
using namespace Konclude::Parser::Expression;
using namespace Konclude::Control::Interface::JNI;


#define JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj, CODE) \
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj); \
	if (jniInstanceManager) { \
		CJNIAxiomExpressionVisitingLoader* builder = jniInstanceManager->getJNIHandler()->getAxiomExpressionBuildingBridgeNativeData(jenv,builderObj); \
		if (builder) { \
			CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler(); \
			CODE \
		} \
	}


// the opcodes of a batch, mirrored from AxiomExpressionBuildingBridge.OP_* on the Java side
enum JNIBatchOpcode {
	OP_OBJECT_INTERSECTION_OF = 1,
	OP_OBJECT_UNION_OF = 2,
	OP_OBJECT_COMPLEMENT_OF = 3,
	OP_OBJECT_ONE_OF = 4,
	OP_OBJECT_SOME_VALUES_FROM = 5,
	OP_OBJECT_ALL_VALUES_FROM = 6,
	OP_OBJECT_HAS_VALUE = 7,
	OP_OBJECT_HAS_SELF = 8,
	OP_OBJECT_MIN_CARDINALITY = 9,
	OP_OBJECT_MAX_CARDINALITY = 10,
	OP_OBJECT_EXACT_CARDINALITY = 11,
	OP_OBJECT_INVERSE_OF = 12,
	OP_DATA_ONE_OF = 13,
	OP_DATA_COMPLEMENT_OF = 14,
	OP_DATA_INTERSECTION_OF = 15,
	OP_DATA_UNION_OF = 16,
	OP_DATATYPE_RESTRICTION = 17,
	OP_DATA_SOME_VALUES_FROM = 18,
	OP_DATA_ALL_VALUES_FROM = 19,
	OP_DATA_HAS_VALUE = 20,
	OP_DATA_MIN_CARDINALITY = 21,
	OP_DATA_MAX_CARDINALITY = 22,
	OP_DATA_EXACT_CARDINALITY = 23,
	OP_SUB_CLASS_OF = 30,
	OP_EQUIVALENT_CLASSES = 31,
	OP_DISJOINT_CLASSES = 32,
	OP_DISJOINT_UNION = 33,
	OP_SUB_OBJECT_PROPERTY_OF = 34,
	OP_SUB_PROPERTY_CHAIN_OF = 35,
	OP_EQUIVALENT_OBJECT_PROPERTIES = 36,
	OP_DISJOINT_OBJECT_PROPERTIES = 37,
	OP_INVERSE_OBJECT_PROPERTIES = 38,
	OP_OBJECT_PROPERTY_DOMAIN = 39,
	OP_OBJECT_PROPERTY_RANGE = 40,
	OP_FUNCTIONAL_OBJECT_PROPERTY = 41,
	OP_INVERSE_FUNCTIONAL_OBJECT_PROPERTY = 42,
	OP_REFLEXIVE_OBJECT_PROPERTY = 43,
	OP_IRREFLEXIVE_OBJECT_PROPERTY = 44,
	OP_SYMMETRIC_OBJECT_PROPERTY = 45,
	OP_ASYMMETRIC_OBJECT_PROPERTY = 46,
	OP_TRANSITIVE_OBJECT_PROPERTY = 47,
	OP_SUB_DATA_PROPERTY_OF = 48,
	OP_EQUIVALENT_DATA_PROPERTIES = 49,
	OP_DISJOINT_DATA_PROPERTIES = 50,
	OP_DATA_PROPERTY_DOMAIN = 51,
	OP_DATA_PROPERTY_RANGE = 52,
	OP_FUNCTIONAL_DATA_PROPERTY = 53,
	OP_DECLARATION = 54,
	OP_CLASS_ASSERTION = 55,
	OP_OBJECT_PROPERTY_ASSERTION = 56,
	OP_NEGATIVE_OBJECT_PROPERTY_ASSERTION = 57,
	OP_DATA_PROPERTY_ASSERTION = 58,
	OP_NEGATIVE_DATA_PROPERTY_ASSERTION = 59,
	OP_SAME_INDIVIDUAL = 60,
	OP_DIFFERENT_INDIVIDUALS = 61,
};

// An argument that is negative refers to the result of an earlier op of this batch (-(k + 1) for the k-th op),
// a positive one is the address of an expression built before.
static inline CBuildExpression* jniBatchResolve(const std::vector<CBuildExpression*>& results, jlong v) {
	return v < 0 ? results[(size_t)(-v - 1)] : (CBuildExpression*)v;
}
#define JNIBATCH_RESOLVE(v) jniBatchResolve(results, (v))

static inline bool jniBatchIsKind(CBuildExpression* expression, cint64 kindFlag) {
	return expression && (expression->getExpressionKindFlags() & kindFlag) != 0;
}

// the typed builder methods for the frequent ops, when the arguments have the expected kinds; this skips the
// CExpressionSplitter of the untyped methods (which stays the fallback for anything else)
#define JNIBATCH_LIST_KIND(list, kindFlag, allOfKind) JNIBATCH_NEED(1); jint n = (jint)ops[pos++]; JNIBATCH_NEED(n); CEXPRESSIONLIST<CBuildExpression*> list; list.reserve(n); bool allOfKind = true; \
	for (jint i = 0; i < n; ++i) { CBuildExpression* e = JNIBATCH_RESOLVE(ops[pos++]); allOfKind = allOfKind && jniBatchIsKind(e, kindFlag); list.append(e); }
#define JNIBATCH_NEED(k) if (pos + (k) > length) { return (jint)results.size(); }
#define JNIBATCH_LIST(listType, list) JNIBATCH_NEED(1); jint n = (jint)ops[pos++]; JNIBATCH_NEED(n); CEXPRESSIONLIST<listType> list; list.reserve(n); \
	for (jint i = 0; i < n; ++i) { list.append((listType)JNIBATCH_RESOLVE(ops[pos++])); }
#define JNIBATCH_ARG1(a) JNIBATCH_NEED(1); CBuildExpression* a = JNIBATCH_RESOLVE(ops[pos++]);
#define JNIBATCH_ARG2(a, b) JNIBATCH_NEED(2); CBuildExpression* a = JNIBATCH_RESOLVE(ops[pos++]); CBuildExpression* b = JNIBATCH_RESOLVE(ops[pos++]);
#define JNIBATCH_ARG3(a, b, c) JNIBATCH_NEED(3); CBuildExpression* a = JNIBATCH_RESOLVE(ops[pos++]); CBuildExpression* b = JNIBATCH_RESOLVE(ops[pos++]); CBuildExpression* c = JNIBATCH_RESOLVE(ops[pos++]);
#define JNIBATCH_ARG2N(a, b, n) JNIBATCH_NEED(3); CBuildExpression* a = JNIBATCH_RESOLVE(ops[pos++]); CBuildExpression* b = JNIBATCH_RESOLVE(ops[pos++]); int n = (int)ops[pos++];

// Executes the ops of a batch in order; returns the number of ops that were built, which is less than the
// number of ops in the batch if one of them failed (the Java side reports that).
static jint buildExpressionBatch(CJNIAxiomExpressionVisitingLoader* builder, const jlong* ops, jint length) {
	std::vector<CBuildExpression*> results;
	results.reserve((size_t)length / 3 + 1);
	jint pos = 0;
	while (pos < length) {
		jint opcode = (jint)ops[pos++];
		CBuildExpression* expression = nullptr;
		bool axiom = false;
		switch (opcode) {
			case OP_OBJECT_INTERSECTION_OF: {
				JNIBATCH_LIST_KIND(list, CBuildExpression::EKFCLASSTERM, allClasses);
				if (allClasses) {
					expression = builder->getObjectIntersectionOf(*(CEXPRESSIONLIST<CClassTermExpression*>*)&list);
				} else {
					expression = builder->getObjectIntersectionOf(list);
				}
				break;
			}
			case OP_OBJECT_UNION_OF: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getObjectUnionOf(list); break; }
			case OP_OBJECT_COMPLEMENT_OF: { JNIBATCH_ARG1(a); expression = builder->getObjectComplementOf(a); break; }
			case OP_OBJECT_ONE_OF: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getObjectOneOf(list); break; }
			case OP_OBJECT_SOME_VALUES_FROM: {
				JNIBATCH_ARG2(p, f);
				if (jniBatchIsKind(p, CBuildExpression::EKFOBJECTPROPERTYTERM) && jniBatchIsKind(f, CBuildExpression::EKFCLASSTERM)) {
					expression = builder->getObjectSomeValuesFrom((CObjectPropertyTermExpression*)p, (CClassTermExpression*)f);
				} else {
					expression = builder->getObjectSomeValuesFrom(p, f);
				}
				break;
			}
			case OP_OBJECT_ALL_VALUES_FROM: { JNIBATCH_ARG2(p, f); expression = builder->getObjectAllValuesFrom(p, f); break; }
			case OP_OBJECT_HAS_VALUE: { JNIBATCH_ARG2(p, f); expression = builder->getObjectHasValue(p, f); break; }
			case OP_OBJECT_HAS_SELF: { JNIBATCH_ARG1(p); expression = builder->getObjectHasSelf(p); break; }
			case OP_OBJECT_MIN_CARDINALITY: { JNIBATCH_ARG2N(p, f, n); expression = builder->getObjectMinCardinality(p, f, n); break; }
			case OP_OBJECT_MAX_CARDINALITY: { JNIBATCH_ARG2N(p, f, n); expression = builder->getObjectMaxCardinality(p, f, n); break; }
			case OP_OBJECT_EXACT_CARDINALITY: { JNIBATCH_ARG2N(p, f, n); expression = builder->getObjectExactlyCardinality(p, f, n); break; }
			case OP_OBJECT_INVERSE_OF: { JNIBATCH_ARG1(p); expression = builder->getInverseObjectPropertyOf(p); break; }
			case OP_DATA_ONE_OF: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDataOneOf(list); break; }
			case OP_DATA_COMPLEMENT_OF: { JNIBATCH_ARG1(r); expression = builder->getDataComplementOf(r); break; }
			case OP_DATA_INTERSECTION_OF: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDataIntersectionOf(list); break; }
			case OP_DATA_UNION_OF: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDataUnionOf(list); break; }
			case OP_DATATYPE_RESTRICTION: { JNIBATCH_ARG1(dt); JNIBATCH_LIST(CDataFacetRestrictionExpression*, list); expression = builder->getDatatypeRestriction((CDatatypeExpression*)dt, list); break; }
			case OP_DATA_SOME_VALUES_FROM: { JNIBATCH_ARG2(p, r); expression = builder->getDataSomeValuesFrom(p, r); break; }
			case OP_DATA_ALL_VALUES_FROM: { JNIBATCH_ARG2(p, r); expression = builder->getDataAllValuesFrom(p, r); break; }
			case OP_DATA_HAS_VALUE: {
				JNIBATCH_ARG2(p, l);
				if (jniBatchIsKind(p, CBuildExpression::EKFDATAPROPERTYTERM) && jniBatchIsKind(l, CBuildExpression::EKFDATARANGETERM)) {
					expression = builder->getDataHasValue((CDataPropertyTermExpression*)p, (CDataRangeTermExpression*)l);
				} else {
					expression = builder->getDataHasValue(p, l);
				}
				break;
			}
			case OP_DATA_MIN_CARDINALITY: { JNIBATCH_ARG2N(p, r, n); expression = builder->getDataMinCardinality(p, r, n); break; }
			case OP_DATA_MAX_CARDINALITY: { JNIBATCH_ARG2N(p, r, n); expression = builder->getDataMaxCardinality(p, r, n); break; }
			case OP_DATA_EXACT_CARDINALITY: { JNIBATCH_ARG2N(p, r, n); expression = builder->getDataExactCardinality(p, r, n); break; }

			case OP_SUB_CLASS_OF: {
				JNIBATCH_ARG2(a, b);
				if (jniBatchIsKind(a, CBuildExpression::EKFCLASSTERM) && jniBatchIsKind(b, CBuildExpression::EKFCLASSTERM)) {
					expression = builder->getSubClassOf((CClassTermExpression*)a, (CClassTermExpression*)b);
				} else {
					expression = builder->getSubClassOf(a, b);
				}
				axiom = true;
				break;
			}
			case OP_EQUIVALENT_CLASSES: {
				JNIBATCH_LIST_KIND(list, CBuildExpression::EKFCLASSTERM, allClasses);
				if (allClasses) {
					expression = builder->getEquivalentClasses(*(CEXPRESSIONLIST<CClassTermExpression*>*)&list);
				} else {
					expression = builder->getEquivalentClasses(list);
				}
				axiom = true;
				break;
			}
			case OP_DISJOINT_CLASSES: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDisjointClasses(list); axiom = true; break; }
			case OP_DISJOINT_UNION: { JNIBATCH_ARG1(c); JNIBATCH_LIST(CClassTermExpression*, list); expression = builder->getDisjointUnion((CClassExpression*)c, list); axiom = true; break; }
			case OP_SUB_OBJECT_PROPERTY_OF: { JNIBATCH_ARG2(a, b); expression = builder->getSubObjectPropertyOf(a, b); axiom = true; break; }
			case OP_SUB_PROPERTY_CHAIN_OF: { JNIBATCH_ARG1(s); JNIBATCH_LIST(CObjectPropertyTermExpression*, list); expression = builder->getSubObjectPropertyOf(list, (CObjectPropertyTermExpression*)s); axiom = true; break; }
			case OP_EQUIVALENT_OBJECT_PROPERTIES: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getEquivalentObjectProperties(list); axiom = true; break; }
			case OP_DISJOINT_OBJECT_PROPERTIES: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDisjointObjectProperties(list); axiom = true; break; }
			case OP_INVERSE_OBJECT_PROPERTIES: { JNIBATCH_ARG2(p, q); expression = builder->getInverseObjectProperties(p, q); axiom = true; break; }
			case OP_OBJECT_PROPERTY_DOMAIN: { JNIBATCH_ARG2(p, c); expression = builder->getObjectPropertyDomainExpression(p, c); axiom = true; break; }
			case OP_OBJECT_PROPERTY_RANGE: { JNIBATCH_ARG2(p, c); expression = builder->getObjectPropertyRangeExpression(p, c); axiom = true; break; }
			case OP_FUNCTIONAL_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getFunctionalObjectProperty(p); axiom = true; break; }
			case OP_INVERSE_FUNCTIONAL_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getInverseFunctionalObjectProperty(p); axiom = true; break; }
			case OP_REFLEXIVE_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getReflexiveObjectProperty(p); axiom = true; break; }
			case OP_IRREFLEXIVE_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getIrreflexiveObjectProperty(p); axiom = true; break; }
			case OP_SYMMETRIC_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getSymmetricObjectProperty(p); axiom = true; break; }
			case OP_ASYMMETRIC_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getAsymmetricObjectProperty(p); axiom = true; break; }
			case OP_TRANSITIVE_OBJECT_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getTransetiveObjectProperty(p); axiom = true; break; }
			case OP_SUB_DATA_PROPERTY_OF: { JNIBATCH_ARG2(a, b); expression = builder->getSubDataPropertyOf(a, b); axiom = true; break; }
			case OP_EQUIVALENT_DATA_PROPERTIES: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getEquivalentDataProperties(list); axiom = true; break; }
			case OP_DISJOINT_DATA_PROPERTIES: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDisjointDataProperties(list); axiom = true; break; }
			case OP_DATA_PROPERTY_DOMAIN: { JNIBATCH_ARG2(p, c); expression = builder->getDataPropertyDomainExpression(p, c); axiom = true; break; }
			case OP_DATA_PROPERTY_RANGE: { JNIBATCH_ARG2(p, r); expression = builder->getDataPropertyRangeExpression(p, r); axiom = true; break; }
			case OP_FUNCTIONAL_DATA_PROPERTY: { JNIBATCH_ARG1(p); expression = builder->getFunctionalDataProperty(p); axiom = true; break; }
			case OP_DECLARATION: { JNIBATCH_ARG1(e); expression = builder->getDeclaration(e); axiom = true; break; }
			case OP_CLASS_ASSERTION: { JNIBATCH_ARG2(c, i); expression = builder->getClassAssertion(c, i); axiom = true; break; }
			case OP_OBJECT_PROPERTY_ASSERTION: { JNIBATCH_ARG3(p, s, o); expression = builder->getObjectPropertyAssertion(p, s, o); axiom = true; break; }
			case OP_NEGATIVE_OBJECT_PROPERTY_ASSERTION: { JNIBATCH_ARG3(p, s, o); expression = builder->getNegativeObjectPropertyAssertion(p, s, o); axiom = true; break; }
			case OP_DATA_PROPERTY_ASSERTION: { JNIBATCH_ARG3(p, s, l); expression = builder->getDataPropertyAssertion(p, s, l); axiom = true; break; }
			case OP_NEGATIVE_DATA_PROPERTY_ASSERTION: { JNIBATCH_ARG3(p, s, l); expression = builder->getNegativeDataPropertyAssertion(p, s, l); axiom = true; break; }
			case OP_SAME_INDIVIDUAL: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getSameIndividual(list); axiom = true; break; }
			case OP_DIFFERENT_INDIVIDUALS: { JNIBATCH_LIST(CBuildExpression*, list); expression = builder->getDifferentIndividuals(list); axiom = true; break; }
			default:
				return (jint)results.size();
		}
		if (!expression) {
			return (jint)results.size();
		}
		if (axiom) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
		results.push_back(expression);
	}
	return (jint)results.size();
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_expectAxioms(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong axiomCount) {
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		builder->reserveForAxioms((cint64)axiomCount);
	)
}


JNIEXPORT jint JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildBatch(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlongArray opsArray, jint length) {
	jint built = -1;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		if (length <= 0 || !opsArray) {
			built = 0;
		} else {
			// copied out instead of pinned, so that the JVM is never kept from collecting while a batch is built
			std::vector<jlong> ops((size_t)length);
			jenv->GetLongArrayRegion(opsArray, 0, length, ops.data());
			if (!jenv->ExceptionCheck()) {
				built = buildExpressionBatch(builder, ops.data(), length);
			}
		}
	)
	return built;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLClass(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject classObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getClass(jniHandler->getString(jenv,string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			//LOG(INFO,"::Konclude::Main",logTr("Building class '%1' at address '%2'.").arg(jniHandler->getString(jenv,string)).arg((long)expression),0);
			classObj = jenv->NewGlobalRef(classObj);
			mappingHash->inserObjectMapping(expression,classObj);
		}		
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectProperty(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject propertyObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectProberty(jniHandler->getString(jenv,string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			propertyObj = jenv->NewGlobalRef(propertyObj);
			mappingHash->inserObjectMapping(expression,propertyObj);
		}
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLNamedIndividual(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject namedIndi) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getNamedIndividual(jniHandler->getString(jenv,string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			namedIndi = jenv->NewGlobalRef(namedIndi);
			mappingHash->inserObjectMapping(expression,namedIndi);
		}
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLAnonymousIndividual(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject namedIndi) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getAnonymousIndividual("",jniHandler->getString(jenv,string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			namedIndi = jenv->NewGlobalRef(namedIndi);
			mappingHash->inserObjectMapping(expression,namedIndi);
		}
	)
	return (jlong)expression;
}










JNIEXPORT void JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_initArumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		builder->initArgumentExpressionList();		
	)
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_closeArumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		builder->closeArgumentExpressionList();		
	)
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_addArgument(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong argument) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		builder->addArgumentExpression((CBuildExpression*)argument);		
	)
}

























JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectIntersectionFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getObjectIntersectionOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectUnionFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getObjectUnionOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectComplement(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong compExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectComplementOf((CBuildExpression*)compExp);
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectSomeValuesFrom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectSomeValuesFrom((CBuildExpression*)propExp,(CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectAllValuesFrom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectAllValuesFrom((CBuildExpression*)propExp,(CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectInverseOf(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong invObjProp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getInverseObjectPropertyOf((CBuildExpression*)invObjProp);
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectOneOfFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getObjectOneOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectHasSelf(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectHasSelf((CBuildExpression*)propertyExp);
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLExactCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectExactlyCardinality((CBuildExpression*)propertyExp,(CBuildExpression*)classExp,(int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLMaxCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectMaxCardinality((CBuildExpression*)propertyExp,(CBuildExpression*)classExp,(int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLMinCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectMinCardinality((CBuildExpression*)propertyExp,(CBuildExpression*)classExp,(int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectHasValue(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectHasValue((CBuildExpression*)propExp,(CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}














JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSubClassOfAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong subClassExp, jlong superClassExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getSubClassOf((CBuildExpression*)subClassExp,(CBuildExpression*)superClassExp);
		//LOG(INFO,"::Konclude::Main",logTr("Building and adding OWLSubClassAxiom with '%1' and '%2'.").arg((long)subClassExp).arg((long)superClassExp),0);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLEquivalentClassesFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getEquivalentClasses(*expList);
		delete expList;
		if (expression) {
			//LOG(INFO,"::Konclude::Main",logTr("Building and adding OWLEquivalentClassesAxiom."),0);
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSubObjectPropertyOfAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong subPropertyExp, jlong superPropertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getSubObjectPropertyOf((CBuildExpression*)subPropertyExp,(CBuildExpression*)superPropertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLEquivalentObjectPropertiesFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getEquivalentObjectProperties(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSubPropertyChainOfAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong superProperty) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		CEXPRESSIONLIST<CObjectPropertyTermExpression*>* propExpList = (CEXPRESSIONLIST<CObjectPropertyTermExpression*>*)expList;
		expression = builder->getSubObjectPropertyOf(*propExpList,(CObjectPropertyTermExpression*)superProperty);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}








JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLInverseFunctionalObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getInverseFunctionalObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLInverseObjectPropertiesAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong invPropertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getInverseObjectProperties((CBuildExpression*)propertyExp,(CBuildExpression*)invPropertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}






JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSameIndividualAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getSameIndividual(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLIrreflexiveObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getIrreflexiveObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLTransitiveObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getTransetiveObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLClassAssertionAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong classExp, jlong indiExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getClassAssertion((CBuildExpression*)classExp,(CBuildExpression*)indiExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSymmetricObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getSymmetricObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDeclarationAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong entityExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getDeclaration((CBuildExpression*)entityExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDisjointUnionAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong classExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		CEXPRESSIONLIST<CClassTermExpression*>* classExpList = (CEXPRESSIONLIST<CClassTermExpression*>*)expList;
		expression = builder->getDisjointUnion((CClassExpression*)classExp,*classExpList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLFunctionalObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getFunctionalObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLNegativeObjectPropertyAssertionAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong indi1Exp, jlong indi2Exp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getNegativeObjectPropertyAssertion((CBuildExpression*)propExp,(CBuildExpression*)indi1Exp,(CBuildExpression*)indi2Exp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectPropertyAssertionAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong indi1Exp, jlong indi2Exp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectPropertyAssertion((CBuildExpression*)propExp,(CBuildExpression*)indi1Exp,(CBuildExpression*)indi2Exp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectPropertyRangeAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong classExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectPropertyRangeExpression((CBuildExpression*)propExp,(CBuildExpression*)classExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDisjointObjectPropertiesAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDisjointObjectProperties(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDifferentIndividualsAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDifferentIndividuals(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLObjectPropertyDomainAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong classExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getObjectPropertyDomainExpression((CBuildExpression*)propExp,(CBuildExpression*)classExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDisjointClassesAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDisjointClasses(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLReflexiveObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getReflexiveObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLAsymmetricObjectPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv,builderObj,bridgeObj,
		expression = builder->getAsymmetricObjectProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}










JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLLiteral(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong datatypeExp, jstring string, jobject literalObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CDataLexicalValueExpression* lexExpression = builder->getDataLexicalValue(jniHandler->getString(jenv, string));
		expression = builder->getDataLiteral(lexExpression, (CDatatypeExpression*)datatypeExp);
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(lexExpression)) {
			literalObj = jenv->NewGlobalRef(literalObj);
			mappingHash->inserObjectMapping(lexExpression, literalObj);
		}
	)
	return (jlong)expression;
}






JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLFacetRestriction(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring facetString, jobject facetObject, jlong literal) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CDataFacetExpression* facetExpression = builder->getDataFacet(jniHandler->getString(jenv, facetString));
		expression = builder->getDataFacetRestriction((CBuildExpression*)literal,facetExpression);
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(facetExpression)) {
			facetObject = jenv->NewGlobalRef(facetObject);
			mappingHash->inserObjectMapping(facetExpression, facetObject);
		}
	)
	return (jlong)expression;
}






JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataSomeValuesFrom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataSomeValuesFrom((CBuildExpression*)propExp, (CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataAllValuesFrom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataAllValuesFrom((CBuildExpression*)propExp, (CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataHasValue(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong fillerExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataHasValue((CBuildExpression*)propExp, (CBuildExpression*)fillerExp);
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataMinCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataMinCardinality((CBuildExpression*)propertyExp, (CBuildExpression*)classExp, (int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataMaxCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataMaxCardinality((CBuildExpression*)propertyExp, (CBuildExpression*)classExp, (int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataExactCardinality(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp, jlong classExp, jlong cardinality) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataExactCardinality((CBuildExpression*)propertyExp, (CBuildExpression*)classExp, (int)cardinality);
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataProperty(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject propertyObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataProberty(jniHandler->getString(jenv, string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			propertyObj = jenv->NewGlobalRef(propertyObj);
			mappingHash->inserObjectMapping(expression, propertyObj);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataPropertyDomainAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong classExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataPropertyDomainExpression((CBuildExpression*)propExp, (CBuildExpression*)classExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataPropertyRangeAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong dataExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataPropertyRangeExpression((CBuildExpression*)propExp, (CBuildExpression*)dataExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLNegativeDataPropertyAssertionAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong indiExp, jlong dataExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getNegativeDataPropertyAssertion((CBuildExpression*)propExp, (CBuildExpression*)indiExp, (CBuildExpression*)dataExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataPropertyAssertionAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propExp, jlong indiExp, jlong dataExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataPropertyAssertion((CBuildExpression*)propExp, (CBuildExpression*)indiExp, (CBuildExpression*)dataExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDisjointDataPropertiesAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDisjointDataProperties(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLFunctionalDataPropertyAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong propertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getFunctionalDataProperty((CBuildExpression*)propertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}


JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLEquivalentDataPropertiesAxiomFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getEquivalentDataProperties(*expList);
		delete expList;
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDatatype(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jstring string, jobject datatypeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDatatype(jniHandler->getString(jenv, string));
		CJNIEntityMappingHash* mappingHash = builder->getOntologyRevisionData()->getEntityMappingHash();
		if (!mappingHash->getMappedObject(expression)) {
			datatypeObj = jenv->NewGlobalRef(datatypeObj);
			mappingHash->inserObjectMapping(expression, datatypeObj);
		}
	)
	return (jlong)expression;
}







JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataOneOfFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDataOneOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataComplementOf(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong compExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getDataComplementOf((CBuildExpression*)compExp);
	)
	return (jlong)expression;
}






JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataIntersectionOfFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDataIntersectionOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}



JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataUnionOfFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CBuildExpression*>* expList = builder->popArgumentExpressionList();
		expression = builder->getDataUnionOf(*expList);
		delete expList;
	)
	return (jlong)expression;
}




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDatatypeRestrictionFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong datatype) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* expList = (CEXPRESSIONLIST<CDataFacetRestrictionExpression*>*)builder->popArgumentExpressionList();
		expression = builder->getDatatypeRestriction((CDatatypeExpression*)datatype,*expList);
		delete expList;
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLSubDataPropertyOfAxiom(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong subPropertyExp, jlong superPropertyExp) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		expression = builder->getSubDataPropertyOf((CBuildExpression*)subPropertyExp, (CBuildExpression*)superPropertyExp);
		if (expression) {
			builder->tellOntologyAxiom((CAxiomExpression*)expression);
		}
	)
	return (jlong)expression;
}



#endif
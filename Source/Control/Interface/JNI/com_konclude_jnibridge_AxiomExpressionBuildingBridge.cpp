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




JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDataUnionOfFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong datatype) {
	CBuildExpression* expression = nullptr;
	JNIAXIOMEXPRESSIONBUILDINGBRDIGELOADER(jenv, builderObj, bridgeObj,
		CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* expList = (CEXPRESSIONLIST<CDataFacetRestrictionExpression*>*)builder->popArgumentExpressionList();
		expression = builder->getDatatypeRestriction((CDatatypeExpression*)datatype,*expList);
		delete expList;
	)
	return (jlong)expression;
}





JNIEXPORT jlong JNICALL Java_com_konclude_jnibridge_AxiomExpressionBuildingBridge_buildOWLDatatypeRestrictionFromArgumentList(JNIEnv * jenv, jobject builderObj, jobject bridgeObj, jlong subPropertyExp, jlong superPropertyExp) {
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
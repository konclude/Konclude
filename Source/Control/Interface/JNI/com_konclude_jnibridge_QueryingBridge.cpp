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
#include "Control/Interface/JNI/com_konclude_jnibridge_QueryingBridge.h"
#include "Control/Interface/JNI/CJNIInstanceManager.h"
#include "Control/Interface/JNI/CJNIEntityMappingHash.h"
#include "Control/Interface/JNI/CJNIHandler.h"
#include "Control/Interface/JNI/CJNIQueryProcessor.h"
#include "Control/Interface/JNI/CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback.h"
#include "Control/Interface/JNI/CJNIEntityExpressionMappedObjectSetVisitingCallback.h"


using namespace Konclude::Control::Interface::JNI;
using namespace Konclude::Parser::Expression;



JNIEXPORT jboolean JNICALL Java_com_konclude_jnibridge_QueryingBridge_checkIsOWLOntologyConsistent(JNIEnv* jenv, jobject processorObj, jobject bridgeObj) {
	bool consistent = false;
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIQueryProcessor* processor = jniInstanceManager->getJNIHandler()->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			consistent = processor->checkIsOntologyConsistent();
		}
	}
	return (jboolean)consistent;
}


JNIEXPORT jdoubleArray JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLReasoningProgress(JNIEnv* jenv, jobject processorObj, jobject bridgeObj) {
	// called from another thread than the queries while one of them runs, so nothing of the
	// handler's thread bound state is used, only the field ids, which are valid on any thread
	QVector<double> values(CJNIQueryProcessor::PROGRESS_VALUE_COUNT,0.);
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIQueryProcessor* processor = jniInstanceManager->getJNIHandler()->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			processor->queryReasoningProgress(values);
		}
	}
	jdoubleArray array = jenv->NewDoubleArray(values.size());
	if (array) {
		jenv->SetDoubleArrayRegion(array,0,values.size(),values.constData());
	}
	return array;
}



JNIEXPORT jintArray JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLClassHierarchy(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jobject jvisitCallback) {
	// null when there is no completed classification, otherwise the edges between the nodes that the
	// callback received, as pairs of positions, parent first
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (!jniInstanceManager) {
		return nullptr;
	}
	CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
	CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
	if (!processor) {
		return nullptr;
	}
	CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback visitCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
			jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
	QVector<qint32> edges;
	if (!processor->queryOntologyClassHierarchy(&visitCallback,edges)) {
		return nullptr;
	}
	jintArray array = jenv->NewIntArray(edges.size());
	if (array) {
		jenv->SetIntArrayRegion(array,0,edges.size(),(const jint*)edges.constData());
	}
	return array;
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLSubClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject clsObj, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
					jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologySubClasses(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLSuperClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject clsObj, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
					jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologySuperClasses(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLEquivalentClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject clsObj, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			//LOG(INFO,"::Konclude::Main",logTr("Processing equivalent classes query for '%1'.").arg(jniHandler->getString(jenv,jstr)),0);
			CJNIEntityExpressionMappedObjectSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,jniHandler->getObjectSetCallbackListenerJNIData()->getVisitObjectMethodID());
			if (!processor->queryOntologyEquivalentClasses(jniHandler->getString(jenv,jstr),visitCallback)) {
				LOG(ERROR,"::Konclude::Main",logTr("Equivalent classes query for '%1' failed.").arg(jniHandler->getString(jenv,jstr)),0);
			}
			delete visitCallback;
		}
	}
}







JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLSubObjectProperties(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject propObj, jobject jvisitCallback, jboolean direct, jboolean inverses) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologySubObjectProperties(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLSuperObjectProperties(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject propObj, jobject jvisitCallback, jboolean direct, jboolean inverses) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologySuperObjectProperties(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLEquivalentObjectProperties(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject propObj, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			//LOG(INFO,"::Konclude::Main",logTr("Processing equivalent object properties query for '%1'.").arg(jniHandler->getString(jenv,jstr)),0);
			CJNIEntityExpressionMappedObjectSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,jniHandler->getObjectSetCallbackListenerJNIData()->getVisitObjectMethodID());
			if (!processor->queryOntologyEquivalentObjectProperties(jniHandler->getString(jenv,jstr),visitCallback)) {
				LOG(ERROR,"::Konclude::Main",logTr("Equivalent object properties query for '%1' failed.").arg(jniHandler->getString(jenv,jstr)),0);
			}
			delete visitCallback;
		}
	}
}





JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLInstances(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject clsObj, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologyInstances(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLTypes(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject indiObj, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologyTypes(jniHandler->getString(jenv,jstr),(bool)direct,visitCallback);
			delete visitCallback;
		}
	}
}




JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLSameIndividuals(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring jstr, jobject indiObj, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,jniHandler->getObjectSetCallbackListenerJNIData()->getVisitObjectMethodID());
			processor->queryOntologySameIndividuals(jniHandler->getString(jenv,jstr),visitCallback);
			delete visitCallback;
		}
	}
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLObjectPropertySources(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring indistr, jobject indiObj, jstring propstr, jobject propObj, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologyObjectPropertySources(jniHandler->getString(jenv,indistr),jniHandler->getString(jenv,propstr),visitCallback);
			delete visitCallback;
		}
	}
}



JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLObjectPropertyTargets(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jstring indistr, jobject indiObj, jstring propstr, jobject propObj, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	if (jniInstanceManager) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIQueryProcessor* processor = jniHandler->getQueryingBridgeNativeData(jenv,processorObj);
		if (processor) {
			CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
			processor->queryOntologyObjectPropertyTargets(jniHandler->getString(jenv,indistr),jniHandler->getString(jenv,propstr),visitCallback);
			delete visitCallback;
		}
	}
}





// The queries about a class expression. The expression is built beforehand through the
// AxiomExpressionBuildingBridge that initOWLClassExpressionBuilder binds to this querying bridge,
// and is handed over by its address, like the arguments of the building bridge. A query that is
// not answered is reported by a KoncludeReasonerException, since an empty answer is a valid one.

// the processor of the querying bridge, or null with a KoncludeReasonerException scheduled
static CJNIQueryProcessor* getClassExpressionQueryProcessor(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, CJNIInstanceManager** jniInstanceManagerOut) {
	CJNIInstanceManager* jniInstanceManager = CJNIHandler::getJNIInstanceManager(jenv,bridgeObj);
	CJNIQueryProcessor* processor = nullptr;
	if (jniInstanceManager) {
		processor = jniInstanceManager->getJNIHandler()->getQueryingBridgeNativeData(jenv,processorObj);
	}
	if (!processor) {
		CJNIHandler::throwKoncludeException(jenv,"The querying bridge has not been initialised, so it cannot be asked about a class expression.");
	}
	*jniInstanceManagerOut = jniInstanceManager;
	return processor;
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_initOWLClassExpressionBuilder(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jobject builderObj) {
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		CJNIAxiomExpressionVisitingLoader* builder = processor->getClassExpressionBuilder();
		if (builder) {
			jniInstanceManager->getJNIHandler()->setAxiomExpressionBuildingBridgeNativeData(jenv,builderObj,builder);
		} else {
			CJNIHandler::throwKoncludeException(jenv,"The builder for the class expressions of the queries could not be created, no ontology revision is available for it.");
		}
	}
}


JNIEXPORT jboolean JNICALL Java_com_konclude_jnibridge_QueryingBridge_checkIsOWLClassExpressionSatisfiable(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jlong classExpression) {
	bool satisfiable = false;
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		QString errorMessage;
		if (!processor->checkIsClassExpressionSatisfiable((CBuildExpression*)classExpression,satisfiable,errorMessage)) {
			CJNIHandler::throwKoncludeException(jenv,errorMessage);
		}
	}
	return (jboolean)satisfiable;
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLClassExpressionSubClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jlong classExpression, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
		QString errorMessage;
		bool answered = processor->queryClassExpressionSubClasses((CBuildExpression*)classExpression,(bool)direct,visitCallback,errorMessage);
		delete visitCallback;
		if (!answered) {
			CJNIHandler::throwKoncludeException(jenv,errorMessage);
		}
	}
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLClassExpressionSuperClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jlong classExpression, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
		QString errorMessage;
		bool answered = processor->queryClassExpressionSuperClasses((CBuildExpression*)classExpression,(bool)direct,visitCallback,errorMessage);
		delete visitCallback;
		if (!answered) {
			CJNIHandler::throwKoncludeException(jenv,errorMessage);
		}
	}
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLClassExpressionEquivalentClasses(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jlong classExpression, jobject jvisitCallback) {
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIEntityExpressionMappedObjectSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,jniHandler->getObjectSetCallbackListenerJNIData()->getVisitObjectMethodID());
		QString errorMessage;
		bool answered = processor->queryClassExpressionEquivalentClasses((CBuildExpression*)classExpression,visitCallback,errorMessage);
		delete visitCallback;
		if (!answered) {
			CJNIHandler::throwKoncludeException(jenv,errorMessage);
		}
	}
}


JNIEXPORT void JNICALL Java_com_konclude_jnibridge_QueryingBridge_queryOWLClassExpressionInstances(JNIEnv* jenv, jobject processorObj, jobject bridgeObj, jlong classExpression, jobject jvisitCallback, jboolean direct) {
	CJNIInstanceManager* jniInstanceManager = nullptr;
	CJNIQueryProcessor* processor = getClassExpressionQueryProcessor(jenv,processorObj,bridgeObj,&jniInstanceManager);
	if (processor) {
		CJNIHandler* jniHandler = jniInstanceManager->getJNIHandler();
		CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback* visitCallback = new CJNIEntityExpressionMappedObjectSetOfSetVisitingCallback(jniInstanceManager->getOntologyRevisionData(),jenv,jvisitCallback,
				jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getVisitObjectMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getStartVisitObjectSetMethodID(),jniHandler->getSetOfObjectSetCallbackListenerJNIData()->getEndVisitObjectSetMethodID());
		QString errorMessage;
		bool answered = processor->queryClassExpressionInstances((CBuildExpression*)classExpression,(bool)direct,visitCallback,errorMessage);
		delete visitCallback;
		if (!answered) {
			CJNIHandler::throwKoncludeException(jenv,errorMessage);
		}
	}
}



#endif
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



#endif
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

#include "CJNIHandler.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE

				jfieldID CJNIHandler::mBaseBridgeFieldID = nullptr;

				CJNIHandler::CJNIHandler(JNIEnv* jenv) : mJenv(jenv),
						mAxiomExpressionBuildingBridgeData(jenv,"[Lcom/konclude/jnibridge/AxiomExpressionBuildingBridge;"),
						mQueryingBridgeData(jenv,"[Lcom/konclude/jnibridge/QueryingBridge;"),
						mObjectSetCallbackListener(jenv,"[Lcom/konclude/jnibridge/ObjectSetCallbackListener;"),
						mSetOfObjectSetCallbackListener(jenv,"[Lcom/konclude/jnibridge/SetOfObjectSetCallbackListener;") {
				}


				CJNIHandler::~CJNIHandler() {
				}


				CJNIInstanceManager* CJNIHandler::getJNIInstanceManager(JNIEnv* jenv, jobject obj) {
					CJNIInstanceManager* instanceManger = (CJNIInstanceManager*)jenv->GetLongField(obj,getJNIInstanceManagerBaseFieldID(jenv));
					return instanceManger;
				}

				bool CJNIHandler::setJNIInstanceManager(JNIEnv* jenv, jobject obj, CJNIInstanceManager* instanceManger) {
					jfieldID baseFieldID = getJNIInstanceManagerBaseFieldID(jenv);
					if (baseFieldID) {
						jenv->SetLongField(obj,baseFieldID,(jlong)instanceManger);
						return true;
					}
					return false;
				}


				jfieldID CJNIHandler::getJNIInstanceManagerBaseFieldID(JNIEnv* jenv) {
					if (!mBaseBridgeFieldID) {
						jclass baseClass = jenv->FindClass("Lcom/konclude/jnibridge/KoncludeReasonerBridge;");
						if (!baseClass) {
							throwGeneralException(jenv,"Cannot find Java class com.konclude.jnibridge.KoncludeReasonerBridge to initialize JNI Konclude Reasoner Bridge");
							return 0;
						}

						mBaseBridgeFieldID = jenv->GetFieldID(baseClass,"mNativeDataPointer","J");
						if (!mBaseBridgeFieldID) {
							throwGeneralException(jenv,"Cannot find field mNativeDataPointer for class com.konclude.jnibridge.KoncludeReasonerBridge to initialize JNI Konclude Reasoner Bridge");
							return 0;
						}
					}
					return mBaseBridgeFieldID;
				}

				void CJNIHandler::throwException(JNIEnv* jenv, const char* className, const char* reason) {
					jclass cls = jenv->FindClass(className);
					jenv->ThrowNew(cls,reason);
				}


				void CJNIHandler::throwException(JNIEnv* jenv, const char* className) {
					jclass cls = jenv->FindClass(className);
					jmethodID ctorID = jenv->GetMethodID(cls,"<init>","()V");
					jobject obj = jenv->NewObject(cls,ctorID);
					jenv->Throw((jthrowable)obj);
				}

				void CJNIHandler::throwGeneralException(JNIEnv* jenv, const char* reason) {
					throwException(jenv,"Ljava/lang/Exception;",reason);
				}


				void CJNIHandler::throwKoncludeException(JNIEnv* jenv, const char* reason) {
					throwException(jenv,"Lcom/konclude/jnibridge/KoncludeReasonerException;",reason);
				}


				void CJNIHandler::throwKoncludeException(JNIEnv* jenv, const QString& reason) {
					throwException(jenv,"Lcom/konclude/jnibridge/KoncludeReasonerException;",reason.toLocal8Bit().constData());
				}


				CJNIHandlingClassData* CJNIHandler::getAxiomExpressionBuildingBridgeJNIData() {
					return &mAxiomExpressionBuildingBridgeData;
				}


				CJNIHandlingClassData* CJNIHandler::getQueryingBridgeJNIData() {
					return &mQueryingBridgeData;
				}


				CJNIHandlingClassData* CJNIHandler::getObjectSetCallbackListenerJNIData() {
					return &mObjectSetCallbackListener;
				}

				CJNIHandlingClassData* CJNIHandler::getSetOfObjectSetCallbackListenerJNIData() {
					return &mSetOfObjectSetCallbackListener;
				}

				QString CJNIHandler::getString(JNIEnv* jenv, jstring string) {
					const char* stringChrs = jenv->GetStringUTFChars(string,0);
					return QString::fromUtf8(stringChrs);					
				}


				CJNIHandler* CJNIHandler::setAxiomExpressionBuildingBridgeNativeData(JNIEnv* jenv, jobject obj, CJNIAxiomExpressionVisitingLoader* loader) {
					jenv->SetLongField(obj,getAxiomExpressionBuildingBridgeJNIData()->getNativeDataFieldID(),(jlong)loader);
					return this;
				}

				CJNIAxiomExpressionVisitingLoader* CJNIHandler::getAxiomExpressionBuildingBridgeNativeData(JNIEnv* jenv, jobject obj) {
					return (CJNIAxiomExpressionVisitingLoader*)jenv->GetLongField(obj,getAxiomExpressionBuildingBridgeJNIData()->getNativeDataFieldID());
				}



				CJNIHandler* CJNIHandler::setQueryingBridgeNativeData(JNIEnv* jenv, jobject obj, CJNIQueryProcessor* processor) {
					jenv->SetLongField(obj,getQueryingBridgeJNIData()->getNativeDataFieldID(),(jlong)processor);
					return this;
				}

				CJNIQueryProcessor* CJNIHandler::getQueryingBridgeNativeData(JNIEnv* jenv, jobject obj) {
					return (CJNIQueryProcessor*)jenv->GetLongField(obj,getQueryingBridgeJNIData()->getNativeDataFieldID());
				}



#endif



			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLER_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLER_H

// Libraries includes
#ifdef KONCLUDE_COMPILE_JNI_INTERFACE
#include <jni.h>
#endif


// Namespace includes
#include "JNISettings.h"
#include "CJNIHandlingClassData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIHandler {

					// public methods
					public:
#ifdef KONCLUDE_COMPILE_JNI_INTERFACE
						//! Constructor
						CJNIHandler(JNIEnv* jenv);

						//! Destructor
						virtual ~CJNIHandler();





						static bool setJNIInstanceManager(JNIEnv* jenv, jobject obj, CJNIInstanceManager* instanceManger);
						static CJNIInstanceManager* getJNIInstanceManager(JNIEnv* jenv, jobject obj);


						static jfieldID getJNIInstanceManagerBaseFieldID(JNIEnv* jenv);
						

						static void throwException(JNIEnv* jenv, const char* className, const char* reason);
						static void throwException(JNIEnv* jenv, const char* className);
						static void throwGeneralException(JNIEnv* jenv, const char* reason);
						static void throwKoncludeException(JNIEnv* jenv, const char* reason);
						static void throwKoncludeException(JNIEnv* jenv, const QString& reason);


						QString getString(JNIEnv* jenv, jstring string);


						CJNIHandlingClassData* getAxiomExpressionBuildingBridgeJNIData();
						CJNIHandlingClassData* getQueryingBridgeJNIData();
						CJNIHandlingClassData* getObjectSetCallbackListenerJNIData();
						CJNIHandlingClassData* getSetOfObjectSetCallbackListenerJNIData();


						CJNIHandler* setAxiomExpressionBuildingBridgeNativeData(JNIEnv* jenv, jobject obj, CJNIAxiomExpressionVisitingLoader* loader);
						CJNIAxiomExpressionVisitingLoader* getAxiomExpressionBuildingBridgeNativeData(JNIEnv* jenv, jobject obj);


						CJNIHandler* setQueryingBridgeNativeData(JNIEnv* jenv, jobject obj, CJNIQueryProcessor* processor);
						CJNIQueryProcessor* getQueryingBridgeNativeData(JNIEnv* jenv, jobject obj);


					// protected methods
					protected:

					// protected variables
					protected:

						static jfieldID mBaseBridgeFieldID;

						JNIEnv* mJenv;

						CJNIHandlingClassData mAxiomExpressionBuildingBridgeData;
						CJNIHandlingClassData mQueryingBridgeData;

						CJNIHandlingClassData mObjectSetCallbackListener;
						CJNIHandlingClassData mSetOfObjectSetCallbackListener;


					// private methods
					private:

					// private variables
					private:
#endif

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLER_H

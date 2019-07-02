/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLINGCLASSDATA_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLINGCLASSDATA_H

// Libraries includes
#ifdef KONCLUDE_COMPILE_JNI_INTERFACE
#include <jni.h>
#endif


// Namespace includes
#include "JNISettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIHandlingClassData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIHandlingClassData {

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE

					// public methods
					public:
						//! Constructor
						CJNIHandlingClassData(JNIEnv* jenv, const char* arrayClassName);

						//! Destructor
						virtual ~CJNIHandlingClassData();

	
						jclass getClassID();
						jclass getArrayClassID();

						jfieldID getNativeDataFieldID();

						jmethodID getConstrutorMethodID();
						jmethodID getVisitObjectMethodID();
						jmethodID getStartVisitObjectSetMethodID();
						jmethodID getEndVisitObjectSetMethodID();


					// protected methods
					protected:

					// protected variables
					protected:

						jclass mClassID;
						jclass mArrayClassID;
						jfieldID mNativeDataFieldID;
						jmethodID mConstructorMethodID;
						jmethodID mVisitingMethodID;
						jmethodID mStartVisitObjectSetMethodID;
						jmethodID mEndVisitObjectSetMethodID;

						const char* mArrayClassName;
						const char* mClassName;

						JNIEnv* mJenv;
						

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

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIHANDLINGCLASSDATA_H

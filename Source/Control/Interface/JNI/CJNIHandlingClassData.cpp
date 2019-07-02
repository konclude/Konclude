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

#include "CJNIHandlingClassData.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE

				CJNIHandlingClassData::CJNIHandlingClassData(JNIEnv* jenv, const char* arrayClassName) {
					mArrayClassName = arrayClassName;
					mClassName = arrayClassName+1;

					mJenv = jenv;
					mClassID = nullptr;
					mArrayClassID = nullptr;
					mNativeDataFieldID = nullptr;
					mConstructorMethodID = nullptr;
					mVisitingMethodID = nullptr;
					mEndVisitObjectSetMethodID = nullptr;
					mStartVisitObjectSetMethodID = nullptr;
				}

				CJNIHandlingClassData::~CJNIHandlingClassData() {
					if (mClassID) {
						mJenv->DeleteGlobalRef(mClassID);
					}
					if (mArrayClassID) {
						mJenv->DeleteGlobalRef(mArrayClassID);
					}
				}


				jclass CJNIHandlingClassData::getClassID() {
					if (!mClassID) {
						mClassID = mJenv->FindClass(mClassName);
						mClassID = (jclass)mJenv->NewGlobalRef(mClassID);
					}
					return mClassID;
				}

				jclass CJNIHandlingClassData::getArrayClassID() {
					if (!mArrayClassID) {
						mArrayClassID = mJenv->FindClass(mArrayClassName);
						mArrayClassID = (jclass)mJenv->NewGlobalRef(mArrayClassID);
					}
					return mArrayClassID;
				}

				jfieldID CJNIHandlingClassData::getNativeDataFieldID() {
					if (!mNativeDataFieldID) {
						mNativeDataFieldID = mJenv->GetFieldID(getClassID(),"mNativeDataPointer","J");
					}
					return mNativeDataFieldID;
				}

				jmethodID CJNIHandlingClassData::getConstrutorMethodID() {
					if (!mConstructorMethodID) {
						mConstructorMethodID = mJenv->GetMethodID(getClassID(),"<init>","()V");
					}
					return mConstructorMethodID;
				}

				jmethodID CJNIHandlingClassData::getVisitObjectMethodID() {
					if (!mVisitingMethodID) {
						mVisitingMethodID = mJenv->GetMethodID(getClassID(),"visitObject","(Ljava/lang/Object;)V");
					}
					return mVisitingMethodID;
				}

				jmethodID CJNIHandlingClassData::getEndVisitObjectSetMethodID() {
					if (!mEndVisitObjectSetMethodID) {
						mEndVisitObjectSetMethodID = mJenv->GetMethodID(getClassID(),"endObjectSet","()V");
					}
					return mEndVisitObjectSetMethodID;
				}

				jmethodID CJNIHandlingClassData::getStartVisitObjectSetMethodID() {
					if (!mStartVisitObjectSetMethodID) {
						mStartVisitObjectSetMethodID = mJenv->GetMethodID(getClassID(),"startObjectSet","()V");
					}
					return mStartVisitObjectSetMethodID;
				}
#endif


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

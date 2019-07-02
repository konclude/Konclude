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

#include "CJNIEntityExpressionMappedObjectSetVisitingCallback.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


#ifdef KONCLUDE_COMPILE_JNI_INTERFACE


				CJNIEntityExpressionMappedObjectSetVisitingCallback::CJNIEntityExpressionMappedObjectSetVisitingCallback(CJNIOntologyRevisionData* ontRevData, JNIEnv* jenv, jobject jvisitor, jmethodID jvisitObjectMethod) {
					mJenv = jenv;
					mJVisitor = jvisitor;
					mJVisitObjectMethod = jvisitObjectMethod;
					mOntRevData = ontRevData;
					mEntityMappingHash = ontRevData->getEntityMappingHash();
				}

				CJNIEntityExpressionMappedObjectSetVisitingCallback::~CJNIEntityExpressionMappedObjectSetVisitingCallback() {					
				}

				
				
				CEntityExpressionSetResultVisitingCallback* CJNIEntityExpressionMappedObjectSetVisitingCallback::visitEntityExpression(CBuildExpression* entityExp, CConcreteOntology* ontology) {
					jobject obj = mEntityMappingHash->getMappedObject(entityExp);
					if (obj) {
						//LOG(INFO,"::Konclude::JNI::VisitingCallback",logTr("Visiting '%1'.").arg((long)obj),0);
						mJenv->CallVoidMethod(mJVisitor,mJVisitObjectMethod,obj);
					}
					return this;
				}

#endif


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

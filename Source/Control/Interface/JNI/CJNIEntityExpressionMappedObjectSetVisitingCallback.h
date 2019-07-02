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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIENTITYEXPRESSIONMAPPEDOBJECTSETVISITINGCALLBACK_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIENTITYEXPRESSIONMAPPEDOBJECTSETVISITINGCALLBACK_H

// Libraries includes
#ifdef KONCLUDE_COMPILE_JNI_INTERFACE
#include <jni.h>
#endif

// Namespace includes
#include "JNISettings.h"
#include "CJNIOntologyRevisionData.h"
#include "CJNICommandProcessor.h"
#include "CJNIInstanceManager.h"

// Other includes
#include "Reasoner/Query/CEntityExpressionSetResultVisitingCallback.h"

#include "Parser/Expressions/CBuildExpression.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Query;
	using namespace Parser::Expression;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIAxiomLoader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIEntityExpressionMappedObjectSetVisitingCallback : public CEntityExpressionSetResultVisitingCallback {

#ifdef KONCLUDE_COMPILE_JNI_INTERFACE

					// public methods
					public:
						//! Constructor
						CJNIEntityExpressionMappedObjectSetVisitingCallback(CJNIOntologyRevisionData* ontRevData, JNIEnv* jenv, jobject jvisitor, jmethodID jvisitObjectMethod);

						//! Destructor
						virtual ~CJNIEntityExpressionMappedObjectSetVisitingCallback();


						virtual CEntityExpressionSetResultVisitingCallback* visitEntityExpression(CBuildExpression* entityExp, CConcreteOntology* ontology);



					// protected methods
					protected:

					// protected variables
					protected:
						CJNIOntologyRevisionData* mOntRevData;
						CJNIEntityMappingHash* mEntityMappingHash;
						JNIEnv* mJenv;
						jobject mJVisitor;
						jmethodID mJVisitObjectMethod;



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

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIENTITYEXPRESSIONMAPPEDOBJECTSETVISITINGCALLBACK_H

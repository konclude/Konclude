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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H

// Libraries includes
#include <QStack>

// Namespace includes
#include "JNISettings.h"
#include "CJNIOntologyRevisionData.h"
#include "CJNICommandProcessor.h"
#include "CJNIInstanceManager.h"

// Other includes
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

#include "Parser/Expressions/CBuildExpression.h"

#include "Reasoner/Query/CSetOfEntityExpressionSetResultVisitingCallback.h"
#include "Reasoner/Query/CEquivalentClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CEquivalentPropertiesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperPropertiesResultVisitCallbackQuery.h"

#include "Reasoner/Query/CInstancesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSameIndividualsResultVisitCallbackQuery.h"
#include "Reasoner/Query/CTypesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CObjectPropertySourcesTargetsResultVisitCallbackQuery.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Generator;
	using namespace Reasoner::Query;
	using namespace Parser::Expression;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIQueryProcessor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIQueryProcessor {

					// public methods
					public:
						//! Constructor
						CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData);

						//! Destructor
						virtual ~CJNIQueryProcessor();


						bool checkIsOntologyConsistent();


						bool queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback);


						bool queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback);



						bool queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);


					// protected methods
					protected:

					// protected variables
					protected:
						CJNIInstanceManager* mJNIInstanceManager;
						CJNICommandProcessor* mJNICommandProcessor;
						CJNIOntologyRevisionData* mOntRevData;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H

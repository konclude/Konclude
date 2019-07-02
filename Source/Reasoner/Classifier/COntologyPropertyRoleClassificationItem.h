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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYPROPERTYROLECLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYPROPERTYROLECLASSIFICATIONITEM_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "ClassifierSettings.h"
#include "CPropertyClassificationComputationItem.h"
#include "CClassifiedCallbackDataContext.h"
#include "CClassifierStatistics.h"
#include "COntologyClassificationItem.h"

// Other includes
#include "Reasoner/Classification/CPropertyRoleClassification.h"
#include "Reasoner/Classification/CClassificationCalculationStatisticsCollection.h"
#include "Reasoner/Classification/CClassificationStatisticsCollectionStrings.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingStep.h"
#include "Reasoner/Ontology/COntologyProcessingSteps.h"
#include "Reasoner/Ontology/COntologyProcessingStepData.h"
#include "Reasoner/Ontology/CConceptNegationPair.h"

#include "Reasoner/Consistiser/CIndividualDependenceTrackingCollector.h"

#include "Reasoner/Taxonomy/CRolePropertiesHierarchy.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Concurrent/Callback/CCallbackListExecuter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Classification;
		using namespace Consistiser;

		using namespace Kernel::Task;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COntologyPropertyRoleClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyPropertyRoleClassificationItem : public CPropertyRoleClassification, public COntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					COntologyPropertyRoleClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COntologyPropertyRoleClassificationItem();

					virtual COntologyPropertyRoleClassificationItem *initClassificationItem(CConcreteOntology *ontology, CRolePropertiesHierarchy *hierarchy, bool classifyDataProperties);


					virtual CRolePropertiesHierarchy *getHierarchy();
					virtual CTBox *getTBox();

					virtual QHash<CSatisfiableCalculationJob*,CPropertyClassificationComputationItem*>* getComputationItemHash();


					CPropertyRoleClassification* getPropertyRoleClassification();

					bool isHierarchyConstructed();
					COntologyPropertyRoleClassificationItem* submitHierarchyConstructed();
					COntologyPropertyRoleClassificationItem* setHierarchyConstructionFailed();
					bool isHierarchyConstructionFailed();



					bool isDataRolesClassification();
					bool isObjectRolesClassification();
					COntologyPropertyRoleClassificationItem* setDataRolesClassification(bool classifyDataRoles);


				// protected methods
				protected:

				// protected variables
				protected:
					CRolePropertiesHierarchy *mHierarchy;

					QHash<CSatisfiableCalculationJob*, CPropertyClassificationComputationItem*> mWorkItemHash;

					bool mHierarchyConstructed;
					bool mHierarchyConstructionFailed;

					bool mClassifyDataRoles;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CONTOLOGYPROPERTYROLECLASSIFICATIONITEM_H

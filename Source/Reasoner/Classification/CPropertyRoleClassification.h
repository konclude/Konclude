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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CPROPERTYROLECLASSIFICATION_H
#define KONCLUDE_REASONER_CLASSIFICATION_CPROPERTYROLECLASSIFICATION_H

// Libraries includes


// Namespace includes
#include "ClassificationSettings.h"
#include "CClassificationStatistics.h"

// Other includes
#include "Reasoner/Taxonomy/CRolePropertiesHierarchy.h"

#include "Reasoner/Ontology/CIndividualDependenceTracking.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classification {

			/*! 
			 *
			 *		\class		CPropertyRoleClassification
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPropertyRoleClassification {
				// public methods
				public:
					//! Constructor
					CPropertyRoleClassification();

					virtual ~CPropertyRoleClassification();

					bool hasRolePropertiesHierarchy();
					CRolePropertiesHierarchy* getRolePropertiesHierarchy();
					CPropertyRoleClassification* setRolePropertiesHierarchy(CRolePropertiesHierarchy* rolePropHierarchy);

					bool hasClassificationStatistics();
					CClassificationStatistics* getClassificationStatistics();
					CPropertyRoleClassification* setClassificationStatistics(CClassificationStatistics* statColl);


					bool hasDependentIndividualsTracking();
					bool isIndividualDependentTrackingSet();
					CIndividualDependenceTracking* getDependentIndividualsTracking();
					CPropertyRoleClassification* setDependentIndividualsTracking(CIndividualDependenceTracking* indiDepTracking);

				// protected methods
				protected:

				// protected variables
				protected:
					CRolePropertiesHierarchy* mRolePropertiesHierarchy;
					CClassificationStatistics* mStatsColl;
					CIndividualDependenceTracking* mIndiDepTracking;
					bool mIndiDepTrackingSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CPROPERTYROLECLASSIFICATION_H
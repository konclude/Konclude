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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CPROPERTYCLASSIFICATIONCOMPUTATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CPROPERTYCLASSIFICATIONCOMPUTATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationComputationItem.h"

// Other includes
#include "Reasoner/Ontology/CRole.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Query;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CPropertyClassificationComputationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPropertyClassificationComputationItem : public CClassificationComputationItem {
				// public methods
				public:
					//! Constructor
					CPropertyClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CRole *satisfiableTestedRole);
					CPropertyClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CRole *subsumerTestedRole, CRole *subsumedTestedRole);

					bool isRoleSatisfiableTest();
					bool isRoleSubsumptionTest();

					CRole *getSatisfiableTestedRole();
					CRole *getSubsumerTestedRole();
					CRole *getSubsumedTestedRole();

					CSatisfiableCalculationJob* getSatisfiableCalculationJob();


				// protected methods
				protected:

				// protected variables
				protected:
					CSatisfiableCalculationJob* mSatCalcJob;
					bool mTestValid;
					bool mRoleSatisfiableTest;
					bool mRoleSubsumptionTest;

					CRole* mRole1;
					CRole* mRole2;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CPROPERTYCLASSIFICATIONCOMPUTATIONITEM_H

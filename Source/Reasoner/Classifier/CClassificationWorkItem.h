/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONWORKITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONWORKITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Query;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationWorkItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationWorkItem {
				// public methods
				public:
					//! Constructor
					CClassificationWorkItem(CSatisfiableCalculationJob* satCalcJob, CConcept *satisfiableTestedConcept);
					CClassificationWorkItem(CSatisfiableCalculationJob* satCalcJob, CConcept *subsumerTestedConcept, CConcept *subsumedTestedConcept);

					//! Destructor
					virtual ~CClassificationWorkItem();

					virtual bool isTestValid();
					virtual CClassificationWorkItem *setTestInvalid();

					virtual bool isConceptSatisfiableTest();
					virtual bool isConceptSubsumptionTest();

					virtual CConcept *getSatisfiableTestedConcept();
					virtual CConcept *getSubsumerTestedConcept();
					virtual CConcept *getSubsumedTestedConcept();

					virtual CSatisfiableCalculationJob* getSatisfiableCalculationJob();


					virtual bool testIsTestRedundant(CTaxonomy *taxonomy);

				// protected methods
				protected:

				// protected variables
				protected:
					CSatisfiableCalculationJob* mSatCalcJob;
					bool testValid;
					bool conceptSatisfiableTest;
					bool conceptSubsumptionTest;

					CConcept *con0;
					CConcept *con1;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONWORKITEM_H

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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLESUBSUMPTIONIDENTIFIERADAPTER_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLESUBSUMPTIONIDENTIFIERADAPTER_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Classifier/CConceptSubsumptionRelationObserver.h"
#include "Reasoner/Classifier/CConceptNecessaryIdentifierOccurrenceObserver.h"
#include "Reasoner/Classifier/CConceptPossibleSubsumptionsOccurrenceObserver.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Classifier;

		namespace Kernel {

			namespace Task {

				/*! 
				 *
				 *		\class		CSatisfiableSubsumptionIdentifierAdapter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableSubsumptionIdentifierAdapter {
					// public methods
					public:
						//! Constructor
						CSatisfiableSubsumptionIdentifierAdapter(CConcept *satisfiableTestingConcept = 0, CConcreteOntology *testingOntology = 0, CConceptSubsumptionRelationObserver *conceptSubsumptionObserver = 0, CConceptSubsumptionRelationObserver *multiSubsumptionObserver = 0, CConceptNecessaryIdentifierOccurrenceObserver *conceptNecIdenOccObserver = 0, CConceptPossibleSubsumptionsOccurrenceObserver *conceptPossibleSubsumptionOccObserver = 0);

						virtual CConcept *getSatisfiableTestingConcept();
						virtual CConcreteOntology *getTestingOntology();

						virtual CConceptSubsumptionRelationObserver *getConceptSubsumptionObserver();
						virtual CConceptSubsumptionRelationObserver *getMultiSubsumptionObserver();
						virtual CConceptNecessaryIdentifierOccurrenceObserver *getNecessaryIdentifierOccurrenceObserver();
						virtual CConceptPossibleSubsumptionsOccurrenceObserver *getPossibleSubsumptionsOccurrenceObserver();

					// protected methods
					protected:

					// protected variables
					protected:
						CConcept *satTestingCon;
						CConcreteOntology *testingOnto;
						CConceptSubsumptionRelationObserver *subSumObserver;
						CConceptSubsumptionRelationObserver *multiSubSumObserver;
						CConceptNecessaryIdentifierOccurrenceObserver *necIdenOccObserver;
						CConceptPossibleSubsumptionsOccurrenceObserver *possSubsumOccObserver;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLESUBSUMPTIONIDENTIFIERADAPTER_H

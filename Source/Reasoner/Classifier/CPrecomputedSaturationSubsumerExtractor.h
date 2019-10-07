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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CPRECOMPUTEDSATURATIONSUBSUMEREXTRACTOR_H
#define KONCLUDE_REASONER_CLASSIFIER_CPRECOMPUTEDSATURATIONSUBSUMEREXTRACTOR_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CConceptSubsumerObserver.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Kernel/Task/CSaturationTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"
#include "Reasoner/Kernel/Process/CReferredIndividualTrackingVector.h"

#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"
#include "Reasoner/Consistiser/CIndividualDependenceTrackingObserver.h"
#include "Reasoner/Consistiser/CIndividualDependenceTrackingMarker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistiser;
		using namespace Kernel::Task;
		using namespace Kernel::Process;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CPrecomputedSaturationSubsumerExtractor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPrecomputedSaturationSubsumerExtractor {
				// public methods
				public:
					//! Constructor
					CPrecomputedSaturationSubsumerExtractor(CConcreteOntology* ontology);

					virtual bool extractSubsumers(CConcept* concept, CConceptSubsumerObserver* subsumerObserver, bool* possibleSubsumerFlag = nullptr, CIndividualDependenceTrackingObserver* indDepTrackingObserver = nullptr, CIndividualDependenceTrackingMarker* indiDepTrackMarker = nullptr);
					
					virtual bool getConceptFlags(CConcept* concept, bool* unsatisfiableFlag, bool* insufficientFlag, bool* incompleteProcessedFlag);


					virtual cint64 getSubsumerCount(CConcept* concept);
					virtual CConcept* getDirectSubsumerCount(CConcept* concept, bool (*isDirectSubsumer)(CConcept*, CConcept*));

					virtual bool hasSubsumerConcept(CConcept* concept, CConcept* testSubsumerConcept);

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CSatisfiableCalculationTask* mSatCalcTask;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CPRECOMPUTEDSATURATIONSUBSUMEREXTRACTOR_H

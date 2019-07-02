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

#ifndef KONCLUDE_REASONER_CONSISTISER_CINCREMENTALONTOLOGYPRECOMPUTATIONITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CINCREMENTALONTOLOGYPRECOMPUTATIONITEM_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "ConsistiserSettings.h"
#include "CConsistenceObserver.h"
#include "CSaturationObserver.h"
#include "COntologyPrecomputationItem.h"
#include "CPrecomputationTestingStep.h"
#include "CSaturationConceptDataItem.h"
#include "CSaturationIndividualDataItem.h"
#include "CSaturationIndividualAnalysationObserver.h"


// Other includes
#include "Reasoner/Consistence/CConsistenceData.h"
#include "Reasoner/Consistence/CSaturationData.h"
#include "Reasoner/Consistence/CConsistence.h"

#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"
#include "Reasoner/Ontology/COntologyProcessingConceptSaturationRequirement.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Query/CApproximatedSaturationCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Consistence;
		using namespace Ontology;
		using namespace Query;

		namespace Consistiser {


			typedef QPair<CRole*,TConceptNegPair> TRoleConceptNegTriple;


			/*! 
			 *
			 *		\class		CIncrementalOntologyPrecomputationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIncrementalOntologyPrecomputationItem : public COntologyPrecomputationItem, public CConsistenceObserver {
				// public methods
				public:
					//! Constructor
					CIncrementalOntologyPrecomputationItem();
					~CIncrementalOntologyPrecomputationItem();

					CIncrementalOntologyPrecomputationItem* initTotallyPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyPrecomputationItem* addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement);

					CIncrementalOntologyPrecomputationItem* setConsistenceCheched(bool consistenceChecked);
					bool hasConsistenceCheched();

					CIncrementalOntologyPrecomputationItem* setConsistenceCheckCreated(bool consistenceCheckCreated);
					bool hasConsistenceCheckCreated();

					virtual CConsistenceObserver* tellConsistenceData(CConsistenceData* consData);
					CConsistenceData* getConsistenceData();


					CConsistence* getConsistence();
					CIncrementalOntologyPrecomputationItem* setConsistence(CConsistence* consistence);

					CCalculationConfigurationExtension* getCalculationConfiguration();



					CIncrementalOntologyPrecomputationItem* setPrecomputation(CPrecomputation* precomputation);
					CPrecomputation* getPrecomputation();


					CPrecomputationTestingStep* getConsistencePrecomputationStep();
					bool isConsistenceStepFinished();
					bool isConsistenceStepRequired();
					bool areConsistenceStepProcessingRequirementSatisfied();


					CPrecomputationTestingStep* getCyclePrecomputationStep();
					bool isCycleStepFinished();
					bool isCycleStepRequired();
					bool areCycleStepProcessingRequirementSatisfied();


					CPrecomputationTestingStep* getSaturationPrecomputationStep();
					bool isSaturationStepFinished();
					bool isSaturationStepRequired();
					bool areSaturationStepProcessingRequirementSatisfied();



					CPrecomputationTestingStep* getIndividualPrecomputationStep();
					bool isIndividualStepRunning();
					bool isIndividualStepFinished();
					bool isIndividualStepRequired();
					bool areIndividualStepProcessingRequirementSatisfied();
					CIncrementalOntologyPrecomputationItem* setIndividualStepRunning(bool satStepRunning);


					bool isIndividualComputationRunning();
					CIncrementalOntologyPrecomputationItem* setIndividualComputationRunning(bool indiCompRunning);


					CIncrementalOntologyPrecomputationItem* setIndividualPrecomputationCreated(bool initialized);
					bool hasIndividualPrecomputationCreated();
					CIncrementalOntologyPrecomputationItem* setIndividualPrecomputationChecked(bool checked);
					bool hasIndividualPrecomputationChecked();




					bool areAllStepFinished();
					bool hasRemainingProcessingRequirements();


					CConcreteOntology* getLastConsistentlyTestedVersion();
					CIncrementalOntologyPrecomputationItem* setLastConsistentlyTestedVersion(CConcreteOntology* ontology);


					QSet<CIndividual*>* getDirectlyChangedIndividualSet();

					bool hasDirectlyChangedIndividualsIdentified();
					bool setDirectlyChangedIndividualsIdentified(bool identified);

					cint64 getIncrementalExpansionID();


					QTime* getInitializationTime();

				// protected methods
				protected:

				// protected variables
				protected:
					CPrecomputation* mPrecomputation;

					CConcreteOntology* mLastConsistentlyTestedVersion;
					QSet<CIndividual*> mDirectlyChangedIndividualSet;
					bool mDirectlyChangedIndividualsIdentified;

					CConsistence* mConsistence;
					bool mConsistenceCheckCreated;
					bool mConsistenceCheched;
					QAtomicPointer<CConsistenceData> mConsData;




					bool mIndividualTestRunning;

					bool mIndividualComputationRunning;
					bool mIndividualPrecomputationCreated;
					bool mIndividualPrecomputationChecked;

					CPrecomputationTestingStep* mConsistencePrecomputationStep;
					CPrecomputationTestingStep* mCyclePrecomputationStep;
					CPrecomputationTestingStep* mSaturationPrecomputationStep;
					CPrecomputationTestingStep* mIndividualPrecomputationStep;
					QList<CPrecomputationTestingStep*> mProcessingSteps;


					CCalculationConfigurationExtension* mCalculationConfig;


					QTime mInitTime;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CIncrementalOntologyPrecomputationItem_H

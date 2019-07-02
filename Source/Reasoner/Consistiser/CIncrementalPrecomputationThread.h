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

#ifndef KONCLUDE_REASONER_CONSISTISER_CINCREMENTALPRECOMPUTATIONTHREAD_H
#define KONCLUDE_REASONER_CONSISTISER_CINCREMENTALPRECOMPUTATIONTHREAD_H

// Libraries includes
#include <QStack>
#include <QListIterator>


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputationThread.h"
#include "CPrecomputationTestingItem.h"
#include "CIncrementalConsistencePrecomputationTestingItem.h"
#include "CIncrementalOntologyPrecomputationItem.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheWriter.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheEntryExpandWriteData.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCache.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"
#include "Reasoner/Kernel/Task/CIncrementalConsistenceTaskData.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Manager;
		using namespace Kernel::Cache;
		using namespace Generator;

		namespace Consistiser {

			using namespace Events;



			/*! 
			 *
			 *		\class		CIncrementalPrecomputationThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIncrementalPrecomputationThread : public CPrecomputationThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CIncrementalPrecomputationThread(CReasonerManager* reasonerManager);

					//! Destructor
					virtual ~CIncrementalPrecomputationThread();


				// protected methods
				protected:
					virtual COntologyPrecomputationItem* initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual bool createNextTest();

					virtual bool finishOntologyPrecomputation(CIncrementalOntologyPrecomputationItem* incPreCompItem);
					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce);
					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce);

					bool createIndividualPrecomputationCheck(CIncrementalOntologyPrecomputationItem* incPreCompItem);
					bool createConsistencePrecomputationCheck(CIncrementalOntologyPrecomputationItem* incPreCompItem);

					CConcreteOntology* getLastConsistentlyTestedOntologyVersion(CIncrementalOntologyPrecomputationItem* incPreCompItem);
					QSet<CIndividual*>* getDirectlyChangedIndividuals(CConcreteOntology* lastOntologyVersion, CIncrementalOntologyPrecomputationItem* incPreCompItem);
					QSet<CIndividual*>* getDirectlyChangedIndividualsFromLastConsistentlyTestedOntology(CIncrementalOntologyPrecomputationItem* incPreCompItem);
					


					bool logIncrementalReasoningInfos(CIncrementalOntologyPrecomputationItem* incPreCompItem);

				// protected variables
				protected:
					CReuseCompletionGraphCacheWriter* mReuseCompletionGraphCacheWriter;
					CBackendRepresentativeMemoryCache* mBackendAssocCache;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CINCREMENTALPRECOMPUTATIONTHREAD_H

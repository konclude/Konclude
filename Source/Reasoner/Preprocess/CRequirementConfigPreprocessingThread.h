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

#ifndef KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGTHREAD_H
#define KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGTHREAD_H

// Libraries includes


// Namespace includes
#include "PreprocessSettings.h"
#include "CPreprocessingThread.h"
#include "CPreprocessingTestingItem.h"
#include "CRequirementConfigPreprocessingItem.h"
#include "COntologyConfigDependedPreProcesser.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyRedlandTriplesDataExpressionMapper.h"
#include "Reasoner/Generator/CActiveEntitySubExpressionVisitorUpdater.h"

#ifdef KONCLUDE_REDLAND_INTEGRATION
#include "Reasoner/Triples/CRedlandStoredTriplesIndividualAssertionIndexer.h"
#include "Reasoner/Triples/CRedlandStoredTriplesIndividualAssertionConvertionIndexer.h"
#include "Reasoner/Triples/CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer.h"
#endif // !KONCLUDE_REDLAND_INTEGRATION

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Manager;
		using namespace Generator;
		using namespace Triples;

		namespace Preprocess {

			using namespace Events;


			/*! 
			 *
			 *		\class		CRequirementConfigPreprocessingThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRequirementConfigPreprocessingThread : public CPreprocessingThread, public CLogDomain {
				// public methods
				public:
					//! Constructor
					CRequirementConfigPreprocessingThread(CReasonerManager* reasonerManager);

					//! Destructor
					~CRequirementConfigPreprocessingThread();


				// protected methods
				protected:
					virtual COntologyPreprocessingItem* initializeOntologyPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual bool createNextTest();

					virtual bool finishOntologyPreprocessing(CRequirementConfigPreprocessingItem* totallyPreCompItem);
					virtual bool preprocessingTested(COntologyPreprocessingItem* ontPreCompItem, CPreprocessingTestingItem* preTestItem, CPreprocessingCalculatedCallbackEvent* pcce);

					CConcreteOntology* searchPreviousBuiltOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					CConcreteOntology* searchPreviousConsistentOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					CConcreteOntology* searchPreviousClassesClassifiedOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					CConcreteOntology* searchPreviousClassRealizedOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem);


					virtual bool createBuild(CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					virtual bool createPreprocess(CRequirementConfigPreprocessingItem* reqConfPreCompItem);

					virtual bool countActiveEntites(CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					virtual bool mapTriplesToOWL(CRequirementConfigPreprocessingItem* reqConfPreCompItem);
					virtual bool indexTripleEncodedAssertions(CRequirementConfigPreprocessingItem* reqConfPreCompItem);


				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CREQUIREMENTCONFIGPREPROCESSINGTHREAD_H

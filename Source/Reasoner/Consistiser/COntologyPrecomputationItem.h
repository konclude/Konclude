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

#ifndef KONCLUDE_REASONER_CONSISTISER_CONTOLOGYPRECOMPUTATIONITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CONTOLOGYPRECOMPUTATIONITEM_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputatorStatistics.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"

#include "Reasoner/Consistence/CConsistenceCalculationStatisticsCollection.h"
#include "Reasoner/Consistence/CConsistenceStatisticsCollectionStrings.h"


#include "Concurrent/Callback/CCallbackRememberLinkedExecuter.h"
#include "Concurrent/Callback/CCallbackData.h"
#include "Concurrent/Callback/CCallbackListExecuter.h"

#include "Config/CConfigurationBase.h"
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistence;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		COntologyPrecomputationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyPrecomputationItem {
				// public methods
				public:
					//! Constructor
					COntologyPrecomputationItem();

					COntologyPrecomputationItem* initPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual COntologyPrecomputationItem* addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement);

					bool isPrecomputationFinished();
					COntologyPrecomputationItem* setPrecomputationFinished(bool finished, bool doCallbacks = true);
					COntologyPrecomputationItem* doPrecomputationFinishedCallback(bool doCallbacks);

					COntologyPrecomputationItem* addPrecomputationFinishedCallback(CCallbackData* callback);

					CConfigurationBase* getConfiguration();
					CConcreteOntology* getOntology();


					QSet<CPrecomputationTestingItem*>* getPrecomputationTestingItemSet();
					cint64 getCurrentPrecomputationTestingCount();
					bool hasCurrentPrecomputationTesting();


					CPrecomputatorStatistics* getPrecomputatorStatistics();
					CConsistenceStatisticsCollectionStrings* getConsistenceStatisticsCollectionStrings();
					CConsistenceCalculationStatisticsCollection* getCalculationStatisticsCollection();
					COntologyPrecomputationItem* reuseCalculationStatisticsCollection(CConsistenceCalculationStatisticsCollection* statColl);

					bool isProcessStatisticsCollectionConfigured();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CConfigurationBase* mConfig;

					bool mPrecomputationFinishedFlag;
					CCallbackListExecuter mCallbackExecuter;
					QSet<CPrecomputationTestingItem*> mTestItemSet;


					bool mConfCollectProcessStatistics;
					CPrecomputatorStatistics mPrecomputatorStatistics;
					CConsistenceStatisticsCollectionStrings* mConsistenceStatCollStrings;
					QList<CConsistenceCalculationStatisticsCollection*> mReuseStatCollList;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CONTOLOGYPRECOMPUTATIONITEM_H

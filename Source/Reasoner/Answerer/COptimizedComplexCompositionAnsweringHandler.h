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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONANSWERINGHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONANSWERINGHANDLER_H

// Libraries includes
#include <QList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandler.h"
#include "CAnswererContext.h"
#include "CAnsweringHandlingStatistics.h"
#include "CAnsweringCalculationStatisticsCollectionStrings.h"
#include "CAnsweringStatisticsCollectionStrings.h"
#include "CAnsweringMessageDataCalculationCompleted.h"
#include "CAnsweringMessageDataCalculationAdapter.h"
#include "COptimizedComplexCompositionOntologyAnsweringItem.h"
#include "CComplexQueryCompositionProcessingData.h"


// Other includes



#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Query/CComplexAnsweringCompositionQuery.h"
#include "Reasoner/Query/CQueryUnspecifiedStringError.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"




#include "Config/CConfigDataReader.h"


#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

#include <iostream>

using namespace std;


namespace Konclude {

	using namespace Config;
	using namespace Context;

	namespace Reasoner {

		using namespace Revision;
		using namespace Ontology;
		using namespace Generator;
		using namespace Query;


		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexCompositionAnsweringHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexCompositionAnsweringHandler : public CAnsweringHandler, public CLogDomain {
				// public methods
				public:
					//! Constructor
					COptimizedComplexCompositionAnsweringHandler(COptimizedComplexCompositionOntologyAnsweringItem* ontoAnsweringItem);

					//! Destructor
					virtual ~COptimizedComplexCompositionAnsweringHandler();


					virtual bool addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback = nullptr);
					virtual bool canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery);
					virtual bool canAnsweringComplexQuery(bool composedQuery);


					virtual bool initializeProcessing(CAnswererContext* answererContext);
					virtual bool prepareProcessing(CAnswererContext* answererContext);

					virtual bool continueCalculationCreation(CAnswererContext* answererContext);



					virtual bool processCalculationMessage(CAnsweringMessageData* message, CAnswererContext* answererContext);
					virtual bool processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext);


					virtual bool isAnsweringFinished(CAnswererContext* answererContext);

					virtual CAnsweringProgress* getAnsweringProgress();

					virtual CAnsweringHandler* addAnsweringStatistics(CAnsweringStatistics* stats, bool lastStoredDiff);
					virtual CAnsweringHandler* storeAnsweringStatistics();

				// protected methods
				protected:
					virtual COptimizedComplexCompositionAnsweringHandler* initializeQueryProcessing(CComplexQueryCompositionProcessingData* queryData, CAnswererContext* answererContext);



				// protected variables
				protected:
					COptimizedComplexCompositionOntologyAnsweringItem* mOntoAnsweringItem;
					QList<CComplexQueryCompositionProcessingData*> mPendingQueryProcessingList;

					cint64 mTotalPossibleInstancesCount = 0;
					cint64 mTestedPossibleInstancesCount = 0;
					CAnsweringProgress mAnsweringProgress;
					QTime mAnsweringStartTime;

					CAnswererContext* mAnswererContext;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONANSWERINGHANDLER_H

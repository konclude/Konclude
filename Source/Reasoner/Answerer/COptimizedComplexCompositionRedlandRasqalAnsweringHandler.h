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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONREDLANDRASQALANSWERINGHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONREDLANDRASQALANSWERINGHANDLER_H

// Libraries includes
#include <QList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandler.h"
#include "CAnsweringCalculationHandler.h"
#include "CAnswererContext.h"
#include "CAnsweringHandlingStatistics.h"
#include "CAnsweringCalculationStatisticsCollectionStrings.h"
#include "CAnsweringStatisticsCollectionStrings.h"
#include "CAnsweringMessageDataCalculationCompleted.h"
#include "CAnsweringMessageDataCalculationAdapter.h"
#include "COptimizedComplexCompositionOntologyAnsweringItem.h"
#include "CComplexQueryCompositionProcessingData.h"
#include "COptimizedComplexCompositionAnsweringHandler.h"
#include "CComplexQueryCompositionRedlandRasqalProcessingData.h"
#include "CRedlandRasqalSubQueryProcessingData.h"
#include "CRedlandRasqalModelSubQueryResolver.h"
#include "CAnsweringMessageDataRequirementCompleted.h"
#include "CAnsweringMessageDataRequirementCompletedSubQueryAnswering.h"


// Other includes

#include "Reasoner/Answerer/Conclusion/CVariableBindingsQueryResultWriter.h"


#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Query/CRedlandRasqalBGPsCompositionQuery.h"
#include "Reasoner/Query/CComplexAnsweringCompositionQuery.h"
#include "Reasoner/Query/CQueryUnspecifiedStringError.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersListResult.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateSeparateHashingCollectorBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyQueryExtendedBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyExtendibleQueryExtendedBuilder.h"


#include "Parser/CSPARQLRedlandRasqalReasoningBGPQueryParser.h"


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
	using namespace Parser;

	namespace Reasoner {

		using namespace Revision;
		using namespace Ontology;
		using namespace Generator;
		using namespace Query;


		namespace Answerer {

			using namespace Conclusion;

			/*! 
			 *
			 *		\class		COptimizedComplexCompositionRedlandRasqalAnsweringHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexCompositionRedlandRasqalAnsweringHandler : public COptimizedComplexCompositionAnsweringHandler {
				// public methods
				public:
					//! Constructor
					COptimizedComplexCompositionRedlandRasqalAnsweringHandler(COptimizedComplexCompositionOntologyAnsweringItem* ontoAnsweringItem);

					//! Destructor
					virtual ~COptimizedComplexCompositionRedlandRasqalAnsweringHandler();


					virtual bool addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback = nullptr);
					virtual bool canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery);



					virtual bool continueCalculationCreation(CAnswererContext* answererContext);

					void processRasqal(CRedlandRasqalBGPsCompositionQuery* query, CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData);



					virtual bool processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext);


					virtual bool isAnsweringFinished(CAnswererContext* answererContext);

					virtual CAnsweringProgress* getAnsweringProgress();

				// protected methods
				protected:
					virtual COptimizedComplexCompositionRedlandRasqalAnsweringHandler* initializeQueryProcessing(CComplexQueryCompositionProcessingData* queryData, CAnswererContext* answererContext);

					bool generateSubQueries(CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnswererContext* answererContext);

					bool processSubQuery(CAnswererContext* answererContext, CQuery* subQuery, CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnsweringMessageDataRequirementCompleted* completedMessage);
					
					bool scheduleNextSubQueryProcessing(CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnswererContext* answererContext);


				// protected variables
				protected:
					CComplexQueryCompositionRedlandRasqalProcessingData* mCurrentProcessingRedlandRasqalQueryProcData;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONREDLANDRASQALANSWERINGHANDLER_H

#endif // KONCLUDE_REDLAND_INTEGRATION

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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONREDLANDRASQALPROCESSINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONREDLANDRASQALPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComplexQueryCompositionProcessingData.h"
#include "CRedlandRasqalSubQueryProcessingData.h"
#include "CRedlandRasqalModelSubQueryResolver.h"



// Other includes
#include "Reasoner/Query/CRedlandRasqalBGPsCompositionQuery.h"

#include "Concurrent/Callback/CCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CComplexQueryCompositionRedlandRasqalProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexQueryCompositionRedlandRasqalProcessingData : public CComplexQueryCompositionProcessingData {
				// public methods
				public:
					//! Constructor
					CComplexQueryCompositionRedlandRasqalProcessingData(CRedlandRasqalBGPsCompositionQuery* query = nullptr, CCallbackData* callback = nullptr);

					~CComplexQueryCompositionRedlandRasqalProcessingData();

					
					CRedlandRasqalBGPsCompositionQuery* getRedlandRasqalQuery();


					QVector<CRedlandRasqalSubQueryProcessingData*>* getSubQueryProcessingDataVector();
					CRedlandRasqalSubQueryProcessingData* getSubQueryProcessingData(cint64 subQueryId);

					CRedlandRasqalModelSubQueryResolver* getResolver();
					CComplexQueryCompositionRedlandRasqalProcessingData* setResolver(CRedlandRasqalModelSubQueryResolver* resolver);


					QStringList* getRedlandRasqalErrorWarningMessageList();

					bool areResultDirectlyFromSubQueryStreamed();
					CComplexQueryCompositionRedlandRasqalProcessingData* setResultDirectlyFromSubQueryStreamed(bool directlyStreamed);


					bool hasSubQueriesGenerated();
					CComplexQueryCompositionRedlandRasqalProcessingData* setSubQueriesGenerated(bool generated);


					bool hasRemainingSubQueriesProcessing();
					cint64 getRemainingSubQueriesProcessingCount();

					QSet<CRedlandRasqalSubQueryProcessingData*>* getRemainingSubQueriesProcessingSet();
					QSet<cint64>* getProcessedSubQueryIdSet();


					bool isQueryProcessed();
					CComplexQueryCompositionRedlandRasqalProcessingData* setQueryProcessed(bool processed);

					bool isDependentOntologiesLoaded();
					CComplexQueryCompositionRedlandRasqalProcessingData* setDependentOntologiesLoaded(bool loaded);

					bool isDependentOntologiesIdentified();
					CComplexQueryCompositionRedlandRasqalProcessingData* setDependentOntologiesIdentified(bool identified);

					QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*>* getSubQueryDataHash();
					QHash<cint64, CRedlandRasqalSubQueryProcessingData*>* getIdSubQueryDataHash();

				// protected methods
				protected:

				// protected variables
				protected:
					CRedlandRasqalBGPsCompositionQuery* mQuery;

					QVector<CRedlandRasqalSubQueryProcessingData*> mSubQueryProcDataVec;
					CRedlandRasqalModelSubQueryResolver* mResolver;

					QStringList mMessageList;
					bool mResultDirectlyFromSubQueryStreamed;

					bool mSubQueriesGenerated;
					bool mQueryProcessed;

					bool mDependentOntologiesIdentified;
					bool mDependentOntologiesLoaded;


					QSet<CRedlandRasqalSubQueryProcessingData*> mRemainingSubQueriesProcessingSet;
					QSet<cint64> mProcessedSubQueryIdSet;

					QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*> mSubQueryDataHash;
					QHash<cint64, CRedlandRasqalSubQueryProcessingData*> mIdSubQueryDataHash;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONREDLANDRASQALPROCESSINGDATA_H

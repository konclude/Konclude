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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONPROCESSINGDATA_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComplexQueryProcessingData.h"



// Other includes
#include "Reasoner/Query/CComplexAnsweringCompositionQuery.h"

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
			 *		\class		CComplexQueryCompositionProcessingData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexQueryCompositionProcessingData : public CComplexQueryProcessingData {
				// public methods
				public:
					//! Constructor
					CComplexQueryCompositionProcessingData(CComplexAnsweringCompositionQuery* query = nullptr, CCallbackData* callback = nullptr);

					~CComplexQueryCompositionProcessingData();

					
					CComplexAnsweringCompositionQuery* getQuery();
					

					bool isProcessingError();
					CComplexQueryCompositionProcessingData* setProcessingError(bool error);


					CComplexQueryCompositionProcessingData* incSubQueryWaitingCount(cint64 incCount = 1);
					CComplexQueryCompositionProcessingData* decSubQueryWaitingCount(cint64 decCount = 1);
					bool isSubQueryWaiting();


				// protected methods
				protected:

				// protected variables
				protected:
					CComplexAnsweringCompositionQuery* mQuery;
					bool mProcessingError;

					cint64 mSubQueryWaitingCount;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXQUERYCOMPOSITIONPROCESSINGDATA_H

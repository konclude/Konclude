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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERQUERYDATA_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERQUERYDATA_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageData.h"
#include "CAnswererThread.h"


// Other includes
#include "Reasoner/Query/CComplexAnsweringQuery.h"

#include "Concurrent/Callback/CCallbackData.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {



			/*! 
			 *
			 *		\class		CAnsweringManagerQueryData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringManagerQueryData {
				// public methods
				public:
					//! Constructor
					CAnsweringManagerQueryData(CComplexAnsweringQuery* query, CCallbackData* callback);


					CComplexAnsweringQuery* getQuery();
					CCallbackData* getCallback();
					CAnswererThread* getThread();
					CAnsweringManagerQueryData* setThread(CAnswererThread* thread);

				// protected methods
				protected:

				// protected variables
				protected:
					CAnswererThread* mThread;
					CComplexAnsweringQuery* mQuery;
					CCallbackData* mCallback;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERQUERYDATA_H

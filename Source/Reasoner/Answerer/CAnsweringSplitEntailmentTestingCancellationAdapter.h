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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITENTAILMENTTESTINGCANCELLATIONADAPTER_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITENTAILMENTTESTINGCANCELLATIONADAPTER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"


// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableTaskCancellationAdapter.h"

#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringSplitEntailmentTestingCancellationAdapter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringSplitEntailmentTestingCancellationAdapter : public CSatisfiableTaskCancellationAdapter {
				// public methods
				public:
					//! Constructor
					CAnsweringSplitEntailmentTestingCancellationAdapter();

					
					virtual bool canCancelTaskCalculation();

					CAnsweringSplitEntailmentTestingCancellationAdapter* setEntailmentFound();
					bool hasEntailmentFound();

					cint64 getCurrentEntailmentTestingCount();
					CAnsweringSplitEntailmentTestingCancellationAdapter* incCurrentEntailmentTestingCount(cint64 count = 1);
					CAnsweringSplitEntailmentTestingCancellationAdapter* decCurrentEntailmentTestingCount(cint64 count = 1);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mEntailmentFound = false;
					cint64 mTestingCount = 0;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGSPLITENTAILMENTTESTINGCANCELLATIONADAPTER_H

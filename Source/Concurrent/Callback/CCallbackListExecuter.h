/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_CONCURRENT_CALLBACK_CCALLBACKLISTEXECUTER_H
#define KONCLUDE_CONCURRENT_CALLBACK_CCALLBACKLISTEXECUTER_H

// Libraries includes


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {

		namespace Callback {


			/*! 
			 *
			 *		\class		CCallbackListExecuter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCallbackListExecuter {

				// public methods
				public:
					//! Constructor
					CCallbackListExecuter();
					//! Destructor
					virtual ~CCallbackListExecuter();

					virtual void doCallback();

					virtual CCallbackListExecuter *addCallbackData(CCallbackData *callbackData);
					virtual CCallbackListExecuter *removeCallbackData(CCallbackData *callbackData);

					virtual qint64 getCallbackCount();


				// protected methods
				protected:
					QLinkedList<CCallbackData *> callbackList;

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:
				
			};

		}; // end namespace Callback

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CALLBACK_CCALLBACKLISTEXECUTER_H

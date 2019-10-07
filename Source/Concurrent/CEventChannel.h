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

#ifndef KONCLUDE_CONCURRENT_CEVENTCHANNEL_H
#define KONCLUDE_CONCURRENT_CEVENTCHANNEL_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "Concurrent/CEvent.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CEventChannel
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEventChannel {
			// public methods
			public:
				//! Constructor
				CEventChannel();

				//! Destructor
				virtual ~CEventChannel();

				virtual bool hasEvents() = 0;
				virtual CEventChannel* postEvent(CEvent* processingEvent) = 0;
				virtual bool checkPostEvent(CEvent* processingEvent) = 0;

				virtual double getChannelPriority() = 0;


			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENTCHANNEL_H

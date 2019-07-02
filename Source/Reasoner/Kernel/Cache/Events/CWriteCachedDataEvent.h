/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITECACHEDDATAEVENT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITECACHEDDATAEVENT_H

// Libraries includes
#include <QEvent>
#include <QList>

// Namespace includes


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "../CacheSettings.h"
#include "../CCacheEntryWriteData.h"

#include "Utilities/Memory/CMemoryPool.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				namespace Events {


					/*! 
					 *
					 *		\class		CWriteCachedDataEvent
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CWriteCachedDataEvent : public Concurrent::Events::CCustomEvent {
						// public methods
						public:
							//! Constructor
							CWriteCachedDataEvent(CCacheEntryWriteData* writeData, CMemoryPool* memoryPools);

							static const QEvent::Type EVENTTYPE = EVENTWRITECACHEDDATAENTRY;


							CCacheEntryWriteData* getWriteData();
							CMemoryPool* getMemoryPools();


						// protected methods
						protected:

						// protected variables
						protected:
							CCacheEntryWriteData* mWriteData;
							CMemoryPool* mMemoryPools;

						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITECACHEDDATAEVENT_H

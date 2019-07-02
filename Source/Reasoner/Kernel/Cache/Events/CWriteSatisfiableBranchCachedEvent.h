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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLEBRANCHCACHEDEVENT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLEBRANCHCACHEDEVENT_H

// Libraries includes
#include <QEvent>
#include <QList>

// Namespace includes


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "../CacheSettings.h"
#include "../CCacheValue.h"

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
					 *		\class		CWriteSatisfiableBranchCachedEvent
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CWriteSatisfiableBranchCachedEvent : public Concurrent::Events::CCustomEvent {
						// public methods
						public:
							//! Constructor
							CWriteSatisfiableBranchCachedEvent(cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedList, CMemoryPool* memoryPools);

							static const QEvent::Type EVENTTYPE = EVENTWRITESATISFIABLEBRANCHCACHEDENTRY;


							cint64 getSignature();
							CCACHINGLIST<CCacheValue>* getCacheValueList();
							CCACHINGLIST<CCacheValue>* getBranchedValueList();
							CMemoryPool* getMemoryPools();

						// protected methods
						protected:

						// protected variables
						protected:
							cint64 mSignature;
							CCACHINGLIST<CCacheValue>* mCacheValueList;
							CCACHINGLIST<CCacheValue>* mBranchedValueList;
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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLEBRANCHCACHEDEVENT_H

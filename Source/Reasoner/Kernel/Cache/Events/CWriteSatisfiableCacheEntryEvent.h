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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLECACHEENTRYEVENT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLECACHEENTRYEVENT_H

#include <QEvent>
#include <QList>

#include "Concurrent/Events/CCustomEvent.h"

#include "../CacheSettings.h"
#include "../CCacheValue.h"

namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				namespace Events {


					/*! 
					 *	\class		CWriteSatisfiableCacheEntryEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CWriteSatisfiableCacheEntryEvent : public Concurrent::Events::CCustomEvent {
						// public methods
						public:
							//! Constructor
							CWriteSatisfiableCacheEntryEvent(const QList<CCacheValue> itemList = QList<CCacheValue>(), const QList<CCacheValue> outList = QList<CCacheValue>());

							//! Destructor
							virtual ~CWriteSatisfiableCacheEntryEvent();

							QList<CCacheValue> *getCacheItemList();
							void setCacheItemList(QList<CCacheValue> &list);

							QList<CCacheValue> *getCacheOutcomeList();
							void setCacheOutcomeList(QList<CCacheValue> &list);

						// protected methods
						protected:

						// private members
						private:
							QList<CCacheValue> cacheItemList;
							QList<CCacheValue> cacheOutcomeList;


					};

				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITESATISFIABLECACHEENTRYEVENT_H

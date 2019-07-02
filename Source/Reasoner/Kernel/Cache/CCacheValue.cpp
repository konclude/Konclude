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

#include "CCacheValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CCacheValue::CCacheValue() : CTrible<qint64>(0,0,0) {
				}

				CCacheValue::CCacheValue(const qint64 &first, const qint64 &second, const qint64 &third) : CTrible<qint64>(first,second,third) {
				}

				CCacheValue::CCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier) : CTrible<qint64>(tag,identification,(qint64)cacheValueIdentifier) {
				}


				bool CCacheValue::isCachedConcept() {
					return (CACHEVALUEIDENTIFIER)this->second == CACHEVALCONCEPTONTOLOGYTAG;
				}

				bool CCacheValue::isCachedRole() {
					return (CACHEVALUEIDENTIFIER)this->second == CACHEVALROLEONTOLOGYTAG;
				}

				bool CCacheValue::isCachedTerminator() {
					return (CACHEVALUEIDENTIFIER)this->second == CACHEVALUETERMINATOR;
				}

				bool CCacheValue::isCachedNothing() {
					return (CACHEVALUEIDENTIFIER)this->second == CACHEVALUENOTHING;
				}

				qint64 CCacheValue::getTag() const {
					return this->first.first;
				}

				qint64 CCacheValue::getIdentification() const {
					return this->first.second;
				}

				CCacheValue::CACHEVALUEIDENTIFIER CCacheValue::getCacheValueIdentifier() const {
					return (CACHEVALUEIDENTIFIER)this->second;
				}

				CCacheValue* CCacheValue::initCacheValue(const qint64 &tag, const qint64 &identification, const CACHEVALUEIDENTIFIER &cacheValueIdentifier) {
					set(tag,identification,(cint64)cacheValueIdentifier);
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

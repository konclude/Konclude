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

#include "CStringRefStringHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CStringRefStringHasher::CStringRefStringHasher() {
				mReferenceOnly = false;
			}


			CStringRefStringHasher::CStringRefStringHasher(const QString& string) {
				mReferenceOnly = false;
				mString = string;
			}

			CStringRefStringHasher::CStringRefStringHasher(const QStringRef& stringRef) {
				mReferenceOnly = true;
				mStringRef = stringRef;
			}


			bool CStringRefStringHasher::isReferenceOnly() const {
				return mReferenceOnly;
			}

			bool CStringRefStringHasher::isEmpty() const {
				if (mReferenceOnly) {
					return mStringRef.isEmpty();
				} else {
					return mString.isEmpty();
				}
			}

			const QString& CStringRefStringHasher::getString() const {
				return mString;
			}

			const QStringRef& CStringRefStringHasher::getStringRef() const {
				return mStringRef;
			}

			QStringRef CStringRefStringHasher::toStringRefernce() const {
				if (mReferenceOnly) {
					return mStringRef;
				} else {
					return mString.leftRef(-1);
				}
			}


			bool CStringRefStringHasher::operator==(const CStringRefStringHasher& other) const {
				if (mReferenceOnly && other.mReferenceOnly) {
					return mStringRef == other.mStringRef;
				} else if (mReferenceOnly && !other.mReferenceOnly) {
					return mStringRef == other.mString;
				} else if (!mReferenceOnly && other.mReferenceOnly) {
					return other.mStringRef == mString;
				} else if (!mReferenceOnly && !other.mReferenceOnly) {
					return mString == other.mString;
				}
				return false;
			}

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

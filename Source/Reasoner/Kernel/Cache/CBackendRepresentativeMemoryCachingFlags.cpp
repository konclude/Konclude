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

#include "CBackendRepresentativeMemoryCachingFlags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCachingFlags::CBackendRepresentativeMemoryCachingFlags() {
					mStatusFlags = 0;
				}


				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::initCachingStatusFlags(cint64 flags) {
					mStatusFlags = flags;
					return this;
				}


				bool CBackendRepresentativeMemoryCachingFlags::isCompletelyHandled() {
					return hasStatusFlags(FLAG_COMPLETELY_HANDLED);;
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::setCompletelyHandled(bool completelyHandled) {
					if (completelyHandled) {
						addStatusFlags(FLAG_COMPLETELY_HANDLED);
					} else {
						clearStatusFlags(FLAG_COMPLETELY_HANDLED);
					}
					return this;
				}


				bool CBackendRepresentativeMemoryCachingFlags::isCompletelySaturated() {
					return hasStatusFlags(FLAG_COMPLETELY_SATURATED);
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::setCompletelySaturated(bool completelySaturated) {
					if (completelySaturated) {
						addStatusFlags(FLAG_COMPLETELY_SATURATED);
					} else {
						clearStatusFlags(FLAG_COMPLETELY_SATURATED);
					}
					return this;
				}


				bool CBackendRepresentativeMemoryCachingFlags::isCompletelyPropagated() {
					return hasStatusFlags(FLAG_COMPLETELY_PROPAGATED);
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::setCompletelyPropagated(bool completelyPropagated) {
					if (completelyPropagated) {
						addStatusFlags(FLAG_COMPLETELY_PROPAGATED);
					} else {
						clearStatusFlags(FLAG_COMPLETELY_PROPAGATED);
					}
					return this;
				}



				bool CBackendRepresentativeMemoryCachingFlags::hasNondeterministicElements() {
					return hasStatusFlags(FLAG_NONDETERMINISTIC_ELEMENTS);
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::setNondeterministicElements(bool completelySaturated) {
					if (completelySaturated) {
						addStatusFlags(FLAG_NONDETERMINISTIC_ELEMENTS);
					} else {
						clearStatusFlags(FLAG_NONDETERMINISTIC_ELEMENTS);
					}
					return this;
				}



				bool CBackendRepresentativeMemoryCachingFlags::hasStatusFlags(cint64 flags) {
					return (mStatusFlags & flags) == flags;
				}


				bool CBackendRepresentativeMemoryCachingFlags::hasStatusFlagsPartially(cint64 flags) {
					return (mStatusFlags & flags) != 0;
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::setStatusFlags(cint64 flags) {
					mStatusFlags = flags;
					return this;
				}

				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::addStatusFlags(cint64 flags) {
					mStatusFlags |= flags;
					return this;
				}


				CBackendRepresentativeMemoryCachingFlags* CBackendRepresentativeMemoryCachingFlags::clearStatusFlags(cint64 flags) {
					mStatusFlags &= ~flags;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCachingFlags::getStatusFlags() {
					return mStatusFlags;
				}





			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CSatisfiableTaskRepresentativeBackendUpdatingAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableTaskRepresentativeBackendUpdatingAdapter::CSatisfiableTaskRepresentativeBackendUpdatingAdapter(CIndividualPrecomputationCoordinationHash* indiComCoordHash, bool *unstatFlagPointer) {
					mIndiCompCoordHash = indiComCoordHash;
					mUnstatFlagPointer = unstatFlagPointer;
				}



				CIndividualPrecomputationCoordinationHash* CSatisfiableTaskRepresentativeBackendUpdatingAdapter::getIndividualComputationCoordinationHash() {
					return mIndiCompCoordHash;
				}

				CSatisfiableTaskRepresentativeBackendUpdatingAdapter* CSatisfiableTaskRepresentativeBackendUpdatingAdapter::setIndividualComputationCoordinationHash(CIndividualPrecomputationCoordinationHash* indiComCoordHash) {
					mIndiCompCoordHash = indiComCoordHash;
					return this;
				}


				cint64 CSatisfiableTaskRepresentativeBackendUpdatingAdapter::getFirstIndividualRecomputationId() {
					return mFirstIndividualID;
				}

				cint64 CSatisfiableTaskRepresentativeBackendUpdatingAdapter::getRepresentativeCacheRecomputationId() {
					return mRepCacheCompID;
				}

				CSatisfiableTaskRepresentativeBackendUpdatingAdapter* CSatisfiableTaskRepresentativeBackendUpdatingAdapter::setFirstIndividualRecomputationId(cint64 id) {
					mFirstIndividualID = id;
					return this;
				}

				CSatisfiableTaskRepresentativeBackendUpdatingAdapter* CSatisfiableTaskRepresentativeBackendUpdatingAdapter::setRepresentativeCacheRecomputationId(cint64 id) {
					mRepCacheCompID = id;
					return this;
				}


				CSatisfiableTaskRepresentativeBackendUpdatingAdapter* CSatisfiableTaskRepresentativeBackendUpdatingAdapter::setAllAssertionIndividualId(cint64 id) {
					mAllAssertionIndiID = id;
					return this;
				}

				cint64 CSatisfiableTaskRepresentativeBackendUpdatingAdapter::getAllAssertionIndividualId() {
					return mAllAssertionIndiID;
				}


				bool CSatisfiableTaskRepresentativeBackendUpdatingAdapter::hasUnsatisfiableComputed() {
					if (mUnstatFlagPointer) {
						return *mUnstatFlagPointer;
					}
					return false;
				}



			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

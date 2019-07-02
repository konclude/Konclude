/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CBackendRepresentativeMemoryCardinalityValueLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCardinalityValueLinker::CBackendRepresentativeMemoryCardinalityValueLinker() {
				}

				CBackendRepresentativeMemoryCardinalityValueLinker* CBackendRepresentativeMemoryCardinalityValueLinker::initCardinalityValueLinker(cint64 roleTag, cint64 existentialMaxUsedCardinality, cint64 minimalRestrictingCardinality) {
					setData(roleTag);
					mExistentialMaxUsedCardinality = existentialMaxUsedCardinality;
					mMinimalRestrictingCardinality = minimalRestrictingCardinality;
					return this;
				}
				
				cint64 CBackendRepresentativeMemoryCardinalityValueLinker::getRoleTag() {
					return getData();
				}

				cint64 CBackendRepresentativeMemoryCardinalityValueLinker::getExistentialMaxUsedCardinality() {
					return mExistentialMaxUsedCardinality;
				}

				cint64 CBackendRepresentativeMemoryCardinalityValueLinker::getMinimalRestrictingCardinality() {
					return mMinimalRestrictingCardinality;
				}

				CBackendRepresentativeMemoryCardinalityValueLinker* CBackendRepresentativeMemoryCardinalityValueLinker::updateExistentialMaxUsedCardinality(cint64 existentialMaxUsedCardinality) {
					mExistentialMaxUsedCardinality = qMax(existentialMaxUsedCardinality, mExistentialMaxUsedCardinality);
					return this;
				}

				CBackendRepresentativeMemoryCardinalityValueLinker* CBackendRepresentativeMemoryCardinalityValueLinker::updateMinimalRestrictingCardinality(cint64 minimalRestrictingCardinality) {
					mMinimalRestrictingCardinality = qMax(minimalRestrictingCardinality, mMinimalRestrictingCardinality);
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

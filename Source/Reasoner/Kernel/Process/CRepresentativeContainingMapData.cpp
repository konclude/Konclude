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

#include "CRepresentativeContainingMapData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeContainingMapData::CRepresentativeContainingMapData(CRepresentativeVariableBindingPathSetData* containedRep, bool explContained) {
					mExplicitelyContained = explContained;
					mContainedRep = containedRep;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeContainingMapData::getRepresentativeVariableBindingPathSetData() const {
					return mContainedRep;
				}

				bool CRepresentativeContainingMapData::hasRepresentativeVariableBindingPathSetData() const {
					return mContainedRep != nullptr;
				}

				CRepresentativeContainingMapData* CRepresentativeContainingMapData::setRepresentativeVariableBindingPathSetData(CRepresentativeVariableBindingPathSetData* containedRep) {
					mContainedRep = containedRep;
					return this;
				}


				bool CRepresentativeContainingMapData::isExplicitelyContained() const {
					return mExplicitelyContained;
				}

				CRepresentativeContainingMapData* CRepresentativeContainingMapData::setExplicitelyContained(bool explContained) {
					mExplicitelyContained = explContained;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

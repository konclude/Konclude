/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CRepresentativeVariableBindingPathMapData.h"
#include "CRepresentativeVariableBindingPathSetData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathMapData::CRepresentativeVariableBindingPathMapData(CVariableBindingPath* varBindPath, CVariableBindingPath* resolveVarBindPath, CRepresentativeVariableBindingPathSetData* resRevRepVarBindPath) {
					mVarBindPath = varBindPath;
					mResolveVarBindPath = resolveVarBindPath;
					mResolveRepVarBindPathSetData = resRevRepVarBindPath;
					mResolveRepvarBindPathSetDataID = mResolveRepVarBindPathSetData->getRepresentativeID();
				}


				CRepresentativeVariableBindingPathMapData::CRepresentativeVariableBindingPathMapData(CVariableBindingPath* varBindPath, CRepresentativeVariableBindingPathSetData* resRevRepVarBindPath) {
					mVarBindPath = varBindPath;
					mResolveVarBindPath = varBindPath;
					mResolveRepVarBindPathSetData = resRevRepVarBindPath;
					mResolveRepvarBindPathSetDataID = mResolveRepVarBindPathSetData->getRepresentativeID();
				}


				CVariableBindingPath* CRepresentativeVariableBindingPathMapData::getVariableBindingPath() const {
					return mVarBindPath;
				}

				bool CRepresentativeVariableBindingPathMapData::hasVariableBindingPath() const {
					return mVarBindPath != nullptr;
				}

				CRepresentativeVariableBindingPathMapData* CRepresentativeVariableBindingPathMapData::setVariableBindingPath(CVariableBindingPath* varBindPath) {
					mVarBindPath = varBindPath;
					return this;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathMapData::getResolveRepresentativeVariableBindingPathSetData() const {
					return mResolveRepVarBindPathSetData;
				}

				bool CRepresentativeVariableBindingPathMapData::hasResolveRepresentativeVariableBindingPathSetData() const {
					return mResolveRepVarBindPathSetData != nullptr;
				}

				CRepresentativeVariableBindingPathMapData* CRepresentativeVariableBindingPathMapData::setResolveRepresentativeVariableBindingPathSetData(CRepresentativeVariableBindingPathSetData* resRevRepVarBindPath) {
					mResolveRepVarBindPathSetData = resRevRepVarBindPath;
					mResolveRepvarBindPathSetDataID = mResolveRepVarBindPathSetData->getRepresentativeID();
					return this;
				}

				cint64 CRepresentativeVariableBindingPathMapData::getResolveRepresentativeVariableBindingPathSetDataID() const {
					return mResolveRepvarBindPathSetDataID;
				}


				CVariableBindingPath* CRepresentativeVariableBindingPathMapData::getResolveVariableBindingPath() const {
					return mResolveVarBindPath;
				}

				bool CRepresentativeVariableBindingPathMapData::hasResolveVariableBindingPath() const {
					return mResolveVarBindPath != nullptr;
				}

				CRepresentativeVariableBindingPathMapData* CRepresentativeVariableBindingPathMapData::setResolveVariableBindingPath(CVariableBindingPath* varBindPath) {
					mResolveVarBindPath = varBindPath;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

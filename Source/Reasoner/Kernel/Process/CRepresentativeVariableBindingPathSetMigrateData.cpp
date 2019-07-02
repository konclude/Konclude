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

#include "CRepresentativeVariableBindingPathSetMigrateData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetMigrateData::CRepresentativeVariableBindingPathSetMigrateData(CProcessContext* context) : mProcessContext(context),mVarBindPathMap(context),mRepContainingMap(context) {
				}

				CRepresentativeVariableBindingPathSetMigrateData* CRepresentativeVariableBindingPathSetMigrateData::initRepresentativeVariableBindingPathSetMigrateData(CRepresentativeVariableBindingPathSetMigrateData* data) {
					if (data) {
						mVarBindPathMap = data->mVarBindPathMap;
						mRepContainingMap = data->mRepContainingMap;
					} else {
						mVarBindPathMap.clear();
						mRepContainingMap.clear();
					}
					return this;
				}


				CRepresentativeVariableBindingPathMap* CRepresentativeVariableBindingPathSetMigrateData::getRepresentativeVariableBindingPathMap() {
					return &mVarBindPathMap;
				}

				CRepresentativeContainingMap* CRepresentativeVariableBindingPathSetMigrateData::getRepresentativeContainingMap() {
					return &mRepContainingMap;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CSaturationModifiedProcessUpdateLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationModifiedProcessUpdateLinker::CSaturationModifiedProcessUpdateLinker() : CLinkerBase<CIndividualSaturationProcessNode*,CSaturationModifiedProcessUpdateLinker>(nullptr) {
				}

				CSaturationModifiedProcessUpdateLinker* CSaturationModifiedProcessUpdateLinker::initProcessUpdateLinker(CIndividualSaturationProcessNode* individual, CSaturationModifiedProcessUpdateLinker::MODIFICATIONPROCESSUPDATETYPE updateType) {
					initLinker(individual);
					mUpdateType = updateType;
					return this;
				}


				CSaturationModifiedProcessUpdateLinker* CSaturationModifiedProcessUpdateLinker::setProcessingIndividual(CIndividualSaturationProcessNode* individual) {
					setData(individual);
					return this;
				}

				CIndividualSaturationProcessNode* CSaturationModifiedProcessUpdateLinker::getProcessingIndividual() {
					return getData();
				}

				CSaturationModifiedProcessUpdateLinker::MODIFICATIONPROCESSUPDATETYPE CSaturationModifiedProcessUpdateLinker::getUpdateType() {
					return mUpdateType;
				}

				CSaturationModifiedProcessUpdateLinker* CSaturationModifiedProcessUpdateLinker::setUpdateType(CSaturationModifiedProcessUpdateLinker::MODIFICATIONPROCESSUPDATETYPE updateType) {
					mUpdateType = updateType;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

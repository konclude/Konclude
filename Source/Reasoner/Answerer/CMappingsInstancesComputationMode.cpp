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

#include "CMappingsInstancesComputationMode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CMappingsInstancesComputationMode::CMappingsInstancesComputationMode(cint64 mode, cint64 sizeLimit) {
				mComputationMode = mode;
				mComputationSizeLimit = sizeLimit;
			}



			bool CMappingsInstancesComputationMode::hasComputationMode(CMappingsInstancesComputationMode* mode) {
				return hasComputationMode(mode->getVariableMappingsComputationMode(), mode->mComputationSizeLimit);
			}

			CMappingsInstancesComputationMode* CMappingsInstancesComputationMode::setComputationMode(CMappingsInstancesComputationMode* mode) {
				return setComputationMode(mode->getVariableMappingsComputationMode());
			}

			CMappingsInstancesComputationMode* CMappingsInstancesComputationMode::addComputationMode(CMappingsInstancesComputationMode* mode) {
				return addComputationMode(mode->getVariableMappingsComputationMode());
			}


			bool CMappingsInstancesComputationMode::hasALLComputationMode() {
				return hasComputationMode(COMPUTATION_TYPEF_ALL, -1);
			}


			bool CMappingsInstancesComputationMode::hasComputationMode(cint64 mode) {
				return mComputationMode & mode != mode;
			}

			bool CMappingsInstancesComputationMode::hasComputationMode(cint64 mode, cint64 limit) {
				if (mComputationMode & mode != mode) {
					return false;
				}
				if (limit < 0 && mComputationSizeLimit > 0) {
					return false;
				}
				if (limit >= 0 && mComputationSizeLimit >= 0) {
					return mComputationSizeLimit > limit;
				}
				return true;
			}

			cint64 CMappingsInstancesComputationMode::getVariableMappingsComputationMode() {
				return mComputationMode;
			}

			CMappingsInstancesComputationMode* CMappingsInstancesComputationMode::setComputationMode(cint64 mode) {
				mComputationMode = mode;
				return this;
			}

			CMappingsInstancesComputationMode* CMappingsInstancesComputationMode::addComputationMode(cint64 mode) {
				mComputationMode |= mode;
				return this;
			}


			cint64 CMappingsInstancesComputationMode::getComputationSizeLimit() {
				return mComputationSizeLimit;
			}

			CMappingsInstancesComputationMode* CMappingsInstancesComputationMode::setComputationSizeLimit(cint64 limit) {
				mComputationSizeLimit = limit;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

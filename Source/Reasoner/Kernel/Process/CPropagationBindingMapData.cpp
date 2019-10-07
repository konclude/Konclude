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

#include "CPropagationBindingMapData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBindingMapData::CPropagationBindingMapData(CPropagationBindingDescriptor* propBindDes) {
					mPropBindDes = propBindDes;
					mReapplyConDes = nullptr;
				}

				CPropagationBindingDescriptor* CPropagationBindingMapData::getPropagationBindingDescriptor() const {
					return mPropBindDes;				
				}

				bool CPropagationBindingMapData::hasPropagationBindingDescriptor() const {
					return mPropBindDes != nullptr;				
				}

				CPropagationBindingMapData* CPropagationBindingMapData::setPropagationBindingDescriptor(CPropagationBindingDescriptor* des) {
					mPropBindDes = des;
					return this;
				}

				CPropagationBindingReapplyConceptDescriptor* CPropagationBindingMapData::getReapplyConceptDescriptor() const {
					return mReapplyConDes;
				}

				bool CPropagationBindingMapData::hasReapplyConceptDescriptor() const {
					return mReapplyConDes != nullptr;
				}

				CPropagationBindingMapData* CPropagationBindingMapData::setReapplyConceptDescriptor(CPropagationBindingReapplyConceptDescriptor* reapplyConDes) {
					mReapplyConDes = reapplyConDes->append(mReapplyConDes);
					return this;
				}

				CPropagationBindingMapData* CPropagationBindingMapData::clearReapplyConceptDescriptor() {
					mReapplyConDes = nullptr;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

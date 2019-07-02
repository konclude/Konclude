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

#include "CConceptPilingDescriptorReapplyData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CConceptPilingDescriptorReapplyData::CConceptPilingDescriptorReapplyData() {
					mImpReapplyConPilDes = nullptr;
					mConPilDes = nullptr;
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = 0;
#endif
				}

				CConceptPilingDescriptorReapplyData::CConceptPilingDescriptorReapplyData(const CConceptPilingDescriptorReapplyData& data) {
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					mPilingDepth = data.mPilingDepth;
#endif
					mConPilDes = data.mConPilDes;
					mImpReapplyConPilDes = nullptr;
					CImplicationReapplyConceptPilingDescriptor* impReapplyConPilDesIt = data.mImpReapplyConPilDes;
					while (impReapplyConPilDesIt) {
						CImplicationReapplyConceptPilingDescriptor* impReapplyConPilDes = impReapplyConPilDesIt;
						CImplicationReapplyConceptPilingDescriptor* copiedImpReapplyConPilDes = impReapplyConPilDes->getCopyReapplyConceptPilingDescriptor();
						if (copiedImpReapplyConPilDes) {
							if (impReapplyConPilDes->getNextReapplyConceptPilingDescriptor()) {
								copiedImpReapplyConPilDes->setNextReapplyConceptPilingDescriptor(impReapplyConPilDes->getNextReapplyConceptPilingDescriptor()->getCopyReapplyConceptPilingDescriptor());
							}
							mImpReapplyConPilDes = copiedImpReapplyConPilDes->append(mImpReapplyConPilDes);						
						}
						impReapplyConPilDesIt = impReapplyConPilDesIt->getNext();
					}
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

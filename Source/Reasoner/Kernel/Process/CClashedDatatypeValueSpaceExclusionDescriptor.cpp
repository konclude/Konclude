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

#include "CClashedDatatypeValueSpaceExclusionDescriptor.h"
#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CClashedDatatypeValueSpaceExclusionDescriptor::CClashedDatatypeValueSpaceExclusionDescriptor() : CClashedDependencyDescriptor() {
					mIndividualNode = nullptr;
				}


				CClashedDatatypeValueSpaceExclusionDescriptor* CClashedDatatypeValueSpaceExclusionDescriptor::initClashedDatatypeValueSpaceExclusionDescriptor(CDependencyTrackPoint* depTrackPoint, CIndividualProcessNode* individual) {
					initClashedDependencyDescriptor(depTrackPoint);
					mIndividualNode = individual;
					return this;
				}


				cint64 CClashedDatatypeValueSpaceExclusionDescriptor::getAppropriatedIndividualID() {
					return mIndividualNode->getIndividualNodeID();
				}

				CClashedDatatypeValueSpaceExclusionDescriptor* CClashedDatatypeValueSpaceExclusionDescriptor::setAppropriatedIndividual(CIndividualProcessNode* individualNode) {
					mIndividualNode = individualNode;
					return this;
				}

				CIndividualProcessNode* CClashedDatatypeValueSpaceExclusionDescriptor::getAppropriatedIndividual() {
					return mIndividualNode;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

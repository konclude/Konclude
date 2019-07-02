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

#include "CSaturationIndividualNodeNominalHandlingData.h"
#include "CIndividualSaturationProcessNodeLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeNominalHandlingData::CSaturationIndividualNodeNominalHandlingData(CProcessContext* processContext) {
					mProcessContext = processContext;
					mConceptSaturationProcessLinker = nullptr;
					mIndiProcessLinker = nullptr;
					mSuccConnectedNominalSet = nullptr;
				}

				CSaturationIndividualNodeNominalHandlingData* CSaturationIndividualNodeNominalHandlingData::initNominalHandlingData(CIndividualSaturationProcessNode* indiProcessNode) {
					mIndiProcessNode = indiProcessNode;
					return this;
				}


				CConceptSaturationProcessLinker* CSaturationIndividualNodeNominalHandlingData::getDelayedNominalConceptSaturationProcessLinker() {
					return mConceptSaturationProcessLinker;
				}

				CConceptSaturationProcessLinker* CSaturationIndividualNodeNominalHandlingData::takeDelayedNominalConceptSaturationProcessLinker() {
					CConceptSaturationProcessLinker* conProcLinker = mConceptSaturationProcessLinker;
					if (mConceptSaturationProcessLinker) {
						mConceptSaturationProcessLinker = mConceptSaturationProcessLinker->getNext();
					}
					return conProcLinker;
				}

				CSaturationIndividualNodeNominalHandlingData* CSaturationIndividualNodeNominalHandlingData::setDelayedNominalConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker) {
					mConceptSaturationProcessLinker = conProcessLinker;
					return this;
				}

				CSaturationIndividualNodeNominalHandlingData* CSaturationIndividualNodeNominalHandlingData::addDelayedNominalConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker) {
					mConceptSaturationProcessLinker = conProcessLinker->append(mConceptSaturationProcessLinker);
					return this;
				}

				CSaturationIndividualNodeNominalHandlingData* CSaturationIndividualNodeNominalHandlingData::clearDelayedNominalConceptSaturationProcessLinker() {
					mConceptSaturationProcessLinker = nullptr;
					return this;
				}



				bool CSaturationIndividualNodeNominalHandlingData::isDelayedNominalIndividualSaturationProcessNodeLinkerQueued() {
					if (mIndiProcessLinker) {
						return mIndiProcessLinker->isNegated();
					}
					return false;
				}

				CIndividualSaturationProcessNodeLinker* CSaturationIndividualNodeNominalHandlingData::getDelayedNominalIndividualSaturationProcessNodeLinker(bool create) {
					if (!mIndiProcessLinker && create) {
						mIndiProcessLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(mProcessContext->getUsedMemoryAllocationManager());
						mIndiProcessLinker->initProcessNodeLinker(mIndiProcessNode,false);
					}
					return mIndiProcessLinker;
				}

				CSaturationIndividualNodeNominalHandlingData* CSaturationIndividualNodeNominalHandlingData::setDelayedNominalIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* processNodeLinker) {
					mIndiProcessLinker = processNodeLinker;
					return this;
				}

				CSuccessorConnectedNominalSet* CSaturationIndividualNodeNominalHandlingData::getSuccessorConnectedNominalSet(bool create) {
					if (!mSuccConnectedNominalSet && create) {
						mSuccConnectedNominalSet = CObjectParameterizingAllocator< CSuccessorConnectedNominalSet,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSuccConnectedNominalSet->initSuccessorConnectedNominalSet();
					}
					return mSuccConnectedNominalSet;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

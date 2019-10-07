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

#include "CSaturationNodeCacheUpdater.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeCacheUpdater::CSaturationNodeCacheUpdater(CSaturationNodeAssociatedExpansionCacheContext* context) {
					mIndirectUpdatedStatusIndiNodeCount = 0;
					mDirectUpdatedStatusIndiNodeCount = 0;
					mContext = context;
				}


				CSaturationNodeCacheUpdater* CSaturationNodeCacheUpdater::propagateUnsatisfibility(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CIndividualSaturationProcessNodeStatusFlags flags;
					flags.setClashedFlag(true);
					updateDirectAddingIndividualStatusFlags(node,&flags,context);
					return this;
				}


				CIndividualSaturationProcessNodeStatusUpdateLinker* CSaturationNodeCacheUpdater::createIndividualSaturationUpdateLinker(CSaturationNodeAssociatedExpansionCacheContext* context) {
					CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker = context->getIndividualSaturationUpdateLinker();
					return conSatUpdateLinker;
				}

				void CSaturationNodeCacheUpdater::releaseIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker, CSaturationNodeAssociatedExpansionCacheContext* context) {
					context->addIndividualSaturationUpdateLinker(conSatUpdateLinker);
				}

				bool CSaturationNodeCacheUpdater::requiresDirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CIndividualSaturationProcessNodeStatusFlags* indiDirectFlags = indiNode->getDirectStatusFlags();
					return !indiDirectFlags->hasFlags(addingFlags,true);
				}

				bool CSaturationNodeCacheUpdater::requiresIndirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CIndividualSaturationProcessNodeStatusFlags* indiIndirectFlags = indiNode->getIndirectStatusFlags();
					return !indiIndirectFlags->hasFlags(addingFlags,true);
				}


				void CSaturationNodeCacheUpdater::updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context) {
					if (requiresDirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,context)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(context);
						directUpdateLinker->initUpdateNodeLinker(indiNode);
						CIndividualSaturationProcessNodeStatusFlags* directIndiFlags = indiNode->getDirectStatusFlags();
						directIndiFlags->addFlags(addingFlags);
						++mDirectUpdatedStatusIndiNodeCount;

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,context);

							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresDirectAddingIndividualStatusFlagsUpdate(dependingIndi,addingFlags,context)) {
									CIndividualSaturationProcessNodeStatusFlags* directIndiFlags = dependingIndi->getDirectStatusFlags();
									directIndiFlags->addFlags(addingFlags);
									++mDirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(context)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}

							updateIndirectAddingIndividualStatusFlags(updateIndiNode,addingFlags,context);
						}
					}
				}



				void CSaturationNodeCacheUpdater::updateIndirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CSaturationNodeAssociatedExpansionCacheContext* context) {
					if (requiresIndirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,context)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(context);
						directUpdateLinker->initUpdateNodeLinker(indiNode);
						CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = indiNode->getIndirectStatusFlags();
						indirectIndiFlags->addFlags(addingFlags);
						++mIndirectUpdatedStatusIndiNodeCount;

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,context);


							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresIndirectAddingIndividualStatusFlagsUpdate(dependingIndi,addingFlags,context)) {
									CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = dependingIndi->getIndirectStatusFlags();
									indirectIndiFlags->addFlags(addingFlags);
									++mIndirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(context)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
									for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
										CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
										if (requiresIndirectAddingIndividualStatusFlagsUpdate(sourceIndividual,addingFlags,context)) {
											CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = sourceIndividual->getIndirectStatusFlags();
											indirectIndiFlags->addFlags(addingFlags);
											++mIndirectUpdatedStatusIndiNodeCount;
											directUpdateLinker = createIndividualSaturationUpdateLinker(context)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
										}
									}
								}
							}


							for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = updateIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
								if (requiresIndirectAddingIndividualStatusFlagsUpdate(sourceIndividual,addingFlags,context)) {
									CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = sourceIndividual->getIndirectStatusFlags();
									indirectIndiFlags->addFlags(addingFlags);
									++mIndirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(context)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
								}
							}


						}
					}
				}





			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

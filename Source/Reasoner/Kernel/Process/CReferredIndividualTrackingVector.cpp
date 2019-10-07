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

#include "CReferredIndividualTrackingVector.h"

#include "Reasoner/Kernel/Task/CIncrementalConsistenceTaskData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Task;

			namespace Process {


				CReferredIndividualTrackingVector::CReferredIndividualTrackingVector() {
					mIndiTrackCount = 0;
					mIndiTrackOffset = 0;
					mIndiTrackVector = nullptr;
				}
				CReferredIndividualTrackingVector::~CReferredIndividualTrackingVector() {
					delete [] mIndiTrackVector;
				}

				CReferredIndividualTrackingVector* CReferredIndividualTrackingVector::initReferredIndividualTrackingVector(cint64 indiCount, cint64 indiOffset) {
					mIndiTrackVector = new CReferredIndividualTrackingData[indiCount];
					mIndiTrackCount = indiCount;
					mIndiTrackOffset = indiOffset;
					return this;
				}

				CReferredIndividualTrackingVector* CReferredIndividualTrackingVector::initReferredIndividualTrackingVector(CReferredIndividualTrackingVector* indiTrackVec) {
					mIndiTrackCount = indiTrackVec->mIndiTrackCount;
					mIndiTrackVector = new CReferredIndividualTrackingData[mIndiTrackCount];
					for (cint64 i = 0; i < mIndiTrackCount; ++i) {
						CReferredIndividualTrackingData* refIndiTrackData = &indiTrackVec->mIndiTrackVector[i];
						CReferredIndividualTrackingData* indiTrackData = &mIndiTrackVector[i];
						*indiTrackData = *refIndiTrackData;
					}
					return this;
				}


				CReferredIndividualTrackingData* CReferredIndividualTrackingVector::getReferredIndividualTrackingData(cint64 indiID) {
					cint64 correctedIndiID = indiID+mIndiTrackOffset;
					if (correctedIndiID >= 0 && correctedIndiID < mIndiTrackCount) {
						return &mIndiTrackVector[correctedIndiID];
					}
					return nullptr;
				}

				CReferredIndividualTrackingVector* CReferredIndividualTrackingVector::setIndividualReferred(cint64 indiID) {
					CReferredIndividualTrackingData* indTrackData = getReferredIndividualTrackingData(indiID+mIndiTrackOffset);
					if (indTrackData && !indTrackData->isReferred()) {
						indTrackData->setReferred();
					}
					return this;
				}

				CReferredIndividualTrackingVector* CReferredIndividualTrackingVector::setIndividualReferredAndExtended(cint64 indiID) {
					CReferredIndividualTrackingData* indTrackData = getReferredIndividualTrackingData(indiID+mIndiTrackOffset);
					if (indTrackData && (!indTrackData->isExtended() || !indTrackData->isReferred())) {
						indTrackData->setReferred();
						indTrackData->setExtended();
					}
					return this;
				}


				bool CReferredIndividualTrackingVector::mergeGatheredTrackedIndividualDependences(CIndividualDependenceTracking* indiDepTracking) {
					CReferredIndividualTrackingVector* mergeRefIndiTrackVec = dynamic_cast<CReferredIndividualTrackingVector*>(indiDepTracking);
					if (mergeRefIndiTrackVec) {
						if (mergeRefIndiTrackVec->mIndiTrackVector) {
							for (cint64 i = 0; i = mergeRefIndiTrackVec->mIndiTrackCount; ++i) {
								CReferredIndividualTrackingData* mergeIndiTrackData = &mergeRefIndiTrackVec->mIndiTrackVector[i];
								CReferredIndividualTrackingData* indiTrackData = &mIndiTrackVector[i];
								if (!indiTrackData->isExtended() && mergeIndiTrackData->isExtended()) {
									indiTrackData->setExtended();
								}
								if (!indiTrackData->isReferred() && mergeIndiTrackData->isReferred()) {
									indiTrackData->setReferred();
								}
							}
						}
						return true;
					}
					return false;
				}

				bool CReferredIndividualTrackingVector::areIndividualsAffected(QSet<cint64>* indirectlyChangedIndividualSet, QSet<cint64>* changedCompatibleSet) {
					for (QSet<cint64>::const_iterator it = indirectlyChangedIndividualSet->constBegin(), itEnd = indirectlyChangedIndividualSet->constEnd(); it != itEnd; ++it) {
						cint64 indiID(*it);
						if (indiID+mIndiTrackOffset < mIndiTrackCount) {
							CReferredIndividualTrackingData* indiTrackData = &mIndiTrackVector[indiID+mIndiTrackOffset];
							if (indiTrackData) {
								if (indiTrackData->isExtended()) {
									return true;
								}
								if (indiTrackData->isReferred() && !changedCompatibleSet->contains(indiID+mIndiTrackOffset)) {
									return true;
								}
							}
						}
					}
					return false;
				}



				bool CReferredIndividualTrackingVector::areIndividualsAffected(CConsistenceData* consData) {
					CIncrementalConsistenceTaskData* incConsTaskData = dynamic_cast<CIncrementalConsistenceTaskData*>(consData);
					if (incConsTaskData) {
						//QSet<cint64>* indirectlyChangedIndividualSet = incConsTaskData->getIndirectlyChangedNodeSet();
						//QSet<cint64>* changedCompatibleSet = incConsTaskData->getChangedCompatibleNodeSet();
						//for (QSet<cint64>::const_iterator it = indirectlyChangedIndividualSet->constBegin(), itEnd = indirectlyChangedIndividualSet->constEnd(); it != itEnd; ++it) {
						//	cint64 indiID(*it);
						//	if (indiID+mIndiTrackOffset < mIndiTrackCount) {
						//		CReferredIndividualTrackingData* indiTrackData = &mIndiTrackVector[indiID+mIndiTrackOffset];
						//		if (indiTrackData) {
						//			if (indiTrackData->isExtended()) {
						//				CIndividualProcessNode* indiNode = incConsTaskData->getCompletionGraphCachedSatisfiableTask()->getProcessingDataBox()->getIndividualProcessNodeVector()->getData(indiID+mIndiTrackOffset);
						//				return true;
						//			}
						//			if (indiTrackData->isReferred() && !changedCompatibleSet->contains(indiID+mIndiTrackOffset)) {
						//				CIndividualProcessNode* indiNode = incConsTaskData->getCompletionGraphCachedSatisfiableTask()->getProcessingDataBox()->getIndividualProcessNodeVector()->getData(indiID+mIndiTrackOffset);
						//				return true;
						//			}
						//		}
						//	}
						//}
						return areIndividualsAffected(incConsTaskData->getIndirectlyChangedNodeSet(),incConsTaskData->getChangedCompatibleNodeSet());
					}
					return true;
				}


				CIndividualDependenceTracking* CReferredIndividualTrackingVector::getCopiedIndividualDependencyTracking() {
					CReferredIndividualTrackingVector* indTrackVecCopy = new CReferredIndividualTrackingVector();
					indTrackVecCopy->initReferredIndividualTrackingVector(this);
					return indTrackVecCopy;
				}

				cint64 CReferredIndividualTrackingVector::getDependenceSize() {
					cint64 depCount = 0;
					for (cint64 i = 0; i < mIndiTrackCount; ++i) {
						CReferredIndividualTrackingData* indiTrackData = &mIndiTrackVector[i];
						if (indiTrackData->isReferred()) {
							++depCount;
						}
					}
					return depCount;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

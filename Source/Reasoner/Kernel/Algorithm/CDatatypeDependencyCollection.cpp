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

#include "CDatatypeDependencyCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CDatatypeDependencyCollection::CDatatypeDependencyCollection(CCalculationAlgorithmContext* collectionCalcAlgContext) {
					mCollectionCalcAlgContext = collectionCalcAlgContext;
					mCollectedFirstDependency = nullptr;
					mCollectedAdditionalDependencies = nullptr;
					mCollectionDatatypeDepCreated = false;
					mCollectionDataTriggerDepTrackPoint = nullptr;
					mDepTrackPointSet = CObjectParameterizingAllocator< CPROCESSINGSET<CDependencyTrackPoint*>, CContext* >::allocateAndConstructAndParameterize(mCollectionCalcAlgContext->getUsedTemporaryMemoryAllocationManager(),mCollectionCalcAlgContext->getTaskProcessorContext());
					mTestDepTrackPointSet = mDepTrackPointSet;
				}

				CDatatypeDependencyCollection::CDatatypeDependencyCollection(CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* collectionCalcAlgContext) {
					mCollectionCalcAlgContext = collectionCalcAlgContext;
					mCollectedFirstDependency = depCollection->mCollectedFirstDependency;
					mCollectedAdditionalDependencies = depCollection->mCollectedAdditionalDependencies;
					mCollectionDatatypeDepCreated = depCollection->mCollectionDatatypeDepCreated;
					mCollectionDataTriggerDepTrackPoint = depCollection->mCollectionDataTriggerDepTrackPoint;
					mDepTrackPointSet = nullptr;
					mTestDepTrackPointSet = depCollection->mTestDepTrackPointSet;
				}



				bool CDatatypeDependencyCollection::hasDependencies() {
					return mCollectedFirstDependency != nullptr;
				}


				bool CDatatypeDependencyCollection::addDependency(CDependencyTrackPoint* depTrackPoint) {
					if (depTrackPoint && mCollectionCalcAlgContext) {
						if (!mTestDepTrackPointSet || !mTestDepTrackPointSet->contains(depTrackPoint)) {
							if (mDepTrackPointSet) {
								mDepTrackPointSet->insert(depTrackPoint);
							}
							mCollectionDatatypeDepCreated = false;
							CDATATYPECONNECTIONDependencyNode* connDep = createDATATYPECONNECTIONDependency(depTrackPoint, mCollectionCalcAlgContext);
							if (connDep) {
								if (!mCollectedFirstDependency) {
									mCollectedFirstDependency = connDep;
								} else {
									mCollectedAdditionalDependencies = connDep->append(mCollectedAdditionalDependencies);
								}
							}
							return true;
						}
					}
					return false;
				}



				CDependencyTrackPoint* CDatatypeDependencyCollection::getDatatypeTriggerDependencyTrackPoint(bool create) {
					if (!mCollectionDatatypeDepCreated || mCollectedFirstDependency) {
						mCollectionDatatypeDepCreated = true;
						createDATATYPETRIGGERDependency(mCollectionDataTriggerDepTrackPoint,mCollectedFirstDependency,mCollectedAdditionalDependencies,mCollectionCalcAlgContext);
					}
					return mCollectionDataTriggerDepTrackPoint;
				}



				CDATATYPETRIGGERDependencyNode* CDatatypeDependencyCollection::createDATATYPETRIGGERDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContext* calcAlgContext) {
					CDATATYPETRIGGERDependencyNode* depNode = nullptr;
					depNode = calcAlgContext->getUsedDependencyFactory()->createDATATYPETRIGGERDependency(expContinueDepTrackPoint,prevDepTrackPoint,prevOtherDependencies,calcAlgContext);
					return depNode;
				}


				CDATATYPECONNECTIONDependencyNode* CDatatypeDependencyCollection::createDATATYPECONNECTIONDependency(CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDATATYPECONNECTIONDependencyNode* depNode = nullptr;
					depNode = calcAlgContext->getUsedDependencyFactory()->createDATATYPECONNECTIONDependency(prevDepTrackPoint,calcAlgContext);
					return depNode;
				}






			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CReuseCompletionGraphCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CReuseCompletionGraphCacheHandler::CReuseCompletionGraphCacheHandler(CReuseCompletionGraphCacheReader* reuseCompGraphCacheReader, CReuseCompletionGraphCacheWriter* reuseCompGraphCacheWriter) {
					mReuseCompGraphCacheReader = reuseCompGraphCacheReader;
					mReuseCompGraphCacheWriter = reuseCompGraphCacheWriter;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}


				CReuseCompletionGraphCacheHandler::~CReuseCompletionGraphCacheHandler() {
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheHandler::getReuseCompletionGraphEntry(CIndividualProcessNode* indiProcNode, bool* minimalCompletionGraph, bool* deterministicConnection, CCalculationAlgorithmContext* calcAlgContext) {
					CReuseCompletionGraphCacheEntry* cacheEntry = nullptr;
					if (mReuseCompGraphCacheReader) {
						CProcessingDataBox* dataBox = calcAlgContext->getUsedProcessingDataBox();
						if (indiProcNode) {
							CConcept* initConcept = nullptr;
							CXSortedNegLinker<CConcept*>* initConceptLinkerIt = indiProcNode->getProcessInitializingConceptLinkerIt();
								
							while (!initConcept && initConceptLinkerIt) {
								if (!initConceptLinkerIt->isNegated()) {
									CConcept* tmpConcept = initConceptLinkerIt->getData();
									if (tmpConcept->hasClassName()) {
										initConcept = tmpConcept;
									}
								}
								initConceptLinkerIt = initConceptLinkerIt->getNext();
							}
							if (initConcept) {
								cacheEntry = mReuseCompGraphCacheReader->getCacheEntry(initConcept,minimalCompletionGraph,deterministicConnection);
							}
						}
					}
					return cacheEntry;
				}



				bool CReuseCompletionGraphCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager reuseCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&reuseCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CReuseCompletionGraphCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CReuseCompletionGraphCacheEntryWriteData* commitWriteData = nullptr;
						CReuseCompletionGraphCacheEntryWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CReuseCompletionGraphCacheEntryWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CReuseCompletionGraphCacheEntryWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CReuseCompletionGraphCacheEntryWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mReuseCompGraphCacheWriter->writeExpandCache(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CReuseCompletionGraphCacheHandler::addCacheMessages(CReuseCompletionGraphCacheEntryWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CReuseCompletionGraphCacheEntryWriteData*)expandWriteData->append(mWriteData);
					} else {
						mWriteData = expandWriteData;
					}
					return true;
				}






			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

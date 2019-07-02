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

#include "CNodeSwitchHistory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CNodeSwitchHistory::CNodeSwitchHistory(CProcessContext* processContext) : mContext(processContext) {
					mDownData = nullptr;
					mLevelingCount = 5;
				}


				CNodeSwitchHistory::~CNodeSwitchHistory() {
				}


				CNodeSwitchHistory* CNodeSwitchHistory::initSwitchHistory(CNodeSwitchHistory* prevSwitchHistory) {
					if (prevSwitchHistory) {
						CNodeSwitchHistoryLinkData* prevDownData = prevSwitchHistory->mDownData;
						mDownData = prevDownData;
					} else {
						mDownData = nullptr;
					}
					return this;
				}


				CNodeSwitchHistory* CNodeSwitchHistory::addIndividualProcessNodeSwitch(CIndividualProcessNode* individual, cint64 indiSwitchTag) {
					cint64 indiAncDepth = individual->getIndividualAncestorDepth();
					return addIndividualProcessNodeSwitch(indiAncDepth,individual->getIndividualID(),indiSwitchTag);
				}


				CNodeSwitchHistory* CNodeSwitchHistory::updateLastIndividualProcessNodeSwitch(cint64 indiAncDepth, cint64 indiID) {
					CNodeSwitchHistoryLinkData* data = mDownData;
					while (data) {
						data->mNodeAncestorDepth = qMin(data->mNodeAncestorDepth,indiAncDepth);
						data->mNodeIndividualID = qMin(data->mNodeIndividualID,indiID);		
						data = data->mUpData;
					}
					return this;
				}


				CNodeSwitchHistory* CNodeSwitchHistory::addIndividualProcessNodeSwitch(cint64 indiAncDepth, cint64 indiID, cint64 indiSwitchTag) {
					CNodeSwitchHistoryLinkData* newData = CObjectAllocator< CNodeSwitchHistoryLinkData >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
					newData->mNextData = mDownData;
					if (mDownData) {
						newData->mLevelCount = mDownData->mLevelCount;
					}
					newData->mNodeSwitchTag = indiSwitchTag;
					newData->mNodeAncestorDepth = indiAncDepth;
					newData->mNodeIndividualID = indiID;
					++newData->mLevelCount;
					mDownData = newData;
					updateUpperData(newData,indiSwitchTag);
					return this;
				}


				CNodeSwitchHistory* CNodeSwitchHistory::updateUpperData(CNodeSwitchHistoryLinkData* data, cint64 indiSwitchTag) {
					if ((data->mLevelCount % mLevelingCount) == 0) {
						CNodeSwitchHistoryLinkData* upperData = nullptr;
						CNodeSwitchHistoryLinkData* tmpData = data;
						cint64 minAncDepth = std::numeric_limits<cint64>::max();
						cint64 minIndiID = std::numeric_limits<cint64>::max();
						for (cint64 i = 0; i < mLevelingCount; ++i, tmpData = tmpData->mNextData) {
							minAncDepth = qMin(minAncDepth,tmpData->mNodeAncestorDepth);
							minIndiID = qMin(minIndiID,tmpData->mNodeIndividualID);
						}
						if (tmpData && tmpData->mUpData) {
							upperData = tmpData->mUpData;
						}
						CNodeSwitchHistoryLinkData* newUpperData = CObjectAllocator< CNodeSwitchHistoryLinkData >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
						if (upperData) {
							newUpperData->mLevelCount = upperData->mLevelCount;
						}
						newUpperData->mNextData = upperData;
						newUpperData->mNodeSwitchTag = indiSwitchTag;
						newUpperData->mDownData = data;
						data->mUpData = newUpperData;
						++newUpperData->mLevelCount;
						newUpperData->mNodeAncestorDepth = minAncDepth;
						newUpperData->mNodeIndividualID = minIndiID;
						updateUpperData(newUpperData,indiSwitchTag);
					}
					return this;
				}


				cint64 CNodeSwitchHistory::getMinIndividualAncestorDepth(cint64 indiSwitchTag) {					
					cint64 minAncDepth,minIndiID;
					getMinIndividualAncestorDepthAndNodeID(indiSwitchTag,minAncDepth,minIndiID);
					return minAncDepth;
				}



				cint64 CNodeSwitchHistory::getMinIndividualNodeID(cint64 indiSwitchTag) {					
					cint64 minAncDepth,minIndiID;
					getMinIndividualAncestorDepthAndNodeID(indiSwitchTag,minAncDepth,minIndiID);
					return minIndiID;
				}


				bool CNodeSwitchHistory::getMinIndividualAncestorDepthAndNodeID(cint64 indiSwitchTag, cint64& minAncDepth, cint64& minIndiID) {
					minAncDepth = std::numeric_limits<cint64>::max();
					minIndiID = std::numeric_limits<cint64>::max();
					CNodeSwitchHistoryLinkData* data = mDownData;
					// first walk bottom up
					while (data && data->mNextData) {
						if (data->mNodeSwitchTag == indiSwitchTag) {
							return true;
						}
						if (data->mNextData->mNodeSwitchTag == indiSwitchTag) {
							minIndiID = qMin(minIndiID,data->mNodeIndividualID);
							minAncDepth = qMin(minAncDepth,data->mNodeAncestorDepth);
							return true;
						}
						if (data->mNextData->mNodeSwitchTag < indiSwitchTag) {
							break;
						}
						if (data->mUpData) {
							data = data->mUpData;
						} else {
							minIndiID = qMin(minIndiID,data->mNodeIndividualID);
							minAncDepth = qMin(minAncDepth,data->mNodeAncestorDepth);
							data = data->mNextData;
						}
					}
					// second walk top down
					while (data) {
						while (data->mNextData && data->mNextData->mNodeSwitchTag > indiSwitchTag) {
							minIndiID = qMin(minIndiID,data->mNodeIndividualID);
							minAncDepth = qMin(minAncDepth,data->mNodeAncestorDepth);
							data = data->mNextData;
						}
						if (data->mNextData && data->mNextData->mNodeSwitchTag == indiSwitchTag) {
							minIndiID = qMin(minIndiID,data->mNodeIndividualID);
							minAncDepth = qMin(minAncDepth,data->mNodeAncestorDepth);
							return true;
						} else {
							if (data->mDownData) {
								// go down
								data = data->mDownData;
							} else {
								if (data->mNodeSwitchTag > indiSwitchTag) {
									minIndiID = qMin(minIndiID,data->mNodeIndividualID);
									minAncDepth = qMin(minAncDepth,data->mNodeAncestorDepth);
								}
								data = data->mNextData;
							}
						}
					}
					return true;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

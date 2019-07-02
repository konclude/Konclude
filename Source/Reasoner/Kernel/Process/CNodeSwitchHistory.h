/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CNODESWITCHHISTORY_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CNODESWITCHHISTORY_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNode.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

#include <limits>

namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CNodeSwitchHistory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CNodeSwitchHistory {
					// public methods
					public:
						//! Constructor
						CNodeSwitchHistory(CProcessContext* processContext = nullptr);

						//! Destructor
						virtual ~CNodeSwitchHistory();

						CNodeSwitchHistory* initSwitchHistory(CNodeSwitchHistory* prevSwitchHistory);
						CNodeSwitchHistory* addIndividualProcessNodeSwitch(CIndividualProcessNode* individual, cint64 indiSwitchTag);
						CNodeSwitchHistory* addIndividualProcessNodeSwitch(cint64 indiAncDepth, cint64 indiID, cint64 indiSwitchTag);

						CNodeSwitchHistory* updateLastIndividualProcessNodeSwitch(cint64 indiAncDepth, cint64 indiID);


						cint64 getMinIndividualAncestorDepth(cint64 indiSwitchTag);
						cint64 getMinIndividualNodeID(cint64 indiSwitchTag);

						bool getMinIndividualAncestorDepthAndNodeID(cint64 indiSwitchTag, cint64& indiAncDepth, cint64& indiNodeID);


					// protected methods
					protected:

					// protected variables
					protected:
						class CNodeSwitchHistoryLinkData {
							public:
								inline CNodeSwitchHistoryLinkData() {
									mNextData = nullptr;
									mUpData = nullptr;
									mDownData = nullptr;
									mLevelCount = 0;
									mNodeSwitchTag = 0;
									mNodeAncestorDepth = 0;
									mNodeIndividualID = 0;
								}

								inline CNodeSwitchHistoryLinkData* initCopy(CNodeSwitchHistoryLinkData* prevData) {
									mNextData = prevData->mNextData;
									mUpData = nullptr;
									mDownData = prevData->mDownData;
									mLevelCount = prevData->mLevelCount;
									mNodeSwitchTag = prevData->mNodeSwitchTag;
									mNodeAncestorDepth = prevData->mNodeAncestorDepth;
									mNodeIndividualID = prevData->mNodeIndividualID;
									return this;
								}

								CNodeSwitchHistoryLinkData* mNextData;
								CNodeSwitchHistoryLinkData* mUpData;
								CNodeSwitchHistoryLinkData* mDownData;
								cint64 mLevelCount;
								cint64 mNodeSwitchTag;
								cint64 mNodeAncestorDepth;
								cint64 mNodeIndividualID;
						};

						CProcessContext* mContext;
						CNodeSwitchHistoryLinkData* mDownData;
						cint64 mLevelingCount;

					// private methods
					private:
						CNodeSwitchHistory* updateUpperData(CNodeSwitchHistoryLinkData* data, cint64 indiSwitchTag);

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CNODESWITCHHISTORY_H

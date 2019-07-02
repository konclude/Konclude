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

#include "CBlockingFollowUpdateTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingFollowUpdateTag::CBlockingFollowUpdateTag(cint64 blockingFollowTag) : CProcessTag(blockingFollowTag) {
				}


				CBlockingFollowUpdateTag::~CBlockingFollowUpdateTag() {
				}

				cint64 CBlockingFollowUpdateTag::getBlockingFollowTag() {
					return CProcessTag::getProcessTag();
				}

				CBlockingFollowUpdateTag* CBlockingFollowUpdateTag::setBlockingFollowTag(cint64 blockingFollowTag) {
					CProcessTag::setProcessTag(blockingFollowTag);
					return this;
				}

				CBlockingFollowUpdateTag* CBlockingFollowUpdateTag::setBlockingFollowTag(CProcessTagger* processTagger) {
					return setBlockingFollowTag(processTagger->getCurrentBlockingFollowTag());
				}


				CBlockingFollowUpdateTag* CBlockingFollowUpdateTag::initBlockingFollowTag(cint64 blockingFollowTag) {
					CProcessTag::initProcessTag(blockingFollowTag);
					return this;
				}

				CBlockingFollowUpdateTag* CBlockingFollowUpdateTag::initBlockingFollowTag(CProcessTagger* processTagger) {
					return initBlockingFollowTag(processTagger->getCurrentBlockingFollowTag());
				}

				bool CBlockingFollowUpdateTag::isBlockingFollowTagUpdated(cint64 blockingFollowTag) {
					return CProcessTag::isProcessTagUpdated(blockingFollowTag);
				}

				bool CBlockingFollowUpdateTag::isBlockingFollowTagUpToDate(cint64 blockingFollowTag) {
					return CProcessTag::isProcessTagUpToDate(blockingFollowTag);
				}

				bool CBlockingFollowUpdateTag::updateBlockingFollowTag(cint64 blockingFollowTag) {
					return CProcessTag::updateProcessTag(blockingFollowTag);
				}

				bool CBlockingFollowUpdateTag::updateBlockingFollowTag(CProcessTagger* processTagger) {
					return updateBlockingFollowTag(processTagger->getCurrentBlockingFollowTag());
				}

				bool CBlockingFollowUpdateTag::isBlockingFollowTagUpdated(CProcessTagger* processTagger) {
					return isBlockingFollowTagUpdated(processTagger->getCurrentBlockingFollowTag());
				}

				bool CBlockingFollowUpdateTag::isBlockingFollowTagUpToDate(CProcessTagger* processTagger) {
					return isBlockingFollowTagUpToDate(processTagger->getCurrentBlockingFollowTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

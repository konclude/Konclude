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

#include "CBlockedTestTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockedTestTag::CBlockedTestTag(cint64 blockingAddTag) : CProcessTag(blockingAddTag) {
				}

				cint64 CBlockedTestTag::getBlockedTestTag() {
					return CProcessTag::getProcessTag();
				}

				CBlockedTestTag* CBlockedTestTag::setBlockedTestTag(cint64 blockingAddTag) {
					CProcessTag::setProcessTag(blockingAddTag);
					return this;
				}

				CBlockedTestTag* CBlockedTestTag::initBlockedTestTag(cint64 blockingAddTag) {
					CProcessTag::initProcessTag(blockingAddTag);
					return this;
				}

				bool CBlockedTestTag::isBlockedTestTagUpdated(cint64 blockingAddTag) {
					return CProcessTag::isProcessTagUpdated(blockingAddTag);
				}

				bool CBlockedTestTag::isBlockedTestTagUpToDate(cint64 blockingAddTag) {
					return CProcessTag::isProcessTagUpToDate(blockingAddTag);
				}

				bool CBlockedTestTag::updateBlockedTestTag(cint64 blockingAddTag) {
					return CProcessTag::updateProcessTag(blockingAddTag);
				}

				CBlockedTestTag* CBlockedTestTag::setBlockedTestTag(CProcessTagger* processTagger) {
					return setBlockedTestTag(processTagger->getCurrentBlockingAddTag());
				}

				CBlockedTestTag* CBlockedTestTag::initBlockedTestTag(CProcessTagger* processTagger) {
					return initBlockedTestTag(processTagger->getCurrentBlockingAddTag());
				}

				bool CBlockedTestTag::isBlockedTestTagUpdated(CProcessTagger* processTagger) {
					return isBlockedTestTagUpdated(processTagger->getCurrentBlockingAddTag());
				}

				bool CBlockedTestTag::isBlockedTestTagUpToDate(CProcessTagger* processTagger) {
					return isBlockedTestTagUpToDate(processTagger->getCurrentBlockingAddTag());
				}

				bool CBlockedTestTag::updateBlockedTestTag(CProcessTagger* processTagger) {
					return updateBlockedTestTag(processTagger->getCurrentBlockingAddTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

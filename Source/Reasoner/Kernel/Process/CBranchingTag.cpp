/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CBranchingTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBranchingTag::CBranchingTag(cint64 branchingTag) : CProcessTag(branchingTag) {
				}


				cint64 CBranchingTag::getBranchingTag() {
					return CProcessTag::getProcessTag();
				}

				CBranchingTag* CBranchingTag::setBranchingTag(cint64 branchingTag) {
					CProcessTag::setProcessTag(branchingTag);
					return this;
				}

				CBranchingTag* CBranchingTag::initBranchingTag(cint64 branchingTag) {
					CProcessTag::initProcessTag(branchingTag);
					return this;
				}

				bool CBranchingTag::isBranchingTagUpdated(cint64 branchingTag) {
					return CProcessTag::isProcessTagUpdated(branchingTag);
				}

				bool CBranchingTag::isBranchingTagUpToDate(cint64 branchingTag) {
					return CProcessTag::isProcessTagUpToDate(branchingTag);
				}

				bool CBranchingTag::updateBranchingTag(cint64 branchingTag) {
					return CProcessTag::updateProcessTag(branchingTag);
				}


				bool CBranchingTag::addMaximumBranchingTagCandidate(cint64 branchingTag) {
					if (branchingTag > mProcessTag || branchingTag < 0) {
						mProcessTag = branchingTag;
						return true;
					} 
					return false;
				}


				CBranchingTag* CBranchingTag::incBranchingTag(cint64 incCount) {
					++mProcessTag;
					return this;
				}


				CBranchingTag* CBranchingTag::setBranchingTag(CProcessTagger* processTagger) {
					return setBranchingTag(processTagger->getCurrentBranchingTag());
				}

				CBranchingTag* CBranchingTag::initBranchingTag(CProcessTagger* processTagger) {
					return initBranchingTag(processTagger->getCurrentBranchingTag());
				}

				bool CBranchingTag::isBranchingTagUpdated(CProcessTagger* processTagger) {
					return isProcessTagUpdated(processTagger->getCurrentBranchingTag());
				}

				bool CBranchingTag::isBranchingTagUpToDate(CProcessTagger* processTagger) {
					return isProcessTagUpToDate(processTagger->getCurrentBranchingTag());
				}

				bool CBranchingTag::updateBranchingTag(CProcessTagger* processTagger) {
					return updateBranchingTag(processTagger->getCurrentBranchingTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

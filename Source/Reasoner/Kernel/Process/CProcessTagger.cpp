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

#include "CProcessTagger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessTagger::CProcessTagger() {
					mLocalizationTag = 0;
					mProcessingTag = 0;
					mBranchingTag = 0;
					mBlockingAddTag = 0;
					mNodeSwitchTag = 0;
					mConceptLabelSetModificationTag = 0;
					mCurrentBlockingFollowTag = 0;
				}


				CProcessTagger::~CProcessTagger() {
				}


				CProcessTagger* CProcessTagger::initProcessTagger(CProcessTagger* processTagger) {
					if (processTagger) {
						mLocalizationTag = processTagger->mLocalizationTag;
						mProcessingTag = processTagger->mProcessingTag;
						mBranchingTag = processTagger->mBranchingTag;
						mBlockingAddTag = processTagger->mBlockingAddTag;
						mNodeSwitchTag = processTagger->mNodeSwitchTag;
						mConceptLabelSetModificationTag = processTagger->mConceptLabelSetModificationTag;
						mCurrentBlockingFollowTag = processTagger->mCurrentBlockingFollowTag;
					} else {
						mLocalizationTag = 0;
						mProcessingTag = 0;
						mBranchingTag = 0;
						mBlockingAddTag = 0;
						mNodeSwitchTag = 0;
						mConceptLabelSetModificationTag = 0;
						mCurrentBlockingFollowTag = 0;
					}
					return this;
				}



				CProcessTagger* CProcessTagger::setCurrentLocalizationTag(cint64 locTag) {
					mLocalizationTag = locTag;
					return this;
				}

				cint64 CProcessTagger::getCurrentLocalizationTag() {
					return mLocalizationTag;
				}

				cint64 CProcessTagger::getCurrentProcessingTag() {
					return mProcessingTag;
				}

				cint64 CProcessTagger::getCurrentProcessingTagAndInc() {
					return mProcessingTag++;
				}


				cint64 CProcessTagger::getCurrentBranchingTag() {
					return mBranchingTag;
				}

				cint64 CProcessTagger::getCurrentBlockingAddTag() {
					return mBlockingAddTag;
				}

				cint64 CProcessTagger::getCurrentNodeSwitchTag() {
					return mNodeSwitchTag;
				}

				cint64 CProcessTagger::getCurrentConceptLabelSetModificationTag() {
					return mConceptLabelSetModificationTag;
				}

				cint64 CProcessTagger::getCurrentBlockingFollowTag() {
					return mCurrentBlockingFollowTag;
				}

				CProcessTagger* CProcessTagger::incLocalizationTag() {
					++mLocalizationTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incProcessingTag() {
					++mProcessingTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incBranchingTag() {
					++mBranchingTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incBlockingAddTag() {
					++mBlockingAddTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incNodeSwitchTag() {
					++mNodeSwitchTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incConceptLabelSetModificationTag() {
					++mConceptLabelSetModificationTag;
					return this;
				}

				CProcessTagger* CProcessTagger::incCurrentBlockingFollowTag() {
					++mCurrentBlockingFollowTag;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

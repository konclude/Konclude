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

#include "CProcessingTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessingTag::CProcessingTag(cint64 processingTag) : CProcessTag(processingTag) {
				}


				CProcessingTag::~CProcessingTag() {
				}

				cint64 CProcessingTag::getProcessingTag() {
					return CProcessTag::getProcessTag();
				}

				CProcessingTag* CProcessingTag::setProcessingTag(cint64 processingTag) {
					CProcessTag::setProcessTag(processingTag);
					return this;
				}

				CProcessingTag* CProcessingTag::initProcessingTag(cint64 processingTag) {
					CProcessTag::initProcessTag(processingTag);
					return this;
				}

				bool CProcessingTag::isProcessingTagUpdated(cint64 processingTag) {
					return CProcessTag::isProcessTagUpdated(processingTag);
				}

				bool CProcessingTag::isProcessingTagUpToDate(cint64 processingTag) {
					return CProcessTag::isProcessTagUpToDate(processingTag);
				}

				bool CProcessingTag::updateProcessingTag(cint64 processingTag) {
					return CProcessTag::updateProcessTag(processingTag);
				}

				CProcessingTag* CProcessingTag::setProcessingTag(CProcessTagger* processTagger) {
					return setProcessingTag(processTagger->getCurrentProcessingTag());
				}

				CProcessingTag* CProcessingTag::initProcessingTag(CProcessTagger* processTagger) {
					return initProcessingTag(processTagger->getCurrentProcessingTag());
				}

				bool CProcessingTag::isProcessingTagUpdated(CProcessTagger* processTagger) {
					return isProcessingTagUpdated(processTagger->getCurrentProcessingTag());
				}

				bool CProcessingTag::isProcessingTagUpToDate(CProcessTagger* processTagger) {
					return isProcessingTagUpToDate(processTagger->getCurrentProcessingTag());
				}

				bool CProcessingTag::updateProcessingTag(CProcessTagger* processTagger) {
					return updateProcessingTag(processTagger->getCurrentProcessingTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

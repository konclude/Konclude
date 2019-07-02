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

#include "CConceptLabelSetModificationTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptLabelSetModificationTag::CConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag) : CProcessTag(conceptLabelSetModificationTag) {
				}


				CConceptLabelSetModificationTag::~CConceptLabelSetModificationTag() {
				}

				cint64 CConceptLabelSetModificationTag::getConceptLabelSetModificationTag() {
					return CProcessTag::getProcessTag();
				}

				CConceptLabelSetModificationTag* CConceptLabelSetModificationTag::setConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag) {
					CProcessTag::setProcessTag(conceptLabelSetModificationTag);
					return this;
				}

				CConceptLabelSetModificationTag* CConceptLabelSetModificationTag::initConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag) {
					CProcessTag::initProcessTag(conceptLabelSetModificationTag);
					return this;
				}

				bool CConceptLabelSetModificationTag::isConceptLabelSetModificationTagUpdated(cint64 conceptLabelSetModificationTag) {
					return CProcessTag::isProcessTagUpdated(conceptLabelSetModificationTag);
				}

				bool CConceptLabelSetModificationTag::isConceptLabelSetModificationTagUpToDate(cint64 conceptLabelSetModificationTag) {
					return CProcessTag::isProcessTagUpToDate(conceptLabelSetModificationTag);
				}

				bool CConceptLabelSetModificationTag::updateConceptLabelSetModificationTag(cint64 conceptLabelSetModificationTag) {
					return CProcessTag::updateProcessTag(conceptLabelSetModificationTag);
				}

				CConceptLabelSetModificationTag* CConceptLabelSetModificationTag::setConceptLabelSetModificationTag(CProcessTagger* processTagger) {
					return setConceptLabelSetModificationTag(processTagger->getCurrentConceptLabelSetModificationTag());
				}

				CConceptLabelSetModificationTag* CConceptLabelSetModificationTag::initConceptLabelSetModificationTag(CProcessTagger* processTagger) {
					return initConceptLabelSetModificationTag(processTagger->getCurrentConceptLabelSetModificationTag());
				}

				bool CConceptLabelSetModificationTag::isConceptLabelSetModificationTagUpdated(CProcessTagger* processTagger) {
					return isProcessTagUpdated(processTagger->getCurrentConceptLabelSetModificationTag());
				}

				bool CConceptLabelSetModificationTag::isConceptLabelSetModificationTagUpToDate(CProcessTagger* processTagger) {
					return isProcessTagUpToDate(processTagger->getCurrentConceptLabelSetModificationTag());
				}

				bool CConceptLabelSetModificationTag::updateConceptLabelSetModificationTag(CProcessTagger* processTagger) {
					return updateConceptLabelSetModificationTag(processTagger->getCurrentConceptLabelSetModificationTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

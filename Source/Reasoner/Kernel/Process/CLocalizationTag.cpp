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

#include "CLocalizationTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CLocalizationTag::CLocalizationTag(cint64 localizationTag) : CProcessTag(localizationTag) {
					mRelocalized = false;
				}

				cint64 CLocalizationTag::getLocalizationTag() {
					return CProcessTag::getProcessTag();
				}

				CLocalizationTag* CLocalizationTag::setLocalizationTag(cint64 localizationTag) {
					CProcessTag::setProcessTag(localizationTag);
					return this;
				}

				CLocalizationTag* CLocalizationTag::setLocalizationTag(CProcessTagger* processTagger) {
					return setLocalizationTag(processTagger->getCurrentLocalizationTag());
				}


				CLocalizationTag* CLocalizationTag::initLocalizationTag(cint64 localizationTag) {
					CProcessTag::initProcessTag(localizationTag);
					return this;
				}

				CLocalizationTag* CLocalizationTag::initLocalizationTag(CProcessTagger* processTagger) {
					return initLocalizationTag(processTagger->getCurrentLocalizationTag());
				}

				bool CLocalizationTag::isLocalizationTagUpdated(cint64 localizationTag) {
					return CProcessTag::isProcessTagUpdated(localizationTag);
				}

				bool CLocalizationTag::isLocalizationTagUpToDate(cint64 localizationTag) {
					return CProcessTag::isProcessTagUpToDate(localizationTag);
				}

				bool CLocalizationTag::updateLocalizationTag(cint64 localizationTag) {
					return CProcessTag::updateProcessTag(localizationTag);
				}

				bool CLocalizationTag::updateLocalizationTag(CProcessTagger* processTagger) {
					return updateLocalizationTag(processTagger->getCurrentLocalizationTag());
				}

				bool CLocalizationTag::isLocalizationTagUpdated(CProcessTagger* processTagger) {
					return isLocalizationTagUpdated(processTagger->getCurrentLocalizationTag());
				}

				bool CLocalizationTag::isLocalizationTagUpToDate(CProcessTagger* processTagger) {
					return isLocalizationTagUpToDate(processTagger->getCurrentLocalizationTag());
				}

				bool CLocalizationTag::isRelocalized() {
					return mRelocalized;
				}

				CLocalizationTag* CLocalizationTag::setRelocalized(bool relocalized) {
					if (mRelocalized != relocalized) {
						mRelocalized = relocalized;
					}
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

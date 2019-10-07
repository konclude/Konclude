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

#include "CNodeSwitchTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CNodeSwitchTag::CNodeSwitchTag(cint64 nodeSwitchTag) : CProcessTag(nodeSwitchTag) {
				}


				CNodeSwitchTag::~CNodeSwitchTag() {
				}

				cint64 CNodeSwitchTag::getNodeSwitchTag() {
					return CProcessTag::getProcessTag();
				}

				CNodeSwitchTag* CNodeSwitchTag::setNodeSwitchTag(cint64 nodeSwitchTag) {
					CProcessTag::setProcessTag(nodeSwitchTag);
					return this;
				}

				CNodeSwitchTag* CNodeSwitchTag::initNodeSwitchTag(cint64 nodeSwitchTag) {
					CProcessTag::initProcessTag(nodeSwitchTag);
					return this;
				}

				bool CNodeSwitchTag::isNodeSwitchTagUpdated(cint64 nodeSwitchTag) {
					return CProcessTag::isProcessTagUpdated(nodeSwitchTag);
				}

				bool CNodeSwitchTag::isNodeSwitchTagUpToDate(cint64 nodeSwitchTag) {
					return CProcessTag::isProcessTagUpToDate(nodeSwitchTag);
				}

				bool CNodeSwitchTag::updateNodeSwitchTag(cint64 nodeSwitchTag) {
					return CProcessTag::updateProcessTag(nodeSwitchTag);
				}

				CNodeSwitchTag* CNodeSwitchTag::setNodeSwitchTag(CProcessTagger* processTagger) {
					return setNodeSwitchTag(processTagger->getCurrentNodeSwitchTag());
				}

				CNodeSwitchTag* CNodeSwitchTag::initNodeSwitchTag(CProcessTagger* processTagger) {
					return initNodeSwitchTag(processTagger->getCurrentNodeSwitchTag());
				}

				bool CNodeSwitchTag::isNodeSwitchTagUpdated(CProcessTagger* processTagger) {
					return isNodeSwitchTagUpdated(processTagger->getCurrentNodeSwitchTag());
				}

				bool CNodeSwitchTag::isNodeSwitchTagUpToDate(CProcessTagger* processTagger) {
					return isNodeSwitchTagUpToDate(processTagger->getCurrentNodeSwitchTag());
				}

				bool CNodeSwitchTag::updateNodeSwitchTag(CProcessTagger* processTagger) {
					return updateNodeSwitchTag(processTagger->getCurrentNodeSwitchTag());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

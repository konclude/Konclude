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

#include "CREUSEBACKENDEXPANSIONMODESDependencyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CREUSEBACKENDEXPANSIONMODESDependencyNode::CREUSEBACKENDEXPANSIONMODESDependencyNode(CProcessContext* processContext) : CNonDeterministicDependencyNode(processContext) {
					}


					CREUSEBACKENDEXPANSIONMODESDependencyNode* CREUSEBACKENDEXPANSIONMODESDependencyNode::initREUSEBACKENDEXPANSIONMODESDependencyNode(CBranchTreeNode* branchNode, CDependencyTrackPoint* depTrackPoint) {
						initNonDeterministicDependencyNode(DNTREUSEBACKENDEXPANSIONMODESDEPENDENCY,branchNode,nullptr,nullptr);
						mDepTrackPoint = depTrackPoint;
						updateBranchingTags();

						mFixedReuseDepTrackPoint = nullptr;
						mPriorizedReuseDepTrackPoint = nullptr;
						mInvolvedIndividualIdLinker = nullptr;
						mAffectedIndividualIdLinker = nullptr;
						return this;
					}


					CNonDeterministicDependencyTrackPoint* CREUSEBACKENDEXPANSIONMODESDependencyNode::getFixedReuseDependencyTrackPoint() {
						return mFixedReuseDepTrackPoint;
					}

					CREUSEBACKENDEXPANSIONMODESDependencyNode* CREUSEBACKENDEXPANSIONMODESDependencyNode::setFixedReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						mFixedReuseDepTrackPoint = depTrackPoint;
						return this;
					}


					CNonDeterministicDependencyTrackPoint* CREUSEBACKENDEXPANSIONMODESDependencyNode::getPriorizedReuseDependencyTrackPoint() {
						return mPriorizedReuseDepTrackPoint;
					}

					CREUSEBACKENDEXPANSIONMODESDependencyNode* CREUSEBACKENDEXPANSIONMODESDependencyNode::setPriorizedReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						mPriorizedReuseDepTrackPoint = depTrackPoint;
						return this;
					}

					CXLinker<cint64>* CREUSEBACKENDEXPANSIONMODESDependencyNode::getAffectedIndividualIdLinker() {
						return mAffectedIndividualIdLinker;
					}

					bool CREUSEBACKENDEXPANSIONMODESDependencyNode::addAffectedIndividualIdLinker(CXLinker<cint64>* expectedLinker, CXLinker<cint64>* newLinker) {
						return mAffectedIndividualIdLinker.testAndSetOrdered(expectedLinker, newLinker);
					}

					CXLinker<cint64>* CREUSEBACKENDEXPANSIONMODESDependencyNode::getInvolvedIndividualIdLinker() {
						return mInvolvedIndividualIdLinker;
					}

					bool CREUSEBACKENDEXPANSIONMODESDependencyNode::setInvolvedIndividualIdLinker(CXLinker<cint64>* newLinker) {
						mInvolvedIndividualIdLinker = newLinker;
						return this;
					}



				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

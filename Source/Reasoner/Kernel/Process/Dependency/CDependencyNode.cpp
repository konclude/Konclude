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

#include "CDependencyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CDependencyNode::CDependencyNode(CProcessContext* processContext) : CProcessingTag(processContext->getUsedProcessTagger()->getCurrentProcessingTagAndInc()) {
						mProcessContext = processContext;
						mDepNodeType = DNTUNDEFINED;
						//mAdditionalAheadDepLinker = nullptr;
						mAdditionalAfterDepLinker = nullptr;
					}



					CDependencyNode* CDependencyNode::initDependencyNode(DEPENDENCNODEYTYPE depType, CConceptDescriptor* conceptDescriptor) {
						mConceptDescriptor = conceptDescriptor;
						mDepNodeType = depType;
						//mMarker = nullptr;
						mIndividualNode = nullptr;
						return this;
					}

					CDependencyNode* CDependencyNode::initDependencyNode(DEPENDENCNODEYTYPE depType, CIndividualProcessNode* individualNode, CConceptDescriptor* conceptDescriptor) {
						initDependencyNode(depType,conceptDescriptor);
						mIndividualNode = individualNode;
						return this;
					}


					//CMarker* CDependencyNode::getMarker() {
					//	return mMarker;
					//}

					//CDependencyNode* CDependencyNode::setMarker(CMarker* depMarker) {
					//	mMarker = depMarker;
					//	return this;
					//}
					//
					//CDependencyNode* CDependencyNode::addMarker(CMarker* depMarker) {
					//	if (!mMarker) {
					//		mMarker = depMarker;
					//	} else {
					//		mMarker = depMarker->append(mMarker);
					//	}
					//	return this;
					//}


					bool CDependencyNode::isNonDeterministiDependencyNode() {
						return !isDeterministiDependencyNode();
					}



					CConceptDescriptor* CDependencyNode::getConceptDescriptor() {
						return mConceptDescriptor;
					}

					CDependencyNode* CDependencyNode::setConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
						mConceptDescriptor = conceptDescriptor;
						return this;
					}


					CDependencyTrackPoint* CDependencyNode::getPreviousDependencyTrackPoint() {
						return mDepTrackPoint;
					}


					CDependencyIterator CDependencyNode::getAdditionalDependencyIterator(bool aheadDependencies, bool afterDependencies) {
						return CDependencyIterator(/*aheadDependencies?mAdditionalAheadDepLinker:*/nullptr, afterDependencies?mAdditionalAfterDepLinker:nullptr);
					}

				
					//CDependency* CDependencyNode::getAdditionalAheadDependencies() {
					//	return mAdditionalAheadDepLinker;
					//}

					CDependency* CDependencyNode::getAdditionalAfterDependencies() {
						return mAdditionalAfterDepLinker;
					}

					cint64 CDependencyNode::getAdditionalDependencyCount() {
						cint64 depCount = 0;
						//if (mAdditionalAheadDepLinker) {
						//	depCount += mAdditionalAheadDepLinker->getCount();
						//}
						if (mAdditionalAfterDepLinker) {
							depCount += mAdditionalAfterDepLinker->getCount();
						}
						return depCount;
					}

					bool CDependencyNode::hasAdditionalDependencies() {
						return /*mAdditionalAheadDepLinker || */mAdditionalAfterDepLinker;
					}


					//CDependencyNode* CDependencyNode::addAheadDependency(CDependency* conceptDependency) {
					//	mAdditionalAheadDepLinker = conceptDependency->append(mAdditionalAheadDepLinker);
					//	return this;
					//}

					CDependencyNode* CDependencyNode::addAfterDependency(CDependency* linkDependency) {
						mAdditionalAfterDepLinker = linkDependency->append(mAdditionalAfterDepLinker);
						return this;
					}


					cint64 CDependencyNode::getDependedBranchingTag() {
						cint64 branchLevelTag = 0;
						if (mDepTrackPoint) {
							branchLevelTag = qMax(branchLevelTag,mDepTrackPoint->getBranchingTag());
						}
						//CDependency* conDepLinkerIt = mAdditionalAheadDepLinker;
						//while (conDepLinkerIt && branchLevelTag >= 0) {
						//	CDependencyTrackPoint* trackPoint = conDepLinkerIt->getPreviousDependencyTrackPoint();
						//	if (trackPoint) {
						//		branchLevelTag = qMax(branchLevelTag,trackPoint->getBranchingTag());
						//	} else {
						//		branchLevelTag = -1;
						//	}
						//	conDepLinkerIt = conDepLinkerIt->getNext();
						//}
						CDependency* linkDepLinkerIt = mAdditionalAfterDepLinker;
						while (linkDepLinkerIt && branchLevelTag >= 0) {
							CDependencyTrackPoint* trackPoint = linkDepLinkerIt->getPreviousDependencyTrackPoint();
							if (trackPoint) {
								branchLevelTag = qMax(branchLevelTag,trackPoint->getBranchingTag());
							} else {
								branchLevelTag = -1;
							}
							linkDepLinkerIt = linkDepLinkerIt->getNext();
						}
						return branchLevelTag;
					}

					cint64 CDependencyNode::getDependedBranchingLevel() {
						return getDependedBranchingTag();
					}


					//bool CDependencyNode::hasAdditionalAheadDependencies() {
					//	return mAdditionalAheadDepLinker != nullptr;
					//}

					bool CDependencyNode::hasAdditionalAfterDependencies() {
						return mAdditionalAfterDepLinker != nullptr;
					}

					bool CDependencyNode::hasDependencies() {
						return /*mAdditionalAheadDepLinker || */mAdditionalAfterDepLinker || mDepTrackPoint;
					}

					cint64 CDependencyNode::getDependencyCount() {
						cint64 depCount = 0;
						if (mDepTrackPoint) {
							depCount += 1;
						}
						depCount += getAdditionalDependencyCount();
						return depCount;
					}

					CDependencyNode::DEPENDENCNODEYTYPE CDependencyNode::getDependencyType() {
						return mDepNodeType;
					}

					bool CDependencyNode::isDependencyType(DEPENDENCNODEYTYPE depType) {
						return mDepNodeType == depType;
					}


					bool CDependencyNode::isIndependentBaseDependencyType() {
						return isDependencyType(DNTINDEPENDENTBASE);
					}

					bool CDependencyNode::isUndefinedDependencyType() {
						return isDependencyType(DNTUNDEFINED);
					}

					CIndividualProcessNode* CDependencyNode::getAppropriateIndividualNode() {
						return mIndividualNode;
					}

					bool CDependencyNode::hasAppropriateIndividualNode() {
						return mIndividualNode != nullptr;
					}

					CDependencyNode* CDependencyNode::setAppropriateIndividualNode(CIndividualProcessNode* indiNode) {
						mIndividualNode = indiNode;
						return this;
					}


					bool CDependencyNode::updateDependencyTrackPointBranchingTag(CDependencyTrackPoint* depTrackPoint, cint64 branchinLevelTag) {
						return depTrackPoint->addMaximumBranchingTagCandidate(branchinLevelTag);
					}

					bool CDependencyNode::isRepresentativeResolveDependencyNode() {
						return false;
					}

					bool CDependencyNode::isRepresentativeSelectDependencyNode() {
						return false;
					}

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

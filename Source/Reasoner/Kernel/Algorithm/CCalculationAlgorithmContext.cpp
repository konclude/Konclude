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

#include "CCalculationAlgorithmContext.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CCalculationAlgorithmContext::CCalculationAlgorithmContext() {
					mUsedProcessTagger = nullptr;
					mUsedTempMemMan = nullptr;
					mUsedPrTaskMemMan = nullptr;
					mUsedProcessContext = nullptr;
					mUsedProcessingDataBox = nullptr;
					mUsedIndividualPriorityStrategy = nullptr;
					mUsedConceptPriorityStrategy = nullptr;
					mUsedSatCalcTask = nullptr;
					mUsedTaskProcessorContext = nullptr;
					mUsedTaskPriorityStrategy = nullptr;
					mUsedProcessStatGath = nullptr;
					mMinModificationAncestorDepth = 0;
					mMinModificationIndividualID = 0;
					mMinModificationUpdated = false;
					mUsedBranchTreeNode = nullptr;
					mUsedBaseDepNode = nullptr;
					mUsedUnsatCacheHandler = nullptr;
					mUsedIndividualNodeManager = nullptr;
					mUsedClashDescriptorFactory = nullptr;
					mUsedUnsatCachRetStrategy = nullptr;
					mMaxCompletionGraphCachedIndiNodeID = std::numeric_limits < qint64 >::min();
					mCompletionGraphCachedLocalizationTag = std::numeric_limits < qint64 >::min();
					mCurrentIndiNode = nullptr;
				}


				CCalculationAlgorithmContext::~CCalculationAlgorithmContext() {
				}


				cint64 CCalculationAlgorithmContext::getMaxCompletionGraphCachedIndividualNodeID() {
					return mMaxCompletionGraphCachedIndiNodeID;
				}

				cint64 CCalculationAlgorithmContext::getCompletionGraphCachedLocalizationTag() {
					return mCompletionGraphCachedLocalizationTag;
				}

				bool CCalculationAlgorithmContext::hasCompletionGraphCachedIndividualNodes() {
					return mMaxCompletionGraphCachedIndiNodeID != std::numeric_limits < qint64 >::min();
				}


				bool CCalculationAlgorithmContext::isMinModificationUpdated() {
					return mMinModificationUpdated;
				}

				cint64 CCalculationAlgorithmContext::getMinModificationAncestorDepth() {
					return mMinModificationAncestorDepth;
				}

				CCalculationAlgorithmContext* CCalculationAlgorithmContext::setMinModificationAncestorDepth(cint64 ancDepth) {
					mMinModificationAncestorDepth = ancDepth;
					mMinModificationUpdated = false;
					return this;
				}

				CCalculationAlgorithmContext* CCalculationAlgorithmContext::setMinModificationIndividualID(cint64 indiID) {
					mMinModificationIndividualID = indiID;
					mMinModificationUpdated = false;
					return this;
				}

				cint64 CCalculationAlgorithmContext::getMinModificationIndividualID() {
					return mMinModificationIndividualID;
				}

				CCalculationAlgorithmContext* CCalculationAlgorithmContext::setMinModificationIndividual(CIndividualProcessNode* indiNode) {
					setMinModificationIndividualID(indiNode->getIndividualNodeID());
					setMinModificationAncestorDepth(indiNode->getIndividualAncestorDepth());
					return this;
				}

				CIndividualProcessNode* CCalculationAlgorithmContext::getCurrentIndividualNode() {
					return mCurrentIndiNode;
				}

				CCalculationAlgorithmContext* CCalculationAlgorithmContext::setCurrentIndividualNode(CIndividualProcessNode* indiNode) {
					mCurrentIndiNode = indiNode;
					return this;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

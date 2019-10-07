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

#include "CNodeEdge.h"
#include "CIndividualProcessNode.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CNodeEdge::CNodeEdge(CProcessContext* processContext) {
					mSourceIndividual = nullptr;
					mDestinationIndividual = nullptr;
					initLocalizationTag(processContext->getUsedProcessTagger());
				}

				CNodeEdge* CNodeEdge::initNodeEdge(CNodeEdge* nodeEdge) {
					mSourceIndividual = nodeEdge->mSourceIndividual;
					mDestinationIndividual = nodeEdge->mDestinationIndividual;
					initDependencyTracker(nodeEdge->mDependencyTrackPoint);
					return this;
				}

				CNodeEdge* CNodeEdge::initNodeEdge(CIndividualProcessNode* sourceIndividual, CIndividualProcessNode* destinationIndividual, CDependencyTrackPoint* depTrackPoint) {
					mSourceIndividual = sourceIndividual;
					mDestinationIndividual = destinationIndividual;
					initDependencyTracker(depTrackPoint);
					return this;
				}


				CIndividualProcessNode* CNodeEdge::getSourceIndividual() {
					return mSourceIndividual;
				}

				CIndividualProcessNode* CNodeEdge::getDestinationIndividual() {
					return mDestinationIndividual;
				}

				cint64 CNodeEdge::getSourceIndividualID() {
					cint64 indiID = 0;
					if (mSourceIndividual) {
						indiID = mSourceIndividual->getIndividualNodeID();
					}
					return indiID;
				}

				cint64 CNodeEdge::getDestinationIndividualID() {
					cint64 indiID = 0;
					if (mDestinationIndividual) {
						indiID = mDestinationIndividual->getIndividualNodeID();
					}
					return indiID;
				}


				CNodeEdge* CNodeEdge::setSourceIndividual(CIndividualProcessNode* sourceIndi) {
					mSourceIndividual = sourceIndi;
					return this;
				}

				CNodeEdge* CNodeEdge::setDestinationIndividual(CIndividualProcessNode* destinationIndi) {
					mDestinationIndividual = destinationIndi;
					return this;
				}

				CIndividualProcessNode* CNodeEdge::getOppositeIndividual(CIndividualProcessNode* indi) {
					if (indi) {
						return getOppositeIndividual(indi->getIndividualNodeID());
					}
					return nullptr;
				}

				CIndividualProcessNode* CNodeEdge::getOppositeIndividual(cint64 indiID) {
					if (mSourceIndividual->getIndividualNodeID() == indiID) {
						return mDestinationIndividual;
					}
					return mSourceIndividual;
				}

				cint64 CNodeEdge::getOppositeIndividualID(CIndividualProcessNode* indi) {
					if (indi) {
						return getOppositeIndividualID(indi->getIndividualNodeID());
					}
					return 0;
				}


				cint64 CNodeEdge::getOppositeIndividualID(cint64 indiID) {
					if (mSourceIndividual->getIndividualNodeID() == indiID) {
						return mDestinationIndividual->getIndividualNodeID();
					}
					return mSourceIndividual->getIndividualNodeID();
				}


				bool CNodeEdge::isDestinationIndividual(CIndividualProcessNode* indi) {
					return mDestinationIndividual == indi;
				}

				bool CNodeEdge::isDestinationIndividualID(CIndividualProcessNode* indi) {
					return mDestinationIndividual->getIndividualNodeID() == indi->getIndividualNodeID();
				}

				bool CNodeEdge::isDestinationIndividualID(cint64 indiID) {
					return mDestinationIndividual->getIndividualNodeID() == indiID;
				}

				bool CNodeEdge::isSourceIndividual(CIndividualProcessNode* indi) {
					return mSourceIndividual == indi;
				}

				bool CNodeEdge::isSourceIndividualID(CIndividualProcessNode* indi) {
					return mSourceIndividual->getIndividualNodeID() == indi->getIndividualNodeID();
				}

				bool CNodeEdge::isSourceIndividualID(cint64 indiID) {
					return mSourceIndividual->getIndividualNodeID() == indiID;
				}

				cint64 CNodeEdge::getCoupledIndividualID() {
					return mSourceIndividual->getIndividualNodeID() + mDestinationIndividual->getIndividualNodeID();
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

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
						indiID = mSourceIndividual->getIndividualID();
					}
					return indiID;
				}

				cint64 CNodeEdge::getDestinationIndividualID() {
					cint64 indiID = 0;
					if (mDestinationIndividual) {
						indiID = mDestinationIndividual->getIndividualID();
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
						return getOppositeIndividual(indi->getIndividualID());
					}
					return nullptr;
				}

				CIndividualProcessNode* CNodeEdge::getOppositeIndividual(cint64 indiID) {
					if (mSourceIndividual->getIndividualID() == indiID) {
						return mDestinationIndividual;
					}
					return mSourceIndividual;
				}

				cint64 CNodeEdge::getOppositeIndividualID(CIndividualProcessNode* indi) {
					if (indi) {
						return getOppositeIndividualID(indi->getIndividualID());
					}
					return 0;
				}


				cint64 CNodeEdge::getOppositeIndividualID(cint64 indiID) {
					if (mSourceIndividual->getIndividualID() == indiID) {
						return mDestinationIndividual->getIndividualID();
					}
					return mSourceIndividual->getIndividualID();
				}


				bool CNodeEdge::isDestinationIndividual(CIndividualProcessNode* indi) {
					return mDestinationIndividual == indi;
				}

				bool CNodeEdge::isDestinationIndividualID(CIndividualProcessNode* indi) {
					return mDestinationIndividual->getIndividualID() == indi->getIndividualID();
				}

				bool CNodeEdge::isDestinationIndividualID(cint64 indiID) {
					return mDestinationIndividual->getIndividualID() == indiID;
				}

				bool CNodeEdge::isSourceIndividual(CIndividualProcessNode* indi) {
					return mSourceIndividual == indi;
				}

				bool CNodeEdge::isSourceIndividualID(CIndividualProcessNode* indi) {
					return mSourceIndividual->getIndividualID() == indi->getIndividualID();
				}

				bool CNodeEdge::isSourceIndividualID(cint64 indiID) {
					return mSourceIndividual->getIndividualID() == indiID;
				}

				cint64 CNodeEdge::getCoupledIndividualID() {
					return mSourceIndividual->getIndividualID() + mDestinationIndividual->getIndividualID();
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

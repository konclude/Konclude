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

#include "CIndividualLinkEdge.h"
#include "CIndividualProcessNode.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualLinkEdge::CIndividualLinkEdge(CProcessContext* processContext) : CLinkEdge(processContext), CLinkerBase<CIndividualLinkEdge*,CIndividualLinkEdge>(this) {
					mCreatorIndividual = nullptr;
				}

				CIndividualLinkEdge* CIndividualLinkEdge::initIndividualLinkEdge(CIndividualLinkEdge* link) {
					initLinkEdge(link);
					mCreatorIndividual = link->mCreatorIndividual;
					return this;
				}

				CIndividualLinkEdge* CIndividualLinkEdge::initIndividualLinkEdge(CIndividualProcessNode* creatorIndividual, CIndividualProcessNode* sourceIndividual, CIndividualProcessNode* destinationIndividual, CRole* role, CDependencyTrackPoint* depTrackPoint) {
					initLinkEdge(sourceIndividual,destinationIndividual,role,depTrackPoint);
					mCreatorIndividual = creatorIndividual;
					return this;
				}

				CIndividualProcessNode* CIndividualLinkEdge::getCreatorIndividual() {
					return mCreatorIndividual;
				}

				cint64 CIndividualLinkEdge::getCreatorIndividualID() {
					return mCreatorIndividual->getIndividualNodeID();
				}

				bool CIndividualLinkEdge::isCreatorIndividual(CIndividualProcessNode* indi) {
					return mCreatorIndividual == indi;
				}

				bool CIndividualLinkEdge::isCreatorIndividualID(CIndividualProcessNode* indi) {
					return mCreatorIndividual->getIndividualNodeID() == indi->getIndividualNodeID();
				}

				bool CIndividualLinkEdge::isCreatorIndividualID(cint64 indiID) {
					return mCreatorIndividual->getIndividualNodeID() == indiID;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

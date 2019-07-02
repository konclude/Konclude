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

#include "CBackwardPilingPropagationLink.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBackwardPilingPropagationLink::CBackwardPilingPropagationLink() : CLinkerBase<CBackwardPilingPropagationLink*,CBackwardPilingPropagationLink>(this) {
					mRole = nullptr;
					mSourceIndividual = nullptr;
					mSourcePilingDepth = 0;
				}


				CBackwardPilingPropagationLink* CBackwardPilingPropagationLink::initBackwardPropagationLink(CIndividualPilingProcessNode* sourceIndividual, cint64 sourcePilingDepth, CRole* role, cint64 pilingDepth) {
					initPilingDepth(pilingDepth);
					mRole = role;
					mSourceIndividual = sourceIndividual;
					mSourcePilingDepth = sourcePilingDepth;
					return this;
				}


				CRole* CBackwardPilingPropagationLink::getLinkRole() {
					return mRole;
				}

				CBackwardPilingPropagationLink* CBackwardPilingPropagationLink::setLinkRole(CRole* role) {
					mRole = role;
					return this;
				}

				CIndividualPilingProcessNode* CBackwardPilingPropagationLink::getSourceIndividual() {
					return mSourceIndividual;
				}

				cint64 CBackwardPilingPropagationLink::getSourcePilingDepth() {
					return mSourcePilingDepth;
				}

				CBackwardPilingPropagationLink* CBackwardPilingPropagationLink::setSourcePilingDepth(cint64 pilingDepth) {
					mSourcePilingDepth = pilingDepth;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CSuccessorRoleHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSuccessorRoleHash::CSuccessorRoleHash(CProcessContext* context) : CPROCESSHASH<cint64,CIndividualLinkEdge*>(context),mContext(context) {
				}


				CSuccessorRoleHash* CSuccessorRoleHash::initSuccessorRoleHash(CSuccessorRoleHash* prevRoleSuccHash) {
					if (prevRoleSuccHash) {
						*this = *prevRoleSuccHash;
					} else {
						CPROCESSHASH<cint64,CIndividualLinkEdge*>::clear();
					}
					return this;
				}

				CSuccessorRoleHash* CSuccessorRoleHash::insertSuccessorRoleLink(cint64 indi, CIndividualLinkEdge* link) {
					CPROCESSHASH<cint64,CIndividualLinkEdge*>::insertMulti(indi,link);
					return this;
				}

				CSuccessorRoleIterator CSuccessorRoleHash::getSuccessorRoleIterator(cint64 indi) {
					return CSuccessorRoleIterator(indi,CPROCESSHASH<cint64,CIndividualLinkEdge*>::find(indi),CPROCESSHASH<cint64,CIndividualLinkEdge*>::end());
				}

				bool CSuccessorRoleHash::hasSuccessorIndividualNode(cint64 indi) {
					return CPROCESSHASH<cint64,CIndividualLinkEdge*>::contains(indi);
				}

				CSuccessorRoleHash* CSuccessorRoleHash::removeSuccessor(cint64 indi) {
					CPROCESSHASH<cint64,CIndividualLinkEdge*>::remove(indi);
					return this;
				}

				CSuccessorIterator CSuccessorRoleHash::getSuccessorIterator() {
					return CSuccessorIterator(CPROCESSHASH<cint64,CIndividualLinkEdge*>::begin(),CPROCESSHASH<cint64,CIndividualLinkEdge*>::end());
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

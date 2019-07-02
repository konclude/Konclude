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

#include "CLinkedRolePilingSuccessorHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CLinkedRolePilingSuccessorHash::CLinkedRolePilingSuccessorHash(CProcessContext* context) : mContext(context),mRoleSuccHash(context) {
				}


				CLinkedRolePilingSuccessorHash* CLinkedRolePilingSuccessorHash::initRoleSuccessorHash() {
					mRoleSuccHash.clear();
					return this;
				}

				CLinkedRolePilingSuccessorHash* CLinkedRolePilingSuccessorHash::copyRoleSuccessorHash(CLinkedRolePilingSuccessorHash* copyRoleSuccHash, cint64 maxCopyPilingDepth) {
					// TODO: handle piling depth
					mRoleSuccHash = copyRoleSuccHash->mRoleSuccHash;
					return this;
				}


				CLinkedRolePilingSuccessorHash* CLinkedRolePilingSuccessorHash::addLinkedSuccessor(CRole* role, CIndividualPilingProcessNode* linkedIndi, cint64 pilingDepth) {
					// TODO: handle piling depth
					mRoleSuccHash.insertMulti(role,linkedIndi);
					return this;
				}

				CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>* CLinkedRolePilingSuccessorHash::getLinkedRoleSuccessorHash() {
					return &mRoleSuccHash;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CDisjointSuccessorRoleHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDisjointSuccessorRoleHash::CDisjointSuccessorRoleHash(CProcessContext* context) : mSuccNegDisEdgeHash(context),mContext(context) {
				}

				CDisjointSuccessorRoleHash::~CDisjointSuccessorRoleHash() {
				}

				CDisjointSuccessorRoleHash* CDisjointSuccessorRoleHash::initDisjointSuccessorRoleHash(CDisjointSuccessorRoleHash* prevDisjRoleHash) {
					if (prevDisjRoleHash) {
						mSuccNegDisEdgeHash = prevDisjRoleHash->mSuccNegDisEdgeHash;
					} else {
						mSuccNegDisEdgeHash.clear();
					}
					return this;
				}


				CPROCESSHASH<CRole*,CNegationDisjointEdge*>* CDisjointSuccessorRoleHash::getNegDisRoleHash(cint64 successor, bool located) {
					if (!located) {
						CDisjointSuccessorRoleData* data = nullptr;
						if (mSuccNegDisEdgeHash.tryGetValuePointer(successor,data)) {
							return data->mUseNegDisSet;
						}
						return nullptr;
					} else {
						CDisjointSuccessorRoleData& data = mSuccNegDisEdgeHash[successor];
						if (!data.mLocNegDisSet) {
							CPROCESSHASH<CRole*,CNegationDisjointEdge*>* newNegDisSet = CObjectParameterizingAllocator< CPROCESSHASH<CRole*,CNegationDisjointEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							data.mLocNegDisSet = newNegDisSet;
							data.mUseNegDisSet = newNegDisSet;
						}
						return data.mUseNegDisSet;
					}
				}


				CDisjointSuccessorRoleHash* CDisjointSuccessorRoleHash::insertDisjointSuccessorRoleLink(cint64 succIndi, CNegationDisjointEdge* link) {
					getNegDisRoleHash(succIndi,true)->insert(link->getLinkRole(),link);
					return this;
				}


				bool CDisjointSuccessorRoleHash::hasDisjointSuccessorRoleLink(cint64 succIndi, CRole* role) {
					CPROCESSHASH<CRole*,CNegationDisjointEdge*>* dataHash = getNegDisRoleHash(succIndi,false);
					if (dataHash) {
						return dataHash->contains(role);
					}
					return false;
				}



				CNegationDisjointEdge* CDisjointSuccessorRoleHash::getDisjointSuccessorRoleLink(cint64 succIndi, CRole* role) {
					CNegationDisjointEdge* link = nullptr;
					CPROCESSHASH<CRole*,CNegationDisjointEdge*>* dataHash = getNegDisRoleHash(succIndi,false);
					if (dataHash) {
						link = dataHash->value(role,nullptr);
					}
					return link;
				}


				CDisjointSuccessorRoleHash* CDisjointSuccessorRoleHash::removeDisjointSuccessorRoleLinks(cint64 succIndi) {
					mSuccNegDisEdgeHash.remove(succIndi);
					return this;
				}


				CDisjointSuccessorRoleIterator CDisjointSuccessorRoleHash::getDisjointRoleIterator(cint64 succIndi) {
					CPROCESSHASH<CRole*,CNegationDisjointEdge*>* dataHash = getNegDisRoleHash(succIndi,false);
					if (dataHash) {
						return CDisjointSuccessorRoleIterator(succIndi,dataHash->begin(),dataHash->end());
					}
					return CDisjointSuccessorRoleIterator();
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

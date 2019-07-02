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

#include "CRoleReapplyHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleReapplyHash::CRoleReapplyHash(CProcessContext* processContext) : CPROCESSHASH<CRole*,CReapplyQueue>(processContext) {
				}


				CRoleReapplyHash::~CRoleReapplyHash() {
				}


				CRoleReapplyHash* CRoleReapplyHash::initRoleReapplyHash(CRoleReapplyHash* prevRoleReapplyHash) {
					if (prevRoleReapplyHash) {
						*this = *prevRoleReapplyHash;
					} else {
						CPROCESSHASH<CRole*,CReapplyQueue>::clear();
					}
					return this;
				}



				CReapplyQueue* CRoleReapplyHash::getRoleReapplyQueue(CRole* role, bool create) {
					if (create) {
						return &this->operator[](role);
					} else {
						if (contains(role)) {
							return &this->operator[](role);
						} else {
							return nullptr;
						}
					}
				}


				bool CRoleReapplyHash::containsRoleReapplyQueue(CRole* role) {
					return contains(role);
				}


				CReapplyQueueIterator CRoleReapplyHash::getRoleReapplyIterator(CRole* role, bool clearDynamicReapplyQueue) {
					CReapplyQueue* reapplyQueue = nullptr;
					if (tryGetValuePointer(role,reapplyQueue)) {
						return reapplyQueue->getIterator(clearDynamicReapplyQueue);
					}
					return CReapplyQueueIterator(nullptr,nullptr);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

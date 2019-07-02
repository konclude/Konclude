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

#include "CRoleChain.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CRoleChain::CRoleChain() {
			}

			CRoleChain::~CRoleChain() {
			}


			CRoleChain* CRoleChain::initRoleChain() {
				mTerm = nullptr;
				mTag = 0;
				mRoleChainLinker = nullptr;
				mInverseRoleChainLinker = nullptr;
				return this;
			}

			CRoleChain* CRoleChain::setRoleChainTag(qint64 roleChainTag) {
				mTag = roleChainTag;
				return this;
			}

			qint64 CRoleChain::getRoleChainTag() {
				return mTag;
			}


			CRoleChain* CRoleChain::setTerminology(CTerminology* ontology) {
				mTerm = ontology;
				return this;
			}

			CTerminology* CRoleChain::getTerminology() {
				return mTerm;
			}

			qint64 CRoleChain::getTerminologyTag() const {
				cint64 termTag = 0;
				if (mTerm) {
					termTag = mTerm->getTerminologyID();
				}
				return termTag;
			}

			CXLinker<CRole*>* CRoleChain::getRoleChainLinker() {
				return mRoleChainLinker;
			}

			CRoleChain* CRoleChain::appendRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mRoleChainLinker) {
						mRoleChainLinker = roleChainLinker;
					} else {
						mRoleChainLinker->append(roleChainLinker);
					}
				}
				return this;
			}

			CRoleChain* CRoleChain::prependRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mRoleChainLinker) {
						mRoleChainLinker = roleChainLinker;
					} else {
						mRoleChainLinker = roleChainLinker->append(mRoleChainLinker);
					}
				}
				return this;
			}

			CRoleChain* CRoleChain::setRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				mRoleChainLinker = roleChainLinker;
				return this;
			}

			CXLinker<CRole*>* CRoleChain::getInverseRoleChainLinker() {
				return mInverseRoleChainLinker;
			}

			CRoleChain* CRoleChain::appendInverseRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mInverseRoleChainLinker) {
						mInverseRoleChainLinker = roleChainLinker;
					} else {
						mInverseRoleChainLinker->append(roleChainLinker);
					}
				}
				return this;
			}

			CRoleChain* CRoleChain::prependInverseRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mInverseRoleChainLinker) {
						mInverseRoleChainLinker = roleChainLinker;
					} else {
						mInverseRoleChainLinker = roleChainLinker->append(mInverseRoleChainLinker);
					}
				}
				return this;
			}

			CRoleChain* CRoleChain::setInverseRoleChainLinker(CXLinker<CRole*>* roleChainLinker) {
				mInverseRoleChainLinker = roleChainLinker;
				return this;
			}





		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CRole.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CRole::CRole() {
				initRole();
			}



			CRole *CRole::initRole() {
				superRoles = 0;
				indirectSuperRoles = 0;
				mComplexity = false;
				mFunctional = false;
				mInvFunctional = false;
				mTransetive = false;
				mAsymmetric = false;
				mSymmetric = false;
				mReflexive = false;
				mIrreflexive = false;
				mDataRole = false;
				rangeLinker = 0;
				domainLinker = 0;
				tax = 0;
				mInverseEquivalentRoles = 0;
				mDisjointRoles = 0;
				mRoleChainSubSharingLinker = nullptr;
				mRoleChainSuperSharingLinker = nullptr;
				mInverseRole = nullptr;
				mRoleData = nullptr;
				return this;
			}


			CRole *CRole::init(qint64 roleTag) {
				initRole();
				mTag = roleTag;
				return this;
			}


			CRole *CRole::init(CRole *role) {
				mNameLinker = role->getPropertyNameLinker();
				mTag = role->mTag;
				mDataRole = role->mDataRole;
				superRoles = 0;
				indirectSuperRoles = 0;
				rangeLinker = 0;
				domainLinker = 0;
				tax = 0;
				mInverseEquivalentRoles = 0;
				mDisjointRoles = 0;
				mInverseRole = nullptr;
				mComplexity = role->mComplexity;
				mFunctional = role->mFunctional;
				mInvFunctional = role->mInvFunctional;
				mTransetive = role->mTransetive;
				mAsymmetric = role->mAsymmetric;
				mSymmetric = role->mSymmetric;
				mReflexive = role->mReflexive;
				mIrreflexive = role->mIrreflexive;
				mRoleChainSubSharingLinker = role->mRoleChainSubSharingLinker;
				mRoleChainSuperSharingLinker = role->mRoleChainSuperSharingLinker;
				return this;
			}



			CRole *CRole::initRoleCopy(CRole *role, CMemoryAllocationManager* memMan) {
				mNameLinker = role->getPropertyNameLinker();
				mTag = role->mTag;
				mDataRole = role->mDataRole;
				superRoles = 0;
				indirectSuperRoles = 0;
				rangeLinker = 0;
				domainLinker = 0;
				tax = 0;
				mInverseEquivalentRoles = 0;
				mDisjointRoles = 0;
				mComplexity = role->mComplexity;
				mFunctional = role->mFunctional;
				mInvFunctional = role->mInvFunctional;
				mTransetive = role->mTransetive;
				mAsymmetric = role->mAsymmetric;
				mSymmetric = role->mSymmetric;
				mReflexive = role->mReflexive;
				mIrreflexive = role->mIrreflexive;
				mRoleChainSubSharingLinker = role->mRoleChainSubSharingLinker;
				mRoleChainSuperSharingLinker = role->mRoleChainSuperSharingLinker;
				mInverseRole = role->mInverseRole;

				CSortedNegLinker<CRole *> *superRoleIt = role->superRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					bool negated = superRoleIt->isNegated();
					CSortedNegLinker<CRole *> *superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					superRoleLinker->init(superRole,negated);
					addSuperRoleLinker(superRoleLinker);
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}

				CSortedNegLinker<CRole *> *inverseRolesIt = role->getInverseRoleList();
				while (inverseRolesIt) {
					CSortedNegLinker<CRole *> *invRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					invRoleLinker->init(inverseRolesIt->getData(),inverseRolesIt->isNegated());
					addInverseRoleLinker(invRoleLinker);
					inverseRolesIt = (CSortedNegLinker<CRole *> *)inverseRolesIt->getNext();
				}

				CSortedNegLinker<CRole *> *disjointRolesIt = role->getDisjointRoleList();
				while (disjointRolesIt) {
					CSortedNegLinker<CRole *> *disRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					disRoleLinker->init(disjointRolesIt->getData(),disjointRolesIt->isNegated());
					addDisjointRoleLinker(disRoleLinker);
					disjointRolesIt = (CSortedNegLinker<CRole *> *)disjointRolesIt->getNext();
				}

				CSortedNegLinker<CConcept *> *rangeLinkerIt = role->getRangeConceptList();
				while (rangeLinkerIt) {
					CSortedNegLinker<CConcept *> *rangeConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
					rangeConLinker->init(rangeLinkerIt->getData(),rangeLinkerIt->isNegated());
					addRangeConceptLinker(rangeConLinker);
					rangeLinkerIt = (CSortedNegLinker<CConcept *> *)rangeLinkerIt->getNext();
				}

				CSortedNegLinker<CConcept *> *domainLinkerIt = role->getDomainConceptList();
				while (domainLinkerIt) {
					CSortedNegLinker<CConcept *> *domainConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
					domainConLinker->init(domainLinkerIt->getData(),domainLinkerIt->isNegated());
					addDomainConceptLinker(domainConLinker);
					domainLinkerIt = (CSortedNegLinker<CConcept *> *)domainLinkerIt->getNext();
				}

				return this;
			}



			bool CRole::isDataRole() {
				return mDataRole;
			}

			bool CRole::isObjectRole() {
				return !isDataRole();
			}

			CRole* CRole::setDataRole(bool dataRole) {
				mDataRole = dataRole;
				return this;
			}

			CRole* CRole::setObjectRole(bool objectRole) {
				setDataRole(!objectRole);
				return this;
			}



			CRole* CRole::getInverseRole() {
				return mInverseRole;
			}

			CRole* CRole::setInverseRole(CRole* inverseRole) {
				mInverseRole = inverseRole;
				return this;
			}


			bool CRole::hasInverseRoles() {
				if (mInverseRole) {
					return true;
				}
				CSortedNegLinker<CRole*>* inverseEquivalentRolesIt = mInverseEquivalentRoles;
				while (inverseEquivalentRolesIt) {
					if (inverseEquivalentRolesIt->isNegated()) {
						return true;
					}
					inverseEquivalentRolesIt = inverseEquivalentRolesIt->getNext();
				}
				return false;
			}



			CRole *CRole::addInverseRoleLinker(CSortedNegLinker<CRole *> *inverseRole) {
				if (mInverseEquivalentRoles) {
					mInverseEquivalentRoles = mInverseEquivalentRoles->insertSortedNextSorted(inverseRole);
				} else {
					mInverseEquivalentRoles = inverseRole;
				}
				return this;
			}

			CSortedNegLinker<CRole *> *CRole::getInverseRoleList() {
				return mInverseEquivalentRoles;
			}


			CRole *CRole::setInverseEquivalentRoleList(CSortedNegLinker<CRole *> *invEqRoleList) {
				mInverseEquivalentRoles = invEqRoleList;
				return this;
			}


			CSortedNegLinker<CRole *> *CRole::getInverseEquivalentRoleList() {
				return mInverseEquivalentRoles;
			}


			bool CRole::hasEquivalentRoles() {
				CSortedNegLinker<CRole*>* inverseEquivalentRolesIt = mInverseEquivalentRoles;
				while (inverseEquivalentRolesIt) {
					if (!inverseEquivalentRolesIt->isNegated()) {
						return true;
					}
					inverseEquivalentRolesIt = inverseEquivalentRolesIt->getNext();
				}
				return false;
			}

			CRole *CRole::addEquivalentRoleLinker(CSortedNegLinker<CRole *> *equivalentRole) {
				if (mInverseEquivalentRoles) {
					mInverseEquivalentRoles = mInverseEquivalentRoles->insertSortedNextSorted(equivalentRole);
				} else {
					mInverseEquivalentRoles = equivalentRole;
				}
				return this;
			}

			bool CRole::hasEquivalentRole(CRole* role) {
				CSortedNegLinker<CRole*>* inverseEquivalentRolesIt = mInverseEquivalentRoles;
				while (inverseEquivalentRolesIt) {
					if (!inverseEquivalentRolesIt->isNegated() && inverseEquivalentRolesIt->getData() == role) {
						return true;
					}
					inverseEquivalentRolesIt = inverseEquivalentRolesIt->getNext();
				}
				return false;
			}

			bool CRole::hasInverseRole(CRole* role) {
				if (mInverseRole == role) {
					return true;
				}
				CSortedNegLinker<CRole*>* inverseEquivalentRolesIt = mInverseEquivalentRoles;
				while (inverseEquivalentRolesIt) {
					if (inverseEquivalentRolesIt->isNegated() && inverseEquivalentRolesIt->getData() == role) {
						return true;
					}
					inverseEquivalentRolesIt = inverseEquivalentRolesIt->getNext();
				}
				return false;
			}

			bool CRole::hasDisjointRole(CRole* role) {
				CSortedNegLinker<CRole*>* disjointRolesIt = mDisjointRoles;
				while (disjointRolesIt) {
					if (disjointRolesIt->getData() == role) {
						return true;
					}
					disjointRolesIt = disjointRolesIt->getNext();
				}
				return false;
			}


			CRole *CRole::setDisjointRoleList(CSortedNegLinker<CRole *> *disRoleList) {
				mDisjointRoles = disRoleList;
				return this;
			}



			bool CRole::hasDisjointRoles() {
				return mDisjointRoles != nullptr;
			}

			CRole *CRole::addDisjointRoleLinker(CSortedNegLinker<CRole *> *disjointRole) {
				if (mDisjointRoles) {
					mDisjointRoles = mDisjointRoles->insertSortedNextSorted(disjointRole);
				} else {
					mDisjointRoles = disjointRole;
				}
				return this;
			}

			CSortedNegLinker<CRole *> *CRole::getDisjointRoleList() {
				return mDisjointRoles;
			}


			CSortedNegLinker<CRole *> *CRole::getEquivalentRoleList() {
				return mInverseEquivalentRoles;
			}

			CRole *CRole::setRoleTag(qint64 roleTag) {
				mTag = roleTag;
				return this;
			}

			qint64 CRole::getRoleTag() const {
				return mTag;
			}

			CRole *CRole::setTerminology(CTerminology *ontology) {
				tax = ontology;
				return this;
			}

			CTerminology *CRole::getTerminology() {
				return tax;
			}


			qint64 CRole::getTerminologyTag() const {
				if (tax) {
					return tax->getTerminologyID();
				} else {
					return 0;
				}
			}


			CRole *CRole::setRoleComplexity(bool complexRole) {
				mComplexity = complexRole;
				return this;
			}

			bool CRole::isComplexRole() {
				return mComplexity;
			}

			bool CRole::isSimpleRole() {
				return !mComplexity;
			}

			bool CRole::getRoleComplexity() {
				return mComplexity;
			}

			CRole *CRole::setFunctional(bool functional) {
				mFunctional = functional;
				return this;
			}

			bool CRole::isFunctional() {
				return mFunctional;
			}

			bool CRole::getFunctional() {
				return mFunctional;
			}

			CRole *CRole::setInverseFunctional(bool invFunctional) {
				mInvFunctional = invFunctional;
				return this;
			}

			bool CRole::isInverseFunctional() {
				return mInvFunctional;
			}

			bool CRole::getInverseFunctional() {
				return mInvFunctional;
			}

			CRole *CRole::setTransitive(bool transetive) {
				mTransetive = transetive;
				return this;
			}

			bool CRole::isTransitive() {
				return mTransetive;
			}

			bool CRole::getTransitive() {
				return isTransitive();
			}

			bool CRole::hasRoleData() {
				return mRoleData != nullptr;
			}

			CRoleData* CRole::getRoleData() {
				return mRoleData;
			}

			CRole* CRole::setRoleData(CRoleData* roleData) {
				mRoleData = roleData;
				return this;
			}


			CRole *CRole::setAsymmetric(bool asymmetric) {
				mAsymmetric = asymmetric;
				return this;
			}

			bool CRole::isAsymmetric() {
				return mAsymmetric;
			}

			bool CRole::getAsymmetric() {
				return mAsymmetric;
			}

			CRole *CRole::setSymmetric(bool symmetric) {
				mSymmetric = symmetric;
				return this;
			}

			bool CRole::isSymmetric() {
				return mSymmetric;
			}

			bool CRole::getSymmetric() {
				return mSymmetric;
			}

			CRole *CRole::setReflexive(bool reflexive) {
				mReflexive = reflexive;
				return this;
			}

			bool CRole::isReflexive() {
				return mReflexive;
			}

			bool CRole::getReflexive() {
				return mReflexive;
			}


			CRole *CRole::setIrreflexive(bool irreflexive) {
				mIrreflexive = irreflexive;
				return this;
			}

			bool CRole::isIrreflexive() {
				return mIrreflexive;
			}

			bool CRole::getIrreflexive() {
				return mIrreflexive;
			}



			CSortedNegLinker<CConcept *> *CRole::getRangeConceptList() {
				return rangeLinker;
			}


			CSortedNegLinker<CConcept *> *CRole::getRelativeRangeConceptList(bool roleInversed) {
				if (roleInversed) {
					return domainLinker;
				} else {
					return rangeLinker;
				}
			}

			CSortedNegLinker<CConcept *> *CRole::getDomainRangeConceptList(bool roleInversed) {
				if (roleInversed) {
					return rangeLinker;
				} else {
					return domainLinker;
				}
			}



			bool CRole::hasRangeConceptTag(qint64 rangeConceptTag) {
				CSortedNegLinker<CConcept *> *rangeConIt = rangeLinker;
				while (rangeConIt) {
					qint64 rangeConTag = rangeConIt->getData()->getConceptTag();
					if (rangeConTag == rangeConceptTag) {
						return true;
					} else if (rangeConTag > rangeConceptTag) {
						return false;
					}
					rangeConIt = (CSortedNegLinker<CConcept *> *)rangeConIt->getNext();
				}
				return false;
			}

			bool CRole::hasRangeConcept(CConcept* concept) {
				CSortedNegLinker<CConcept *> *rangeConIt = rangeLinker;
				while (rangeConIt) {
					CConcept* rangeConcept = rangeConIt->getData();
					if (rangeConcept == concept) {
						return true;
					}
					rangeConIt = (CSortedNegLinker<CConcept *> *)rangeConIt->getNext();
				}
				return false;
			}



			bool CRole::hasDomainConcept(CConcept* concept) {
				CSortedNegLinker<CConcept *> *domainConIt = domainLinker;
				while (domainConIt) {
					CConcept* domainConcept = domainConIt->getData();
					if (domainConcept == concept) {
						return true;
					}
					domainConIt = (CSortedNegLinker<CConcept *> *)domainConIt->getNext();
				}
				return false;
			}


			bool CRole::hasDomainConceptTag(qint64 domainConceptTag) {
				CSortedNegLinker<CConcept *> *domainConIt = domainLinker;
				while (domainConIt) {
					qint64 domainConTag = domainConIt->getData()->getConceptTag();
					if (domainConTag == domainConceptTag) {
						return true;
					} else if (domainConTag > domainConceptTag) {
						return false;
					}
					domainConIt = (CSortedNegLinker<CConcept *> *)domainConIt->getNext();
				}
				return false;
			}


			bool CRole::hasDomainOrRangeConceptTag(qint64 conceptTag) {
				return hasRangeConceptTag(conceptTag) || hasDomainConceptTag(conceptTag);
			}



			CRole *CRole::setRangeConceptList(CSortedNegLinker<CConcept *> *rangeConceptList) {
				rangeLinker = rangeConceptList;
				return this;
			}

			CRole *CRole::addRangeConceptLinker(CSortedNegLinker<CConcept *> *rangeConceptLinker) {
				if (rangeLinker) {
					rangeLinker = rangeLinker->insertSortedNextSorted(rangeConceptLinker);
				} else {
					rangeLinker = rangeConceptLinker;
				}
				return this;
			}


			CSortedNegLinker<CConcept *> *CRole::getDomainConceptList() {
				return domainLinker;
			}

			CRole *CRole::setDomainConceptList(CSortedNegLinker<CConcept *> *domainConceptList) {
				domainLinker = domainConceptList;
				return this;
			}

			CRole *CRole::addDomainConceptLinker(CSortedNegLinker<CConcept *> *domainConceptLinker) {
				if (domainLinker) {
					domainLinker = domainLinker->insertSortedNextSorted(domainConceptLinker);
				} else {
					domainLinker = domainConceptLinker;
				}
				return this;
			}



			CRole *CRole::setSuperRoleList(CSortedNegLinker<CRole *> *superRoleList) {
				superRoles = superRoleList;
				return this;
			}


			CRole *CRole::addSuperRoleLinker(CSortedNegLinker<CRole *> *superRoleLinker) {
				if (superRoles) {
					superRoles = superRoles->insertSortedNextSorted(superRoleLinker);
				} else {
					superRoles = superRoleLinker;
				}
				return this;
			}


			CSortedNegLinker<CRole *> *CRole::getSuperRoleList() {
				return superRoles;
			}


			bool CRole::hasSuperRole(CRole* role) {
				CSortedNegLinker<CRole *> *superRoleIt = superRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					if (superRole == role) {
						return true;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}


			bool CRole::hasSuperRoleTag(qint64 tag) {
				CSortedNegLinker<CRole *> *superRoleIt = superRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					qint64 superRoleTag = superRole->getRoleTag();
					if (tag == superRoleTag) {
						return true;
					} else if (superRoleTag > tag) {
						return false;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}


			bool CRole::hasSuperRoleTag(qint64 tag, bool negated) {
				CSortedNegLinker<CRole *> *superRoleIt = superRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					qint64 superRoleTag = superRole->getRoleTag();
					if (tag == superRoleTag && superRoleIt->isNegated() == negated) {
						return true;
					} else if (superRoleTag > tag) {
						return false;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}

			CRole *CRole::setIndirectSuperRoleLinker(CSortedNegLinker<CRole *> *subRoleList) {
				indirectSuperRoles = subRoleList;
				return this;
			}

			CRole *CRole::addIndirectSuperRoleLinker(CSortedNegLinker<CRole *> *subRoleLinker) {
				if (indirectSuperRoles) {
					indirectSuperRoles = indirectSuperRoles->insertSortedNextSorted(subRoleLinker);
				} else {
					indirectSuperRoles = subRoleLinker;
				}
				return this;
			}

			CSortedNegLinker<CRole *> *CRole::getIndirectSuperRoleList() {
				return indirectSuperRoles;
			}


			bool CRole::hasIndirectSuperRole(CRole* role) {
				CSortedNegLinker<CRole *> *superRoleIt = indirectSuperRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					if (superRole == role) {
						return true;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}


			bool CRole::hasIndirectSuperRoleTag(qint64 tag) {
				CSortedNegLinker<CRole *> *superRoleIt = indirectSuperRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					qint64 superRoleTag = superRole->getRoleTag();
					if (tag == superRoleTag) {
						return true;
					} else if (superRoleTag > tag) {
						return false;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}



			bool CRole::hasIndirectSuperRoleTag(qint64 tag, bool negated) {
				CSortedNegLinker<CRole *> *superRoleIt = indirectSuperRoles;
				while (superRoleIt) {
					CRole *superRole = superRoleIt->getData();
					qint64 superRoleTag = superRole->getRoleTag();
					if (tag == superRoleTag && superRoleIt->isNegated() == negated) {
						return true;
					} else if (superRoleTag > tag) {
						return false;
					}
					superRoleIt = (CSortedNegLinker<CRole *> *)superRoleIt->getNext();
				}
				return false;
			}




			bool CRole::operator<=(const CRole &role) {
				if (mTag < role.mTag) {
					return true;
				} else if (mTag > role.mTag) {
					return false;
				} else {
					return getTerminologyTag() <= role.getTerminologyTag();
				}
			}

			bool CRole::operator==(const CRole &role) {
				return mTag == role.mTag && tax == role.tax;
			}


			bool CRole::hasPropertyName() {
				return hasName();
			}

			CRole *CRole::setPropertyNameLinker(CLinker<CName *> *propertyNameLinker) {
				setNameLinker(propertyNameLinker);
				return this;
			}

			CRole *CRole::addPropertyNameLinker(CLinker<CName *> *propertyNameLinker) {
				addNameLinker(propertyNameLinker);
				return this;
			}

			CLinker<CName *> *CRole::getPropertyNameLinker() {
				return getNameLinker();
			}


			bool CRole::hasRoleChainSubSharing() {
				return mRoleChainSubSharingLinker != nullptr;
			}

			CRole *CRole::addRoleChainSubSharingLinker(CXLinker<CRoleChain*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mRoleChainSubSharingLinker) {
						mRoleChainSubSharingLinker = roleChainLinker;
					} else {
						mRoleChainSubSharingLinker = roleChainLinker->append(mRoleChainSubSharingLinker);
					}
				}
				return this;
			}

			CXLinker<CRoleChain*>* CRole::getRoleChainSubSharingLinker() {
				return mRoleChainSubSharingLinker;
			}

			bool CRole::hasRoleChainSubSharingLinker(CRoleChain* roleChain) {
				CXLinker<CRoleChain*>* roleChainIt = mRoleChainSubSharingLinker;
				while (roleChainIt) {
					if (roleChainIt->getData() == roleChain) {
						return true;
					}
					roleChainIt = roleChainIt->getNext();
				}
				return false;
			}


			bool CRole::hasRoleChainSuperSharing() {
				return mRoleChainSuperSharingLinker != nullptr;
			}

			CRole *CRole::addRoleChainSuperSharingLinker(CXLinker<CRoleChain*>* roleChainLinker) {
				if (roleChainLinker) {
					if (!mRoleChainSuperSharingLinker) {
						mRoleChainSuperSharingLinker = roleChainLinker;
					} else {
						mRoleChainSuperSharingLinker = roleChainLinker->append(mRoleChainSuperSharingLinker);
					}
				}
				return this;
			}

			CXLinker<CRoleChain*>* CRole::getRoleChainSuperSharingLinker() {
				return mRoleChainSuperSharingLinker;
			}

			bool CRole::hasRoleChainSuperSharingLinker(CRoleChain* roleChain) {
				CXLinker<CRoleChain*>* roleChainIt = mRoleChainSuperSharingLinker;
				while (roleChainIt) {
					if (roleChainIt->getData() == roleChain) {
						return true;
					}
					roleChainIt = roleChainIt->getNext();
				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

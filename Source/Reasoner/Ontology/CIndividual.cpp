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

#include "CIndividual.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CIndividual::CIndividual(qint64 id) : CTagItem(id) {
				mAssertionConceptLinker = nullptr;
				mAssertionRoleLinker = nullptr;
				mNominalConcept = nullptr;
				mAnonymousIndividual = false;
				mIndividualData = nullptr;
			}



			bool CIndividual::hasIndividualName() {
				return hasName();
			}

			CIndividual *CIndividual::setIndividualNameLinker(CLinker<CName *> *individualNameLinker) {
				setNameLinker(individualNameLinker);
				return this;
			}

			CIndividual *CIndividual::addIndividualNameLinker(CLinker<CName *> *individualNameLinker) {
				addNameLinker(individualNameLinker);
				return this;
			}

			CLinker<CName *> *CIndividual::getIndividualNameLinker() {
				return getNameLinker();
			}

			CConcept* CIndividual::getIndividualNominalConcept() {
				return mNominalConcept;
			}

			CIndividual* CIndividual::setIndividualNominalConcept(CConcept* nomConcept) {
				mNominalConcept = nomConcept;
				return this;
			}


			CConceptAssertionLinker* CIndividual::getAssertionConceptLinker() {
				return mAssertionConceptLinker;
			}

			CIndividual* CIndividual::addAssertionConceptLinker(CConceptAssertionLinker* assConLinker) {
				mAssertionConceptLinker = assConLinker->append(mAssertionConceptLinker);
				return this;
			}

			CIndividual* CIndividual::setAssertionConceptLinker(CConceptAssertionLinker* assConLinker) {
				mAssertionConceptLinker = assConLinker;
				return this;
			}



			CRoleAssertionLinker* CIndividual::getAssertionRoleLinker() {
				return mAssertionRoleLinker;
			}

			CIndividual* CIndividual::addAssertionRoleLinker(CRoleAssertionLinker* assRoleLinker) {
				mAssertionRoleLinker = assRoleLinker->append(mAssertionRoleLinker);
				return this;
			}

			CIndividual* CIndividual::setAssertionRoleLinker(CRoleAssertionLinker* assRoleLinker) {
				mAssertionRoleLinker = assRoleLinker;
				return this;
			}

			CIndividual* CIndividual::initIndividual(qint64 indiID) {
				mTag = indiID;
				mAssertionConceptLinker = nullptr;
				mNominalConcept = nullptr;
				mAnonymousIndividual = false;
				return this;
			}



			CIndividual *CIndividual::initIndividualCopy(CIndividual* individual, CMemoryAllocationManager* memMan) {
				mNameLinker = individual->getIndividualNameLinker();
				mTag = individual->mTag;
				CConceptAssertionLinker* assConIt = individual->mAssertionConceptLinker;
				while (assConIt) {
					CConcept *con = assConIt->getData();
					bool negated = assConIt->isNegated();
					CConceptAssertionLinker *assLinker = CObjectAllocator< CConceptAssertionLinker >::allocateAndConstruct(memMan);
					assLinker->initNegLinker(con,negated);
					addAssertionConceptLinker(assLinker);
					assConIt = assConIt->getNext();
				}

				mNominalConcept = individual->mNominalConcept;
				mAnonymousIndividual = individual->mAnonymousIndividual;
				return this;
			}




			CIndividual *CIndividual::setIndividualID(qint64 id) {
				mTag = id;
				return this;
			}

			qint64 CIndividual::getIndividualID() {
				return mTag;
			}



			bool CIndividual::hasAssertedConcept(CConcept* concept) {
				CConceptAssertionLinker* assertionConceptLinkerIt = mAssertionConceptLinker;
				while (assertionConceptLinkerIt) {
					CConcept* assConcept = assertionConceptLinkerIt->getData();
					if (assConcept->getConceptTag() == concept->getConceptTag()) {
						return true;
					}
					assertionConceptLinkerIt = assertionConceptLinkerIt->getNext();
				}
				return false;
			}

			CIndividual* CIndividual::setAnonymousIndividual(bool anonymous) {
				mAnonymousIndividual = anonymous;
				return this;
			}

			bool CIndividual::isAnonymousIndividual() {
				return mAnonymousIndividual;
			}

			bool CIndividual::hasIndividualData() {
				return mIndividualData != nullptr;
			}


			CIndividualData* CIndividual::getIndividualData() {
				return mIndividualData;
			}

			CIndividual* CIndividual::setIndividualData(CIndividualData* individualData) {
				mIndividualData = individualData;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

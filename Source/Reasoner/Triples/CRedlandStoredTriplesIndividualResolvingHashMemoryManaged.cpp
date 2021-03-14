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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRedlandStoredTriplesIndividualResolvingHashMemoryManaged.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::CRedlandStoredTriplesIndividualResolvingHashMemoryManaged(CConcreteOntology* ontology, CConfiguration* configuration, const QString& anonymousOntologyIdentifier)
				: CMemoryPoolNewAllocationIncreasingContext(500, 500000, 2), CQtManagedRestrictedModificationHash<CRedlandNodeHasher, CIndividual*>(this), mIndiDataMemAllocManager(ontology, configuration), mAnonymousOntologyIdentifier(anonymousOntologyIdentifier) {
			}


			CIndividual*& CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getIndividual(const CRedlandNodeHasher& indiNodeHasher) {
				CIndividual*& indi = CQtManagedRestrictedModificationHash<CRedlandNodeHasher, CIndividual*>::operator[](indiNodeHasher);
				return indi;
			}

			CRedlandStoredTriplesIndividualResolvingHashMemoryManaged* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::insertIndividual(const CRedlandNodeHasher& indiNode, CIndividual* indi) {
				CQtManagedRestrictedModificationHash<CRedlandNodeHasher, CIndividual*>::insert(indiNode, indi);
				return this;
			}


			CIndividual*& CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getIndividual(librdf_node* indiNode) {
				return getIndividual(CRedlandNodeHasher(indiNode));
			}

			CConceptAssertionLinker* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::createAssertionConceptLinker() {
				++mConceptAssertionCount;
				return CObjectAllocator<CConceptAssertionLinker>::allocateAndConstruct(mIndiDataMemAllocManager.getMemoryAllocationManager());
			}
			CRoleAssertionLinker* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::createAssertionRoleLinker() {
				++mObjectPropertyAssertionCount;
				return CObjectAllocator<CRoleAssertionLinker>::allocateAndConstruct(mIndiDataMemAllocManager.getMemoryAllocationManager());
			}

			CDataAssertionLinker* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::createAssertionDataLinker() {
				++mDataPropertyAssertionCount;
				return CObjectAllocator<CDataAssertionLinker>::allocateAndConstruct(mIndiDataMemAllocManager.getMemoryAllocationManager());
			}

			CReverseRoleAssertionLinker* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::createReverseAssertionRoleLinker() {
				return CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mIndiDataMemAllocManager.getMemoryAllocationManager());
			}

			CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getIndividualDataMemoryManager() {
				return &mIndiDataMemAllocManager;
			}

			CDataLiteralNormalizerPreProcess* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getIndividualDataLiteralNormalizer() {
				return &mIndiDataLiteralNormalizer;
			}

			CMemoryPool* CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::takeIndividualDataMemoryPools() {
				return mIndiDataMemAllocManager.takeMemoryPools();
			}

			cint64 CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getConceptAssertionCount() {
				return mConceptAssertionCount;
			}

			cint64 CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getObjectPropertyAssertionCount() {
				return mObjectPropertyAssertionCount;
			}

			cint64 CRedlandStoredTriplesIndividualResolvingHashMemoryManaged::getDataPropertyAssertionCount() {
				return mDataPropertyAssertionCount;
			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

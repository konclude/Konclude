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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CROLE_H
#define KONCLUDE_REASONER_ONTOLOGY_CROLE_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"
#include "CTerminology.h"
#include "CName.h"
#include "CTagItem.h"
#include "CNamedItem.h"
#include "CRoleChain.h"
#include "CRoleData.h"

// Other includes
#include "Utilities/CSortedNegLinker.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CLinker.hpp"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CRole
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRole : public CTagItem, public CNamedItem {
				// public methods
				public:
					//! Constructor
					CRole();

					CRole *initRole();
					CRole *initRoleCopy(CRole *role, CMemoryAllocationManager* memMan);

					CRole *setRoleTag(qint64 roleTag);
					qint64 getRoleTag() const;

					CRole *setTerminology(CTerminology *ontology);
					CTerminology *getTerminology();
					qint64 getTerminologyTag() const;
					

					bool hasPropertyName();
					CRole *setPropertyNameLinker(CLinker<CName *> *propertyNameLinker);
					CRole *addPropertyNameLinker(CLinker<CName *> *propertyNameLinker);
					CLinker<CName *> *getPropertyNameLinker();


					CRole* getInverseRole();
					CRole* setInverseRole(CRole* inverseRole);


					bool isDataRole();
					bool isObjectRole();
					CRole* setDataRole(bool dataRole);
					CRole* setObjectRole(bool objectRole);


					// domain and range methods
					CSortedNegLinker<CConcept *> *getRelativeRangeConceptList(bool roleInversed);
					CSortedNegLinker<CConcept *> *getDomainRangeConceptList(bool roleInversed);


					CSortedNegLinker<CConcept *> *getRangeConceptList();
					CRole *setRangeConceptList(CSortedNegLinker<CConcept *> *rangeConceptList);
					CRole *addRangeConceptLinker(CSortedNegLinker<CConcept *> *rangeConceptLinker);
					bool hasRangeConceptTag(qint64 rangeConceptTag);
					bool hasRangeConcept(CConcept* concept);


					CSortedNegLinker<CConcept *> *getDomainConceptList();
					CRole *setDomainConceptList(CSortedNegLinker<CConcept *> *domainConceptList);
					CRole *addDomainConceptLinker(CSortedNegLinker<CConcept *> *domainConceptLinker);
					bool hasDomainConceptTag(qint64 domainConceptTag);
					bool hasDomainConcept(CConcept* concept);

					bool hasDomainOrRangeConceptTag(qint64 conceptTag);



					// methods for super, inverse, equivalent roles
					CRole *setSuperRoleList(CSortedNegLinker<CRole *> *superRoleList);
					CRole *addSuperRoleLinker(CSortedNegLinker<CRole *> *superRoleLinker);
					CSortedNegLinker<CRole *> *getSuperRoleList();
					bool hasSuperRoleTag(qint64 tag, bool negated);
					bool hasSuperRoleTag(qint64 tag);
					bool hasSuperRole(CRole* role);

					CRole *setIndirectSuperRoleLinker(CSortedNegLinker<CRole *> *superRoleList);
					CRole *addIndirectSuperRoleLinker(CSortedNegLinker<CRole *> *superRoleLinker);
					CSortedNegLinker<CRole *> *getIndirectSuperRoleList();
					bool hasIndirectSuperRoleTag(qint64 tag, bool negated);
					bool hasIndirectSuperRoleTag(qint64 tag);
					bool hasIndirectSuperRole(CRole* role);


					bool hasInverseRoles();
					CRole *addInverseRoleLinker(CSortedNegLinker<CRole *> *inverseRole);
					CSortedNegLinker<CRole *> *getInverseRoleList();
					bool hasInverseRole(CRole* role);

					bool hasEquivalentRoles();
					CRole *addEquivalentRoleLinker(CSortedNegLinker<CRole *> *equivalentRole);
					CSortedNegLinker<CRole *> *getEquivalentRoleList();
					bool hasEquivalentRole(CRole* role);

					CRole *setInverseEquivalentRoleList(CSortedNegLinker<CRole *> *invEqRoleList);
					CSortedNegLinker<CRole *> *getInverseEquivalentRoleList();


					bool hasDisjointRoles();
					CRole *setDisjointRoleList(CSortedNegLinker<CRole *> *disRoleList);
					CRole *addDisjointRoleLinker(CSortedNegLinker<CRole *> *disjointRole);
					CSortedNegLinker<CRole *> *getDisjointRoleList();
					bool hasDisjointRole(CRole* role);


					// methods for other role properties
					CRole *setTransitive(bool transetive);
					bool isTransitive();
					bool getTransitive();

					CRole *setFunctional(bool functional);
					bool isFunctional();
					bool getFunctional();

					CRole *setInverseFunctional(bool invFunctional);
					bool isInverseFunctional();
					bool getInverseFunctional();

					CRole *setAsymmetric(bool asymmetric);
					bool isAsymmetric();
					bool getAsymmetric();

					CRole *setSymmetric(bool symmetric);
					bool isSymmetric();
					bool getSymmetric();

					CRole *setReflexive(bool reflexive);
					bool isReflexive();
					bool getReflexive();

					CRole *setIrreflexive(bool irreflexive);
					bool isIrreflexive();
					bool getIrreflexive();


					CRole* setRoleComplexity(bool complexRole);
					bool isComplexRole();
					bool isSimpleRole();
					bool getRoleComplexity();


					bool hasRoleChainSubSharing();
					CRole *addRoleChainSubSharingLinker(CXLinker<CRoleChain*>* roleChainLinker);
					CXLinker<CRoleChain*>* getRoleChainSubSharingLinker();
					bool hasRoleChainSubSharingLinker(CRoleChain* roleChain);

					bool hasRoleChainSuperSharing();
					CRole *addRoleChainSuperSharingLinker(CXLinker<CRoleChain*>* roleChainLinker);
					CXLinker<CRoleChain*>* getRoleChainSuperSharingLinker();
					bool hasRoleChainSuperSharingLinker(CRoleChain* roleChain);


					bool hasRoleData();
					CRoleData* getRoleData();
					CRole* setRoleData(CRoleData* roleData);


					// deprecated
					CRole *init(qint64 roleTag);
					CRole *init(CRole *role);

					bool operator<=(const CRole &role);
					bool operator==(const CRole &role);


				// protected methods
				protected:

				// private methods
				private:

				// private variables
				private:
					CRoleData* mRoleData;
					CTerminology *tax;
					bool mTransetive;
					bool mFunctional;
					bool mInvFunctional;
					bool mAsymmetric;
					bool mSymmetric;
					bool mReflexive;
					bool mIrreflexive;
					bool mComplexity;
					bool mDataRole;
					CRole* mInverseRole;

					CSortedNegLinker<CConcept *> *rangeLinker;
					CSortedNegLinker<CConcept *> *domainLinker;

					CSortedNegLinker<CRole*>* mInverseEquivalentRoles;

					CSortedNegLinker<CRole*>* mDisjointRoles;

					CSortedNegLinker<CRole *> *superRoles;
					CSortedNegLinker<CRole *> *indirectSuperRoles;

					CXLinker<CRoleChain*>* mRoleChainSuperSharingLinker;
					CXLinker<CRoleChain*>* mRoleChainSubSharingLinker;


			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CROLE_H

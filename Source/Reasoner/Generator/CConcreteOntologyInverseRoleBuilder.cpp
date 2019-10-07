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

#include "CConcreteOntologyInverseRoleBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyInverseRoleBuilder::CConcreteOntologyInverseRoleBuilder() {
			}


			CConcreteOntologyInverseRoleBuilder::~CConcreteOntologyInverseRoleBuilder() {
			}


			CRole* CConcreteOntologyInverseRoleBuilder::createInverseRoleDelayed(CRole* role, QHash<CRole*,CRole*>* roleInverseRoleCreateHash, CConcreteOntology* onto) {
				CRoleVector* roleVec = onto->getRBox()->getRoleVector(true);
				cint64 roleCount = roleVec->getItemCount();

				role = getLocalizedRole(role,onto);
				CRole*& inverseRole = (*roleInverseRoleCreateHash)[role];

				if (!inverseRole) {
					CMemoryAllocationManager* memMan = onto->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					inverseRole = CObjectAllocator< CRole >::allocateAndConstruct(memMan);

					inverseRole->initRole();
					inverseRole->setInverseRole(role);
					cint64 inverseRoleTag = roleVec->getItemCount();
					inverseRole->setRoleTag(inverseRoleTag);
					inverseRole->setRoleComplexity(role->getRoleComplexity());
					roleVec->setData(inverseRole->getRoleTag(),inverseRole);
				}

				return inverseRole;
			}



			bool CConcreteOntologyInverseRoleBuilder::createDelayedInverseRoles(QHash<CRole*,CRole*>* roleInverseRoleCreateHash, CConcreteOntology* onto) {
				bool rolesCreated = false;

				if (!roleInverseRoleCreateHash->isEmpty()) {

					CRoleVector* roleVec = onto->getRBox()->getRoleVector(true);
					cint64 roleCount = roleVec->getItemCount();

					CMemoryAllocationManager* memMan = onto->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();

					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* roleObjPropTermHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* objPropTermRoleHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
					CBUILDLIST<CBuildExpression*>* expressionBuildContainerList = onto->getDataBoxes()->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* expressionBuildHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();

					// collect direct and indirect sub roles
					QHash<qint64, QPair<cint64,bool> > roleTagIndirectSubRoleTagNegationHash;
					QHash<qint64, QPair<cint64,bool> > roleTagSubRoleTagNegationHash;
					for (qint64 i = 0; i < roleCount; ++i) {
						CRole *role = roleVec->getData(i);
						if (role) {
							cint64 roleTag = role->getRoleTag();
							CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
							while (inSuperRoleLinkerIt) {
								CRole* superRole = inSuperRoleLinkerIt->getData();
								cint64 superRoleTag = superRole->getRoleTag();
								bool superRoleNeg = inSuperRoleLinkerIt->isNegated();
								roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
								inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
							}
							CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
							while (superRoleLinkerIt) {
								CRole* superRole = superRoleLinkerIt->getData();
								cint64 superRoleTag = superRole->getRoleTag();
								bool superRoleNeg = superRoleLinkerIt->isNegated();
								roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
								superRoleLinkerIt = superRoleLinkerIt->getNext();
							}
						}
					}


					for (QHash<CRole*,CRole*>::const_iterator crRoIt = roleInverseRoleCreateHash->constBegin(), crRoItEnd = roleInverseRoleCreateHash->constEnd(); crRoIt != crRoItEnd; ++crRoIt) {

						CRole* role = crRoIt.key();
						CRole* inverseRole = crRoIt.value();

						cint64 inverseRoleTag = inverseRole->getRoleTag();

						CSortedNegLinker<CRole*>* invRoleLinker1 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
						invRoleLinker1->init(role,true);
						inverseRole->addSuperRoleLinker(invRoleLinker1);
						CSortedNegLinker<CRole*>* invRoleLinker2 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
						invRoleLinker2->init(inverseRole,true);
						role->addSuperRoleLinker(invRoleLinker2);
						role->setInverseRole(inverseRole);


                        CSortedNegLinker<CRole*>* invEqRoleLinker1 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
						invEqRoleLinker1->init(role,true);
						inverseRole->addEquivalentRoleLinker(invEqRoleLinker1);
                        CSortedNegLinker<CRole*>* invEqRoleLinker2 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
						invEqRoleLinker2->init(inverseRole,true);
						role->addEquivalentRoleLinker(invEqRoleLinker2);



						CObjectPropertyTermExpression* buildExpression = roleObjPropTermHash->value(role);
						CInverseObjectPropertyOfExpression* expression = nullptr;
						expression = new CInverseObjectPropertyOfExpression(buildExpression);
						expressionBuildHash->insert(CExpressionHasher(expression),expression);
						expressionBuildContainerList->append(expression);
						roleObjPropTermHash->insert(role,expression);
						objPropTermRoleHash->insert(expression,role);


						QList< QPair<cint64,bool> > subRoleTagList(roleTagSubRoleTagNegationHash.values(role->getRoleTag()));
						for (QList< QPair<cint64,bool> >::const_iterator it = subRoleTagList.constBegin(), itEnd = subRoleTagList.constEnd(); it != itEnd; ++it) {
							cint64 subRoleTag = it->first;
							CRole* subRole = getLocalizedRole(subRoleTag,onto);
							// add new inverse super role
							CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
							superRoleLinker->init(inverseRole,!it->second);
							subRole->addSuperRoleLinker(superRoleLinker);
						}

						QList< QPair<cint64,bool> > indSubRoleTagList(roleTagIndirectSubRoleTagNegationHash.values(role->getRoleTag()));
						for (QList< QPair<cint64,bool> >::const_iterator it = indSubRoleTagList.constBegin(), itEnd = indSubRoleTagList.constEnd(); it != itEnd; ++it) {
							cint64 subRoleTag = it->first;
							CRole* subRole = getLocalizedRole(subRoleTag,onto);
							// add new inverse super role
							CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
							superRoleLinker->init(inverseRole,!it->second);
							subRole->addIndirectSuperRoleLinker(superRoleLinker);
						}

						// copy indirect super role linker to new inverse role
						CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
						while (inSuperRoleLinkerIt) {
							CRole* superRole = inSuperRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = inSuperRoleLinkerIt->isNegated();

							CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
							invSuperRoleLinker->init(superRole,!superRoleNeg);
							inverseRole->addIndirectSuperRoleLinker(invSuperRoleLinker);

							roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
							inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
						}
						// copy super role linker to new inverse role
						CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
						while (superRoleLinkerIt) {
							CRole* superRole = superRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = superRoleLinkerIt->isNegated();

							CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
							invSuperRoleLinker->init(superRole,!superRoleNeg);
							inverseRole->addSuperRoleLinker(invSuperRoleLinker);

							roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
							superRoleLinkerIt = superRoleLinkerIt->getNext();
						}
						rolesCreated = true;
					}
				}
				return rolesCreated;
			}



			CRole* CConcreteOntologyInverseRoleBuilder::createInverseRole(CRole* role, CConcreteOntology* onto) {

				CRoleVector* roleVec = onto->getRBox()->getRoleVector(true);
				cint64 roleCount = roleVec->getItemCount();

				CMemoryAllocationManager* memMan = onto->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();

				CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* roleObjPropTermHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
				CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* objPropTermRoleHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
				CBUILDLIST<CBuildExpression*>* expressionBuildContainerList = onto->getDataBoxes()->getExpressionDataBoxMapping()->getExpressionBuildListContainer();
				CBUILDHASH<CExpressionHasher,CBuildExpression*>* expressionBuildHash = onto->getDataBoxes()->getExpressionDataBoxMapping()->getStructuralExpressionBuildHash();

				// collect direct and indirect sub roles
				QHash<qint64, QPair<cint64,bool> > roleTagIndirectSubRoleTagNegationHash;
				QHash<qint64, QPair<cint64,bool> > roleTagSubRoleTagNegationHash;
				for (qint64 i = 0; i < roleCount; ++i) {
					CRole *role = roleVec->getData(i);
					if (role) {
						cint64 roleTag = role->getRoleTag();
						CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
						while (inSuperRoleLinkerIt) {
							CRole* superRole = inSuperRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = inSuperRoleLinkerIt->isNegated();
							roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
							inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
						}
						CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
						while (superRoleLinkerIt) {
							CRole* superRole = superRoleLinkerIt->getData();
							cint64 superRoleTag = superRole->getRoleTag();
							bool superRoleNeg = superRoleLinkerIt->isNegated();
							roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(roleTag,superRoleNeg));
							superRoleLinkerIt = superRoleLinkerIt->getNext();
						}
					}
				}

				// build additional inverse role
				role = getLocalizedRole(role,onto);

				CRole* inverseRole = CObjectAllocator< CRole >::allocateAndConstruct(memMan);
				inverseRole->initRole();
				inverseRole->setInverseRole(role);
				cint64 inverseRoleTag = roleVec->getItemCount();
				inverseRole->setRoleTag(inverseRoleTag);
				inverseRole->setRoleComplexity(role->getRoleComplexity());
				roleVec->setData(inverseRole->getRoleTag(),inverseRole);

				CSortedNegLinker<CRole*>* invRoleLinker1 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
				invRoleLinker1->init(role,true);
				inverseRole->addSuperRoleLinker(invRoleLinker1);
				CSortedNegLinker<CRole*>* invRoleLinker2 = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
				invRoleLinker2->init(inverseRole,true);
				role->addSuperRoleLinker(invRoleLinker2);
				role->setInverseRole(inverseRole);




				CObjectPropertyTermExpression* buildExpression = roleObjPropTermHash->value(role);
				CInverseObjectPropertyOfExpression* expression = nullptr;
				expression = new CInverseObjectPropertyOfExpression(buildExpression);
				expressionBuildHash->insert(CExpressionHasher(expression),expression);
				expressionBuildContainerList->append(expression);
				roleObjPropTermHash->insert(role,expression);
				objPropTermRoleHash->insert(expression,role);


				QList< QPair<cint64,bool> > subRoleTagList(roleTagSubRoleTagNegationHash.values(role->getRoleTag()));
				for (QList< QPair<cint64,bool> >::const_iterator it = subRoleTagList.constBegin(), itEnd = subRoleTagList.constEnd(); it != itEnd; ++it) {
					cint64 subRoleTag = it->first;
					CRole* subRole = getLocalizedRole(subRoleTag,onto);
					// add new inverse super role
					CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					superRoleLinker->init(inverseRole,!it->second);
					subRole->addSuperRoleLinker(superRoleLinker);
				}

				QList< QPair<cint64,bool> > indSubRoleTagList(roleTagIndirectSubRoleTagNegationHash.values(role->getRoleTag()));
				for (QList< QPair<cint64,bool> >::const_iterator it = indSubRoleTagList.constBegin(), itEnd = indSubRoleTagList.constEnd(); it != itEnd; ++it) {
					cint64 subRoleTag = it->first;
					CRole* subRole = getLocalizedRole(subRoleTag,onto);
					// add new inverse super role
					CSortedNegLinker<CRole*>* superRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					superRoleLinker->init(inverseRole,!it->second);
					subRole->addIndirectSuperRoleLinker(superRoleLinker);
				}

				// copy indirect super role linker to new inverse role
				CSortedNegLinker<CRole*>* inSuperRoleLinkerIt = role->getIndirectSuperRoleList();
				while (inSuperRoleLinkerIt) {
					CRole* superRole = inSuperRoleLinkerIt->getData();
					cint64 superRoleTag = superRole->getRoleTag();
					bool superRoleNeg = inSuperRoleLinkerIt->isNegated();

					CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					invSuperRoleLinker->init(superRole,!superRoleNeg);
					inverseRole->addIndirectSuperRoleLinker(invSuperRoleLinker);

					roleTagIndirectSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
					inSuperRoleLinkerIt = inSuperRoleLinkerIt->getNext();
				}
				// copy super role linker to new inverse role
				CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getSuperRoleList();
				while (superRoleLinkerIt) {
					CRole* superRole = superRoleLinkerIt->getData();
					cint64 superRoleTag = superRole->getRoleTag();
					bool superRoleNeg = superRoleLinkerIt->isNegated();

					CSortedNegLinker<CRole*>* invSuperRoleLinker = CObjectAllocator< CSortedNegLinker<CRole*> >::allocateAndConstruct(memMan);
					invSuperRoleLinker->init(superRole,!superRoleNeg);
					inverseRole->addSuperRoleLinker(invSuperRoleLinker);

					roleTagSubRoleTagNegationHash.insertMulti(superRoleTag,QPair<cint64,bool>(inverseRoleTag,!superRoleNeg));
					superRoleLinkerIt = superRoleLinkerIt->getNext();
				}
				return inverseRole;
			}



			bool CConcreteOntologyInverseRoleBuilder::hasInverseRole(CRole* role, bool searchInverseEquivalent) {
				return getInverseRole(role,searchInverseEquivalent) != nullptr;
			}


			CRole* CConcreteOntologyInverseRoleBuilder::getInverseRole(CRole* role, bool searchInverseEquivalent) {
				CSortedNegLinker< CRole* >* suberRoleLinkerIt = role->getSuperRoleList();
				while (suberRoleLinkerIt) {
					CRole* superRole = suberRoleLinkerIt->getData();
					bool superRoleNeg = suberRoleLinkerIt->isNegated();
					if (superRoleNeg) {	
						if (!searchInverseEquivalent) {
							return superRole;
						} else {
							CSortedNegLinker< CRole* >* invSuberRoleLinkerIt = superRole->getSuperRoleList();
							while (invSuberRoleLinkerIt) {
								if (invSuberRoleLinkerIt->getData()->getRoleTag() == role->getRoleTag()) {
									return superRole;
								}
								invSuberRoleLinkerIt = invSuberRoleLinkerIt->getNext();
							}
						}
					}
					suberRoleLinkerIt = suberRoleLinkerIt->getNext();
				}
				return nullptr;
			}





			CRole* CConcreteOntologyInverseRoleBuilder::getLocalizedRole(CRole* role, CConcreteOntology* onto) {
				return getLocalizedRole(role->getRoleTag(),onto);
			}


			CRole* CConcreteOntologyInverseRoleBuilder::getLocalizedRole(cint64 roleTag, CConcreteOntology* onto) {
				CRoleVector* roleVec = onto->getRBox()->getRoleVector(true);
				CRole* role = roleVec->getData(roleTag);
				if (!roleVec->hasLocalData(roleTag)) {
					role = CConceptRoleIndividualLocator::getLocatedRole(role,onto);
				}
				return role;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

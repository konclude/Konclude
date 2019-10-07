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

#include "CConceptRoleIndividualLocator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CConceptRoleIndividualLocator::CConceptRoleIndividualLocator() {
			}


			CConceptRoleIndividualLocator::~CConceptRoleIndividualLocator() {
			}


			CConcept* CConceptRoleIndividualLocator::getLocatedConcept(CConcept* concept, CConcreteOntology* ontology) {
				CTBox* tbox = ontology->getDataBoxes()->getTBox();
				CConceptVector* conceptVector = tbox->getConceptVector();
				cint64 conceptTag = concept->getConceptTag();

				if (!conceptVector->hasLocalData(concept->getConceptTag())) {
					CMemoryAllocationManager* memMan = tbox->getBoxContext()->getMemoryAllocationManager();
					CBUILDHASH<CConcept*,CClassTermExpression*>* conceptClassTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();

					CConcept* prevConcept = concept;
					concept = CObjectAllocator<CConcept>::allocateAndConstruct(memMan)->initConceptCopy(prevConcept,memMan);
					conceptVector->setLocalData(conceptTag,concept);

					CClassTermExpression* classTerm = conceptClassTermHash->value(prevConcept);
					if (classTerm) {
						conceptClassTermHash->insert(concept,classTerm);
						CBUILDHASH<CClassTermExpression*,CConcept*>* classTermConceptHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash();
						classTermConceptHash->insert(classTerm,concept);
					}
				}
				return concept;
			}



			CRole* CConceptRoleIndividualLocator::getLocatedRole(CRole* role, CConcreteOntology* ontology) {
				CRBox* rbox = ontology->getDataBoxes()->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector();
				cint64 roleTag = role->getRoleTag();

				if (!roleVector->hasLocalData(role->getRoleTag())) {
					CMemoryAllocationManager* memMan = rbox->getBoxContext()->getMemoryAllocationManager();

					CRole *roleRef = role;
					role = CObjectAllocator<CRole>::allocateAndConstruct(memMan)->initRoleCopy(roleRef,memMan);
					roleVector->setData(roleTag,role);

					if (!role->isDataRole()) {
						CBUILDHASH<CRole*, CObjectPropertyTermExpression*>* roleObjectPropertyTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();
						CObjectPropertyTermExpression* objPropTerm = roleObjectPropertyTermHash->value(roleRef);
						if (objPropTerm) {
							roleObjectPropertyTermHash->insert(role, objPropTerm);

							CBUILDHASH<CObjectPropertyTermExpression*, CRole*>* objectPropertyTermroleHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash();
							objectPropertyTermroleHash->insert(objPropTerm, role);

						}
					} else {
						CBUILDHASH<CRole*, CDataPropertyTermExpression*>* roleDataPropertyTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash();
						CDataPropertyTermExpression* dataPropTerm = roleDataPropertyTermHash->value(roleRef);
						if (dataPropTerm) {
							roleDataPropertyTermHash->insert(role, dataPropTerm);

							CBUILDHASH<CDataPropertyTermExpression*, CRole*>* objectPropertyTermroleHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash();
							objectPropertyTermroleHash->insert(dataPropTerm, role);

						}

					}
				}

				return role;
			}


			CIndividual* CConceptRoleIndividualLocator::getLocatedIndividual(CIndividual* individual, CConcreteOntology* ontology) {
				CABox* abox = ontology->getDataBoxes()->getABox();
				CIndividualVector* indiVector = abox->getIndividualVector();
				cint64 indiTag = individual->getIndividualID();

				CIndividual* localIndi = indiVector->getLocalData(indiTag);
				if (!localIndi) {
					CMemoryAllocationManager* memMan = abox->getBoxContext()->getMemoryAllocationManager();
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* indiIndividualTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();
					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* individualTermIndiHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash();

					localIndi = CObjectAllocator<CIndividual>::allocateAndConstruct(memMan)->initIndividualCopy(individual,memMan);
					indiVector->setLocalData(indiTag,localIndi);

					CIndividualTermExpression* indiTerm = indiIndividualTermHash->value(individual);
					if (indiTerm) {
						indiIndividualTermHash->insert(localIndi,indiTerm);
						individualTermIndiHash->insert(indiTerm,localIndi);
					}
				}
				return localIndi;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

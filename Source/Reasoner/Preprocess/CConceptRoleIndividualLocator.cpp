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
					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* roleObjectPropertyTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash();

					CRole *roleRef = role;
					role = CObjectAllocator<CRole>::allocateAndConstruct(memMan)->initRoleCopy(roleRef,memMan);
					roleVector->setData(roleTag,role);

					CObjectPropertyTermExpression* objPropTerm = roleObjectPropertyTermHash->value(role);
					if (objPropTerm) {
						roleObjectPropertyTermHash->insert(role,objPropTerm);
					}
				}

				return role;
			}


			CIndividual* CConceptRoleIndividualLocator::getLocatedIndividual(CIndividual* individual, CConcreteOntology* ontology) {
				CABox* abox = ontology->getDataBoxes()->getABox();
				CIndividualVector* indiVector = abox->getIndividualVector();
				cint64 indiTag = individual->getIndividualID();

				if (!indiVector->hasLocalData(indiTag)) {
					CMemoryAllocationManager* memMan = abox->getBoxContext()->getMemoryAllocationManager();
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* indiIndividualTermHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash();

					CIndividual* prevIndividual = individual;
					individual = CObjectAllocator<CIndividual>::allocateAndConstruct(memMan)->initIndividualCopy(prevIndividual,memMan);
					indiVector->setLocalData(indiTag,individual);

					CIndividualTermExpression* indiTerm = indiIndividualTermHash->value(prevIndividual);
					if (indiTerm) {
						indiIndividualTermHash->insert(individual,indiTerm);
					}
				}
				return individual;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

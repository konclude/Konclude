/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CEntityExpressionSetResultVisitingCallback.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEntityExpressionSetResultVisitingCallback::CEntityExpressionSetResultVisitingCallback() {
			}

			
			CEntityExpressionSetResultVisitingCallback* CEntityExpressionSetResultVisitingCallback::visitConceptAssociatedEntityExpression(CConcept* concept, CConcreteOntology* ontology) {
				CClassTermExpression* classTermExp = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash()->value(concept);
				if (classTermExp) {
					CClassExpression* classExp = dynamic_cast<CClassExpression*>(classTermExp);
					if (classExp) {
						return visitEntityExpression(classExp,ontology);
					}
				}
				return this;
			}



			CEntityExpressionSetResultVisitingCallback* CEntityExpressionSetResultVisitingCallback::visitRoleAssociatedEntityExpression(CRole* role, CConcreteOntology* ontology) {
				CPropertyTermExpression* propertyTermExp = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleObjectPropertyTermMappingHash()->value(role);
				if (propertyTermExp) {
					CObjectPropertyExpression* propertyExp = dynamic_cast<CObjectPropertyExpression*>(propertyTermExp);
					if (propertyExp) {
						return visitEntityExpression(propertyExp,ontology);
					}
				} else {
					propertyTermExp = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getRoleDataPropertyTermMappingHash()->value(role);
					if (propertyTermExp) {
						CDataPropertyExpression* propertyExp = dynamic_cast<CDataPropertyExpression*>(propertyTermExp);
						if (propertyExp) {
							return visitEntityExpression(propertyExp,ontology);
						}
					}
				}
				return this;
			}


			CEntityExpressionSetResultVisitingCallback* CEntityExpressionSetResultVisitingCallback::visitIndividualAssociatedEntityExpression(CIndividual* indi, CConcreteOntology* ontology) {
				CIndividualTermExpression* indiTermExp = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getIndiIndividulTermMappingHash()->value(indi);
				if (indiTermExp) {
					CNamedIndividualExpression* indiExp = dynamic_cast<CNamedIndividualExpression*>(indiTermExp);
					if (indiExp) {
						return visitEntityExpression(indiExp,ontology);
					}
				}
				return this;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

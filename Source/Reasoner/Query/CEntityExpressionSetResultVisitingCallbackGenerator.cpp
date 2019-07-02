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

#include "CEntityExpressionSetResultVisitingCallbackGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CEntityExpressionSetResultVisitingCallbackGenerator::CEntityExpressionSetResultVisitingCallbackGenerator(CEntityExpressionSetResultVisitingCallback* visitingCallback, CConcreteOntology* ontology) {
				mVisitingCallback = visitingCallback;
				mOntology = ontology;
			}

			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization) {
				conRealization->visitConcepts(item,this);
				return true;
			}

			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
				mVisitingCallback->visitConceptAssociatedEntityExpression(concept,mOntology);
				return true;
			}


			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitRoleInstantiated(CRoleInstantiatedItem* item, CRoleRealization* roleRealization) {
				roleRealization->visitRoles(item,this);
				return true;
			}

			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitRole(CRole* role, CRoleRealization* roleRealization) {
				mVisitingCallback->visitRoleAssociatedEntityExpression(role,mOntology);
				return true;
			}

			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
				roleRealization->visitIndividuals(indiRealItemRef,this);
				return true;
			}


			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization) {
				conRealization->visitIndividuals(indiRealItemRef,this);
				return true;
			}


			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
				CIndividual* indi = indiRef.getIndividual();
				if (!indi) {
					indi = mOntology->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
				}
				if (indi) {
					mVisitingCallback->visitIndividualAssociatedEntityExpression(indi, mOntology);
				}
				return true;
			}


			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization) {
				CIndividual* indi = indiRef.getIndividual();
				if (!indi) {
					indi = mOntology->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
				}
				if (indi) {
					mVisitingCallback->visitIndividualAssociatedEntityExpression(indi, mOntology);
				}
				return true;
			}


			bool CEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
				CIndividual* indi = indiRef.getIndividual();
				if (!indi) {
					indi = mOntology->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
				}
				if (indi) {
					mVisitingCallback->visitIndividualAssociatedEntityExpression(indi, mOntology);
				}
				return true;
			}




		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

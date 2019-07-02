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

#include "CSetOfEntityExpressionSetResultVisitingCallbackGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSetOfEntityExpressionSetResultVisitingCallbackGenerator::CSetOfEntityExpressionSetResultVisitingCallbackGenerator(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, CConcreteOntology* ontology) {
				mVisitingCallback = visitingCallback;
				mOntology = ontology;
			}

			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization) {
				mVisitingCallback->startEntityExpressionSet(mOntology);
				conRealization->visitConcepts(item,this);
				mVisitingCallback->endEntityExpressionSet(mOntology);
				return true;
			}

			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitConcept(CConcept* concept, CConceptRealization* conRealization) {
				mVisitingCallback->visitConceptAssociatedEntityExpression(concept,mOntology);
				return true;
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitRoleInstantiated(CRoleInstantiatedItem* item, CRoleRealization* roleRealization) {
				mVisitingCallback->startEntityExpressionSet(mOntology);
				roleRealization->visitRoles(item,this);
				mVisitingCallback->endEntityExpressionSet(mOntology);
				return true;
			}

			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitRole(CRole* role, CRoleRealization* roleRealization) {
				mVisitingCallback->visitRoleAssociatedEntityExpression(role,mOntology);
				return true;
			}

			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
				mVisitingCallback->startEntityExpressionSet(mOntology);
				roleRealization->visitIndividuals(indiRealItemRef,this);
				mVisitingCallback->endEntityExpressionSet(mOntology);
				return true;
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization) {
				mVisitingCallback->startEntityExpressionSet(mOntology);
				conRealization->visitIndividuals(indiRealItemRef,this);
				mVisitingCallback->endEntityExpressionSet(mOntology);
				return true;
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization) {
				return visitIndividual(indiRef);
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization) {
				return visitIndividual(indiRef);
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
				return visitIndividual(indiRef);
			}


			bool CSetOfEntityExpressionSetResultVisitingCallbackGenerator::visitIndividual(const CIndividualReference& indiRef) {
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

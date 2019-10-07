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

#include "CRoleRealization.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {


			CRoleRealization::CRoleRealization() {
			}


			CRealizationIndividualInstanceItemReference CRoleRealization::getRoleInstanceItemReference(CIndividual* individual) {
				return getRoleInstanceItemReference(CIndividualReference(individual));
			}


			bool CRoleRealization::visitTargetIndividuals(CIndividual* individual, CRole* role, CRoleRealizationInstanceVisitor* visitor) {
				return visitTargetIndividuals(CIndividualReference(individual), role,visitor);
			}

			bool CRoleRealization::visitSourceIndividuals(CIndividual* individual, CRole* role, CRoleRealizationInstanceVisitor* visitor) {
				return visitSourceIndividuals(CIndividualReference(individual), role,visitor);
			}


			bool CRoleRealization::visitSourceIndividualRoles(CIndividual* individual, CRoleRealizationInstantiatedVisitor* visitor) {
				return visitSourceIndividualRoles(CIndividualReference(individual),visitor);
			}

			bool CRoleRealization::visitTargetIndividualRoles(CIndividual* individual, CRoleRealizationInstantiatedVisitor* visitor) {
				return visitTargetIndividualRoles(CIndividualReference(individual),visitor);
			}

			bool CRoleRealization::visitBetweeenIndividualRoles(CIndividual* sourceIndividual, CIndividual* targetIndividual, CRoleRealizationInstantiatedVisitor* visitor) {
				return visitBetweeenIndividualRoles(CIndividualReference(sourceIndividual), CIndividualReference(targetIndividual),visitor);
			}

			bool CRoleRealization::isRoleInstance(CRole* role, CIndividual* sourceIndividual, CIndividual* targetIndividual) {
				return isRoleInstance(role, CIndividualReference(sourceIndividual), CIndividualReference(targetIndividual));
			}





			bool CRoleRealization::visitTargetIndividuals(const CIndividualReference& indiRef, CRole* role, CRoleRealizationInstanceVisitor* visitor) {
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);
				CRealizationIndividualInstanceItemReference indiRealItemRef = getRoleInstanceItemReference(indiRef);
				return visitTargetIndividuals(indiRealItemRef, roleItem, visitor);
			}

			bool CRoleRealization::visitSourceIndividuals(const CIndividualReference& indiRef, CRole* role, CRoleRealizationInstanceVisitor* visitor) {
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);
				CRealizationIndividualInstanceItemReference indiRealItemRef = getRoleInstanceItemReference(indiRef);
				return visitSourceIndividuals(indiRealItemRef, roleItem, visitor);
			}


			bool CRoleRealization::visitSourceIndividualRoles(const CIndividualReference& indiRef, CRoleRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getRoleInstanceItemReference(indiRef);
				return visitSourceIndividualRoles(indiRealItemRef, visitor);
			}

			bool CRoleRealization::visitTargetIndividualRoles(const CIndividualReference& indiRef, CRoleRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getRoleInstanceItemReference(indiRef);
				return visitTargetIndividualRoles(indiRealItemRef, visitor);
			}

			bool CRoleRealization::visitBetweeenIndividualRoles(const CIndividualReference& sourceIndiRef, const CIndividualReference& targetIndiRef, CRoleRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference sourceIndiItem = getRoleInstanceItemReference(sourceIndiRef);
				CRealizationIndividualInstanceItemReference targetIndiItem = getRoleInstanceItemReference(targetIndiRef);
				return visitBetweeenIndividualRoles(sourceIndiItem, targetIndiItem, visitor);
			}

			bool CRoleRealization::isRoleInstance(CRole* role, const CIndividualReference& sourceIndiRef, const CIndividualReference& targetIndiRef) {
				CRealizationIndividualInstanceItemReference sourceIndiItem = getRoleInstanceItemReference(sourceIndiRef);
				CRealizationIndividualInstanceItemReference targetIndiItem = getRoleInstanceItemReference(targetIndiRef);
				return isRoleInstance(role, sourceIndiItem, targetIndiItem);
			}

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

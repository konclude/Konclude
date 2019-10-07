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

#include "CConceptRealization.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {


			CConceptRealization::CConceptRealization() {
			}


			bool CConceptRealization::visitDirectInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor) {
				CConceptInstantiatedItem* conceptItem = getInstantiatedItem(concept);
				return visitDirectInstances(conceptItem,visitor);
			}

			bool CConceptRealization::visitAllInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor) {
				CConceptInstantiatedItem* conceptItem = getInstantiatedItem(concept);
				return visitAllInstances(conceptItem,visitor);
			}

			bool CConceptRealization::visitInstances(CConcept* concept, bool direct, CConceptRealizationInstanceVisitor* visitor) {
				CConceptInstantiatedItem* conceptItem = getInstantiatedItem(concept);
				return visitInstances(conceptItem,direct,visitor);
			}


			bool CConceptRealization::visitTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, bool direct, CConceptRealizationInstantiatedVisitor* visitor) {
				if (direct) {
					return visitDirectTypes(indiRealItemRef,visitor);
				} else {
					return visitAllTypes(indiRealItemRef,visitor);
				}
			}


			CRealizationIndividualInstanceItemReference CConceptRealization::getInstanceItemReference(CIndividual* individual) {
				return getInstanceItemReference(CIndividualReference(individual));
			}



			bool CConceptRealization::visitDirectTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor) {
				return visitDirectTypes(CIndividualReference(individual), visitor);
			}

			bool CConceptRealization::visitAllTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor) {
				return visitAllTypes(CIndividualReference(individual), visitor);
			}

			bool CConceptRealization::visitTypes(CIndividual* individual, bool direct, CConceptRealizationInstantiatedVisitor* visitor) {
				return visitTypes(CIndividualReference(individual), direct, visitor);
			}


			bool CConceptRealization::visitSameIndividuals(CIndividual* individual, CConceptRealizationIndividualVisitor* visitor) {
				return visitSameIndividuals(CIndividualReference(individual), visitor);
			}




			bool CConceptRealization::visitDirectTypes(const CIndividualReference& indiRef, CConceptRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getInstanceItemReference(indiRef);
				return visitDirectTypes(indiRealItemRef, visitor);
			}

			bool CConceptRealization::visitAllTypes(const CIndividualReference& indiRef, CConceptRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getInstanceItemReference(indiRef);
				return visitAllTypes(indiRealItemRef, visitor);
			}

			bool CConceptRealization::visitTypes(const CIndividualReference& indiRef, bool direct, CConceptRealizationInstantiatedVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getInstanceItemReference(indiRef);
				return visitTypes(indiRealItemRef, direct, visitor);
			}


			bool CConceptRealization::visitSameIndividuals(const CIndividualReference& indiRef, CConceptRealizationIndividualVisitor* visitor) {
				CRealizationIndividualInstanceItemReference indiRealItemRef = getInstanceItemReference(indiRef);
				return visitIndividuals(indiRealItemRef, visitor);
			}

			bool CConceptRealization::isConceptInstance(const CIndividualReference& indiRef, CConcept* concept) {
				return isConceptInstance(getInstanceItemReference(indiRef), concept);
			}

			bool CConceptRealization::isConceptInstance(CIndividual* individual, CConcept* concept) {
				return isConceptInstance(getInstanceItemReference(individual), concept);
			}



			bool CConceptRealization::visitInstances(CConceptInstantiatedItem* item, bool direct, CConceptRealizationInstanceVisitor* visitor) {
				if (direct) {
					return visitDirectInstances(item,visitor);
				} else {
					return visitAllInstances(item,visitor);
				}
			}

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

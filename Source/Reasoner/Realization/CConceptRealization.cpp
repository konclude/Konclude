/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

			bool CConceptRealization::visitInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor) {
				CConceptInstantiatedItem* conceptItem = getInstantiatedItem(concept);
				return visitInstances(conceptItem,visitor);
			}

			bool CConceptRealization::visitInstances(CConcept* concept, bool direct, CConceptRealizationInstanceVisitor* visitor) {
				CConceptInstantiatedItem* conceptItem = getInstantiatedItem(concept);
				return visitInstances(conceptItem,direct,visitor);
			}

			bool CConceptRealization::visitDirectTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor) {
				CConceptInstanceItem* individualItem = getInstanceItem(individual);
				return visitDirectTypes(individualItem,visitor);
			}

			bool CConceptRealization::visitTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor) {
				CConceptInstanceItem* individualItem = getInstanceItem(individual);
				return visitTypes(individualItem,visitor);
			}

			bool CConceptRealization::visitTypes(CIndividual* individual, bool direct, CConceptRealizationInstantiatedVisitor* visitor) {
				CConceptInstanceItem* individualItem = getInstanceItem(individual);
				return visitTypes(individualItem,direct,visitor);;
			}

			
			bool CConceptRealization::visitSameIndividuals(CIndividual* individual, CConceptRealizationIndividualVisitor* visitor) {
				CConceptInstanceItem* individualItem = getInstanceItem(individual);
				return visitIndividuals(individualItem,visitor);
			}

			bool CConceptRealization::visitTypes(CConceptInstanceItem* item, bool direct, CConceptRealizationInstantiatedVisitor* visitor) {
				if (direct) {
					return visitDirectTypes(item,visitor);
				} else {
					return visitTypes(item,visitor);
				}
			}

			bool CConceptRealization::visitInstances(CConceptInstantiatedItem* item, bool direct, CConceptRealizationInstanceVisitor* visitor) {
				if (direct) {
					return visitDirectInstances(item,visitor);
				} else {
					return visitInstances(item,visitor);
				}
			}

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

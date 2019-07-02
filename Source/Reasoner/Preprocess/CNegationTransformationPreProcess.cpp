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

#include "CNegationTransformationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CNegationTransformationPreProcess::CNegationTransformationPreProcess() {
			}


			CNegationTransformationPreProcess::~CNegationTransformationPreProcess() {
			}


			CConcreteOntology *CNegationTransformationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				CTBox* tbox = ontology->getDataBoxes()->getTBox();
				CABox *abox = ontology->getDataBoxes()->getABox();
				CRBox *rbox = ontology->getDataBoxes()->getRBox();

				CConceptVector *concepts = tbox->getConceptVector();
				qint64 itemConCounts = concepts->getItemCount();
				for (qint64 i = 0; i < itemConCounts; ++i) {
					CConcept *concept = concepts->getLocalData(i);

					if (concept && concept->getOperatorCode() != CCNOT) {

						CSortedNegLinker<CConcept *> *operands = concept->getOperandList();
						while (operands) {
							while (operands->getData()->getDefinitionOperatorTag() == CCNOT) {
								CConcept *compConcept = operands->getData()->getOperandList()->getData();
								operands->setData(compConcept);
								operands->setNegated(!operands->isNegated());
							}
							operands = operands->getNext();
						}
					}
				}


				CRoleVector *roles = rbox->getRoleVector();
				qint64 itemRolCounts = roles->getItemCount();
				for (qint64 i = 0; i < itemRolCounts; ++i) {
					CRole *role = roles->getLocalData(i);

					if (role) {

						CSortedNegLinker<CConcept *> *operandsDomIt = role->getDomainConceptList();
						while (operandsDomIt) {
							while (operandsDomIt->getData()->getDefinitionOperatorTag() == CCNOT) {
								CConcept *compConcept = operandsDomIt->getData()->getOperandList()->getData();
								operandsDomIt->setData(compConcept);
								operandsDomIt->setNegated(!operandsDomIt->isNegated());
							}
							operandsDomIt = operandsDomIt->getNext();
						}

						CSortedNegLinker<CConcept *> *operandsRanIt = role->getRangeConceptList();
						while (operandsRanIt) {
							while (operandsRanIt->getData()->getDefinitionOperatorTag() == CCNOT) {
								CConcept *compConcept = operandsRanIt->getData()->getOperandList()->getData();
								operandsRanIt->setData(compConcept);
								operandsRanIt->setNegated(!operandsRanIt->isNegated());
							}
							operandsRanIt = operandsRanIt->getNext();
						}
					}
				}

				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

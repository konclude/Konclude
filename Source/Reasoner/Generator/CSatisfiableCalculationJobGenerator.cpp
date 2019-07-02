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

#include "CSatisfiableCalculationJobGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CSatisfiableCalculationJobGenerator::CSatisfiableCalculationJobGenerator(CConcreteOntology *ontology) {
				mOntology = ontology;
			}


			CSatisfiableCalculationJobGenerator::~CSatisfiableCalculationJobGenerator() {
			}



			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(CConcept* concept, bool negation, CIndividual* individual, CSatisfiableCalculationJob* satCalcJob) {
				return getSatisfiableCalculationJob(QList< QPair<CConcept*,bool> >()<<QPair<CConcept*,bool>(concept,negation),individual,satCalcJob);
			}


			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(CConcept* concept1, bool negation1, CConcept* concept2, bool negation2, CIndividual* individual, CSatisfiableCalculationJob* satCalcJob) {
				return getSatisfiableCalculationJob(QList< QPair<CConcept*,bool> >()<<QPair<CConcept*,bool>(concept1,negation1)<<QPair<CConcept*,bool>(concept2,negation2),individual,satCalcJob);
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList<CConcept*>& conceptList, CIndividual* individual, CSatisfiableCalculationJob* satCalcJob) {
				QList< QPair<CConcept*,bool> > conNegList;
				FOREACHIT (CConcept* concept, conceptList) {
					conNegList.append(QPair<CConcept*,bool>(concept,false));
				}
				return getSatisfiableCalculationJob(conNegList,individual,satCalcJob);
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList< QPair<CConcept*,bool> >& conceptNegationList, CIndividual* individual, CSatisfiableCalculationJob* satCalcJob) {
				if (!satCalcJob) {
					satCalcJob = new CSatisfiableCalculationJob();
				}
				satCalcJob->setOntology(mOntology);
				CSatisfiableCalculationConceptConstruct* satCalcConstruct = new CSatisfiableCalculationConceptConstruct();

				for (QList< QPair<CConcept*,bool> >::const_iterator it = conceptNegationList.constBegin(), itEnd = conceptNegationList.constEnd(); it != itEnd; ++it) {
					CConcept* concept = it->first;
					bool negation = it->second;

					CXSortedNegLinker<CConcept*>* conNegLinker = new CXSortedNegLinker<CConcept*>(concept,negation);
					satCalcConstruct->addConstructConceptLinker(conNegLinker);
				}

				if (individual) {
					satCalcConstruct->setIndividual(individual);
				} else {
					satCalcConstruct->setRelativeNewNodeID(satCalcJob->getNextRelativeNodeID());
				}
				satCalcJob->addSatisfiableCalculationConstruct(satCalcConstruct);
				return satCalcJob;
			}



			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList< CIndividual* >& indiList, CSatisfiableCalculationJob* satCalcJob) {
				if (!satCalcJob) {
					satCalcJob = new CSatisfiableCalculationJob();
				}
				satCalcJob->setOntology(mOntology);
				for (QList< CIndividual* >::const_iterator it = indiList.constBegin(), itEnd = indiList.constEnd(); it != itEnd; ++it) {
					CIndividual* individual = (*it);
					CSatisfiableCalculationConceptConstruct* satCalcConstruct = new CSatisfiableCalculationConceptConstruct();
					if (individual) {
						satCalcConstruct->setIndividual(individual);
					} else {
						satCalcConstruct->setRelativeNewNodeID(satCalcJob->getNextRelativeNodeID());
					}
					satCalcJob->addSatisfiableCalculationConstruct(satCalcConstruct);
				}
				return satCalcJob;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

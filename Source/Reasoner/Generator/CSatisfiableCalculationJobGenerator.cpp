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
				return getSatisfiableCalculationJob(concept, negation,CIndividualReference(individual),satCalcJob);
			}


			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(CConcept* concept, bool negation, const CIndividualReference& indiRef, CSatisfiableCalculationJob* satCalcJob) {
				return getSatisfiableCalculationJob(QList< QPair<CConcept*, bool> >() << QPair<CConcept*, bool>(concept, negation), indiRef, satCalcJob);
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

			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList< QPair<CConcept*, bool> >& conceptNegationList, CIndividual* individual, CSatisfiableCalculationJob* satCalcJob) {
				return getSatisfiableCalculationJob(conceptNegationList, CIndividualReference(individual), satCalcJob);
			}

			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList< QPair<CConcept*,bool> >& conceptNegationList, const CIndividualReference& indiRef, CSatisfiableCalculationJob* satCalcJob) {
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

				if (indiRef.isNonEmpty()) {
					if (indiRef.getIndividual()) {
						satCalcConstruct->setIndividual(indiRef.getIndividual());
					} else {
						satCalcConstruct->setIndividualID(indiRef.getIndividualID());
					}
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


			CSatisfiableCalculationJob* CSatisfiableCalculationJobGenerator::getSatisfiableCalculationJob(const QList<CIndividualReference>& indiList, CSatisfiableCalculationJob* satCalcJob) {
				if (!satCalcJob) {
					satCalcJob = new CSatisfiableCalculationJob();
				}
				satCalcJob->setOntology(mOntology);
				for (QList<CIndividualReference>::const_iterator it = indiList.constBegin(), itEnd = indiList.constEnd(); it != itEnd; ++it) {
					CIndividualReference individualReference = (*it);
					CSatisfiableCalculationConceptConstruct* satCalcConstruct = new CSatisfiableCalculationConceptConstruct();
					if (individualReference.isNonEmpty()) {
						if (individualReference.getIndividual()) {
							satCalcConstruct->setIndividual(individualReference.getIndividual());
						} else {
							satCalcConstruct->setIndividualID(individualReference.getIndividualID());
						}
					}
					satCalcJob->addSatisfiableCalculationConstruct(satCalcConstruct);
				}
				return satCalcJob;
			}

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

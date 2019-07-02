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

#include "COntologyAssignmentTransformationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			COntologyAssignmentTransformationPreProcess::COntologyAssignmentTransformationPreProcess(CConcreteOntology *ontologyAssignment) {
				ontology = ontologyAssignment;
			}


			COntologyAssignmentTransformationPreProcess::~COntologyAssignmentTransformationPreProcess() {
			}


			CConcreteOntology *COntologyAssignmentTransformationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				CMBox *mBox = ontology->getDataBoxes()->getMBox();
				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CABox *aBox = ontology->getDataBoxes()->getABox();

				if (mBox) {
					if (ontology) {
						qint64 ontID = ontology->getTerminologyID();

						CConceptVector *concepts = tBox->getConceptVector();
						qint64 conceptCount = tBox->getConceptCount();
						for (qint64 i = 0; i < conceptCount; ++i) {
							CConcept *concept = concepts->getLocalData(i);
							if (concept) {
								concept->setTerminology(ontology);
							}
						}

						CRoleVector *roles = rBox->getRoleVector();
						qint64 roleCount = rBox->getRoleCount();
						for (qint64 i = 0; i < roleCount; ++i) {
							CRole *role = roles->getLocalData(i);
							if (role) {
								role->setTerminology(ontology);
							}
						}

					}
				}
				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "COntologyStructureInspectionPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			COntologyStructureInspectionPreProcess::COntologyStructureInspectionPreProcess() {
			}


			COntologyStructureInspectionPreProcess::~COntologyStructureInspectionPreProcess() {
			}

			CConcreteOntology *COntologyStructureInspectionPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				COntologyInspector ontIns;
				ontIns.createGCIConceptSet(ontology->getDataBoxes()->getTBox());
				COntologyStructureSummary* ontStructSum = ontIns.createStructureSummary(ontology);

				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();
				if (nonELConstructsUsed) {
					ontIns.analyzeOntologyConceptsStructureFlags(ontology,ontStructSum);
				} else {
					ontStructSum->setNominalOccurrence(false);
					ontStructSum->setOnlyELConceptClasses(true);
					ontStructSum->setOnlyDeterministicConceptClasses(true);
				}

				LOG(INFO,"::Konclude::Reasoner::Kernel::Preprocess::StructureInspection",logTr("Detected Ontology Expressiveness: '%1'.").arg(ontology->getStructureSummary()->getExpressivenessString()),this);

				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

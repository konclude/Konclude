/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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

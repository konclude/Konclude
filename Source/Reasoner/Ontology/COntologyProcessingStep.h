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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGSTEP_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGSTEP_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyProcessingRequirement.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyProcessingStep
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyProcessingStep {
				// public methods
				public:

					enum PROCESSINGSTEPTYPE {
						OPSTRIPLESMAPPING = 0, OPSACTIVECOUNT = 1, OPSBUILD = 2, OPSPREPROCESS = 3, OPSTRIPLESINDEXING = 4, OPSCONSISTENCY = 5, OPSPRECOMPUTEINDIVIDUAL = 6, OPSPRECOMPUTESATURATION = 7, OPSPRECOMPUTECYCLES = 8, OPSPRECOMPUTEOCCURRENCESTATISTICS = 9,
						OPSCLASSCLASSIFY = 10, OPSOBJECTROPERTYCLASSIFY = 11, OPSDATAROPERTYCLASSIFY = 12, OPSINITREALIZE = 13, OPSCONCEPTREALIZE = 14, OPSROLEREALIZE = 15, OPSSAMEINDIVIDUALSREALIZE = 16, OPSANSWERCOMPLEXQUERY = 17
					};
					enum PROCESSORTYPE {
						OPPREPROCESSOR, OPPRECOMPUTER, OPCLASSCLASSIFIER, OPOBJECTPROPERTYCLASSIFIER, OPDATAPROPERTYCLASSIFIER, OPREALIZER, OPANSWERER
					};

					//! Constructor
					COntologyProcessingStep(PROCESSINGSTEPTYPE processingType, cint64 id, PROCESSORTYPE processorType);

					PROCESSINGSTEPTYPE getOntologyProcessingType();
					cint64 getProcessingStepID();

					COntologyProcessingStep* addProcessingRequirement(COntologyProcessingRequirement* requirement);

					PROCESSORTYPE getProcessorType();

					QList<COntologyProcessingRequirement*>* getProcessingRequirementList();

					bool areAllRequirementsSatisfied(COntology* ontology);

					const QString getProcessingStepName();


				// protected methods
				protected:

				// protected variables
				protected:
					PROCESSINGSTEPTYPE mProcessingType;
					cint64 mProcessingStepID;
					PROCESSORTYPE mProcessorType;

					QList<COntologyProcessingRequirement*> mProcessingRequirementList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGSTEP_H

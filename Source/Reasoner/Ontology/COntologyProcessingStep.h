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

					enum PROCESSINGSTEPTYPE { OPSACTIVECOUNT = 0, OPSBUILD = 1, OPSPREPROCESS = 2, OPSCONSISTENCY = 3, OPSPRECOMPUTEINDIVIDUAL = 4, OPSPRECOMPUTESATURATION = 5, OPSPRECOMPUTECYCLES = 6, OPSCLASSCLASSIFY = 7, OPSCONCEPTREALIZE = 8, OPSROLEREALIZE = 9, OPSSAMEINDIVIDUALSREALIZE = 10 };
					enum PROCESSORTYPE { OPPREPROCESSOR, OPPRECOMPUTER, OPCLASSCLASSIFIER, OPREALIZER };

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

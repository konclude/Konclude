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

#ifndef KONCLUDE_REASONER_CONSISTISER_CSATURATIONCOMMONDISJUNCTCONCEPTSEXTRACTOR_H
#define KONCLUDE_REASONER_CONSISTISER_CSATURATIONCOMMONDISJUNCTCONCEPTSEXTRACTOR_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Consistence/CSaturationData.h"

#include "Reasoner/Kernel/Task/CSaturationTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"

#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistence;
		using namespace Kernel::Task;
		using namespace Kernel::Process;

		namespace Consistiser {


			/*! 
			 *
			 *		\class		CSaturationCommonDisjunctConceptsExtractor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationCommonDisjunctConceptsExtractor {
				// public methods
				public:
					//! Constructor
					CSaturationCommonDisjunctConceptsExtractor();

					bool extractCommonDisjunctConcepts(CConcreteOntology* ontology, CSaturationData* saturationData, QSet<CConcept*>* disjunctionConceptSet);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CSATURATIONCOMMONDISJUNCTCONCEPTSEXTRACTOR_H

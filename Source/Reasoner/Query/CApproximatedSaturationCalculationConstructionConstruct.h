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

#ifndef KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONCONSTRUCTIONCONSTRUCT_H
#define KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONCONSTRUCTIONCONSTRUCT_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CIndividual.h"

#include "Utilities/Container/CSortedNegLinker.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"
#include "Reasoner/Consistiser/CSaturationIndividualReferenceLinking.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistiser;

		namespace Query {

			/*! 
			 *
			 *		\class		CApproximatedSaturationCalculationConstructionConstruct
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CApproximatedSaturationCalculationConstructionConstruct : public CLinkerBase<CApproximatedSaturationCalculationConstructionConstruct*,CApproximatedSaturationCalculationConstructionConstruct> {
				// public methods
				public:
					//! Constructor
					CApproximatedSaturationCalculationConstructionConstruct();

					CApproximatedSaturationCalculationConstructionConstruct* getNextConstruct();

					CConcept* getConstructConcept();
					CApproximatedSaturationCalculationConstructionConstruct* setConstructConcept(CConcept* concept);

					CRole* getConstructRole();
					CApproximatedSaturationCalculationConstructionConstruct* setConstructRole(CRole* role);

					bool getConstructConceptNegation();
					CApproximatedSaturationCalculationConstructionConstruct* setConstructConceptNegation(bool negation);

					CApproximatedSaturationCalculationConstructionConstruct* setIndividual(CIndividual* individual);
					CIndividual* getIndividual();

					CApproximatedSaturationCalculationConstructionConstruct* setIndividualID(cint64 individualID);
					cint64 getIndividualID();

					bool getQueueProcessing();
					CApproximatedSaturationCalculationConstructionConstruct* setQueueProcessing(bool queueProcessing);

					CSaturationConceptReferenceLinking* getSaturationConceptReferenceLinking();
					CApproximatedSaturationCalculationConstructionConstruct* setSaturationConceptReferenceLinking(CSaturationConceptReferenceLinking* satConRefLinking);
					
					CSaturationIndividualReferenceLinking* getSaturationIndividualReferenceLinking();
					CApproximatedSaturationCalculationConstructionConstruct* setSaturationIndividualReferenceLinking(CSaturationIndividualReferenceLinking* satIndiRefLinking);

				// protected methods
				protected:

				// protected variables
				protected:
					CSaturationConceptReferenceLinking* mSatConRefLinking;
					CSaturationIndividualReferenceLinking* mSatIndiRefLinking;
					CConcept* mConstructConcept;
					bool mConstructConceptNegation;
					CIndividual* mIndividual;
					CRole* mConstructRole;
					cint64 mIndiID;
					bool mQueueProcessing;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONCONSTRUCTIONCONSTRUCT_H

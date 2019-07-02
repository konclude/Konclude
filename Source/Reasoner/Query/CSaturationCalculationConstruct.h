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

#ifndef KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONCONSTRUCT_H
#define KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONCONSTRUCT_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CIndividual.h"

#include "Utilities/Container/CSortedNegLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CSaturationCalculationConstruct
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationCalculationConstruct : public CLinkerBase<CSaturationCalculationConstruct*,CSaturationCalculationConstruct> {
				// public methods
				public:
					//! Constructor
					CSaturationCalculationConstruct();

					CSaturationCalculationConstruct* getNextConstruct();

					CConcept* getConstructConcept();
					CSaturationCalculationConstruct* setConstructConcept(CConcept* concept);

					bool getConstructConceptNegation();
					CSaturationCalculationConstruct* setConstructConceptNegation(bool negation);

					CSaturationCalculationConstruct* setIndividual(CIndividual* individual);
					CIndividual* getIndividual();

					CSaturationCalculationConstruct* setIndividualID(cint64 individualID);
					cint64 getIndividualID();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mConstructConcept;
					bool mConstructConceptNegation;
					CIndividual* mIndividual;
					cint64 mIndiID;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATURATIONCALCULATIONCONSTRUCT_H

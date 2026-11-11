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

#ifndef KONCLUDE_REASONER_REALIZER_CINDIVIDUALDATALITERALTESTINGITEM_H
#define KONCLUDE_REASONER_REALIZER_CINDIVIDUALDATALITERALTESTINGITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizingTestingItem.h"
#include "COptimizedRepresentativeKPSetOntologyRealizingItem.h"


// Other includes
#include "Reasoner/Ontology/CIndividualReference.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		CIndividualDataLiteralTestingItem
			 *
			 *		\brief		Verifies a non-deterministically derived data value with a satisfiability
			 *					test of the individual together with the negated data value.
			 *
			 */
			class CIndividualDataLiteralTestingItem : public CRealizingTestingItem {
				// public methods
				public:
					//! Constructor
					CIndividualDataLiteralTestingItem(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem, COptimizedRepresentativeKPSetDataLiteralRealizationData* data, COptimizedRepresentativeKPSetDataLiteralCandidate* candidate, const CIndividualReference& indiRef, COntologyRealizingDynamicRequirmentProcessingData* procData);

					virtual REALIZINGTESTINGTYPE getRealizingTestingType();

					COptimizedRepresentativeKPSetDataLiteralRealizationData* getDataLiteralRealizationData();
					COptimizedRepresentativeKPSetDataLiteralCandidate* getDataLiteralCandidate();
					CIndividualReference getIndividualReference();

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedRepresentativeKPSetDataLiteralRealizationData* mData;
					COptimizedRepresentativeKPSetDataLiteralCandidate* mCandidate;
					CIndividualReference mIndividualReference;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CINDIVIDUALDATALITERALTESTINGITEM_H

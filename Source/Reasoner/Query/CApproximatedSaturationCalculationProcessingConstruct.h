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

#ifndef KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONPROCESSINGCONSTRUCT_H
#define KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONPROCESSINGCONSTRUCT_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"

// Other includes
#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Consistiser;

		namespace Query {

			/*! 
			 *
			 *		\class		CApproximatedSaturationCalculationProcessingConstruct
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CApproximatedSaturationCalculationProcessingConstruct : public CLinkerBase<CApproximatedSaturationCalculationProcessingConstruct*,CApproximatedSaturationCalculationProcessingConstruct> {
				// public methods
				public:
					//! Constructor
					CApproximatedSaturationCalculationProcessingConstruct();

					CApproximatedSaturationCalculationProcessingConstruct* getNextConstruct();

					CSaturationConceptReferenceLinking* getSaturationConceptReferenceLinking();
					CApproximatedSaturationCalculationProcessingConstruct* setSaturationConceptReferenceLinking(CSaturationConceptReferenceLinking* satConRefLinking);

				// protected methods
				protected:

				// protected variables
				protected:
					CSaturationConceptReferenceLinking* mSatConRefLinking;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CAPPROXIMATEDSATURATIONCALCULATIONPROCESSINGCONSTRUCT_H

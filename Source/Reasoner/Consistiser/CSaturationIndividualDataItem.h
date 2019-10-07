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

#ifndef KONCLUDE_REASONER_CONSISTISER_CSATURATIONINDIVIDUALDATAITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CSATURATIONINDIVIDUALDATAITEM_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CSaturationIndividualReferenceLinking.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Consistiser {



			/*! 
			 *
			 *		\class		CSaturationIndividualDataItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationIndividualDataItem : public CLinkerBase<CIndividual*,CSaturationIndividualDataItem>, public CSaturationIndividualReferenceLinking {
				// public methods
				public:
					//! Constructor
					CSaturationIndividualDataItem();


					CSaturationIndividualDataItem* initConceptSaturationTestingItem(CIndividual* individual, cint64 saturationID);

					CIndividual* getIndividual();


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

#endif // KONCLUDE_REASONER_CONSISTISER_CSATURATIONINDIVIDUALDATAITEM_H

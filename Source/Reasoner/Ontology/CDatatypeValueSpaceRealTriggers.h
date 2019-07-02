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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEREALTRIGGERS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEREALTRIGGERS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceCompareTriggers.h"
#include "CDatatypeValueSpaceConceptTriggeringData.h"
#include "CDatatypeValueSpaceTriggeringMap.h"
#include "CDataLiteralRealValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpaceRealTriggers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceRealTriggers : public CDatatypeValueSpaceCompareTriggers {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceRealTriggers(CContext* boxContext);

					CDatatypeValueSpaceRealTriggers* initValueSpaceRealTriggers(CDatatypeValueSpaceRealTriggers* data);



					CDatatypeValueSpaceConceptTriggeringData* getRealConceptTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getRationalConceptTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getDecimalConceptTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getIntegerConceptTriggeringData();


					CDatatypeValueSpaceRealTriggers* addRealConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceRealTriggers* addRationalConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceRealTriggers* addDecimalConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceRealTriggers* addIntegerConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);




				// protected methods
				protected:

				// protected variables
				protected:

					CDatatypeValueSpaceConceptTriggeringData mValueSpateRationalTriggerData;
					CDatatypeValueSpaceConceptTriggeringData mValueSpateDecimalTriggerData;
					CDatatypeValueSpaceConceptTriggeringData mValueSpateIntegerTriggerData;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACEREALTRIGGERS_H

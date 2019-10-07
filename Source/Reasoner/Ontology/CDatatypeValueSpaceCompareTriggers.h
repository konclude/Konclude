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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACECOMPARETRIGGERS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACECOMPARETRIGGERS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceTriggers.h"
#include "CDatatypeValueSpaceConceptTriggeringData.h"
#include "CDatatypeValueSpaceTriggeringMap.h"
#include "CDataLiteralCompareValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpaceCompareTriggers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceCompareTriggers : public CDatatypeValueSpaceTriggers {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceCompareTriggers(CContext* boxContext);

					CDatatypeValueSpaceCompareTriggers* initValueSpaceCompareTriggers(CDatatypeValueSpaceCompareTriggers* data);


					CDatatypeValueSpaceTriggeringMap* getValueSpaceTriggeringMap();

					CDatatypeValueSpaceConceptTriggeringData* getValueConceptTriggeringData(CDataLiteralCompareValue* value, bool createOrLocalize = true);

					CDatatypeValueSpaceConceptTriggeringData* getMinValueConceptTriggeringData(CDataLiteralCompareValue* value, bool inclusive, bool createOrLocalize = true);
					CDatatypeValueSpaceConceptTriggeringData* getMaxValueConceptTriggeringData(CDataLiteralCompareValue* value, bool inclusive, bool createOrLocalize = true);


					CDatatypeValueSpaceCompareTriggers* addValueConceptTrigger(CDataLiteralCompareValue* value, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceCompareTriggers* addMinValueConceptTrigger(CDataLiteralCompareValue* value, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceCompareTriggers* addMaxValueConceptTrigger(CDataLiteralCompareValue* value, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);


					CDatatypeValueSpaceCompareTriggers* addMinMaxValueConceptTrigger(CDataLiteralCompareValue* value, bool minRestriction, bool inclusive, CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);


				// protected methods
				protected:

				// protected variables
				protected:


					CDatatypeValueSpaceTriggeringMap mValueTriggerMap;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACECOMPARETRIGGERS_H

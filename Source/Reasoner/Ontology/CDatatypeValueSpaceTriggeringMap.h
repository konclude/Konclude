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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGMAP_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGMAP_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CBoxContext.h"
#include "CDataLiteralCompareValue.h"
#include "CDatatypeValueSpaceTriggeringMapData.h"
#include "CDatatypeValueSpaceTriggeringMapArranger.h"
#include "CDatatypeValueSpaceTriggeringIterator.h"


// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

				/*! 
				 *
				 *		\class		CDatatypeValueSpaceTriggeringMap
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueSpaceTriggeringMap : public CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData> {
					// public methods
					public:
						//! Constructor
						CDatatypeValueSpaceTriggeringMap(CBoxContext* boxContext);

						CDatatypeValueSpaceTriggeringMap* initValueSpaceTriggeringMap(CDatatypeValueSpaceTriggeringMap* map);



						CDatatypeValueSpaceTriggeringData* getDatatypeValueTriggeringData(CDataLiteralCompareValue* value, bool createOrLocalize = false);
						CDatatypeValueSpaceTriggeringData* getDatatypeValueTriggeringData(CDataLiteralCompareValue* value, CDatatypeValueSpaceTriggeringMapData& valueMapData, bool createOrLocalize);


						CDatatypeValueSpaceTriggeringIterator getLeftTriggeringIterator(CDataLiteralCompareValue* value, bool inclusive);
						CDatatypeValueSpaceTriggeringIterator getRightTriggeringIterator(CDataLiteralCompareValue* value, bool inclusive);

						CDatatypeValueSpaceTriggeringIterator getTriggeringIterator(CDataLiteralCompareValue* value, bool left, bool inclusive);


						CDatatypeValueSpaceTriggeringIterator getIntervalTriggeringIterator(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, bool fromMinToMax);


						bool countIntervalMinMaxValueTriggers(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, cint64& minTriggerCount, cint64& maxTriggerCount, cint64& valueTriggerCount);
						bool countIntervalMinMaxValueTriggers(CDatatypeValueSpaceTriggeringIterator iterator, cint64& minTriggerCount, cint64& maxTriggerCount, cint64& valueTriggerCount);

						bool countIntervalValueTriggers(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive, cint64& valueTriggerCount);


						bool getIntervalMinMaxTriggerFreeInterval(CDataLiteralCompareValue* minValue, bool minInclusive, CDataLiteralCompareValue* maxValue, bool maxInclusive,
							CDataLiteralCompareValue*& freeLeftValue, bool& freeLeftInclusive, CDataLiteralCompareValue*& freeRightValue, bool& freeRightInclusive);

					// protected methods
					protected:


					// protected variables
					protected:
						CBoxContext* mBoxContext;



					// private methods
					private:

					// private variables
					private:

				};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGMAP_H

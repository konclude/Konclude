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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESTRINGVALUESPACEMAP_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESTRINGVALUESPACEMAP_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeStringValueData.h"
#include "CDatatypeStringValueSpaceMapData.h"
#include "CDatatypeStringValueSpaceMapArranger.h"
#include "CClashedDependencyDescriptor.h"
#include "CDatatypeValueSpaceValuesCounter.h"
#include "CDatatypeValueSpaceStringValuesCounter.h"
#include "CDatatypeValueSpaceDependencyCollector.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceStringType.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeStringValueSpaceMap
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeStringValueSpaceMap : public CPROCESSMAP<CDatatypeStringValueSpaceMapArranger,CDatatypeStringValueSpaceMapData> {
					// public methods
					public:
						//! Constructor
						CDatatypeStringValueSpaceMap(CProcessContext* processContext);

						CDatatypeStringValueSpaceMap* initDatatypeStringValueSpaceMap(CDatatypeStringValueSpaceMap* map);
						CDatatypeStringValueSpaceMap* initDatatypeStringValueSpaceMap(CDatatypeValueSpaceStringType* valueSpaceType);


						bool hasExcludedMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDatatypeStringValueExclusionType* exclusionType);
						bool hasExcludedMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDatatypeStringValueExclusionType* exclusionType);

						bool isValueExcluded(CDataLiteralStringValue* value, CDatatypeStringValueExclusionType* exclusionType);


						bool restrictToValue(CDataLiteralStringValue* value, CDependencyTrackPoint* depTrackPoint);

						bool excludeMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);
						bool excludeMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);

						bool excludeMinimum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType);
						bool excludeMaximum(CDataLiteralStringValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType);

						bool excludeValue(CDataLiteralStringValue* value, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType = nullptr);
						bool excludeInterval(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType = nullptr);
						bool excludeAll(CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType = nullptr);


						bool areAllValuesRestricted(CDatatypeStringValueExclusionType* exclusionType);

						bool isRestrictedToWithLanguageTagsValues();
						bool isRestrictedToWithoutLanguageTagsValues();


						bool restrictToWithLanguageTagsValues(CDependencyTrackPoint* depTrackPoint);
						bool restrictToWithoutLanguageTagsValues(CDependencyTrackPoint* depTrackPoint);


						bool testValueSpaceReturnClashed();
						bool addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector, CDatatypeStringValueExclusionType* exclusionType);


						bool getAbsoluteMinimumValue(CDataLiteralStringValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);
						bool getAbsoluteMaximumValue(CDataLiteralStringValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);


						bool countAllValues(CDatatypeValueSpaceValuesCounter* counter);
						bool countIntervalValues(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter);
						bool countValueValues(CDataLiteralStringValue* value, CDatatypeValueSpaceValuesCounter* counter);


						bool addIntervalExclusionDependencies(CDataLiteralStringValue* leftValue, bool leftValueInclusive, CDataLiteralStringValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addValueExclusionDependencies(CDataLiteralStringValue* value, CDatatypeValueSpaceDependencyCollector* depCollector);


						bool getNextPossibleDataValue(CDataLiteralStringValue* nextValue, CDataLiteralStringValue* lastValue);

					// protected methods
					protected:



						bool adaptNewStringValueDataToNeighbours(CDatatypeStringValueData* valueData);

						CDatatypeStringValueData* getDatatypeStringValueData(CDataLiteralStringValue* value, bool createOrLocalize = false, bool* newValueInsertion = nullptr);
						CDatatypeStringValueData* getDatatypeStringValueData(CDataLiteralStringValue* value, CDatatypeStringValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion);

						bool isDataIntervalExcluded(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType);
						bool isDataIntervalExcluded(CDataLiteralStringValue* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType);
						bool isDataIntervalExcluded(CDatatypeStringValueData* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeStringValueExclusionType* exclusionType);


						bool isLeftDataIntervalExcluded(CDatatypeStringValueData* dataValue, CDatatypeStringValueExclusionType* exclusionType);
						bool isRightDataIntervalExcluded(CDatatypeStringValueData* dataValue, CDatatypeStringValueExclusionType* exclusionType);

						bool isDataValueExcluded(CDatatypeStringValueData* dataValue, CDatatypeStringValueExclusionType* exclusionType);
						bool isDataValueExcluded(CDataLiteralStringValue* dataValue, CDatatypeStringValueExclusionType* exclusionType);


						bool excludeData(CDatatypeStringValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force);
						bool excludeDataValue(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force);
						bool excludeLeftDataInterval(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force);
						bool excludeRightDataInterval(CDatatypeStringValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeStringValueExclusionType* exclusionType, bool force);




						bool addDataValueExclusionDependencies(CDatatypeStringValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addDataIntervalExclusionDependencies(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector);

						bool addLeftIntervalExclusionDependencies(CDatatypeStringValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addRightIntervalExclusionDependencies(CDatatypeStringValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addExclusionDependencies(CDatatypeStringValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector);



						bool countDataIntervalValues(CDataLiteralStringValue* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeStringValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeStringValueData* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDataLiteralStringValue* leftValueExcluded, CDatatypeStringValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeStringValueData* leftValueExcluded, CDataLiteralStringValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);

						bool countDataValue(CDataLiteralStringValue* value, CDatatypeStringValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataValue(CDatatypeStringValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter);

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CDatatypeStringValueData* mAbsoluteMinimumExclusionValue;
						CDatatypeStringValueData* mAbsoluteMaximumExclusionValue;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESTRINGVALUESPACEMAP_H

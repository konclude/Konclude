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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUESPACEMAP_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUESPACEMAP_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeRealValueData.h"
#include "CDatatypeRealValueSpaceMapData.h"
#include "CDatatypeRealValueSpaceMapArranger.h"
#include "CClashedDependencyDescriptor.h"
#include "CDatatypeValueSpaceValuesCounter.h"
#include "CDatatypeValueSpaceRealValuesCounter.h"
#include "CDatatypeValueSpaceDependencyCollector.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceRealType.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeRealValueSpaceMap
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeRealValueSpaceMap : public CPROCESSMAP<CDatatypeRealValueSpaceMapArranger,CDatatypeRealValueSpaceMapData> {
					// public methods
					public:
						//! Constructor
						CDatatypeRealValueSpaceMap(CProcessContext* processContext);

						CDatatypeRealValueSpaceMap* initDatatypeRealValueSpaceMap(CDatatypeRealValueSpaceMap* map);
						CDatatypeRealValueSpaceMap* initDatatypeRealValueSpaceMap(CDatatypeValueSpaceRealType* valueSpaceType);


						bool hasExcludedMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDatatypeRealValueExclusionType* exclusionType);
						bool hasExcludedMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDatatypeRealValueExclusionType* exclusionType);

						bool isValueExcluded(CDataLiteralRealValue* value, CDatatypeRealValueExclusionType* exclusionType);


						bool restrictToValue(CDataLiteralRealValue* value, CDependencyTrackPoint* depTrackPoint);

						bool excludeMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);
						bool excludeMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);

						bool excludeMinimum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType);
						bool excludeMaximum(CDataLiteralRealValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType);

						bool excludeValue(CDataLiteralRealValue* value, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType = nullptr);
						bool excludeInterval(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType = nullptr);
						bool excludeAll(CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType = nullptr);


						bool areAllValuesRestricted(CDatatypeRealValueExclusionType* exclusionType);

						bool isRestrictedToRationalValues();
						bool isRestrictedToNonRationalValues();

						bool isRestrictedToDecimalValues();
						bool isRestrictedToNonDecimalValues();

						bool isRestrictedToIntegerValues();
						bool isRestrictedToNonIntegerValues();

						bool restrictToIntegerValues(CDependencyTrackPoint* depTrackPoint);
						bool restrictToNonIntegerValues(CDependencyTrackPoint* depTrackPoint);

						bool restrictToDecimalValues(CDependencyTrackPoint* depTrackPoint);
						bool restrictToNonDecimalValues(CDependencyTrackPoint* depTrackPoint);

						bool restrictToRationalValues(CDependencyTrackPoint* depTrackPoint);
						bool restrictToNonRationalValues(CDependencyTrackPoint* depTrackPoint);

						bool testValueSpaceReturnClashed();
						bool addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector, CDatatypeRealValueExclusionType* exclusionType);


						bool getAbsoluteMinimumValue(CDataLiteralRealValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);
						bool getAbsoluteMaximumValue(CDataLiteralRealValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);


						bool countAllValues(CDatatypeValueSpaceValuesCounter* counter);
						bool countIntervalValues(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter);
						bool countValueValues(CDataLiteralRealValue* value, CDatatypeValueSpaceValuesCounter* counter);


						bool addIntervalExclusionDependencies(CDataLiteralRealValue* leftValue, bool leftValueInclusive, CDataLiteralRealValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addValueExclusionDependencies(CDataLiteralRealValue* value, CDatatypeValueSpaceDependencyCollector* depCollector);


						bool getNextPossibleDataValue(CDataLiteralRealValue* nextValue, CDataLiteralRealValue* lastValue);

					// protected methods
					protected:



						bool adaptNewRealValueDataToNeighbours(CDatatypeRealValueData* valueData);

						CDatatypeRealValueData* getDatatypeRealValueData(CDataLiteralRealValue* value, bool createOrLocalize = false, bool* newValueInsertion = nullptr);
						CDatatypeRealValueData* getDatatypeRealValueData(CDataLiteralRealValue* value, CDatatypeRealValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion);

						bool isDataIntervalExcluded(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType);
						bool isDataIntervalExcluded(CDataLiteralRealValue* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType);
						bool isDataIntervalExcluded(CDatatypeRealValueData* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeRealValueExclusionType* exclusionType);

						bool containsIntervalIntegers(CDataLiteralRealValue* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, cint64 requiredIntegerCount = 1);

						bool isLeftDataIntervalExcluded(CDatatypeRealValueData* dataValue, CDatatypeRealValueExclusionType* exclusionType);
						bool isRightDataIntervalExcluded(CDatatypeRealValueData* dataValue, CDatatypeRealValueExclusionType* exclusionType);

						bool isDataValueExcluded(CDatatypeRealValueData* dataValue, CDatatypeRealValueExclusionType* exclusionType);
						bool isDataValueExcluded(CDataLiteralRealValue* dataValue, CDatatypeRealValueExclusionType* exclusionType);


						bool excludeData(CDatatypeRealValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force);
						bool excludeDataValue(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force);
						bool excludeLeftDataInterval(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force);
						bool excludeRightDataInterval(CDatatypeRealValueData* dataValue, CDependencyTrackPoint* depTrackPoint, CDatatypeRealValueExclusionType* exclusionType, bool force);




						bool addDataValueExclusionDependencies(CDatatypeRealValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addDataIntervalExclusionDependencies(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector);

						bool addLeftIntervalExclusionDependencies(CDatatypeRealValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addRightIntervalExclusionDependencies(CDatatypeRealValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addExclusionDependencies(CDatatypeRealValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector);



						bool countDataIntervalValues(CDataLiteralRealValue* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeRealValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeRealValueData* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDataLiteralRealValue* leftValueExcluded, CDatatypeRealValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeRealValueData* leftValueExcluded, CDataLiteralRealValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);

						bool countDataValue(CDataLiteralRealValue* value, CDatatypeRealValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataValue(CDatatypeRealValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter);

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CDatatypeRealValueData* mAbsoluteMinimumExclusionValue;
						CDatatypeRealValueData* mAbsoluteMaximumExclusionValue;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEREALVALUESPACEMAP_H

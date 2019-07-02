/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEMAP_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEMAP_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeCompareValueData.h"
#include "CDatatypeCompareValueSpaceMapData.h"
#include "CDatatypeCompareValueSpaceMapArranger.h"
#include "CClashedDependencyDescriptor.h"
#include "CDatatypeValueSpaceValuesCounter.h"
#include "CDatatypeValueSpaceDependencyCollector.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceCompareType.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeCompareValueSpaceMap
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeCompareValueSpaceMap : public CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData> {
					// public methods
					public:
						//! Constructor
						CDatatypeCompareValueSpaceMap(CProcessContext* processContext);

						CDatatypeCompareValueSpaceMap* initDatatypeCompareValueSpaceMap(CDatatypeCompareValueSpaceMap* map);
						CDatatypeCompareValueSpaceMap* initDatatypeCompareValueSpaceMap(CDatatypeValueSpaceCompareType* valueSpaceType);


						bool hasExcludedMinimum(CDataLiteralCompareValue* value, bool valueInclusively);
						bool hasExcludedMaximum(CDataLiteralCompareValue* value, bool valueInclusively);

						bool isValueExcluded(CDataLiteralCompareValue* value);


						bool restrictToValue(CDataLiteralCompareValue* value, CDependencyTrackPoint* depTrackPoint);

						bool excludeMinimum(CDataLiteralCompareValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);
						bool excludeMaximum(CDataLiteralCompareValue* value, bool valueInclusively, CDependencyTrackPoint* depTrackPoint);


						bool excludeValue(CDataLiteralCompareValue* value, CDependencyTrackPoint* depTrackPoint);
						bool excludeInterval(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDependencyTrackPoint* depTrackPoint);
						bool excludeAll(CDependencyTrackPoint* depTrackPoint);



						bool testValueSpaceReturnClashed();
						bool addValueSpaceDependencies(CDatatypeValueSpaceDependencyCollector* depCollector);


						bool getAbsoluteMinimumValue(CDataLiteralCompareValue*& minValue, bool& minValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);
						bool getAbsoluteMaximumValue(CDataLiteralCompareValue*& maxValue, bool& maxValueInclusive, CDependencyTrackPoint** depTrackPoint = nullptr);


						bool countAllValues(CDatatypeValueSpaceValuesCounter* counter);
						bool countIntervalValues(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceValuesCounter* counter);
						bool countValueValues(CDataLiteralCompareValue* value, CDatatypeValueSpaceValuesCounter* counter);


						bool addIntervalExclusionDependencies(CDataLiteralCompareValue* leftValue, bool leftValueInclusive, CDataLiteralCompareValue* rightValue, bool rightValueInclusive, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addValueExclusionDependencies(CDataLiteralCompareValue* value, CDatatypeValueSpaceDependencyCollector* depCollector);


						bool getNextPossibleDataValue(CDataLiteralCompareValue* nextValue, CDataLiteralCompareValue* lastValue);

					// protected methods
					protected:



						bool adaptNewCompareValueDataToNeighbours(CDatatypeCompareValueData* valueData);

						CDatatypeCompareValueData* getDatatypeCompareValueData(CDataLiteralCompareValue* value, bool createOrLocalize = false, bool* newValueInsertion = nullptr);
						CDatatypeCompareValueData* getDatatypeCompareValueData(CDataLiteralCompareValue* value, CDatatypeCompareValueSpaceMapData& valueMapData, bool createOrLocalize, bool* newValueInsertion);

						bool isDataIntervalExcluded(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded);
						bool isDataIntervalExcluded(CDataLiteralCompareValue* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded);
						bool isDataIntervalExcluded(CDatatypeCompareValueData* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded);


						bool isLeftDataIntervalExcluded(CDatatypeCompareValueData* dataValue);
						bool isRightDataIntervalExcluded(CDatatypeCompareValueData* dataValue);

						bool isDataValueExcluded(CDatatypeCompareValueData* dataValue);
						bool isDataValueExcluded(CDataLiteralCompareValue* dataValue);


						bool excludeData(CDatatypeCompareValueDataExclusion* exlcusionData, CDependencyTrackPoint* depTrackPoint, bool force);
						bool excludeDataValue(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force);
						bool excludeLeftDataInterval(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force);
						bool excludeRightDataInterval(CDatatypeCompareValueData* dataValue, CDependencyTrackPoint* depTrackPoint, bool force);




						bool addDataValueExclusionDependencies(CDatatypeCompareValueData* dataValue, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addDataIntervalExclusionDependencies(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceDependencyCollector* depCollector);

						bool addLeftIntervalExclusionDependencies(CDatatypeCompareValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addRightIntervalExclusionDependencies(CDatatypeCompareValueData* valueData, CDatatypeValueSpaceDependencyCollector* depCollector);
						bool addExclusionDependencies(CDatatypeCompareValueDataExclusion* exclusionData, CDatatypeValueSpaceDependencyCollector* depCollector);



						bool countDataIntervalValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded, CDatatypeCompareValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeCompareValueData* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDataLiteralCompareValue* leftValueExcluded, CDatatypeCompareValueData* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataIntervalValues(CDatatypeCompareValueData* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded, CDatatypeValueSpaceValuesCounter* counter);

						bool countDataValue(CDataLiteralCompareValue* value, CDatatypeCompareValueDataExclusion* exlcusionData, CDatatypeValueSpaceValuesCounter* counter);
						bool countDataValue(CDatatypeCompareValueData* dataValue, CDatatypeValueSpaceValuesCounter* counter);



						virtual bool representsInfinitelyManyValues(CDataLiteralCompareValue* value) = 0;
						virtual bool representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) = 0;
						virtual bool getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) = 0;
						virtual bool getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) = 0;
						virtual cuint64 getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) = 0;


					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CDatatypeCompareValueData* mAbsoluteMinimumExclusionValue;
						CDatatypeCompareValueData* mAbsoluteMaximumExclusionValue;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUESPACEMAP_H

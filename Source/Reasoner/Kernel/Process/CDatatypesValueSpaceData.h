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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESVALUESPACEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESVALUESPACEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeRealValueSpaceData.h"
#include "CDatatypeUnknownValueSpaceData.h"
#include "CDatatypeStringValueSpaceData.h"
#include "CDatatypeBooleanValueSpaceData.h"
#include "CDatatypeDoubleValueSpaceData.h"
#include "CDatatypeFloatValueSpaceData.h"
#include "CDatatypeIRIValueSpaceData.h"
#include "CDatatypeBinaryHexDataValueSpaceData.h"
#include "CDatatypeBinaryBase64DataValueSpaceData.h"
#include "CDatatypeDateTimeValueSpaceData.h"
#include "CDatatypeXMLValueSpaceData.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceUnknownType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceStringType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceBooleanType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceDoubleType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceFloatType.h"
#include "Reasoner/Ontology/CDatatypeValueSpaceIRIType.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypesValueSpaceData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypesValueSpaceData {
					// public methods
					public:
						//! Constructor
						CDatatypesValueSpaceData(CProcessContext* processContext);

						CDatatypesValueSpaceData* initDatatypesValueSpaceData(CDatatypesValueSpaceData* data = nullptr);


						CDatatypeValueSpaceData* getValueSpace(CDatatypeValueSpaceType* valueSpaceType, bool createOrLocalize = true);


						CDatatypeRealValueSpaceData* getRealValueSpace(CDatatypeValueSpaceRealType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeUnknownValueSpaceData* getUnknownValueSpace(CDatatypeValueSpaceUnknownType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeStringValueSpaceData* getStringValueSpace(CDatatypeValueSpaceStringType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeBooleanValueSpaceData* getBooleanValueSpace(CDatatypeValueSpaceBooleanType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeDoubleValueSpaceData* getDoubleValueSpace(CDatatypeValueSpaceDoubleType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeFloatValueSpaceData* getFloatValueSpace(CDatatypeValueSpaceFloatType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeIRIValueSpaceData* getIRIValueSpace(CDatatypeValueSpaceIRIType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeBinaryBase64DataValueSpaceData* getBinaryBase64DataValueSpace(CDatatypeValueSpaceBinaryBase64DataType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeBinaryHexDataValueSpaceData* getBinaryHexDataValueSpace(CDatatypeValueSpaceBinaryHexDataType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeXMLValueSpaceData* getXMLValueSpace(CDatatypeValueSpaceXMLType* valueSpaceType, bool createOrLocalize = true);
						CDatatypeDateTimeValueSpaceData* getDateTimeValueSpace(CDatatypeValueSpaceDateTimeType* valueSpaceType, bool createOrLocalize = true);


						bool isValueSpaceTriggeringQueued();
						CDatatypesValueSpaceData* setValueSpaceTriggeringQueued(bool queued);

						bool isValueSpaceTriggeringRequired();
						CDatatypesValueSpaceData* setValueSpaceTriggeringRequired(bool required);

						bool isValueSpaceCountingRequired();
						CDatatypesValueSpaceData* setValueSpaceCountingRequired(bool required);

						bool isValueSpaceClashCheckingRequired();
						CDatatypesValueSpaceData* setValueSpaceClashCheckingRequired(bool required);

						bool isValueSpaceSatisfiabilityCheckingQueued();
						CDatatypesValueSpaceData* setValueSpaceSatisfiabilityCheckingQueued(bool queued);

						bool isValueSpaceSatisfiabilityCheckingRequired();
						CDatatypesValueSpaceData* setValueSpaceSatisfiabilityCheckingRequired(bool required);

						CDatatypeValueSpaceType* getLastTriggeredValueSpaceType();
						CDatatypesValueSpaceData* setLastTriggeredValueSpaceType(CDatatypeValueSpaceType* valueSpaceType);

						CDatatypesValueSpaceData* setValueSapceModified(CDatatypeValueSpaceData* valueSpace);

						cint64 getLastTriggeringDisjointCount();
						CDatatypesValueSpaceData* setLastTriggeringDisjointCount(cint64 disjointCount);


						CDatatypeValueSpaceValuesCounter* getValuesCounter();

						bool isValueSpaceCounted();
						CDatatypesValueSpaceData* setValueSpaceCounted(bool counted);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						cint64 mLastTriggeringDisjointCount;

						bool mValueSpaceTriggeringQueued;
						bool mValueSpaceTriggeringRequired;
						CDatatypeValueSpaceType* mLastTriggeredValueSpaceType;

						bool mValueSpaceCounted;
						bool mValueSpaceCountingRequired;
						CDatatypeValueSpaceValuesCounter mValuesCounter;

						bool mValueSpaceSatisfiabilityCheckingRequired;
						bool mValueSpaceSatisfiabilityCheckingQueued;
						bool mValueSpaceClashCheckingRequired;

						CDatatypeRealValueSpaceData* mUseRealValueSpace;
						CDatatypeRealValueSpaceData* mLocRealValueSpace;

						CDatatypeUnknownValueSpaceData* mUseUnknownValueSpace;
						CDatatypeUnknownValueSpaceData* mLocUnknownValueSpace;

						CDatatypeStringValueSpaceData* mUseStringValueSpace;
						CDatatypeStringValueSpaceData* mLocStringValueSpace;

						CDatatypeBooleanValueSpaceData* mUseBooleanValueSpace;
						CDatatypeBooleanValueSpaceData* mLocBooleanValueSpace;

						CDatatypeDoubleValueSpaceData* mUseDoubleValueSpace;
						CDatatypeDoubleValueSpaceData* mLocDoubleValueSpace;

						CDatatypeFloatValueSpaceData* mUseFloatValueSpace;
						CDatatypeFloatValueSpaceData* mLocFloatValueSpace;

						CDatatypeIRIValueSpaceData* mUseIRIValueSpace;
						CDatatypeIRIValueSpaceData* mLocIRIValueSpace;

						CDatatypeBinaryBase64DataValueSpaceData* mUseBinaryBase64DataValueSpace;
						CDatatypeBinaryBase64DataValueSpaceData* mLocBinaryBase64DataValueSpace;

						CDatatypeBinaryHexDataValueSpaceData* mUseBinaryHexDataValueSpace;
						CDatatypeBinaryHexDataValueSpaceData* mLocBinaryHexDataValueSpace;

						CDatatypeXMLValueSpaceData* mUseXMLValueSpace;
						CDatatypeXMLValueSpaceData* mLocXMLValueSpace;

						CDatatypeDateTimeValueSpaceData* mUseDateTimeValueSpace;
						CDatatypeDateTimeValueSpaceData* mLocDateTimeValueSpace;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPESVALUESPACEDATA_H

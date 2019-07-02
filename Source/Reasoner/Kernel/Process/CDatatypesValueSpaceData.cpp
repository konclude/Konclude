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

#include "CDatatypesValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypesValueSpaceData::CDatatypesValueSpaceData(CProcessContext* processContext) {
					mProcessContext = processContext;

				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::initDatatypesValueSpaceData(CDatatypesValueSpaceData* data) {
					mLocRealValueSpace = nullptr;
					mUseRealValueSpace = nullptr;
					mLocStringValueSpace = nullptr;
					mUseStringValueSpace = nullptr;
					mLocIRIValueSpace = nullptr;
					mUseIRIValueSpace = nullptr;
					mLocXMLValueSpace = nullptr;
					mUseXMLValueSpace = nullptr;
					mUseBinaryBase64DataValueSpace = nullptr;
					mLocBinaryBase64DataValueSpace = nullptr;
					mUseDateTimeValueSpace = nullptr;
					mLocDateTimeValueSpace = nullptr;
					mUseBinaryHexDataValueSpace = nullptr;
					mLocBinaryHexDataValueSpace = nullptr;
					mLocBooleanValueSpace = nullptr;
					mUseBooleanValueSpace = nullptr;
					mLocDoubleValueSpace = nullptr;
					mUseDoubleValueSpace = nullptr;
					mLocFloatValueSpace = nullptr;
					mLocCustomValueSpacesTriggers = nullptr;
					mUseCustomValueSpacesTriggers = nullptr;
					mUseFloatValueSpace = nullptr;
					mLocUnknownValueSpace = nullptr;
					mUseUnknownValueSpace = nullptr;
					mValueSpaceTriggeringQueued = false;
					mLastTriggeredValueSpaceType = nullptr;
					mValueSpaceTriggeringRequired = false;
					mValueSpaceCountingRequired = false;
					mValueSpaceClashCheckingRequired = false;
					mValueSpaceSatisfiabilityCheckingRequired = false;
					mValueSpaceSatisfiabilityCheckingQueued = false;
					mLastTriggeringDisjointCount = 0;
					mValuesCounter.resetValueCounter();
					mValueSpaceCounted = false;
					if (data) {
						mUseRealValueSpace = data->mUseRealValueSpace;
						mUseStringValueSpace = data->mUseStringValueSpace;
						mUseIRIValueSpace = data->mUseIRIValueSpace;
						mUseXMLValueSpace = data->mUseXMLValueSpace;
						mUseBinaryBase64DataValueSpace = data->mUseBinaryBase64DataValueSpace;
						mUseDateTimeValueSpace = data->mUseDateTimeValueSpace;
						mUseBinaryHexDataValueSpace = data->mUseBinaryHexDataValueSpace;
						mUseBooleanValueSpace = data->mUseBooleanValueSpace;
						mUseDoubleValueSpace = data->mUseDoubleValueSpace;
						mUseFloatValueSpace = data->mUseFloatValueSpace;
						mUseUnknownValueSpace = data->mUseUnknownValueSpace;
						mUseCustomValueSpacesTriggers = data->mUseCustomValueSpacesTriggers;
						mValueSpaceTriggeringQueued = data->mValueSpaceTriggeringQueued;
						mLastTriggeredValueSpaceType = data->mLastTriggeredValueSpaceType;
						mValueSpaceTriggeringRequired = data->mValueSpaceTriggeringRequired;
						mValueSpaceCountingRequired = data->mValueSpaceCountingRequired;
						mValueSpaceClashCheckingRequired = data->mValueSpaceClashCheckingRequired;
						mValueSpaceSatisfiabilityCheckingRequired = data->mValueSpaceSatisfiabilityCheckingRequired;
						mValueSpaceSatisfiabilityCheckingQueued = data->mValueSpaceSatisfiabilityCheckingQueued;
						mLastTriggeringDisjointCount = data->mLastTriggeringDisjointCount;
						mValuesCounter.initValueCounter(&data->mValuesCounter);
						mValueSpaceCounted = data->mValueSpaceCounted;
					}
					return this;
				}


				CDatatypeValueSpaceData* CDatatypesValueSpaceData::getValueSpace(CDatatypeValueSpaceType* valueSpaceType, bool createOrLocalize) {
					CDatatypeValueSpaceData* valueSpace = nullptr;
					if (CDatatypeValueSpaceType::VALUESPACEREALTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getRealValueSpace((CDatatypeValueSpaceRealType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEUNKNOWNTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getUnknownValueSpace((CDatatypeValueSpaceUnknownType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACESTRINGTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getStringValueSpace((CDatatypeValueSpaceStringType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEIRITYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getIRIValueSpace((CDatatypeValueSpaceIRIType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEBOOLEANTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getBooleanValueSpace((CDatatypeValueSpaceBooleanType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEDOUBLETYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getDoubleValueSpace((CDatatypeValueSpaceDoubleType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEFLOATTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getFloatValueSpace((CDatatypeValueSpaceFloatType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEHEXDATATYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getBinaryHexDataValueSpace((CDatatypeValueSpaceBinaryHexDataType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEBASE64DATATYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getBinaryBase64DataValueSpace((CDatatypeValueSpaceBinaryBase64DataType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEXMLTYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getXMLValueSpace((CDatatypeValueSpaceXMLType*)valueSpaceType,createOrLocalize);
					} else if (CDatatypeValueSpaceType::VALUESPACEDATETIMETYPE == valueSpaceType->getValueSpaceType()) {
						valueSpace = getDateTimeValueSpace((CDatatypeValueSpaceDateTimeType*)valueSpaceType,createOrLocalize);
					}
					return valueSpace;
				}


				CDatatypeRealValueSpaceData* CDatatypesValueSpaceData::getRealValueSpace(CDatatypeValueSpaceRealType* valueSpaceType, bool createOrLocalize) {
					if (!mLocRealValueSpace && createOrLocalize) {
						mLocRealValueSpace = CObjectParameterizingAllocator< CDatatypeRealValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseRealValueSpace) {
							mLocRealValueSpace->copyRealValueSpaceData(mUseRealValueSpace);
						} else {
							mLocRealValueSpace->initRealValueSpaceData(valueSpaceType);
						}
						mUseRealValueSpace = mLocRealValueSpace;
					}
					return mUseRealValueSpace;
				}


				CDatatypeUnknownValueSpaceData* CDatatypesValueSpaceData::getUnknownValueSpace(CDatatypeValueSpaceUnknownType* valueSpaceType, bool createOrLocalize) {
					if (!mLocUnknownValueSpace && createOrLocalize) {
						mLocUnknownValueSpace = CObjectParameterizingAllocator< CDatatypeUnknownValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseUnknownValueSpace) {
							mLocUnknownValueSpace->copyUnknownValueSpaceData(mUseUnknownValueSpace);
						} else {
							mLocUnknownValueSpace->initUnknownValueSpaceData(valueSpaceType);
						}
						mUseUnknownValueSpace = mLocUnknownValueSpace;
					}
					return mUseUnknownValueSpace;
				}





				CDatatypeStringValueSpaceData* CDatatypesValueSpaceData::getStringValueSpace(CDatatypeValueSpaceStringType* valueSpaceType, bool createOrLocalize) {
					if (!mLocStringValueSpace && createOrLocalize) {
						mLocStringValueSpace = CObjectParameterizingAllocator< CDatatypeStringValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseStringValueSpace) {
							mLocStringValueSpace->copyStringValueSpaceData(mUseStringValueSpace);
						} else {
							mLocStringValueSpace->initStringValueSpaceData(valueSpaceType);
						}
						mUseStringValueSpace = mLocStringValueSpace;
					}
					return mUseStringValueSpace;
				}




				CDatatypeIRIValueSpaceData* CDatatypesValueSpaceData::getIRIValueSpace(CDatatypeValueSpaceIRIType* valueSpaceType, bool createOrLocalize) {
					if (!mLocIRIValueSpace && createOrLocalize) {
						mLocIRIValueSpace = CObjectParameterizingAllocator< CDatatypeIRIValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseIRIValueSpace) {
							mLocIRIValueSpace->copyIRIValueSpaceData(mUseIRIValueSpace);
						} else {
							mLocIRIValueSpace->initIRIValueSpaceData(valueSpaceType);
						}
						mUseIRIValueSpace = mLocIRIValueSpace;
					}
					return mUseIRIValueSpace;
				}



				CDatatypeXMLValueSpaceData* CDatatypesValueSpaceData::getXMLValueSpace(CDatatypeValueSpaceXMLType* valueSpaceType, bool createOrLocalize) {
					if (!mLocXMLValueSpace && createOrLocalize) {
						mLocXMLValueSpace = CObjectParameterizingAllocator< CDatatypeXMLValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseXMLValueSpace) {
							mLocXMLValueSpace->copyXMLValueSpaceData(mUseXMLValueSpace);
						} else {
							mLocXMLValueSpace->initXMLValueSpaceData(valueSpaceType);
						}
						mUseXMLValueSpace = mLocXMLValueSpace;
					}
					return mUseXMLValueSpace;
				}



				CDatatypeBinaryBase64DataValueSpaceData* CDatatypesValueSpaceData::getBinaryBase64DataValueSpace(CDatatypeValueSpaceBinaryBase64DataType* valueSpaceType, bool createOrLocalize) {
					if (!mLocBinaryBase64DataValueSpace && createOrLocalize) {
						mLocBinaryBase64DataValueSpace = CObjectParameterizingAllocator< CDatatypeBinaryBase64DataValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseBinaryBase64DataValueSpace) {
							mLocBinaryBase64DataValueSpace->copyBinaryBase64DataValueSpaceData(mUseBinaryBase64DataValueSpace);
						} else {
							mLocBinaryBase64DataValueSpace->initBinaryBase64DataValueSpaceData(valueSpaceType);
						}
						mUseBinaryBase64DataValueSpace = mLocBinaryBase64DataValueSpace;
					}
					return mUseBinaryBase64DataValueSpace;
				}



				CDatatypeDateTimeValueSpaceData* CDatatypesValueSpaceData::getDateTimeValueSpace(CDatatypeValueSpaceDateTimeType* valueSpaceType, bool createOrLocalize) {
					if (!mLocDateTimeValueSpace && createOrLocalize) {
						mLocDateTimeValueSpace = CObjectParameterizingAllocator< CDatatypeDateTimeValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseDateTimeValueSpace) {
							mLocDateTimeValueSpace->copyDateTimeValueSpaceData(mUseDateTimeValueSpace);
						} else {
							mLocDateTimeValueSpace->initDateTimeValueSpaceData(valueSpaceType);
						}
						mUseDateTimeValueSpace = mLocDateTimeValueSpace;
					}
					return mUseDateTimeValueSpace;
				}

				CDatatypeBinaryHexDataValueSpaceData* CDatatypesValueSpaceData::getBinaryHexDataValueSpace(CDatatypeValueSpaceBinaryHexDataType* valueSpaceType, bool createOrLocalize) {
					if (!mLocBinaryHexDataValueSpace && createOrLocalize) {
						mLocBinaryHexDataValueSpace = CObjectParameterizingAllocator< CDatatypeBinaryHexDataValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseBinaryHexDataValueSpace) {
							mLocBinaryHexDataValueSpace->copyBinaryHexDataValueSpaceData(mUseBinaryHexDataValueSpace);
						} else {
							mLocBinaryHexDataValueSpace->initBinaryHexDataValueSpaceData(valueSpaceType);
						}
						mUseBinaryHexDataValueSpace = mLocBinaryHexDataValueSpace;
					}
					return mUseBinaryHexDataValueSpace;
				}

				CDatatypeBooleanValueSpaceData* CDatatypesValueSpaceData::getBooleanValueSpace(CDatatypeValueSpaceBooleanType* valueSpaceType, bool createOrLocalize) {
					if (!mLocBooleanValueSpace && createOrLocalize) {
						mLocBooleanValueSpace = CObjectParameterizingAllocator< CDatatypeBooleanValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseBooleanValueSpace) {
							mLocBooleanValueSpace->copyBooleanValueSpaceData(mUseBooleanValueSpace);
						} else {
							mLocBooleanValueSpace->initBooleanValueSpaceData(valueSpaceType);
						}
						mUseBooleanValueSpace = mLocBooleanValueSpace;
					}
					return mUseBooleanValueSpace;
				}



				CDatatypeDoubleValueSpaceData* CDatatypesValueSpaceData::getDoubleValueSpace(CDatatypeValueSpaceDoubleType* valueSpaceType, bool createOrLocalize) {
					if (!mLocDoubleValueSpace && createOrLocalize) {
						mLocDoubleValueSpace = CObjectParameterizingAllocator< CDatatypeDoubleValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseDoubleValueSpace) {
							mLocDoubleValueSpace->copyDoubleValueSpaceData(mUseDoubleValueSpace);
						} else {
							mLocDoubleValueSpace->initDoubleValueSpaceData(valueSpaceType);
						}
						mUseDoubleValueSpace = mLocDoubleValueSpace;
					}
					return mUseDoubleValueSpace;
				}



				CDatatypeFloatValueSpaceData* CDatatypesValueSpaceData::getFloatValueSpace(CDatatypeValueSpaceFloatType* valueSpaceType, bool createOrLocalize) {
					if (!mLocFloatValueSpace && createOrLocalize) {
						mLocFloatValueSpace = CObjectParameterizingAllocator< CDatatypeFloatValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseFloatValueSpace) {
							mLocFloatValueSpace->copyFloatValueSpaceData(mUseFloatValueSpace);
						} else {
							mLocFloatValueSpace->initFloatValueSpaceData(valueSpaceType);
						}
						mUseFloatValueSpace = mLocFloatValueSpace;
					}
					return mUseFloatValueSpace;
				}



				CDatatypeValueSpacesTriggers* CDatatypesValueSpaceData::getCustomValueSpacesTriggers(CDatatypeValueSpacesTriggers* defaultValueSpaceTriggers, bool createOrLocalize) {
					if (!mLocCustomValueSpacesTriggers && createOrLocalize) {
						mLocCustomValueSpacesTriggers = CObjectParameterizingAllocator< CDatatypeValueSpacesTriggers, CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						
						if (mUseCustomValueSpacesTriggers) {
							mLocCustomValueSpacesTriggers->initValueSpacesTriggers(mUseCustomValueSpacesTriggers);
						}
						else {
							mLocCustomValueSpacesTriggers->initValueSpacesTriggers(defaultValueSpaceTriggers);
						}
						mUseCustomValueSpacesTriggers = mLocCustomValueSpacesTriggers;
					}
					if (mUseCustomValueSpacesTriggers) {
						return mUseCustomValueSpacesTriggers;
					} else {
						return defaultValueSpaceTriggers;
					}
				}




				bool CDatatypesValueSpaceData::isValueSpaceTriggeringQueued() {
					return mValueSpaceTriggeringQueued;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceTriggeringQueued(bool queued) {
					mValueSpaceTriggeringQueued = queued;
					return this;
				}

				CDatatypeValueSpaceType* CDatatypesValueSpaceData::getLastTriggeredValueSpaceType() {
					return mLastTriggeredValueSpaceType;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setLastTriggeredValueSpaceType(CDatatypeValueSpaceType* valueSpaceType) {
					mLastTriggeredValueSpaceType = valueSpaceType;
					return this;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSapceModified(CDatatypeValueSpaceData* valueSpace) {
					valueSpace->setValueSpaceModification();
					mValueSpaceTriggeringRequired = true;
					mValueSpaceCountingRequired = true;
					mValueSpaceClashCheckingRequired = true;
					mValueSpaceSatisfiabilityCheckingRequired = true;
					return this;
				}

				bool CDatatypesValueSpaceData::isValueSpaceTriggeringRequired() {
					return mValueSpaceTriggeringRequired;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceTriggeringRequired(bool required) {
					mValueSpaceTriggeringRequired = required;
					return this;
				}

				bool CDatatypesValueSpaceData::isValueSpaceCountingRequired() {
					return mValueSpaceCountingRequired;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceCountingRequired(bool required) {
					mValueSpaceCountingRequired = required;
					return this;
				}


				bool CDatatypesValueSpaceData::isValueSpaceClashCheckingRequired() {
					return mValueSpaceClashCheckingRequired;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceClashCheckingRequired(bool required) {
					mValueSpaceClashCheckingRequired = required;
					return this;
				}

				bool CDatatypesValueSpaceData::isValueSpaceSatisfiabilityCheckingQueued() {
					return mValueSpaceSatisfiabilityCheckingQueued;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceSatisfiabilityCheckingQueued(bool queued) {
					mValueSpaceSatisfiabilityCheckingQueued = queued;
					return this;
				}

				bool CDatatypesValueSpaceData::isValueSpaceSatisfiabilityCheckingRequired() {
					return mValueSpaceSatisfiabilityCheckingRequired;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceSatisfiabilityCheckingRequired(bool required) {
					mValueSpaceSatisfiabilityCheckingRequired = required;
					return this;
				}

				cint64 CDatatypesValueSpaceData::getLastTriggeringDisjointCount() {
					return mLastTriggeringDisjointCount;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setLastTriggeringDisjointCount(cint64 disjointCount) {
					mLastTriggeringDisjointCount = disjointCount;
					return this;
				}

				CDatatypeValueSpaceValuesCounter* CDatatypesValueSpaceData::getValuesCounter() {
					return &mValuesCounter;
				}

				bool CDatatypesValueSpaceData::isValueSpaceCounted() {
					return mValueSpaceCounted;
				}

				CDatatypesValueSpaceData* CDatatypesValueSpaceData::setValueSpaceCounted(bool counted) {
					mValueSpaceCounted = counted;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

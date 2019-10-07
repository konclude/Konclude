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

#include "CDatatypeValueSpacesTriggers.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpacesTriggers::CDatatypeValueSpacesTriggers(CContext* boxContext) : mBoxContext(boxContext),
					mRealValueSpaceTriggers(boxContext),mStringValueSpaceTriggers(boxContext),mBooleanValueSpaceTriggers(boxContext),
					mDoubleValueSpaceTriggers(boxContext),mFloatValueSpaceTriggers(boxContext),mIRIValueSpaceTriggers(boxContext),
					mBinaryHexDataValueSpaceTriggers(boxContext),mBinaryBase64DataValueSpaceTriggers(boxContext),
					mXMLValueSpaceTriggers(boxContext),mDateTimeValueSpaceTriggers(boxContext) {
			}



			CDatatypeValueSpacesTriggers* CDatatypeValueSpacesTriggers::initValueSpacesTriggers(CDatatypeValueSpacesTriggers* data) {
				if (data) {
					mRealValueSpaceTriggers.initValueSpaceRealTriggers(&data->mRealValueSpaceTriggers);
					mStringValueSpaceTriggers.initValueSpaceStringTriggers(&data->mStringValueSpaceTriggers);
					mBooleanValueSpaceTriggers.initValueSpaceBooleanTriggers(&data->mBooleanValueSpaceTriggers);
					mDoubleValueSpaceTriggers.initValueSpaceDoubleTriggers(&data->mDoubleValueSpaceTriggers);
					mFloatValueSpaceTriggers.initValueSpaceFloatTriggers(&data->mFloatValueSpaceTriggers);
					mIRIValueSpaceTriggers.initValueSpaceIRITriggers(&data->mIRIValueSpaceTriggers);
					mXMLValueSpaceTriggers.initValueSpaceXMLTriggers(&data->mXMLValueSpaceTriggers);
					mBinaryHexDataValueSpaceTriggers.initValueSpaceBinaryHexDataTriggers(&data->mBinaryHexDataValueSpaceTriggers);
					mBinaryBase64DataValueSpaceTriggers.initValueSpaceBinaryBase64DataTriggers(&data->mBinaryBase64DataValueSpaceTriggers);
					mDateTimeValueSpaceTriggers.initValueSpaceDateTimeTriggers(&data->mDateTimeValueSpaceTriggers);
				} else {
					mRealValueSpaceTriggers.initValueSpaceRealTriggers(nullptr);
					mStringValueSpaceTriggers.initValueSpaceStringTriggers(nullptr);
					mBooleanValueSpaceTriggers.initValueSpaceBooleanTriggers(nullptr);
					mDoubleValueSpaceTriggers.initValueSpaceDoubleTriggers(nullptr);
					mFloatValueSpaceTriggers.initValueSpaceFloatTriggers(nullptr);
					mIRIValueSpaceTriggers.initValueSpaceIRITriggers(nullptr);
					mXMLValueSpaceTriggers.initValueSpaceXMLTriggers(nullptr);
					mBinaryHexDataValueSpaceTriggers.initValueSpaceBinaryHexDataTriggers(nullptr);
					mBinaryBase64DataValueSpaceTriggers.initValueSpaceBinaryBase64DataTriggers(nullptr);
					mDateTimeValueSpaceTriggers.initValueSpaceDateTimeTriggers(nullptr);
				}
				return this;
			}



			CDatatypeValueSpaceRealTriggers* CDatatypeValueSpacesTriggers::getRealValueSpaceTriggers() {
				return &mRealValueSpaceTriggers;
			}


			CDatatypeValueSpaceStringTriggers* CDatatypeValueSpacesTriggers::getStringValueSpaceTriggers() {
				return &mStringValueSpaceTriggers;
			}


			CDatatypeValueSpaceBooleanTriggers* CDatatypeValueSpacesTriggers::getBooleanValueSpaceTriggers() {
				return &mBooleanValueSpaceTriggers;
			}

			CDatatypeValueSpaceDoubleTriggers* CDatatypeValueSpacesTriggers::getDoubleValueSpaceTriggers() {
				return &mDoubleValueSpaceTriggers;
			}

			CDatatypeValueSpaceFloatTriggers* CDatatypeValueSpacesTriggers::getFloatValueSpaceTriggers() {
				return &mFloatValueSpaceTriggers;
			}

			CDatatypeValueSpaceIRITriggers* CDatatypeValueSpacesTriggers::getIRIValueSpaceTriggers() {
				return &mIRIValueSpaceTriggers;
			}

			CDatatypeValueSpaceXMLTriggers* CDatatypeValueSpacesTriggers::getXMLValueSpaceTriggers() {
				return &mXMLValueSpaceTriggers;
			}

			CDatatypeValueSpaceDateTimeTriggers* CDatatypeValueSpacesTriggers::getDateTimeValueSpaceTriggers() {
				return &mDateTimeValueSpaceTriggers;
			}

			CDatatypeValueSpaceBinaryHexDataTriggers* CDatatypeValueSpacesTriggers::getBinaryHexDataValueSpaceTriggers() {
				return &mBinaryHexDataValueSpaceTriggers;
			}

			CDatatypeValueSpaceBinaryBase64DataTriggers* CDatatypeValueSpacesTriggers::getBinaryBase64DataValueSpaceTriggers() {
				return &mBinaryBase64DataValueSpaceTriggers;
			}


			CDatatypeValueSpaceTriggers* CDatatypeValueSpacesTriggers::getValueSpaceTriggers(CDatatypeValueSpaceType* valueSpaceType) {
				if (valueSpaceType) {
					if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEREALTYPE) {
						return &mRealValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACESTRINGTYPE) {
						return &mStringValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEBOOLEANTYPE) {
						return &mBooleanValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEDOUBLETYPE) {
						return &mDoubleValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEFLOATTYPE) {
						return &mFloatValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEIRITYPE) {
						return &mIRIValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEXMLTYPE) {
						return &mXMLValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEDATETIMETYPE) {
						return &mDateTimeValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEHEXDATATYPE) {
						return &mBinaryHexDataValueSpaceTriggers;
					} else if (valueSpaceType->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACEBASE64DATATYPE) {
						return &mBinaryBase64DataValueSpaceTriggers;
					}
				}
				return nullptr;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

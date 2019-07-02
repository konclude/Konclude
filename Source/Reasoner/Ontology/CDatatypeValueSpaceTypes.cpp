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

#include "CDatatypeValueSpaceTypes.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceTypes* CDatatypeValueSpaceTypes::mValueSpaceTypes = nullptr;
			QMutex* CDatatypeValueSpaceTypes::mCreationMutex = new QMutex();


			CDatatypeValueSpaceTypes::CDatatypeValueSpaceTypes() {
				mValueSpaceTypeLinker = nullptr;
				mRealValueSpaceType = new CDatatypeValueSpaceRealType();
				mUnknownValueSpaceType = new CDatatypeValueSpaceUnknownType();
				mStringValueSpaceType = new CDatatypeValueSpaceStringType();
				mIRIValueSpaceType = new CDatatypeValueSpaceIRIType();
				mXMLValueSpaceType = new CDatatypeValueSpaceXMLType();
				mBooleanValueSpaceType = new CDatatypeValueSpaceBooleanType();
				mDoubleValueSpaceType = new CDatatypeValueSpaceDoubleType();
				mFloatValueSpaceType = new CDatatypeValueSpaceFloatType();
				mBinaryHexValueSpaceType = new CDatatypeValueSpaceBinaryHexDataType();
				mBinaryBase64ValueSpaceType = new CDatatypeValueSpaceBinaryBase64DataType();
				mDateTimeValueSpaceType = new CDatatypeValueSpaceDateTimeType();
				mBinaryHexValueSpaceType->setNext(mBooleanValueSpaceType);
				mBinaryBase64ValueSpaceType->setNext(mBinaryHexValueSpaceType);
				mDateTimeValueSpaceType->setNext(mBinaryBase64ValueSpaceType);
				mFloatValueSpaceType->setNext(mDateTimeValueSpaceType);
				mDoubleValueSpaceType->setNext(mFloatValueSpaceType);
				mRealValueSpaceType->setNext(mDoubleValueSpaceType);
				mIRIValueSpaceType->setNext(mRealValueSpaceType);
				mXMLValueSpaceType->setNext(mIRIValueSpaceType);
				mStringValueSpaceType->setNext(mXMLValueSpaceType);
				mUnknownValueSpaceType->setNext(mStringValueSpaceType);
				mValueSpaceTypeLinker = mUnknownValueSpaceType;
			}


			CDatatypeValueSpaceTypes* CDatatypeValueSpaceTypes::getValueSpaceTypes() {
				if (!mValueSpaceTypes) {
					mCreationMutex->lock();
					if (!mValueSpaceTypes) {
						mValueSpaceTypes = new CDatatypeValueSpaceTypes();
					}
					mCreationMutex->unlock();
				}
				return mValueSpaceTypes;
			}


			CDatatypeValueSpaceType* CDatatypeValueSpaceTypes::getValueSpaceTypeLinker() {
				return mValueSpaceTypeLinker;
			}


			CDatatypeValueSpaceRealType* CDatatypeValueSpaceTypes::getValueSpaceRealType() {
				return mRealValueSpaceType;
			}


			CDatatypeValueSpaceUnknownType* CDatatypeValueSpaceTypes::getValueSpaceUnkownType() {
				return mUnknownValueSpaceType;
			}

			CDatatypeValueSpaceStringType* CDatatypeValueSpaceTypes::getValueSpaceStringType() {
				return mStringValueSpaceType;
			}

			CDatatypeValueSpaceBooleanType* CDatatypeValueSpaceTypes::getValueSpaceBooleanType() {
				return mBooleanValueSpaceType;
			}

			CDatatypeValueSpaceDoubleType* CDatatypeValueSpaceTypes::getValueSpaceDoubleType() {
				return mDoubleValueSpaceType;
			}

			CDatatypeValueSpaceFloatType* CDatatypeValueSpaceTypes::getValueSpaceFloatType() {
				return mFloatValueSpaceType;
			}

			CDatatypeValueSpaceIRIType* CDatatypeValueSpaceTypes::getValueSpaceIRIType() {
				return mIRIValueSpaceType;
			}

			CDatatypeValueSpaceXMLType* CDatatypeValueSpaceTypes::getValueSpaceXMLType() {
				return mXMLValueSpaceType;
			}

			CDatatypeValueSpaceDateTimeType* CDatatypeValueSpaceTypes::getValueSpaceDateTimeType() {
				return mDateTimeValueSpaceType;
			}

			CDatatypeValueSpaceBinaryHexDataType* CDatatypeValueSpaceTypes::getValueSpaceBinaryHexDataType() {
				return mBinaryHexValueSpaceType;
			}

			CDatatypeValueSpaceBinaryBase64DataType* CDatatypeValueSpaceTypes::getValueSpaceBinaryBase64DataType() {
				return mBinaryBase64ValueSpaceType;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

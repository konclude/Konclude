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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPES_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPES_H

// Libraries includes
#include <QMutex>


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceType.h"
#include "CDatatypeValueSpaceRealType.h"
#include "CDatatypeValueSpaceUnknownType.h"
#include "CDatatypeValueSpaceStringType.h"
#include "CDatatypeValueSpaceBooleanType.h"
#include "CDatatypeValueSpaceDoubleType.h"
#include "CDatatypeValueSpaceFloatType.h"
#include "CDatatypeValueSpaceIRIType.h"
#include "CDatatypeValueSpaceXMLType.h"
#include "CDatatypeValueSpaceDateTimeType.h"
#include "CDatatypeValueSpaceBinaryHexDataType.h"
#include "CDatatypeValueSpaceBinaryBase64DataType.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDatatypeValueSpaceTypes
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceTypes {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceTypes();


					static CDatatypeValueSpaceTypes* getValueSpaceTypes();

					CDatatypeValueSpaceRealType* getValueSpaceRealType();
					CDatatypeValueSpaceUnknownType* getValueSpaceUnkownType();
					CDatatypeValueSpaceStringType* getValueSpaceStringType();
					CDatatypeValueSpaceBooleanType* getValueSpaceBooleanType();
					CDatatypeValueSpaceDoubleType* getValueSpaceDoubleType();
					CDatatypeValueSpaceFloatType* getValueSpaceFloatType();
					CDatatypeValueSpaceIRIType* getValueSpaceIRIType();
					CDatatypeValueSpaceBinaryHexDataType* getValueSpaceBinaryHexDataType();
					CDatatypeValueSpaceBinaryBase64DataType* getValueSpaceBinaryBase64DataType();
					CDatatypeValueSpaceXMLType* getValueSpaceXMLType();
					CDatatypeValueSpaceDateTimeType* getValueSpaceDateTimeType();

					CDatatypeValueSpaceType* getValueSpaceTypeLinker();


				// protected methods
				protected:

				// protected variables
				protected:
					static QMutex* mCreationMutex;
					static CDatatypeValueSpaceTypes* mValueSpaceTypes;


					CDatatypeValueSpaceRealType* mRealValueSpaceType;
					CDatatypeValueSpaceUnknownType* mUnknownValueSpaceType;
					CDatatypeValueSpaceStringType* mStringValueSpaceType;
					CDatatypeValueSpaceBooleanType* mBooleanValueSpaceType;
					CDatatypeValueSpaceDoubleType* mDoubleValueSpaceType;
					CDatatypeValueSpaceFloatType* mFloatValueSpaceType;
					CDatatypeValueSpaceIRIType* mIRIValueSpaceType;
					CDatatypeValueSpaceBinaryHexDataType* mBinaryHexValueSpaceType;
					CDatatypeValueSpaceBinaryBase64DataType* mBinaryBase64ValueSpaceType;
					CDatatypeValueSpaceXMLType* mXMLValueSpaceType;
					CDatatypeValueSpaceDateTimeType* mDateTimeValueSpaceType;

					CDatatypeValueSpaceType* mValueSpaceTypeLinker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPES_H

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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERALBINARYDATAVALUE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERALBINARYDATAVALUE_H

// Libraries includes
#include <QString>


// Namespace includes
#include "OntologySettings.h"
#include "CDataLiteralValue.h"
#include "CDataLiteralCompareValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDataLiteralBinaryDataValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralBinaryDataValue : public CDataLiteralCompareValue {
				// public methods
				public:
					//! Constructor
					CDataLiteralBinaryDataValue(CContext* context);


					virtual CDataLiteralCompareValue* initValue(CDataLiteralCompareValue* value);
					CDataLiteralBinaryDataValue* initValueFromInfiniteLength();
					CDataLiteralBinaryDataValue* initValueFromLength(cuint64 length);
					CDataLiteralBinaryDataValue* initValueFromData(unsigned char* data, cint64 length);

					CDataLiteralBinaryDataValue* initValue(CDataLiteralBinaryDataValue* value);

					CDataLiteralBinaryDataValue* clearValue();

					CDataLiteralBinaryDataValue* incrementBinaryData();
					cuint64 getBinaryDataID();


					bool isInfiniteLength();
					cuint64 getLength();

					unsigned char* getByteDataArray();
					cint64 getByteDataLength();


					bool isEqualTo(CDataLiteralValue* value);

					bool isLessEqualThan(CDataLiteralCompareValue* value);
					bool isLessThan(CDataLiteralCompareValue* value);
					bool isEqualTo(CDataLiteralCompareValue* value);
					bool isGreaterEqualThan(CDataLiteralCompareValue* value);
					bool isGreaterThan(CDataLiteralCompareValue* value);

					bool isLessEqualThan(CDataLiteralBinaryDataValue* value);
					bool isLessThan(CDataLiteralBinaryDataValue* value);
					bool isEqualTo(CDataLiteralBinaryDataValue* value);
					bool isGreaterEqualThan(CDataLiteralBinaryDataValue* value);
					bool isGreaterThan(CDataLiteralBinaryDataValue* value);


				// protected methods
				protected:
					bool isZeroData();

				// protected variables
				protected:
					CContext* mContext;
					bool mInfiniteLength;
					cuint64 mLength;
					unsigned char* mByteDataArray;
					cint64 mByteDataArraySize;
					cint64 mByteDataLength;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERALBINARYDATAVALUE_H

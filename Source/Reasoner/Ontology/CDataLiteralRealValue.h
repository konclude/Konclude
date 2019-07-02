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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERALREALVALUE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERALREALVALUE_H

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
			 *		\class		CDataLiteralRealValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralRealValue : public CDataLiteralCompareValue {
				// public methods
				public:
					//! Constructor
					CDataLiteralRealValue();

					static const cint64 DLRV_INTEGER_FLAG		= 0x01;
					static const cint64 DLRV_DECIMAL_FLAG		= 0x02;
					static const cint64 DLRV_RATIONAL_FLAG		= 0x04;

					virtual CDataLiteralCompareValue* initValue(CDataLiteralCompareValue* value);
					CDataLiteralRealValue* initRationalValueFromInfinite(bool negative);
					CDataLiteralRealValue* initRationalValueFromFraction(cint64 numeratorValue, cint64 denominatorValue);
					CDataLiteralRealValue* initRationalValueFromFraction(cuint64 numeratorValue, cuint64 denominatorValue, bool negative);
					CDataLiteralRealValue* initRationalValueFromDecimal(cint64 integerValue, cint64 decimalValue, cint64 precedingZerors = 0);
					CDataLiteralRealValue* initRationalValueFromDecimal(cuint64 integerValue, cuint64 decimalValue, bool negative, cint64 precedingZerors = 0);
					CDataLiteralRealValue* initRationalValueFromInteger(cint64 value);
					CDataLiteralRealValue* initRationalValueFromInteger(cuint64 value, bool negation);

					CDataLiteralRealValue* initRationalValueFromNextIntegerValue(CDataLiteralRealValue* value);

					CDataLiteralRealValue* initValue(CDataLiteralRealValue* value);

					CDataLiteralRealValue* clearValue();
					CDataLiteralRealValue* clearFlags();


					virtual QString getValueString();

					bool hasFlag(cint64 flag);


					virtual DATA_LITERAL_VALUE_TYPE getDataValueType();

					bool isNegative();
					bool isInfinite();
					cuint64 getInteger();
					cuint64 getNumerator();
					cuint64 getDenominator();

					bool isInfiniteBigNumber();

					cint64 getPosNegInteger();


					CDataLiteralRealValue* setNegative(bool negative);
					CDataLiteralRealValue* setInteger(cuint64 integer);
					CDataLiteralRealValue* setNumerator(cuint64 numerator);
					CDataLiteralRealValue* setDenominator(cuint64 denominator);


					bool isEqualTo(CDataLiteralValue* value);

					bool isLessEqualThan(CDataLiteralCompareValue* value);
					bool isLessThan(CDataLiteralCompareValue* value);
					bool isEqualTo(CDataLiteralCompareValue* value);
					bool isGreaterEqualThan(CDataLiteralCompareValue* value);
					bool isGreaterThan(CDataLiteralCompareValue* value);

					bool isLessEqualThan(CDataLiteralRealValue* value);
					bool isLessThan(CDataLiteralRealValue* value);
					bool isEqualTo(CDataLiteralRealValue* value);
					bool isGreaterEqualThan(CDataLiteralRealValue* value);
					bool isGreaterThan(CDataLiteralRealValue* value);

				// protected methods
				protected:
					CDataLiteralRealValue* identifyFlags();
					bool normalize();

				// protected variables
				protected:
					bool mNegative;
					bool mInfinite;
					cuint64 mInteger;
					cuint64 mNumerator;
					cuint64 mDenominator;

					cint64 mFlags;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERALREALVALUE_H

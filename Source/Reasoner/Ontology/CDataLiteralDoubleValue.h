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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERALDOUBLEVALUE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERALDOUBLEVALUE_H

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
			 *		\class		CDataLiteralDoubleValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralDoubleValue : public CDataLiteralCompareValue {
				// public methods
				public:
					//! Constructor
					CDataLiteralDoubleValue();

					virtual CDataLiteralCompareValue* initValue(CDataLiteralCompareValue* value);
					CDataLiteralDoubleValue* initValueFromDouble(double doubleValue);
					CDataLiteralDoubleValue* initValueFromInfinite(bool negative);

					CDataLiteralDoubleValue* initValue(CDataLiteralDoubleValue* value);

					CDataLiteralDoubleValue* clearValue();


					virtual DATA_LITERAL_VALUE_TYPE getDataValueType();

					double getDouble();

					bool isInfinite();
					bool isNegative();


					bool isEqualTo(CDataLiteralValue* value);

					bool isLessEqualThan(CDataLiteralCompareValue* value);
					bool isLessThan(CDataLiteralCompareValue* value);
					bool isEqualTo(CDataLiteralCompareValue* value);
					bool isGreaterEqualThan(CDataLiteralCompareValue* value);
					bool isGreaterThan(CDataLiteralCompareValue* value);

					bool isLessEqualThan(CDataLiteralDoubleValue* value);
					bool isLessThan(CDataLiteralDoubleValue* value);
					bool isEqualTo(CDataLiteralDoubleValue* value);
					bool isGreaterEqualThan(CDataLiteralDoubleValue* value);
					bool isGreaterThan(CDataLiteralDoubleValue* value);

					virtual QString getValueString();


				// protected methods
				protected:

				// protected variables
				protected:
					double mDouble;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERALDOUBLEVALUE_H

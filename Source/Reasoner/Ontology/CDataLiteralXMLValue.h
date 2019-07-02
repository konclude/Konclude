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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERALXMLVALUE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERALXMLVALUE_H

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
			 *		\class		CDataLiteralXMLValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteralXMLValue : public CDataLiteralCompareValue {
				// public methods
				public:
					//! Constructor
					CDataLiteralXMLValue();


					virtual CDataLiteralCompareValue* initValue(CDataLiteralCompareValue* value);
					CDataLiteralXMLValue* initValueFromInfiniteLength();
					CDataLiteralXMLValue* initValueFromLength(cuint64 length);
					CDataLiteralXMLValue* initValueFromXMLString(const QString& xmlString);

					CDataLiteralXMLValue* initValue(CDataLiteralXMLValue* value);

					CDataLiteralXMLValue* clearValue();


					virtual DATA_LITERAL_VALUE_TYPE getDataValueType();

					bool isInfiniteLength();
					cuint64 getLength();

					QString getXMLValue();


					bool isEqualTo(CDataLiteralValue* value);

					bool isLessEqualThan(CDataLiteralCompareValue* value);
					bool isLessThan(CDataLiteralCompareValue* value);
					bool isEqualTo(CDataLiteralCompareValue* value);
					bool isGreaterEqualThan(CDataLiteralCompareValue* value);
					bool isGreaterThan(CDataLiteralCompareValue* value);

					bool isLessEqualThan(CDataLiteralXMLValue* value);
					bool isLessThan(CDataLiteralXMLValue* value);
					bool isEqualTo(CDataLiteralXMLValue* value);
					bool isGreaterEqualThan(CDataLiteralXMLValue* value);
					bool isGreaterThan(CDataLiteralXMLValue* value);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mInfiniteLength;
					cuint64 mLength;
					QString mXMLString;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERALXMLVALUE_H

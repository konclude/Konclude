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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATALITERAL_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATALITERAL_H

// Libraries includes
#include <QString>


// Namespace includes
#include "OntologySettings.h"
#include "CDatatype.h"
#include "CDataLiteralValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDataLiteral
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDataLiteral {
				// public methods
				public:
					//! Constructor
					CDataLiteral(CContext* context);
					~CDataLiteral();

					CDataLiteral* initDataLiteral(const QString& lexicalDataLiteralValueString, CDatatype* datatype);

					QString getLexicalDataLiteralValueString();

					CDatatype* getDatatype();

					CDataLiteralValue* getDataLiteralValue();
					CDataLiteral* setDataLiteralValue(CDataLiteralValue* dataValue);

				// protected methods
				protected:
					QChar* createStringByteDataArray(const QString& string);

				// protected variables
				protected:
					CContext* mContext;
					CDatatype* mDatatype;
					CDataLiteralValue* mDataValue;

					QChar* mLexicalLiteralStringByteDataArray;
					cint64 mLexicalLiteralStringByteDataArraySize;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATALITERAL_H

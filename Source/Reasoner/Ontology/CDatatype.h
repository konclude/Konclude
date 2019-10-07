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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPE_H

// Libraries includes
#include <QString>


// Namespace includes
#include "OntologySettings.h"
#include "CTagItem.h"
#include "CDatatypeValueSpaceType.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDatatype
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatype : public CTagItem {
				// public methods
				public:

					enum DATATYPE_BASIC_TYPE { DBT_NON, DBT_REAL, DBT_STRING, DBT_BOOLEAN, DBT_DOUBLE, DBT_FLOAT, DBT_IRI, DBT_HEXBINARY, DBT_BASE64BINARY, DBT_XML, DBT_DATETIME };
					enum DATATYPE_TYPE { DT_NON, DT_REAL, DT_DECIMAL, DT_INTEGER, DT_RATIONAL, DT_PLAINLITERAL, DT_STRING, DT_BOOLEAN, DT_DOUBLE, DT_FLOAT, DT_IRI, DT_XML };

					//! Constructor
					CDatatype();

					CDatatype* initDatatype(const QString& datatypeIRI, bool basic = false);
					CDatatype* initDatatype(const QString& datatypeIRI, cint64 datatypeTag, bool basic);

					CDatatype* setDatatypeTag(qint64 datatypeTag);
					cint64 getDatatypeTag() const;

					bool isBasicDatatype();
					CDatatype* setBasicDatatype(bool basic);


					CDatatype* getBasicDatatype();
					CDatatype* setBasicDatatype(CDatatype* basicDatatpye);

					DATATYPE_BASIC_TYPE getBasicDatatypeType();
					CDatatype* setBasicDatatypeType(DATATYPE_BASIC_TYPE basicDatatpyeType);

					DATATYPE_TYPE getDatatypeType();
					CDatatype* setDatatypeType(DATATYPE_TYPE datatpyeType);

					const QString& getDatatypeIRI();

					CConcept* getDatatypeConcept();
					bool hasDatatypeConcept();
					CDatatype* setDatatypeConcept(CConcept* concept);

					CConcept* getDefinitionConcept();
					CDatatype* setDefinitionConcept(CConcept* concept);

					CDatatypeValueSpaceType* getValueSpaceType();
					CDatatype* setValueSpaceType(CDatatypeValueSpaceType* valueSpaceType);

					bool isNormalized();
					CDatatype* setNormalized(bool normalized);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mBasicDatatype;
					bool mNormalized;
					QString mDatatypeIRI;
					CConcept* mDatatypeConcept;
					CConcept* mDefinitionConcept;
					CDatatypeValueSpaceType* mValueSpaceType;
					CDatatype* mDatatpyeBasic;
					DATATYPE_BASIC_TYPE mBasicDatatpyeType;
					DATATYPE_TYPE mDatatpyeType;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPE_H

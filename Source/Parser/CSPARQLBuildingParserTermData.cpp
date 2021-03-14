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

#include "CSPARQLBuildingParserTermData.h"


namespace Konclude {

	namespace Parser {



		CSPARQLBuildingParserTermData::CSPARQLBuildingParserTermData(const QString& term) {
			mTermString = term;
			mTermType = UNKNON_TYPE;
		}


		QString CSPARQLBuildingParserTermData::getTermString() {
			return mTermString;
		}


		CSPARQLBuildingParserTermData::TermType CSPARQLBuildingParserTermData::getTermType() {
			return mTermType;
		}

		CSPARQLBuildingParserTermData* CSPARQLBuildingParserTermData::setTermType(CSPARQLBuildingParserTermData::TermType type) {
			mTermType = type;
			return this;
		}


		bool CSPARQLBuildingParserTermData::isVariableTerm() {
			if (mTermType == VARIABLE_TYPE || mTermType == CLASS_VARIABLE_TYPE || mTermType == PROPERTY_VARIABLE_TYPE || mTermType == OBJECT_PROPERTY_VARIABLE_TYPE || mTermType == DATA_PROPERTY_VARIABLE_TYPE || 
					mTermType == INDIVIDUAL_VARIABLE_TYPE || mTermType == NAMED_INDIVIDUAL_VARIABLE_TYPE || mTermType == ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE || mTermType == DATA_LITERAL_VARIABLE_TYPE) {
				return true;
			} else {
				return false;
			}
		}

		bool CSPARQLBuildingParserTermData::isNonVariableTerm() {
			return !isVariableTerm() && getTermType() != UNKNON_TYPE;
		}


		QList<CSPARQLBuildingParserTripleData*>* CSPARQLBuildingParserTermData::getOccuringTripleList() {
			return &mOccuringTripleList;
		}


		CSPARQLBuildingParserTermData* CSPARQLBuildingParserTermData::addOccuringTriple(CSPARQLBuildingParserTripleData* tripleData) {
			mOccuringTripleList.append(tripleData);
			return this;
		}


		bool CSPARQLBuildingParserTermData::isClassTerm() {
			return mTermType == CLASS_VARIABLE_TYPE || mTermType == CLASS_TYPE;
		}

		bool CSPARQLBuildingParserTermData::isPropertyTerm() {
			return mTermType == PROPERTY_VARIABLE_TYPE || mTermType == OBJECT_PROPERTY_VARIABLE_TYPE || mTermType == DATA_PROPERTY_VARIABLE_TYPE || mTermType == PROPERTY_TYPE || mTermType == OBJECT_PROPERTY_TYPE || mTermType == DATA_PROPERTY_TYPE;
		}


		bool CSPARQLBuildingParserTermData::isObjectPropertyTerm() {
			return mTermType == OBJECT_PROPERTY_VARIABLE_TYPE || mTermType == OBJECT_PROPERTY_TYPE;
		}

		bool CSPARQLBuildingParserTermData::isDataPropertyTerm() {
			return mTermType == DATA_PROPERTY_VARIABLE_TYPE || mTermType == DATA_PROPERTY_TYPE;
		}

		bool CSPARQLBuildingParserTermData::isIndividualTerm() {
			return mTermType == INDIVIDUAL_VARIABLE_TYPE || mTermType == NAMED_INDIVIDUAL_VARIABLE_TYPE || mTermType == ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE || mTermType == NAMED_INDIVIDUAL_TYPE || mTermType == ANONYMOUS_INDIVIDUAL_TYPE;
		}

		bool CSPARQLBuildingParserTermData::isLiteralTerm() {
			return mTermType == DATA_LITERAL_VARIABLE_TYPE || mTermType == DATA_LITERAL_TYPE;
		}

		bool CSPARQLBuildingParserTermData::isCompletelyTyped() {
			return mTermType == CLASS_VARIABLE_TYPE || mTermType == OBJECT_PROPERTY_VARIABLE_TYPE || mTermType == DATA_PROPERTY_VARIABLE_TYPE || mTermType == NAMED_INDIVIDUAL_VARIABLE_TYPE || mTermType == ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE || mTermType == DATA_LITERAL_VARIABLE_TYPE ||
				mTermType == CLASS_TYPE || mTermType == DATA_LITERAL_TYPE || mTermType == OBJECT_PROPERTY_TYPE || mTermType == DATA_PROPERTY_TYPE || mTermType == NAMED_INDIVIDUAL_TYPE || mTermType == ANONYMOUS_INDIVIDUAL_TYPE;
		}


	}; // end namespace Parser

}; // end namespace Konclude

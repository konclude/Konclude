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

#include "CSPARQLBuildingParserTripleData.h"


namespace Konclude {

	namespace Parser {



		CSPARQLBuildingParserTripleData::CSPARQLBuildingParserTripleData() {
			for (auto& termData : mTermDatas) {
				termData = nullptr;
			}
		}


		CSPARQLBuildingParserTripleData::CSPARQLBuildingParserTripleData(CSPARQLBuildingParserTermData* termData1, CSPARQLBuildingParserTermData* termData2, CSPARQLBuildingParserTermData* termData3) {
			for (auto& mTermData : mTermDatas) {
				mTermData = nullptr;
			}
			setTermData(0, termData1);
			setTermData(1, termData2);
			setTermData(2, termData3);
		}


		CSPARQLBuildingParserTripleData* CSPARQLBuildingParserTripleData::setTermData(cint64 index, CSPARQLBuildingParserTermData* termData) {
			mTermDatas[index] = termData;
			if (termData) {
				termData->addOccuringTriple(this);
			}
			return this;
		}


		CSPARQLBuildingParserTermData* CSPARQLBuildingParserTripleData::getTermData(cint64 index) {
			return mTermDatas[index];
		}


		bool CSPARQLBuildingParserTripleData::areTermsCompletelyTyped() {
			for (auto& termData : mTermDatas) {
				if (termData && !termData->isCompletelyTyped()) {
					return false;
				}
			}
			return true;
		}


	}; // end namespace Parser

}; // end namespace Konclude

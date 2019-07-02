/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CBooleanQueryResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CBooleanQueryResult::CBooleanQueryResult(bool booleanResult) {
				result = booleanResult;
			}


			CBooleanQueryResult::~CBooleanQueryResult() {
			}


			CBooleanQueryResult *CBooleanQueryResult::setResult(bool booleanResult) {
				result = booleanResult;
				return this;				
			}

			bool CBooleanQueryResult::getResult() {
				return result;
			}



			QString CBooleanQueryResult::getQueryResultString() {
				if (result) {
					return QString("true");
				} else {
					return QString("false");
				}
			}


			bool CBooleanQueryResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					CBooleanQueryResult *otherBoolQueryResult = dynamic_cast<CBooleanQueryResult *>(otherQueryResult);
					if (otherBoolQueryResult) {
						return isResultEquivalentTo(otherBoolQueryResult);
					} 
				}
				return false;
			}


			bool CBooleanQueryResult::isResultEquivalentTo(CBooleanQueryResult *otherQueryResult) {
				if (otherQueryResult) {
					return otherQueryResult->getResult() == result;
				} else {
					return false;
				}
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

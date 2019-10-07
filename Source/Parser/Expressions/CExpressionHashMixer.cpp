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

#include "CExpressionHashMixer.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CExpressionHashMixer::CExpressionHashMixer() {
			}


			CExpressionHashMixer::~CExpressionHashMixer() {
			}

			cint64 CExpressionHashMixer::getHashValueMixed(cint64 hashValue1) {
				cint64 hashValue = hashValue1;
				return hashValue;
			}

			cint64 CExpressionHashMixer::getHashValueMixed(cint64 hashValue1, cint64 hashValue2) {
				cint64 hashValue = hashValue1;
				hashValue = ((hashValue*13)-1)^hashValue2;
				return hashValue;
			}

			cint64 CExpressionHashMixer::getHashValueMixed(cint64 hashValue1, cint64 hashValue2, cint64 hashValue3) {
				cint64 hashValue = hashValue1;
				hashValue = ((hashValue*13)-1)^hashValue2;
				hashValue = ((hashValue*13)-1)^hashValue3;
				return hashValue;
			}

			cint64 CExpressionHashMixer::getHashValueMixed(cint64 hashValue1, cint64 hashValue2, cint64 hashValue3, cint64 hashValue4) {
				cint64 hashValue = hashValue1;
				hashValue = ((hashValue*13)-1)^hashValue2;
				hashValue = ((hashValue*13)-1)^hashValue3;
				hashValue = ((hashValue*13)-1)^hashValue4;
				return hashValue;
			}

			cint64 CExpressionHashMixer::getHashValueMixed(cint64 hashValue1, cint64 hashValue2, cint64 hashValue3, cint64 hashValue4, cint64 hashValue5) {
				cint64 hashValue = hashValue1;
				hashValue = ((hashValue * 13) - 1) ^ hashValue2;
				hashValue = ((hashValue * 13) - 1) ^ hashValue3;
				hashValue = ((hashValue * 13) - 1) ^ hashValue4;
				hashValue = ((hashValue * 13) - 1) ^ hashValue5;
				return hashValue;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

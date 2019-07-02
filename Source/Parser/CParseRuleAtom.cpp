/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CParseRuleAtom.h"


namespace Konclude {

	namespace Parser {


		CParseRuleAtom::CParseRuleAtom(ATOMTYPE atomType) {
			mAtomType = atomType;
			mAtomExpression = nullptr;
		}

		CParseRuleAtom* CParseRuleAtom::addVariableString(const QString& variableString) {
			if (mVariableName1.isEmpty()) {
				mVariableName1 = variableString;
			} else if (mVariableName2.isEmpty()) {
				mVariableName2 = variableString;
			}
			return this;
		}

		const QString CParseRuleAtom::getOtherVariable(const QString& variableString, bool& inversed) {
			if (variableString == mVariableName1) {
				inversed = false;
				return mVariableName2;
			} else {
				inversed = true;
				return mVariableName1;
			}
		}


	}; // end namespace Parser

}; // end namespace Konclude

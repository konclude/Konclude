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

#include "CVariableAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CVariableAssociator::CVariableAssociator(CExpressionVariable* variable) {
				mVariable = variable;
			}


			CVariableAssociator::~CVariableAssociator() {
			}

			CExpressionVariable* CVariableAssociator::getEntity() {
				return mVariable;
			}

			CVariableAssociator* CVariableAssociator::setEntity(CExpressionVariable* variable) {
				mVariable = variable;
				return this;
			}

			cint64 CVariableAssociator::getStructuralHashValue() {
				return (cint64)mVariable;
			}

			bool CVariableAssociator::compareStructuralEquivalence(const CVariableAssociator& variableAssociator) {
				return variableAssociator.mVariable == mVariable;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

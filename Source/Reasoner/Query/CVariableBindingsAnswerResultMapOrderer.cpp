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

#include "CVariableBindingsAnswerResultMapOrderer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CVariableBindingsAnswerResultMapOrderer::CVariableBindingsAnswerResultMapOrderer(CVariableBindingsListAnswerResult* bindingResult, CVariableBindingsAnswerResultMapOrdering* odering) {
				mBindingResult = bindingResult;
				mOdering = odering;
			}


			bool CVariableBindingsAnswerResultMapOrderer::operator<(const CVariableBindingsAnswerResultMapOrderer& orderer) const {
				cint64 count = mOdering->getVariableCount();
				bool stillEqual = true;
				for (cint64 varNr = 0; varNr < count && stillEqual; ++varNr) {
					cint64 varIdx = mOdering->getVariableIndex(varNr);
					CVariableBindingResult* binding = mBindingResult->getResult(varIdx);
					CVariableBindingResult* otherBinding = orderer.mBindingResult->getResult(varIdx);
					if (!mOdering->isAscending(varNr)) {
						if (isLess(binding, otherBinding, stillEqual, count == varNr)) {
							return true;
						}
					} else {
						if (isLess(otherBinding, binding, stillEqual, count == varNr)) {
							return true;
						}
					}
				}
				return false;
			}

			bool CVariableBindingsAnswerResultMapOrderer::isLess(CVariableBindingResult* binding1, CVariableBindingResult* binding2, bool& stillEqual, bool lastComparison) const {
				bool compared = false;
				if (!compared && binding1->getVariableBindingType() == CVariableBindingResult::VBTANONYMOUSINDIVIDUAL && binding2->getVariableBindingType() == CVariableBindingResult::VBTNAMEDINDIVIDUAL) {
					return true;
				}
				if (!compared && binding1->getVariableBindingType() == CVariableBindingResult::VBTLITERAL && binding2->getVariableBindingType() == CVariableBindingResult::VBTLITERAL) {
					CVariableBindingStringDataResult* stringDataBinding1 = dynamic_cast<CVariableBindingStringDataResult*>(binding1);
					CVariableBindingStringDataResult* stringDataBinding2 = dynamic_cast<CVariableBindingStringDataResult*>(binding2);
					if (stringDataBinding1 && stringDataBinding2) {
						CDataLiteral* dataLiteral1 = stringDataBinding1->getDataLiteral();
						CDataLiteral* dataLiteral2 = stringDataBinding2->getDataLiteral();
						CDataLiteralValue* value1 = dataLiteral1->getDataLiteralValue();
						CDataLiteralValue* value2 = dataLiteral2->getDataLiteralValue();
						if (value1->getDataValueType() == value2->getDataValueType()) {
							CDataLiteralCompareValue* compValue1 = dynamic_cast<CDataLiteralCompareValue*>(value1);
							CDataLiteralCompareValue* compValue2 = dynamic_cast<CDataLiteralCompareValue*>(value2);
							if (compValue1 && compValue2) {
								if (compValue1->isLessThan(compValue2)) {
									return true;
								} else if (stillEqual && !compValue1->isEqualTo(compValue2)) {
									stillEqual = false;
								}
								compared = true;
							}
						}
					}
				}
				int comp = 0;
				if (!compared) {
					comp = binding1->getQueryResultString().compare(binding2->getQueryResultString(), Qt::CaseSensitive);
				}
				if (comp > 0) {
					return true;
				}
				if (stillEqual && comp != 0) {
					stillEqual = false;
				}
				if (stillEqual && lastComparison && mOdering->isDistinct()) {
					return binding1 < binding2;
				}
				return false;
			}


			CVariableBindingsListAnswerResult* CVariableBindingsAnswerResultMapOrderer::getBindingResult() const {
				return mBindingResult;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

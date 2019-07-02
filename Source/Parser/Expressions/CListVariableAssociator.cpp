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

#include "CListVariableAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListVariableAssociator::CListVariableAssociator(CExpressionVariable *variable1, CExpressionVariable *variable2, CExpressionVariable *variable3) {
				if (variable1) {
					mVariableList.append(variable1);
				}
				if (variable2) {
					mVariableList.append(variable2);
				}
				if (variable3) {
					mVariableList.append(variable3);
				}
			}

			CListVariableAssociator::CListVariableAssociator(const CEXPRESSIONLIST<CExpressionVariable*>& variableList) : mVariableList(variableList) {
			}

			CListVariableAssociator::CListVariableAssociator(CEXPRESSIONLIST<CExpressionVariable*>* variableList) : mVariableList(*variableList) {
			}


			CListVariableAssociator* CListVariableAssociator::initExpressionAssociator(CExpressionVariable *variable1, CExpressionVariable *variable2, CExpressionVariable *variable3) {
				if (variable1) {
					if (mVariableList.isEmpty()) {
						mVariableList.append(variable1);
					} else {
						mVariableList.replace(0,variable1);
					}

					if (variable2) {
						if (mVariableList.size() < 2) {
							mVariableList.append(variable2);
						} else {
							mVariableList.replace(1,variable2);
						}

						if (variable3) {
							if (mVariableList.size() < 3) {
								mVariableList.append(variable3);
							} else {
								mVariableList.replace(2,variable3);
							}
						} else {
							while (mVariableList.size() > 2) {
								mVariableList.removeLast();
							}
						}

					} else {
						while (mVariableList.size() > 1) {
							mVariableList.removeLast();
						}
					}

				} else {
					mVariableList.clear();
				}
				return this;
			}


			CEXPRESSIONLIST<CExpressionVariable*>* CListVariableAssociator::getVariableList() {
				return &mVariableList;
			}

			const CEXPRESSIONLIST<CExpressionVariable*>* CListVariableAssociator::getConstVariableList() const {
				return &mVariableList;
			}


			cint64 CListVariableAssociator::getVariableCount() {
				return mVariableList.count();
			}

			cint64 CListVariableAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				cint64 tmpValue1 = 0;
				cint64 tmpValue2 = 1;
				FOREACHIT (CExpressionVariable* variable, mVariableList) {
					tmpValue1 += cint64(variable);
					tmpValue2 *= cint64(variable);
				}
				hashValue = tmpValue1+tmpValue2;
				return hashValue;
			}

			bool CListVariableAssociator::compareStructuralEquivalence(const CListVariableAssociator& listExpressionAssociator) {
				if (mVariableList.count() != listExpressionAssociator.mVariableList.count()) {
					return false;
				}
				FOREACHIT (CExpressionVariable* variable, mVariableList) {
					cint64 containedCount = listExpressionAssociator.mVariableList.count(variable);
					if (containedCount <= 0) {
						return false;
					} else if (containedCount > 1) {
						if (mVariableList.count(variable) != containedCount) {
							return false;
						}
					}
				}
				return true;
			}



		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

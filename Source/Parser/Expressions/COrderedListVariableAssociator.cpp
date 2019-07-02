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

#include "COrderedListVariableAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedListVariableAssociator::COrderedListVariableAssociator(CExpressionVariable *variable1, CExpressionVariable *variable2, CExpressionVariable *variable3) 
					: CListVariableAssociator(variable1,variable2,variable3) {
			}


			COrderedListVariableAssociator::COrderedListVariableAssociator(const CEXPRESSIONLIST<CExpressionVariable*>& variableList) : CListVariableAssociator(*(CEXPRESSIONLIST<CExpressionVariable*>*)&variableList) {
			}


			COrderedListVariableAssociator::~COrderedListVariableAssociator() {
			}


			cint64 COrderedListVariableAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				FOREACHIT (CExpressionVariable* variable, mVariableList) {
					hashValue += CExpressionHashMixer::getHashValueMixed(hashValue,cint64(variable));
				}
				return hashValue;
			}

			bool COrderedListVariableAssociator::compareStructuralEquivalence(const CListVariableAssociator& listExpressionAssociator) {
				const CEXPRESSIONLIST<CExpressionVariable*>* otherListExp = listExpressionAssociator.getConstVariableList();
				if (mVariableList.count() != otherListExp->count()) {
					return false;
				}
				for (CEXPRESSIONLIST<CExpressionVariable*>::const_iterator it1 = mVariableList.constBegin(), it1End = mVariableList.constEnd(),
					it2 = otherListExp->constBegin(), it2End = otherListExp->constEnd(); it1 != it1End && it2 != it2End; ++it1,++it2) {
						if (*it1 != *it2) {
							return false;
						}
				}
				return true;
			}

			bool COrderedListVariableAssociator::compareStructuralEquivalence(const COrderedListVariableAssociator& orderedListExpressionAssociator) {
				const CEXPRESSIONLIST<CExpressionVariable*>* otherListExp = orderedListExpressionAssociator.getConstVariableList();
				if (mVariableList.count() != otherListExp->count()) {
					return false;
				}
				for (CEXPRESSIONLIST<CExpressionVariable*>::const_iterator it1 = mVariableList.constBegin(), it1End = mVariableList.constEnd(),
					it2 = otherListExp->constBegin(), it2End = otherListExp->constEnd(); it1 != it1End && it2 != it2End; ++it1,++it2) {
						if (*it1 != *it2) {
							return false;
						}
				}
				return true;
			}

			CEXPRESSIONLIST<CExpressionVariable*>* COrderedListVariableAssociator::getOrderedVariableList() {
				return &mVariableList;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

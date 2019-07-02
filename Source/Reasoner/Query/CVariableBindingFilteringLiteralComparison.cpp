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

#include "CVariableBindingFilteringLiteralComparison.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {



			CVariableBindingFilteringLiteralComparison::CVariableBindingFilteringLiteralComparison(COMPARING_TYPE comparingType, CExpressionVariable* variableExp, CDataLiteralExpression* dataLiteralExp) : CVariableBindingFiltering(CVariableBindingFiltering::LITERAL_COMPARISON) {
				mComparingType = comparingType;
				mVariableExp = variableExp;
				mDataLiteralExp = dataLiteralExp;
				mDataLiteral = nullptr;
			}

			CVariableBindingFilteringLiteralComparison::COMPARING_TYPE CVariableBindingFilteringLiteralComparison::getFilteringType() {
				return mComparingType;
			}

			CExpressionVariable* CVariableBindingFilteringLiteralComparison::getVariableExpression() {
				return mVariableExp;
			}
				
			CDataLiteralExpression* CVariableBindingFilteringLiteralComparison::getDataLiteralExpression() {
				return mDataLiteralExp;
			}

			CDataLiteral* CVariableBindingFilteringLiteralComparison::getDataLiteral() {
				return mDataLiteral;
			}

			CVariableBindingFilteringLiteralComparison* CVariableBindingFilteringLiteralComparison::setDataLiteral(CDataLiteral* dataLiteral) {
				mDataLiteral = dataLiteral;
				return this;
			}


			CVariableBindingFilteringAnswerMapping* CVariableBindingFilteringLiteralComparison::createFitleringAnswerMapping(QHash<CExpressionVariable*, cint64> varPosHash) {
				CVariableBindingFilteringIndexArrayAnswerMapping* indexArrayAnswerMapping = new CVariableBindingFilteringIndexArrayAnswerMapping(1);
				indexArrayAnswerMapping->getVariableIndexArray()[0] = varPosHash.value(mVariableExp, 0);
				return indexArrayAnswerMapping;
			}


			bool CVariableBindingFilteringLiteralComparison::isFiltered(CVariableBindingFilteringAnswerMapping* answeringMapping, CVariableBindingsAnswerResult* bindingsAnswer) {
				CVariableBindingFilteringIndexArrayAnswerMapping* indexArrayAnswerMapping = (CVariableBindingFilteringIndexArrayAnswerMapping*)answeringMapping;
				cint64 varPos = indexArrayAnswerMapping->getVariableIndexArray()[0];
				CVariableBindingsListAnswerResult* listBindingsAnswer = (CVariableBindingsListAnswerResult*)bindingsAnswer;
				CVariableBindingResult* bindingResult = listBindingsAnswer->getResult(varPos);
				if (bindingResult) {
					CVariableBindingStringDataResult* stringDataResult = (CVariableBindingStringDataResult*)bindingResult;
					CDataLiteral* answerDataLiteral = stringDataResult->getDataLiteral();
					CDataLiteralValue* answerDataLiteralValue = answerDataLiteral->getDataLiteralValue();
					CDataLiteralValue* dataLiteralValue = mDataLiteral->getDataLiteralValue();
					if (answerDataLiteralValue && dataLiteralValue) {
						CDataLiteralCompareValue* compDataValue = dynamic_cast<CDataLiteralCompareValue*>(dataLiteralValue);
						CDataLiteralCompareValue* compAnswerDataValue = dynamic_cast<CDataLiteralCompareValue*>(answerDataLiteralValue);
						if (compDataValue && compAnswerDataValue && compDataValue->getDataValueType() == compAnswerDataValue->getDataValueType()) {

							if (mComparingType == LESS) {
								return compAnswerDataValue->isLessEqualThan(compDataValue);
							} else if (mComparingType == LESS_EQUAL) {
								return compAnswerDataValue->isLessEqualThan(compDataValue);
							} else if (mComparingType == EQUAL) {
								return compAnswerDataValue->isEqualTo(compDataValue);
							} else if (mComparingType == UNEQUAL) {
								return compAnswerDataValue->isEqualTo(compDataValue);
							} else if (mComparingType == GREATER_EQUAL) {
								return compAnswerDataValue->isGreaterEqualThan(compDataValue);
							} else if (mComparingType == GREATER) {
								return compAnswerDataValue->isGreaterThan(compDataValue);
							}
						}
					}
				}
				QString filterString;
				if (mDataLiteral) {
					if (mDataLiteral->getDataLiteralValue()) {
						filterString = mDataLiteral->getDataLiteralValue()->getValueString();
					} else {
						filterString = mDataLiteral->getLexicalDataLiteralValueString();
					}
				}
				QString variableBindingString = bindingResult->getQueryResultString();
				int compValue = variableBindingString.compare(filterString);
				if (mComparingType == LESS) {
					return compValue < 0;
				} else if (mComparingType == LESS_EQUAL) {
					return compValue <= 0;
				} else if (mComparingType == EQUAL) {
					return compValue == 0;
				} else if (mComparingType == UNEQUAL) {
					return compValue != 0;
				} else if (mComparingType == GREATER_EQUAL) {
					return compValue >= 0;
				} else if (mComparingType == GREATER) {
					return compValue > 0;
				}
				return false;
			}


			
		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

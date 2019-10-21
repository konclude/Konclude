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

#include "CParsingExpressionSplitter.h"


namespace Konclude {

	namespace Parser {

		CParsingExpressionSplitter::CParsingExpressionSplitter(const CEXPRESSIONLIST<CBuildExpression*>& expressions) : mExpressionList(expressions) {
			splitExpressions();
		}

		CParsingExpressionSplitter::CParsingExpressionSplitter(CEXPRESSIONLIST<CBuildExpression*>* expressions) : mExpressionList(*expressions) {
			splitExpressions();
		}


		CParsingExpressionSplitter::CParsingExpressionSplitter(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
			if (expression1) {
				mExpressionList.append(expression1);
			}
			if (expression2) {
				mExpressionList.append(expression2);
			}
			if (expression3) {
				mExpressionList.append(expression3);
			}
			splitExpressions();
		}

		CParsingExpressionSplitter::~CParsingExpressionSplitter() {
		}

		bool CParsingExpressionSplitter::splitExpressions() {
			FOREACHIT (CBuildExpression* expression, mExpressionList) {
				CClassTermExpression* classExpression = dynamic_cast<CClassTermExpression*>(expression);
				if (classExpression) {
					mClassList.append(classExpression);
					CObjectIndividualVariableExpression* varExpression = dynamic_cast<CObjectIndividualVariableExpression*>(classExpression);
					if (varExpression) {
						mObjIndiVarList.append(varExpression);
					} else {
						mNotObjIndiVarList.append(varExpression);
					}
				}
				CObjectPropertyTermExpression* objectPropertyExpression = dynamic_cast<CObjectPropertyTermExpression*>(expression);
				if (objectPropertyExpression) {
					mObjectPropertyList.append(objectPropertyExpression);
				}
				CIndividualTermExpression* individualExpression = dynamic_cast<CIndividualTermExpression*>(expression);
				if (individualExpression) {
					mIndiList.append(individualExpression);
				}
				CDataPropertyTermExpression* dataPropertyExpression = dynamic_cast<CDataPropertyTermExpression*>(expression);
				if (dataPropertyExpression) {
					mDataPropertyList.append(dataPropertyExpression);
				} 
				CDataRangeTermExpression* dataRangeExpression = dynamic_cast<CDataRangeTermExpression*>(expression);
				if (dataRangeExpression) {
					mDataRangeList.append(dataRangeExpression);
				}
				CDataLiteralTermExpression* dataLitExpression = dynamic_cast<CDataLiteralTermExpression*>(expression);
				if (dataRangeExpression) {
					mDataLitList.append(dataLitExpression);
				}
			}
			return true;
		}


		bool CParsingExpressionSplitter::proofExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions, cint64 requiredIndividualExpressions, cint64 requiredVariableExpressions, cint64 requiredNotVariableExpressions, cint64 dataRangeExpressions, cint64 dataPropertyExpressions) {
			if (mClassList.count() < requiredClassExpressions) {
				LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Class-Expressions, extracted Class-Expressions %2.").arg(requiredClassExpressions).arg(mClassList.count()),this);
				return false;
			}
			if (mObjectPropertyList.count() < requiredObjectPropertyExpressions) {
				LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 ObjectProperty-Expressions, extracted ObjectProperty-Expressions %2.").arg(requiredObjectPropertyExpressions).arg(mObjectPropertyList.count()),this);
				return false;
			}
			if (mIndiList.count() < requiredIndividualExpressions) {
				LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Individual-Expressions, extracted Individual-Expressions %2.").arg(requiredIndividualExpressions).arg(mIndiList.count()),this);
				return false;
			}
			if (mObjIndiVarList.count() < requiredVariableExpressions) {
				LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Variable-Expressions, extracted Variable-Expressions %2.").arg(requiredVariableExpressions).arg(mObjIndiVarList.count()),this);
				return false;
			}
			if (mNotObjIndiVarList.count() < requiredNotVariableExpressions) {
				LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Non-Variable-Expressions, extracted Non-Variable-Expressions %2.").arg(requiredNotVariableExpressions).arg(mNotObjIndiVarList.count()),this);
				return false;
			}
			if (mDataRangeList.count() < dataRangeExpressions) {
				LOG(ERROR, "::Konclude::Reasoner::Generator::ExpressionSplitter", logTr("Couldn't extract minimal required %1 DataRange-Expressions, extracted DataRange-Expressions %2.").arg(dataRangeExpressions).arg(mDataRangeList.count()), this);
				return false;
			}
			if (mDataPropertyList.count() < dataPropertyExpressions) {
				LOG(ERROR, "::Konclude::Reasoner::Generator::ExpressionSplitter", logTr("Couldn't extract minimal required %1 DataProperty-Expressions, extracted DataProperty-Expressions %2.").arg(dataPropertyExpressions).arg(mDataPropertyList.count()), this);
				return false;
			}
			return true;
		}


		CEXPRESSIONLIST<CDataRangeTermExpression*>* CParsingExpressionSplitter::getDataRangeTermExpressionList() {
			return &mDataRangeList;
		}


		CEXPRESSIONLIST<CDataLiteralTermExpression*>* CParsingExpressionSplitter::getDataLiteralTermExpressionList() {
			return &mDataLitList;
		}

		CEXPRESSIONLIST<CDataPropertyTermExpression*>* CParsingExpressionSplitter::getDataPropertyTermExpressionList() {
			return &mDataPropertyList;
		}


		CEXPRESSIONLIST<CClassTermExpression*>* CParsingExpressionSplitter::getClassTermExpressionList() {
			return &mClassList;
		}

		CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CParsingExpressionSplitter::getObjectPropertyTermExpressionList() {
			return &mObjectPropertyList;
		}

		CEXPRESSIONLIST<CIndividualTermExpression*>* CParsingExpressionSplitter::getIndividualTermExpressionList() {
			return &mIndiList;
		}


		CEXPRESSIONLIST<CObjectIndividualVariableExpression*>* CParsingExpressionSplitter::getClassVariableTermExpressionList() {
			return &mObjIndiVarList;
		}

		CEXPRESSIONLIST<CClassTermExpression*>* CParsingExpressionSplitter::getClassNotVariableTermExpressionList() {
			return &mNotObjIndiVarList;
		}


		CObjectIndividualVariableExpression* CParsingExpressionSplitter::getFirstClassVariableTermExpression() {
			return mObjIndiVarList.first();
		}

		CObjectIndividualVariableExpression* CParsingExpressionSplitter::getSecondClassVariableTermExpression() {
			return mObjIndiVarList.at(1);
		}


		CClassTermExpression* CParsingExpressionSplitter::getFirstClassTermExpression() {
			return mClassList.first();
		}

		CClassTermExpression* CParsingExpressionSplitter::getFirstOptionalClassTermExpression() {
			if (mClassList.isEmpty()) {
				return nullptr;
			}
			return mClassList.first();
		}


		CClassTermExpression* CParsingExpressionSplitter::getSecondClassTermExpression() {
			return mClassList.at(1);
		}

		CObjectPropertyTermExpression* CParsingExpressionSplitter::getFirstObjectPropertyTermExpression() {
			return mObjectPropertyList.first();
		}

		CObjectPropertyTermExpression* CParsingExpressionSplitter::getSecondObjectPropertyTermExpression() {
			return mObjectPropertyList.at(1);
		}

		CIndividualTermExpression* CParsingExpressionSplitter::getFirstIndividualTermExpression() {
			return mIndiList.first();
		}

		CIndividualTermExpression* CParsingExpressionSplitter::getSecondIndividualTermExpression() {
			return mIndiList.at(1);
		}

		CDataPropertyTermExpression* CParsingExpressionSplitter::getFirstDataPropertyTermExpression() {
			return mDataPropertyList.first();
		}

		CDataPropertyTermExpression* CParsingExpressionSplitter::getSecondDataPropertyTermExpression() {
			return mDataPropertyList.at(1);
		}


		CDataRangeTermExpression* CParsingExpressionSplitter::getFirstDataRangeTermExpression() {
			return mDataRangeList.first();
		}

		CDataRangeTermExpression* CParsingExpressionSplitter::getSecondDataRangeTermExpression() {
			return mDataRangeList.at(1);
		}

		CDataLiteralTermExpression* CParsingExpressionSplitter::getFirstDataLiteralTermExpression() {
			return mDataLitList.first();
		}

		CDataLiteralTermExpression* CParsingExpressionSplitter::getSecondDataLiteralTermExpression() {
			return mDataLitList.at(1);
		}


	}; // end namespace Parser

}; // end namespace Konclude
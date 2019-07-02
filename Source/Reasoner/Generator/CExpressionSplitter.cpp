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

#include "CExpressionSplitter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CExpressionSplitter::CExpressionSplitter(const CEXPRESSIONLIST<CBuildExpression*>& expressions) : mExpressionList(expressions) {
				splitExpressions();
			}

			CExpressionSplitter::CExpressionSplitter(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
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

			CExpressionSplitter::~CExpressionSplitter() {
			}

			bool CExpressionSplitter::splitExpressions() {
				FOREACHIT (CBuildExpression* expression, mExpressionList) {
					CClassTermExpression* classExpression = dynamic_cast<CClassTermExpression*>(expression);
					if (classExpression) {
						mClassList.append(classExpression);
					}
					CObjectPropertyTermExpression* objectPropertyExpression = dynamic_cast<CObjectPropertyTermExpression*>(expression);
					if (objectPropertyExpression) {
						mObjectPropertyList.append(objectPropertyExpression);
					}
					CIndividualTermExpression* individualExpression = dynamic_cast<CIndividualTermExpression*>(expression);
					if (individualExpression) {
						mIndiList.append(individualExpression);
					}
				}
				return true;
			}


			bool CExpressionSplitter::proofExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions, cint64 requiredIndividualExpressions) {
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
				return true;
			}

			CEXPRESSIONLIST<CClassTermExpression*>* CExpressionSplitter::getClassTermExpressionList() {
				return &mClassList;
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CExpressionSplitter::getObjectPropertyTermExpressionList() {
				return &mObjectPropertyList;
			}

			CEXPRESSIONLIST<CIndividualTermExpression*>* CExpressionSplitter::getIndividualTermExpressionList() {
				return &mIndiList;
			}

			CClassTermExpression* CExpressionSplitter::getFirstClassTermExpression() {
				return mClassList.first();
			}

			CClassTermExpression* CExpressionSplitter::getFirstOptionalClassTermExpression() {
				if (mClassList.isEmpty()) {
					return nullptr;
				}
				return mClassList.first();
			}


			CClassTermExpression* CExpressionSplitter::getSecondClassTermExpression() {
				return mClassList.at(1);
			}

			CObjectPropertyTermExpression* CExpressionSplitter::getFirstObjectPropertyTermExpression() {
				return mObjectPropertyList.first();
			}

			CObjectPropertyTermExpression* CExpressionSplitter::getSecondObjectPropertyTermExpression() {
				return mObjectPropertyList.at(1);
			}

			CIndividualTermExpression* CExpressionSplitter::getFirstIndividualTermExpression() {
				return mIndiList.first();
			}

			CIndividualTermExpression* CExpressionSplitter::getSecondIndividualTermExpression() {
				return mIndiList.at(1);
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
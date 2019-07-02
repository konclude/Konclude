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

#include "CListExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListExpressionAssociator::CListExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2, CBuildExpression *expression3) {
				if (expression1) {
					mExpressionList.append(expression1);
				}
				if (expression2) {
					mExpressionList.append(expression2);
				}
				if (expression3) {
					mExpressionList.append(expression3);
				}
			}

			CListExpressionAssociator::CListExpressionAssociator(const CEXPRESSIONLIST<CBuildExpression*>& expressionList) : mExpressionList(expressionList) {
			}

			CListExpressionAssociator::CListExpressionAssociator(CEXPRESSIONLIST<CBuildExpression*>* expressionList) : mExpressionList(*expressionList) {
			}


			CListExpressionAssociator* CListExpressionAssociator::initExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2, CBuildExpression *expression3) {
				if (expression1) {
					if (mExpressionList.isEmpty()) {
						mExpressionList.append(expression1);
					} else {
						mExpressionList.replace(0,expression1);
					}

					if (expression2) {
						if (mExpressionList.size() < 2) {
							mExpressionList.append(expression2);
						} else {
							mExpressionList.replace(1,expression2);
						}

						if (expression3) {
							if (mExpressionList.size() < 3) {
								mExpressionList.append(expression3);
							} else {
								mExpressionList.replace(2,expression3);
							}
						} else {
							while (mExpressionList.size() > 2) {
								mExpressionList.removeLast();
							}
						}

					} else {
						while (mExpressionList.size() > 1) {
							mExpressionList.removeLast();
						}
					}

				} else {
					mExpressionList.clear();
				}
				return this;
			}


			CEXPRESSIONLIST<CBuildExpression*>* CListExpressionAssociator::getExpressionList() {
				return &mExpressionList;
			}

			const CEXPRESSIONLIST<CBuildExpression*>* CListExpressionAssociator::getConstExpressionList() const {
				return &mExpressionList;
			}


			cint64 CListExpressionAssociator::getExpressionCount() {
				return mExpressionList.count();
			}

			cint64 CListExpressionAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				cint64 tmpValue1 = 0;
				cint64 tmpValue2 = 1;
				FOREACHIT (CBuildExpression* expression, mExpressionList) {
					tmpValue1 += cint64(expression);
					tmpValue2 *= cint64(expression);
				}
				hashValue = tmpValue1*tmpValue2;
				return hashValue;
			}

			bool CListExpressionAssociator::compareStructuralEquivalence(const CListExpressionAssociator& listExpressionAssociator) {
				if (mExpressionList.count() != listExpressionAssociator.mExpressionList.count()) {
					return false;
				}
				FOREACHIT (CBuildExpression* expression, mExpressionList) {
					cint64 containedCount = listExpressionAssociator.mExpressionList.count(expression);
					if (containedCount <= 0) {
						return false;
					} else if (containedCount > 1) {
						if (mExpressionList.count(expression) != containedCount) {
							return false;
						}
					}
				}
				return true;
			}

			bool CListExpressionAssociator::visitSubExpressions(CBuildExpression* expression, CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {		
					for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it = mExpressionList.constBegin(), itEnd = mExpressionList.constEnd(); it != itEnd; ++it) {
						subExpressionVisitor->visitSubExpression(expression,*it,subExpressionVisitor);
					}
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

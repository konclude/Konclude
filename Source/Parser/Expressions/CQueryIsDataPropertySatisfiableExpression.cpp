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

#include "CQueryIsDataPropertySatisfiableExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryIsDataPropertySatisfiableExpression::CQueryIsDataPropertySatisfiableExpression(const QString& name, CDataPropertyTermExpression* dataPropTermExp) 
					: CNameAssociator(name),CDataPropertyTermExpressionAssociator(dataPropTermExp) {
			}


			CQueryIsDataPropertySatisfiableExpression::~CQueryIsDataPropertySatisfiableExpression() {
			}


			CBuildExpression::ExpressionType CQueryIsDataPropertySatisfiableExpression::getType() {
				return BETISDATAPROPERTYSATISFIABLE;
			}


			cint64 CQueryIsDataPropertySatisfiableExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryIsDataPropertySatisfiableExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryIsDataPropertySatisfiableExpression* compExpCast = dynamic_cast<CQueryIsDataPropertySatisfiableExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryIsDataPropertySatisfiableExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

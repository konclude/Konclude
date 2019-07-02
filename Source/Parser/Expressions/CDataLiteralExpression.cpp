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

#include "CDataLiteralExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDataLiteralExpression::CDataLiteralExpression(CDataLexicalValueExpression* dataLexicalValueExpression, CDatatypeExpression* dataTypeExpression) : CDataLexicalValueExpressionAssociator(dataLexicalValueExpression),CDatatypeExpressionAssociator(dataTypeExpression) {
			}


			CDataLiteralExpression::~CDataLiteralExpression() {
			}



			CBuildExpression::ExpressionType CDataLiteralExpression::getType() {
				return BETDATALITERAL;
			}


			cint64 CDataLiteralExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataLexicalValueExpressionAssociator::getStructuralHashValue(),CDatatypeExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDataLiteralExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDataLiteralExpression* compExpCast = dynamic_cast<CDataLiteralExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDatatypeExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataLexicalValueExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}



			bool CDataLiteralExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataLexicalValueExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDatatypeExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

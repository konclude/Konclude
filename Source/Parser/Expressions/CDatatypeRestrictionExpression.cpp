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

#include "CDatatypeRestrictionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDatatypeRestrictionExpression::CDatatypeRestrictionExpression(CDatatypeExpression* datatypeExpression, CDataFacetRestrictionExpression* expression1, CDataFacetRestrictionExpression* expression2) 
					: CDatatypeExpressionAssociator(datatypeExpression), CListDataFacetRestrictionExpressionAssociator(expression1,expression2) {
			}

			CDatatypeRestrictionExpression::CDatatypeRestrictionExpression(CDatatypeExpression* datatypeExpression, const CEXPRESSIONLIST<CDataFacetRestrictionExpression*>& expressionList) 
				: CDatatypeExpressionAssociator(datatypeExpression), CListDataFacetRestrictionExpressionAssociator(expressionList) {
			}


			CDatatypeRestrictionExpression::~CDatatypeRestrictionExpression() {
			}


			CBuildExpression::ExpressionType CDatatypeRestrictionExpression::getType() {
				return BETDATATYPERESTRICTION;
			}


			cint64 CDatatypeRestrictionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDatatypeExpressionAssociator::getStructuralHashValue(),CListDataFacetRestrictionExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDatatypeRestrictionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDatatypeRestrictionExpression* compExpCast = dynamic_cast<CDatatypeRestrictionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDatatypeExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CListDataFacetRestrictionExpressionAssociator::compareStructuralEquivalence(*compExpCast);;
			}

			bool CDatatypeRestrictionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDatatypeExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CListDataFacetRestrictionExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;		
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

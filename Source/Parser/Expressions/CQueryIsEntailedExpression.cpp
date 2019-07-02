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

#include "CQueryIsEntailedExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryIsEntailedExpression::CQueryIsEntailedExpression(const QString& name, const CEXPRESSIONLIST<CAxiomExpression*>& axiomExpList) 
					: CNameAssociator(name),CListAxiomExpressionAssociator(axiomExpList) {
			}


			CQueryIsEntailedExpression::~CQueryIsEntailedExpression() {
			}


			CEXPRESSIONLIST<CEntailmentAxiomExpressionAssociation*>* CQueryIsEntailedExpression::getConstructedTestAxiomExpressions() {
				return &mConstructedAxiomExpList;
			}


			CQueryIsEntailedExpression *CQueryIsEntailedExpression::addConstructedTestAxiomExpressions(CEntailmentAxiomExpressionAssociation *conBuildExp) {
				mConstructedAxiomExpList.append(conBuildExp);
				return this;
			}


			CBuildExpression::ExpressionType CQueryIsEntailedExpression::getType() {
				return BETISENTAILED;
			}


			cint64 CQueryIsEntailedExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListAxiomExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryIsEntailedExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryIsEntailedExpression* compExpCast = dynamic_cast<CQueryIsEntailedExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListAxiomExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryIsEntailedExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListAxiomExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

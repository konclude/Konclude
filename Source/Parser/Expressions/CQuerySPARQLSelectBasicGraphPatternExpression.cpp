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

#include "CQuerySPARQLSelectBasicGraphPatternExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQuerySPARQLSelectBasicGraphPatternExpression::CQuerySPARQLSelectBasicGraphPatternExpression(const QString& name, const QList<CAxiomExpression*>& axiomExpressionList, const QList<CExpressionVariable*>& answerVariables, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinct, cint64 limit, cint64 offset)
					: CQuerySPARQLBasicGraphPatternExpression(name, axiomExpressionList, filteringList), COrderedListVariableAssociator(answerVariables), COrderedListOrderingTermExpressionAssociator(orderingList) {
				mDistinct = distinct;
				mLimit = limit;
				mOffset = offset;
			}


			CQuerySPARQLSelectBasicGraphPatternExpression::CQuerySPARQLSelectBasicGraphPatternExpression(const QString& name, const QList<CAxiomExpression*>& axiomExpressionList, const QList<CExpressionVariable*>& answerVariables, const QList<CExpressionVariable*>& ignoreCardVariables, bool distinct, cint64 limit, cint64 offset)
					: CQuerySPARQLBasicGraphPatternExpression(name, axiomExpressionList, CEXPRESSIONLIST<CFilteringTermExpression*>()), COrderedListVariableAssociator(answerVariables), COrderedListOrderingTermExpressionAssociator(CEXPRESSIONLIST<COrderingTermExpression*>()), mIgnoreCardVarList(ignoreCardVariables) {
				mDistinct = distinct;
				mLimit = limit;
				mOffset = offset;				
			}


			CQuerySPARQLSelectBasicGraphPatternExpression::~CQuerySPARQLSelectBasicGraphPatternExpression() {
			}


			CBuildExpression::ExpressionType CQuerySPARQLSelectBasicGraphPatternExpression::getType() {
				return BETSPARQLSELECTBASICGRAPHPATTERN;
			}


			cint64 CQuerySPARQLSelectBasicGraphPatternExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed(CQuerySPARQLBasicGraphPatternExpression::getStructuralHashValue(), COrderedListVariableAssociator::getStructuralHashValue(), COrderedListOrderingTermExpressionAssociator::getStructuralHashValue(), (cint64)mDistinct);
				return hashValue;
			}

			bool CQuerySPARQLSelectBasicGraphPatternExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQuerySPARQLSelectBasicGraphPatternExpression* compExpCast = dynamic_cast<CQuerySPARQLSelectBasicGraphPatternExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CQuerySPARQLBasicGraphPatternExpression::compareStructuralEquivalence(compExpCast) && COrderedListVariableAssociator::compareStructuralEquivalence(*(COrderedListVariableAssociator*)compExpCast) && COrderedListOrderingTermExpressionAssociator::compareStructuralEquivalence(*(COrderedListOrderingTermExpressionAssociator*)compExpCast) && mIgnoreCardVarList.compareStructuralEquivalence(compExpCast->mIgnoreCardVarList);
			}

			bool CQuerySPARQLSelectBasicGraphPatternExpression::isDistinct() {
				return mDistinct;
			}

			cint64 CQuerySPARQLSelectBasicGraphPatternExpression::getLimit() {
				return mLimit;
			}

			cint64 CQuerySPARQLSelectBasicGraphPatternExpression::getOffset() {
				return mOffset;
			}

			QList<CExpressionVariable*>* CQuerySPARQLSelectBasicGraphPatternExpression::getIgnoreCardinalityVariableList() {
				return mIgnoreCardVarList.getVariableList();
			}


			bool CQuerySPARQLSelectBasicGraphPatternExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CQuerySPARQLBasicGraphPatternExpression::visitSubExpressions(subExpressionVisitor);
					COrderedListOrderingTermExpressionAssociator::visitSubExpressions(this, subExpressionVisitor);
					for (CExpressionVariable* varExp : mVariableList) {
						subExpressionVisitor->visitEntity(this, varExp, subExpressionVisitor);
					}
					return true;
				}
				return false;
			}




		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

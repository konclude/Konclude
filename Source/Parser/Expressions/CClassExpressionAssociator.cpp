/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CClassExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CClassExpressionAssociator::CClassExpressionAssociator(CClassExpression *expression) 
					: CExpressionAssociator(expression) {
			}


			CClassExpressionAssociator::~CClassExpressionAssociator() {
			}

			CClassExpression* CClassExpressionAssociator::getClassExpression() {
				// bad cast to avoid lot of reimplementation 
				return (CClassExpression*)getExpression();
			}

			bool CClassExpressionAssociator::compareStructuralEquivalence(const CClassExpressionAssociator& classExpressionAssociator) {
				return CExpressionAssociator::compareStructuralEquivalence(classExpressionAssociator);
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

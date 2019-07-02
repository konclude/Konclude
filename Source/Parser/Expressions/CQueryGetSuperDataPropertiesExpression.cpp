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

#include "CQueryGetSuperDataPropertiesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetSuperDataPropertiesExpression::CQueryGetSuperDataPropertiesExpression(const QString& name, bool direct, CDataPropertyTermExpression* dataPropTermExp) 
					: CNameAssociator(name),CDataPropertyTermExpressionAssociator(dataPropTermExp),mDirect(direct) {
			}


			CQueryGetSuperDataPropertiesExpression::~CQueryGetSuperDataPropertiesExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetSuperDataPropertiesExpression::getType() {
				return BETGETSUPERDATAPROPERTIES;
			}


			cint64 CQueryGetSuperDataPropertiesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),(cint64)mDirect);
				return hashValue;
			}

			bool CQueryGetSuperDataPropertiesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetSuperDataPropertiesExpression* compExpCast = dynamic_cast<CQueryGetSuperDataPropertiesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				if (compExpCast->mDirect != mDirect) {
					return false;
				}
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryGetSuperDataPropertiesExpression::isDirect() {
				return mDirect;
			}

			bool CQueryGetSuperDataPropertiesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

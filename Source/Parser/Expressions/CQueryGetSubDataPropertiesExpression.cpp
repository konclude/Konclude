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

#include "CQueryGetSubDataPropertiesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetSubDataPropertiesExpression::CQueryGetSubDataPropertiesExpression(const QString& name, bool direct, CDataPropertyTermExpression* dataPropTermExp) 
					: CNameAssociator(name),CDataPropertyTermExpressionAssociator(dataPropTermExp),mDirect(direct) {
			}


			CQueryGetSubDataPropertiesExpression::~CQueryGetSubDataPropertiesExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetSubDataPropertiesExpression::getType() {
				return BETGETSUBDATAPROPERTIES;
			}


			cint64 CQueryGetSubDataPropertiesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),(cint64)mDirect);
				return hashValue;
			}

			bool CQueryGetSubDataPropertiesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetSubDataPropertiesExpression* compExpCast = dynamic_cast<CQueryGetSubDataPropertiesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				if (compExpCast->mDirect != mDirect) {
					return false;
				}
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryGetSubDataPropertiesExpression::isDirect() {
				return mDirect;
			}

			bool CQueryGetSubDataPropertiesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

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

#include "CQueryGetDataPropertyFactsExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetDataPropertyFactsExpression::CQueryGetDataPropertyFactsExpression(const QString& name, DATAPROPERTYFACTTYPE factType, CIndividualTermExpression* indiTermExp, CDataPropertyTermExpression* dataPropTermExp, CDataLiteralTermExpression* dataLiteralTermExp, bool negative)
					: CNameAssociator(name),CIndividualTermExpressionAssociator(indiTermExp),CDataPropertyTermExpressionAssociator(dataPropTermExp),CDataLiteralTermExpressionAssociator(dataLiteralTermExp) {
				mFactType = factType;
				mNegative = negative;
			}


			CQueryGetDataPropertyFactsExpression::~CQueryGetDataPropertyFactsExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetDataPropertyFactsExpression::getType() {
				switch (mFactType) {
					case DPFTDATAPROPERTIESOFSOURCE: return BETGETDATAPROPERTIESOFSOURCE;
					case DPFTDATAPROPERTIESBETWEEN: return BETGETDATAPROPERTIESBETWEEN;
					case DPFTDATAPROPERTIESOFLITERAL: return BETGETDATAPROPERTIESOFLITERAL;
					case DPFTDATAPROPERTYSOURCES: return BETGETDATAPROPERTYSOURCES;
					case DPFTFLATTENEDDATAPROPERTYSOURCES: return BETGETFLATTENEDDATAPROPERTYSOURCES;
				}
				return BETGETDATAPROPERTIESOFSOURCE;
			}


			CQueryGetDataPropertyFactsExpression::DATAPROPERTYFACTTYPE CQueryGetDataPropertyFactsExpression::getDataPropertyFactType() {
				return mFactType;
			}


			bool CQueryGetDataPropertyFactsExpression::isNegative() {
				return mNegative;
			}


			QString CQueryGetDataPropertyFactsExpression::getDataPropertyFactTypeString(DATAPROPERTYFACTTYPE factType) {
				switch (factType) {
					case DPFTDATAPROPERTIESOFSOURCE: return QString("GetDataPropertiesOfSource");
					case DPFTDATAPROPERTIESBETWEEN: return QString("GetDataPropertiesBetween");
					case DPFTDATAPROPERTIESOFLITERAL: return QString("GetDataPropertiesOfLiteral");
					case DPFTDATAPROPERTYSOURCES: return QString("GetDataPropertySources");
					case DPFTFLATTENEDDATAPROPERTYSOURCES: return QString("GetFlattenedDataPropertySources");
				}
				return QString("UnknownDataPropertyFactRequest");
			}


			cint64 CQueryGetDataPropertyFactsExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				hashValue = CExpressionHashMixer::getHashValueMixed(hashValue,CDataPropertyTermExpressionAssociator::getStructuralHashValue());
				hashValue = CExpressionHashMixer::getHashValueMixed(hashValue,CDataLiteralTermExpressionAssociator::getStructuralHashValue());
				hashValue = CExpressionHashMixer::getHashValueMixed(hashValue,mNegative ? 1 : 0);
				return hashValue;
			}

			bool CQueryGetDataPropertyFactsExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetDataPropertyFactsExpression* compExpCast = dynamic_cast<CQueryGetDataPropertyFactsExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				}
				return CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataLiteralTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && mNegative == compExpCast->mNegative;
			}


			bool CQueryGetDataPropertyFactsExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataLiteralTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

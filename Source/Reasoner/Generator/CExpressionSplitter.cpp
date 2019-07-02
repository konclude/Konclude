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

#include "CExpressionSplitter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CExpressionSplitter::CExpressionSplitter(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				splitExpressions(expressions);
			}

			CExpressionSplitter::CExpressionSplitter(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) {
				if (expression1) {
					addSplittedExpression(expression1);
				}
				if (expression2) {
					addSplittedExpression(expression2);
				}
				if (expression3) {
					addSplittedExpression(expression3);
				}
			}


			bool CExpressionSplitter::splitExpressions(const CEXPRESSIONLIST<CBuildExpression*>& expressions) {
				bool allSplitted = true;
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it = expressions.constBegin(), itEnd = expressions.constEnd(); it != itEnd; ++it) {
					CBuildExpression* expression(*it);
					allSplitted &= addSplittedExpression(expression);
				}
				return allSplitted;
			}



			bool CExpressionSplitter::addSplittedExpression(CBuildExpression* expression) {
				CClassTermExpression* classExpression = dynamic_cast<CClassTermExpression*>(expression);
				if (classExpression) {
					mClassList.append(classExpression);
					return true;
				} else {
					CObjectPropertyTermExpression* objectPropertyExpression = dynamic_cast<CObjectPropertyTermExpression*>(expression);
					if (objectPropertyExpression) {
						mObjectPropertyList.append(objectPropertyExpression);
						return true;
					} else {
						CIndividualTermExpression* individualExpression = dynamic_cast<CIndividualTermExpression*>(expression);
						if (individualExpression) {
							mIndiList.append(individualExpression);
							return true;
						} else {
							CDataPropertyTermExpression* dataPropertyExpression = dynamic_cast<CDataPropertyTermExpression*>(expression);
							if (dataPropertyExpression) {
								mDataPropertyList.append(dataPropertyExpression);
								return true;
							} else {
								CDataRangeTermExpression* dataRangeExpression = dynamic_cast<CDataRangeTermExpression*>(expression);
								if (dataRangeExpression) {
									mDataRangeList.append(dataRangeExpression);
									CDataLiteralExpression* dataLiteralExpression = dynamic_cast<CDataLiteralExpression*>(expression);
									if (dataLiteralExpression) {
										mDataLiteralList.append(dataLiteralExpression);
									} else {
										CDatatypeExpression* datatypeExpression = dynamic_cast<CDatatypeExpression*>(expression);
										if (datatypeExpression) {
											mDatatypeList.append(datatypeExpression);
										} 
									}
									return true;
								} else {
									CDataFacetRestrictionExpression* dataFacetRestExpression = dynamic_cast<CDataFacetRestrictionExpression*>(expression);
									if (dataFacetRestExpression) {
										mDataFacetRestrictionList.append(dataFacetRestExpression);
										return true;
									} else {
										CDataFacetExpression* dataFacetExpression = dynamic_cast<CDataFacetExpression*>(expression);
										if (dataFacetExpression) {
											mDataFacetList.append(dataFacetExpression);
											return true;
										}
									}
								}
							}
						}
					}
				}
				return false;
			}



			bool CExpressionSplitter::testForClassExpressions(cint64 requiredClassExpressions) {
				if (mClassList.count() < requiredClassExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForObjectPropertyExpressions(cint64 requiredObjectPropertyExpressions) {
				if (mObjectPropertyList.count() < requiredObjectPropertyExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForIndividualExpressions(cint64 requiredIndividualExpressions) {
				if (mIndiList.count() < requiredIndividualExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForDataPropertyExpressions(cint64 dataPropertyExpressions) {				
				if (mDataPropertyList.count() < dataPropertyExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForDataRangeExpressions(cint64 dataRangeExpressions) {
				if (mDataRangeList.count() < dataRangeExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForDataLiteralExpressions(cint64 literalExpressions) {
				if (mDataLiteralList.count() < literalExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForDataFacetRestrictionExpressions(cint64 dataFacetRestrictionExpressions) {
				if (mDataFacetRestrictionList.count() < dataFacetRestrictionExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForDataFacetExpressions(cint64 dataFacetExpressions) {
				if (mDataFacetList.count() < dataFacetExpressions) {
					return false;
				}
				return true;
			}

			bool CExpressionSplitter::testForExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions, cint64 requiredIndividualExpressions, cint64 dataRangeExpressions, cint64 dataPropertyExpressions, cint64 dataLiteralExpressions, cint64 datatypeExpressions, cint64 dataFacetRestrictionExpressions, cint64 dataFacetExpressions) {
				if (mClassList.count() < requiredClassExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Class-Expressions, extracted Class-Expressions %2.").arg(requiredClassExpressions).arg(mClassList.count()),this);
					return false;
				}
				if (mObjectPropertyList.count() < requiredObjectPropertyExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 ObjectProperty-Expressions, extracted ObjectProperty-Expressions %2.").arg(requiredObjectPropertyExpressions).arg(mObjectPropertyList.count()),this);
					return false;
				}
				if (mIndiList.count() < requiredIndividualExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Individual-Expressions, extracted Individual-Expressions %2.").arg(requiredIndividualExpressions).arg(mIndiList.count()),this);
					return false;
				}
				if (mDataRangeList.count() < dataRangeExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 DataRange-Expressions, extracted DataRange-Expressions %2.").arg(dataRangeExpressions).arg(mDataRangeList.count()),this);
					return false;
				}
				if (mDataPropertyList.count() < dataPropertyExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 DataProperty-Expressions, extracted DataProperty-Expressions %2.").arg(dataPropertyExpressions).arg(mDataPropertyList.count()),this);
					return false;
				}
				if (mDataLiteralList.count() < dataLiteralExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 DataLiteral-Expressions, extracted DataLiteral-Expressions %2.").arg(dataLiteralExpressions).arg(mDataLiteralList.count()),this);
					return false;
				}
				if (mDatatypeList.count() < datatypeExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 Datatype-Expressions, extracted Datatype-Expressions %2.").arg(datatypeExpressions).arg(mDatatypeList.count()),this);
					return false;
				}
				if (mDataFacetRestrictionList.count() < dataFacetRestrictionExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 DataFacetRestriction-Expressions, extracted DataFacetRestriction-Expressions %2.").arg(dataFacetRestrictionExpressions).arg(mDataFacetRestrictionList.count()),this);
					return false;
				}
				if (mDataFacetList.count() < dataFacetExpressions) {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ExpressionSplitter",logTr("Couldn't extract minimal required %1 DataFacet-Expressions, extracted DataFacet-Expressions %2.").arg(dataFacetExpressions).arg(mDataFacetList.count()),this);
					return false;
				}
				return true;
			}



			CEXPRESSIONLIST<CClassTermExpression*>* CExpressionSplitter::getClassTermExpressionList() {
				return &mClassList;
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CExpressionSplitter::getObjectPropertyTermExpressionList() {
				return &mObjectPropertyList;
			}

			CEXPRESSIONLIST<CIndividualTermExpression*>* CExpressionSplitter::getIndividualTermExpressionList() {
				return &mIndiList;
			}

			CEXPRESSIONLIST<CDataRangeTermExpression*>* CExpressionSplitter::getDataRangeTermExpressionList() {
				return &mDataRangeList;
			}

			CEXPRESSIONLIST<CDataLiteralExpression*>* CExpressionSplitter::getDataLiteralExpressionList() {
				return &mDataLiteralList;
			}


			CEXPRESSIONLIST<CDatatypeExpression*>* CExpressionSplitter::getDatatypeExpressionList() {
				return &mDatatypeList;
			}

			CEXPRESSIONLIST<CDataPropertyTermExpression*>* CExpressionSplitter::getDataPropertyTermExpressionList() {
				return &mDataPropertyList;
			}


			CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* CExpressionSplitter::getDataFacetRestrictionList() {
				return &mDataFacetRestrictionList;
			}

			CEXPRESSIONLIST<CDataFacetExpression*>* CExpressionSplitter::getDataFacetList() {
				return &mDataFacetList;
			}


			CClassTermExpression* CExpressionSplitter::getFirstClassTermExpression() {
				return mClassList.first();
			}

			CClassTermExpression* CExpressionSplitter::getFirstOptionalClassTermExpression() {
				if (mClassList.isEmpty()) {
					return nullptr;
				}
				return mClassList.first();
			}

			CDataRangeTermExpression* CExpressionSplitter::getFirstOptionalDataRangeTermExpression() {
				if (mDataRangeList.isEmpty()) {
					return nullptr;
				}
				return mDataRangeList.first();
			}

			CClassTermExpression* CExpressionSplitter::getSecondClassTermExpression() {
				return mClassList.at(1);
			}

			CObjectPropertyTermExpression* CExpressionSplitter::getFirstObjectPropertyTermExpression() {
				return mObjectPropertyList.first();
			}

			CObjectPropertyTermExpression* CExpressionSplitter::getSecondObjectPropertyTermExpression() {
				return mObjectPropertyList.at(1);
			}

			CIndividualTermExpression* CExpressionSplitter::getFirstIndividualTermExpression() {
				return mIndiList.first();
			}

			CIndividualTermExpression* CExpressionSplitter::getSecondIndividualTermExpression() {
				return mIndiList.at(1);
			}


			CDataPropertyTermExpression* CExpressionSplitter::getFirstDataPropertyTermExpression() {
				return mDataPropertyList.first();
			}

			CDataPropertyTermExpression* CExpressionSplitter::getSecondDataPropertyTermExpression() {
				return mDataPropertyList.at(1);
			}


			CDataRangeTermExpression* CExpressionSplitter::getFirstDataRangeTermExpression() {
				return mDataRangeList.first();
			}

			CDataRangeTermExpression* CExpressionSplitter::getSecondDataRangeTermExpression() {
				return mDataRangeList.at(1);
			}


			CDataLiteralExpression* CExpressionSplitter::getFirstDataLiteralExpression() {
				return mDataLiteralList.first();
			}

			CDataLiteralExpression* CExpressionSplitter::getSecondDataLiteralExpression() {
				return mDataLiteralList.at(1);
			}


			CDatatypeExpression* CExpressionSplitter::getFirstDatatypeExpression() {
				return mDatatypeList.first();
			}

			CDatatypeExpression* CExpressionSplitter::getSecondDatatypeExpression() {
				return mDatatypeList.at(1);
			}


			CDataFacetRestrictionExpression* CExpressionSplitter::getFirstDataFacetRestrictionExpression() {
				return mDataFacetRestrictionList.first();
			}

			CDataFacetRestrictionExpression* CExpressionSplitter::getSecondDataFacetRestrictionExpression() {
				return mDataFacetRestrictionList.at(1);
			}

			CDataFacetExpression* CExpressionSplitter::getFirstDataFacetExpression() {
				return mDataFacetList.first();
			}

			CDataFacetExpression* CExpressionSplitter::getSecondDataFacetExpression() {
				return mDataFacetList.at(1);
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
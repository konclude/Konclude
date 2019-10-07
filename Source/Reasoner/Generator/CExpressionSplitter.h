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

#ifndef KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H
#define KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H


// Libraries includes
#include <QList>


// Namespace includes


// Other includes
#include "Utilities/CNegLinker.hpp"

#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataRangeTermExpression.h"
#include "Parser/Expressions/CDataLiteralTermExpression.h"
#include "Parser/Expressions/CDatatypeExpression.h"
#include "Parser/Expressions/CDataFacetRestrictionExpression.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Generator {

			/*! 
			 *
			 *		\class		CExpressionSplitter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CExpressionSplitter  {
				// public methods
				public:
					//! Constructor
					CExpressionSplitter(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					CExpressionSplitter(CBuildExpression* expression1 = nullptr, CBuildExpression* expression2 = nullptr, CBuildExpression* expression3 = nullptr);


					bool testForExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions = 0, cint64 requiredIndividualExpressions = 0, cint64 dataRangeExpressions = 0, cint64 dataPropertyExpressions = 0, cint64 dataLiteralExpressions = 0, cint64 datatypeExpressions = 0, cint64 dataFacetRestrictionExpressions = 0, cint64 dataFacetExpressions = 0);

					bool testForClassExpressions(cint64 requiredClassExpressions);
					bool testForObjectPropertyExpressions(cint64 requiredObjectPropertyExpressions);
					bool testForIndividualExpressions(cint64 requiredIndividualExpressions);
					bool testForDataPropertyExpressions(cint64 dataPropertyExpressions);
					bool testForDataRangeExpressions(cint64 dataRangeExpressions);
					bool testForDataLiteralTermExpressions(cint64 literalExpressions);
					bool testForDatatypeExpressions(cint64 datatypeExpressions);
					bool testForDataFacetRestrictionExpressions(cint64 dataFacetRestrictionExpressions);
					bool testForDataFacetExpressions(cint64 dataFacetExpressions);

					CEXPRESSIONLIST<CClassTermExpression*>* getClassTermExpressionList();
					CEXPRESSIONLIST<CObjectPropertyTermExpression*>* getObjectPropertyTermExpressionList();
					CEXPRESSIONLIST<CDataPropertyTermExpression*>* getDataPropertyTermExpressionList();
					CEXPRESSIONLIST<CDataRangeTermExpression*>* getDataRangeTermExpressionList();
					CEXPRESSIONLIST<CIndividualTermExpression*>* getIndividualTermExpressionList();
					CEXPRESSIONLIST<CDataLiteralTermExpression*>* getDataLiteralTermExpressionList();
					CEXPRESSIONLIST<CDatatypeExpression*>* getDatatypeExpressionList();
					CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* getDataFacetRestrictionList();
					CEXPRESSIONLIST<CDataFacetExpression*>* getDataFacetList();

					CClassTermExpression* getFirstClassTermExpression();
					CClassTermExpression* getFirstOptionalClassTermExpression();
					CClassTermExpression* getSecondClassTermExpression();

					CObjectPropertyTermExpression* getFirstObjectPropertyTermExpression();
					CObjectPropertyTermExpression* getSecondObjectPropertyTermExpression();

					CDataPropertyTermExpression* getFirstDataPropertyTermExpression();
					CDataPropertyTermExpression* getSecondDataPropertyTermExpression();


					CDataRangeTermExpression* getFirstDataRangeTermExpression();
					CDataRangeTermExpression* getFirstOptionalDataRangeTermExpression();
					CDataRangeTermExpression* getSecondDataRangeTermExpression();


					CDataLiteralTermExpression* getFirstDataLiteralTermExpression();
					CDataLiteralTermExpression* getSecondDataLiteralTermExpression();

					CDatatypeExpression* getFirstDatatypeExpression();
					CDatatypeExpression* getSecondDatatypeExpression();

					CDataFacetRestrictionExpression* getFirstDataFacetRestrictionExpression();
					CDataFacetRestrictionExpression* getSecondDataFacetRestrictionExpression();

					CDataFacetExpression* getFirstDataFacetExpression();
					CDataFacetExpression* getSecondDataFacetExpression();

					CIndividualTermExpression* getFirstIndividualTermExpression();
					CIndividualTermExpression* getSecondIndividualTermExpression();


				// protected methods
				protected:
					bool splitExpressions(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
					bool addSplittedExpression(CBuildExpression* expression);

				// protected variables
				protected:
					CEXPRESSIONLIST<CClassTermExpression*> mClassList;
					CEXPRESSIONLIST<CObjectPropertyTermExpression*> mObjectPropertyList;
					CEXPRESSIONLIST<CIndividualTermExpression*> mIndiList;
					CEXPRESSIONLIST<CDataPropertyTermExpression*> mDataPropertyList;
					CEXPRESSIONLIST<CDataRangeTermExpression*> mDataRangeList;
					CEXPRESSIONLIST<CDataLiteralTermExpression*> mDataLiteralTermList;
					CEXPRESSIONLIST<CDatatypeExpression*> mDatatypeList;
					CEXPRESSIONLIST<CDataFacetRestrictionExpression*> mDataFacetRestrictionList;
					CEXPRESSIONLIST<CDataFacetExpression*> mDataFacetList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H

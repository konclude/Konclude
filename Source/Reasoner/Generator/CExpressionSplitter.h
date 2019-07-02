/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H
#define KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes


// Other includes
#include "Utilities/CTrible.hpp"
#include "Utilities/CNegLinker.hpp"
#include "Utilities/CHashableIntConvertedNTuble.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"

#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"

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
					//! Destructor
					virtual ~CExpressionSplitter();

					bool proofExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions = 0, cint64 requiredIndividualExpressions = 0);

					CEXPRESSIONLIST<CClassTermExpression*>* getClassTermExpressionList();
					CEXPRESSIONLIST<CObjectPropertyTermExpression*>* getObjectPropertyTermExpressionList();
					CEXPRESSIONLIST<CIndividualTermExpression*>* getIndividualTermExpressionList();

					CClassTermExpression* getFirstClassTermExpression();
					CClassTermExpression* getFirstOptionalClassTermExpression();
					CClassTermExpression* getSecondClassTermExpression();

					CObjectPropertyTermExpression* getFirstObjectPropertyTermExpression();
					CObjectPropertyTermExpression* getSecondObjectPropertyTermExpression();

					CIndividualTermExpression* getFirstIndividualTermExpression();
					CIndividualTermExpression* getSecondIndividualTermExpression();


				// protected methods
				protected:
					bool splitExpressions();

				// protected variables
				protected:
					CEXPRESSIONLIST<CBuildExpression*> mExpressionList;
					CEXPRESSIONLIST<CClassTermExpression*> mClassList;
					CEXPRESSIONLIST<CObjectPropertyTermExpression*> mObjectPropertyList;
					CEXPRESSIONLIST<CIndividualTermExpression*> mIndiList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CEXPRESSIONSPLITTER_H

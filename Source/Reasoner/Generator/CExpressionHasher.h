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

#ifndef KONCLUDE_REASONER_GENERATOR_CEXPRESSIONHASHER_H
#define KONCLUDE_REASONER_GENERATOR_CEXPRESSIONHASHER_H


// Libraries includes
#include <QHash>

// Namespace includes


// Other includes
#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Generator {


			/*! 
			 *
			 *		\class		CExpressionHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CExpressionHasher  {
				// public methods
				public:
					//! Constructor
					CExpressionHasher(CBuildExpression* expression);

					cint64 getExpressionHashValue() const;

					bool operator==(const CExpressionHasher& expressionHasher) const;

				// protected methods
				protected:

				// protected variables
				protected:
					CBuildExpression* mExpression;
					cint64 mHashValue;

				// private methods
				private:

				// private variables
				private:

			};

			//inline bool operator==(const CExpressionHasher& expressionHasher1, const CExpressionHasher& expressionHasher2) {
			//	return expressionHasher1.operator==(expressionHasher2);
			//}

			inline uint qHash(const CExpressionHasher& expressionHasher) {
				qint64 key = (qint64)expressionHasher.getExpressionHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CEXPRESSIONHASHER_H

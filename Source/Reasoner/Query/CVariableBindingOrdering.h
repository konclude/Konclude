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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGORDERING_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGORDERING_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Parser/Expressions/CExpressionVariable.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;
	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingOrdering
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingOrdering : public CLinkerBase<CExpressionVariable*,CVariableBindingOrdering> {
				// public methods
				public:

					enum ORDERING_TYPE { ASC, DESC };

					//! Constructor
					CVariableBindingOrdering();
					CVariableBindingOrdering(CExpressionVariable* variableExpression, ORDERING_TYPE orderingType);


					CVariableBindingOrdering* initOrdering(CExpressionVariable* variableExpression, ORDERING_TYPE orderingType);


					CExpressionVariable* getOrderingVariableExpression();
					ORDERING_TYPE getOrderingType();

				// protected methods
				protected:

				// protected variables
				protected:
					ORDERING_TYPE mOrderingType;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGORDERING_H

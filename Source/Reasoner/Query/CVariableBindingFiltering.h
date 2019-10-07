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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERING_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERING_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CVariableBindingsAnswerResult.h"
#include "CVariableBindingFilteringAnswerMapping.h"


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
			 *		\class		CVariableBindingFiltering
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingFiltering : public CLinkerBase<CVariableBindingFiltering*,CVariableBindingFiltering> {
				// public methods
				public:

					enum FILTERING_TYPE { LITERAL_COMPARISON };

					//! Constructor
					CVariableBindingFiltering(FILTERING_TYPE filteringType);

					FILTERING_TYPE getFilteringType();

					virtual CVariableBindingFilteringAnswerMapping* createFitleringAnswerMapping(QHash<CExpressionVariable*,cint64> varPosHash) = 0;
					virtual bool isFiltered(CVariableBindingFilteringAnswerMapping* answeringMapping, CVariableBindingsAnswerResult* bindingsAnswer) = 0;

				// protected methods
				protected:

				// protected variables
				protected:
					FILTERING_TYPE mFilteringType;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERING_H

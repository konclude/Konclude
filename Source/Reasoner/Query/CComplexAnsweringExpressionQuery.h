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

#ifndef KONCLUDE_REASONER_QUERY_CCOMPLEXANSWERINGEXPRESSIONQUERY_H
#define KONCLUDE_REASONER_QUERY_CCOMPLEXANSWERINGEXPRESSIONQUERY_H

// Libraries includes


// Namespace includes
#include "CComplexAnsweringQuery.h"
#include "CVariableBindingOrdering.h"
#include "CVariableBindingFiltering.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CComplexAnsweringExpressionQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexAnsweringExpressionQuery : public CComplexAnsweringQuery {
				// public methods
				public:
					//! Constructor
					CComplexAnsweringExpressionQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CConfigurationBase* configuration, const QString& queryName);

					//! Destructor
					virtual ~CComplexAnsweringExpressionQuery();




					CVariableBindingOrdering* getResultOrderingLinker();
					CComplexAnsweringExpressionQuery* setResultOrderingLinker(CVariableBindingOrdering* odering);
					CComplexAnsweringExpressionQuery* addResultOrdering(CVariableBindingOrdering* odering);

					CVariableBindingFiltering* getResultFilteringLinker();
					CComplexAnsweringExpressionQuery* setResultFilteringLinker(CVariableBindingFiltering* filtering);
					CComplexAnsweringExpressionQuery* addResultFiltering(CVariableBindingFiltering* filtering);


				// protected methods
				protected:

				// protected variables
				protected:


					CVariableBindingOrdering* mResultOrdering;
					CVariableBindingFiltering* mResultFiltering;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CCOMPLEXANSWERINGEXPRESSIONQUERY_H

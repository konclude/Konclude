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

#include "CComplexAnsweringExpressionQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexAnsweringExpressionQuery::CComplexAnsweringExpressionQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CConfigurationBase* configuration, const QString& queryName)
					: CComplexAnsweringQuery(baseOntology, expressionOntology, configuration, queryName) {
				mResultOrdering = nullptr;
				mResultFiltering = nullptr;
			}

			CComplexAnsweringExpressionQuery::~CComplexAnsweringExpressionQuery() {
			}
			
			

			CVariableBindingOrdering* CComplexAnsweringExpressionQuery::getResultOrderingLinker() {
				return mResultOrdering;
			}

			CComplexAnsweringExpressionQuery* CComplexAnsweringExpressionQuery::setResultOrderingLinker(CVariableBindingOrdering* odering) {
				while (mResultOrdering) {
					CVariableBindingOrdering* tmpOrdering = mResultOrdering;
					mResultOrdering = mResultOrdering->getNext();
					delete tmpOrdering;
				}
				mResultOrdering = odering;
				return this;
			}

			CComplexAnsweringExpressionQuery* CComplexAnsweringExpressionQuery::addResultOrdering(CVariableBindingOrdering* odering) {
				if (mResultOrdering) {
					mResultOrdering->append(odering);
				} else {
					mResultOrdering = odering;
				}
				return this;
			}


			CVariableBindingFiltering* CComplexAnsweringExpressionQuery::getResultFilteringLinker() {
				return mResultFiltering;
			}

			CComplexAnsweringExpressionQuery* CComplexAnsweringExpressionQuery::setResultFilteringLinker(CVariableBindingFiltering* filtering) {
				while (mResultFiltering) {
					CVariableBindingFiltering* tmpOrdering = mResultFiltering;
					mResultFiltering = mResultFiltering->getNext();
					delete tmpOrdering;
				}
				mResultFiltering = filtering;
				return this;
			}

			CComplexAnsweringExpressionQuery* CComplexAnsweringExpressionQuery::addResultFiltering(CVariableBindingFiltering* filtering) {
				if (mResultFiltering) {
					mResultFiltering->append(filtering);
				} else {
					mResultFiltering = filtering;
				}
				return this;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

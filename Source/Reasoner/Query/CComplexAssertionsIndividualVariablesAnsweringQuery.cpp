/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CComplexAssertionsIndividualVariablesAnsweringQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexAssertionsIndividualVariablesAnsweringQuery::CComplexAssertionsIndividualVariablesAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QList<CAxiomExpression*>& axiomExpressions, const QList<CExpressionVariable*>& disVariables, CConfigurationBase* configuration, const QString& queryName) : CComplexAnsweringQuery(baseOntology,expressionOntology,configuration,queryName) {
				mAxiomExpressions = axiomExpressions;
				mDisVariables = disVariables;
				mDistinct = false;
				mBooleanEntailmentResult = false;
				mResultLimit = -1;
				mResultOffset = 0;
			}

			CComplexAssertionsIndividualVariablesAnsweringQuery::~CComplexAssertionsIndividualVariablesAnsweringQuery() {
			}

			QList<CAxiomExpression*>* CComplexAssertionsIndividualVariablesAnsweringQuery::getAxiomExpressions() {
				return &mAxiomExpressions;
			}


			bool CComplexAssertionsIndividualVariablesAnsweringQuery::isDistinctRequired() {
				return mDistinct;
			}

			CComplexAssertionsIndividualVariablesAnsweringQuery* CComplexAssertionsIndividualVariablesAnsweringQuery::setDistinctRequired(bool distinctRequired) {
				mDistinct = distinctRequired;
				return this;
			}


			QList<CExpressionVariable*>* CComplexAssertionsIndividualVariablesAnsweringQuery::getDistinguishedVariableExpressions() {
				return &mDisVariables;
			}

			bool CComplexAssertionsIndividualVariablesAnsweringQuery::isBooleanEntailmentResultRequired() {
				return mBooleanEntailmentResult;
			}

			CComplexAssertionsIndividualVariablesAnsweringQuery* CComplexAssertionsIndividualVariablesAnsweringQuery::setBooleanEntailmentResultRequired(bool booleanEntailmentResultRequired) {
				mBooleanEntailmentResult = booleanEntailmentResultRequired;
				return this;
			}

			cint64 CComplexAssertionsIndividualVariablesAnsweringQuery::getResultLimit() {
				return mResultLimit;
			}

			CComplexAssertionsIndividualVariablesAnsweringQuery* CComplexAssertionsIndividualVariablesAnsweringQuery::setResultLimit(cint64 resultLimit) {
				mResultLimit = resultLimit;
				return this;
			}

			cint64 CComplexAssertionsIndividualVariablesAnsweringQuery::getResultOffset() {
				return mResultOffset;
			}

			CComplexAssertionsIndividualVariablesAnsweringQuery* CComplexAssertionsIndividualVariablesAnsweringQuery::setResultOffset(cint64 resultOffset) {
				mResultOffset = resultOffset;
				return this;
			}

			cint64 CComplexAssertionsIndividualVariablesAnsweringQuery::getResultLimitIncludingOffset() {
				if (mResultLimit == -1) {
					return mResultLimit;
				}
				return mResultOffset + mResultLimit;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

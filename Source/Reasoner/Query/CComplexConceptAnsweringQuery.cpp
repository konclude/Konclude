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

#include "CComplexConceptAnsweringQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexConceptAnsweringQuery::CComplexConceptAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CConfigurationBase* configuration, const QString& queryName) : CComplexAnsweringExpressionQuery(baseOntology,expressionOntology,configuration,queryName) {
				mExpressionOfInterest = nullptr;
				mSuperClassNodesRequired = false;
				mSubClassNodesRequired = false;
				mEquivalentClassNodesRequired = false;
				mInstancesRequired = false;
				mSatisfiableRequired = false;
				mFlattened = false;
				mDirect = false;
			}

			CComplexConceptAnsweringQuery::~CComplexConceptAnsweringQuery() {
			}

			bool CComplexConceptAnsweringQuery::isSuperClassNodesComputationRequired() {
				return mSuperClassNodesRequired;
			}

			bool CComplexConceptAnsweringQuery::isSubClassNodesComputationRequired() {
				return mSubClassNodesRequired;
			}

			bool CComplexConceptAnsweringQuery::isEquivalentClassNodesComputationRequired() {
				return mEquivalentClassNodesRequired;
			}

			bool CComplexConceptAnsweringQuery::isInstancesComputationRequired() {
				return mInstancesRequired;
			}

			bool CComplexConceptAnsweringQuery::isSatisfiableComputationRequired() {
				return mSatisfiableRequired;
			}


			bool CComplexConceptAnsweringQuery::isDirect() {
				return mDirect;
			}

			bool CComplexConceptAnsweringQuery::isFlattened() {
				return mFlattened;
			}

			CComplexConceptAnsweringQuery* CComplexConceptAnsweringQuery::setDirect(bool direct) {
				mDirect = direct;
				return this;
			}

			CComplexConceptAnsweringQuery* CComplexConceptAnsweringQuery::setFlattened(bool flattened) {
				mFlattened = flattened;
				return this;
			}


			CBuildExpression* CComplexConceptAnsweringQuery::getExpressionOfInterest() {
				return mExpressionOfInterest;
			}




		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

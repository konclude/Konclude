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

#include "COntologyBuildExpressionCounter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyBuildExpressionCounter::COntologyBuildExpressionCounter(COntologyContext* ontoContext) {
				mOntoContext = ontoContext;

				mConsistencyProblematicExpressionsChecked = false;
				mConsistencyProblematicExpressions = true;

				mExpCountVec = CObjectAllocator< cint64 >::allocateAndConstructArray(mOntoContext->getMemoryAllocationManager(),mExpTypeCount);

				resetBuildExpressionCount();				
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::referenceBuildExpressionCounter(COntologyBuildExpressionCounter* buildExpCounter) {
				mConsistencyProblematicExpressions = true;
				mConsistencyProblematicExpressionsChecked = false;
				if (buildExpCounter) {
					for (cint64 i = 0; i < mExpTypeCount; ++i) {
						mExpCountVec[i] = buildExpCounter->mExpCountVec[i];
					}
				} else {
					resetBuildExpressionCount();
				}
				return this;
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::resetBuildExpressionCount() {
				for (cint64 i = 0; i < mExpTypeCount; ++i) {
					mExpCountVec[i] = 0;
				}

				mMinimumMaxObjectCardinality = 0;
				mMaximumMaxObjectCardinality = 0;
				mMinimumMinObjectCardinality = 0;
				mMaximumMinObjectCardinality = 0;

				mMinimumMinDataCardinality = 0;
				mMaximumMinDataCardinality = 0;
				mMinimumMaxDataCardinality = 0;
				mMaximumMaxDataCardinality = 0;

				mRefferedBottomClassCount = 0;
				mRefferedBottomObjectPropertyCount = 0;
				mRefferedBottomDataPropertyCount = 0;

				return this;
			}

			cint64& COntologyBuildExpressionCounter::getBuildExpressionCount(CBuildExpression::ExpressionType expType) {
				return mExpCountVec[(cint64)expType];
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::incBuildExpressionCount(CBuildExpression::ExpressionType expType, cint64 incCount) {
				mExpCountVec[(cint64)expType] += incCount;
				return this;
			}


			bool COntologyBuildExpressionCounter::hasBuildExpressions(CBuildExpression::ExpressionType expType){
				return mExpCountVec[(cint64)expType] > 0;
			}



			cint64 COntologyBuildExpressionCounter::getMinimumMaxObjectCardinality() {
				return mMinimumMaxObjectCardinality;
			}

			cint64 COntologyBuildExpressionCounter::getMaximumMaxObjectCardinality() {
				return mMaximumMaxObjectCardinality;
			}


			cint64 COntologyBuildExpressionCounter::getMinimumMinObjectCardinality() {
				return mMinimumMinObjectCardinality;
			}

			cint64 COntologyBuildExpressionCounter::getMaximumMinObjectCardinality() {
				return mMaximumMinObjectCardinality;
			}


			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::setMinObjectCardinalityMinimumMaximumCandidate(cint64 cardinality) {
				mMaximumMinObjectCardinality = qMax(cardinality,mMaximumMinObjectCardinality);
				mMinimumMinObjectCardinality = qMin(cardinality,mMinimumMinObjectCardinality);
				return this;
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::setMaxObjectCardinalityMinimumMaximumCandidate(cint64 cardinality) {
				mMaximumMaxObjectCardinality = qMax(cardinality,mMaximumMaxObjectCardinality);
				mMinimumMaxObjectCardinality = qMin(cardinality,mMinimumMaxObjectCardinality);
				return this;
			}



			cint64 COntologyBuildExpressionCounter::getMinimumMaxDataCardinality() {
				return mMinimumMaxDataCardinality;
			}

			cint64 COntologyBuildExpressionCounter::getMaximumMaxDataCardinality() {
				return mMaximumMaxDataCardinality;
			}


			cint64 COntologyBuildExpressionCounter::getMinimumMinDataCardinality() {
				return mMinimumMinDataCardinality;
			}

			cint64 COntologyBuildExpressionCounter::getMaximumMinDataCardinality() {
				return mMaximumMinDataCardinality;
			}



			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::setMinDataCardinalityMinimumMaximumCandidate(cint64 cardinality) {
				mMaximumMinDataCardinality = qMax(cardinality,mMaximumMinDataCardinality);
				mMinimumMinDataCardinality = qMin(cardinality,mMinimumMinDataCardinality);
				return this;
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::setMaxDataCardinalityMinimumMaximumCandidate(cint64 cardinality) {
				mMaximumMaxDataCardinality = qMax(cardinality,mMaximumMaxDataCardinality);
				mMinimumMaxDataCardinality = qMin(cardinality,mMinimumMaxDataCardinality);
				return this;
			}


			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::incRefferedBottomClassCount(cint64 incCount) {
				mRefferedBottomClassCount += incCount;
				return this;
			}

			cint64 COntologyBuildExpressionCounter::getRefferedBottomClassCount() {
				return mRefferedBottomClassCount;
			}


			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::incRefferedBottomObjectPropertyCount(cint64 incCount) {
				mRefferedBottomObjectPropertyCount += incCount;
				return this;
			}

			cint64 COntologyBuildExpressionCounter::getRefferedBottomObjectPropertyCount() {
				return mRefferedBottomObjectPropertyCount;
			}

			COntologyBuildExpressionCounter* COntologyBuildExpressionCounter::incRefferedBottomDataPropertyCount(cint64 incCount) {
				mRefferedBottomDataPropertyCount += incCount;
				return this;
			}

			cint64 COntologyBuildExpressionCounter::getRefferedBottomDataPropertyCount() {
				return mRefferedBottomDataPropertyCount;
			}


			bool COntologyBuildExpressionCounter::hasConsistencyProblematicExpressions() {
				if (!mConsistencyProblematicExpressionsChecked) {

					mConsistencyProblematicExpressions = false;
					if (hasBuildExpressions(CBuildExpression::BETDISJOINTCLASSES)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDISJOINTUNION)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETOBJECTCOMPLEMENTOF)) {
						mConsistencyProblematicExpressions = true;
					}

					if (hasBuildExpressions(CBuildExpression::BETDISJOINTOBJECTPROPERTIES)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETIRREFLEXIVEPROPERTY)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETASYMMETRICPROPERTY)) {
						mConsistencyProblematicExpressions = true;
					}

					if (hasBuildExpressions(CBuildExpression::BETDIFFERENTINDIVIDUAL)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION)) {
						mConsistencyProblematicExpressions = true;
					}


					if (hasBuildExpressions(CBuildExpression::BETDISJOINTDATAPROPERTIES)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATASOMEVALUEFROM)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATAHASVALUE)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATAMINCARDINALITY)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATAEXACTCARDINALITY)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATASOMEVALUEFROM)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETDATAPROPERTYASSERTION)) {
						mConsistencyProblematicExpressions = true;
					}


					if (hasBuildExpressions(CBuildExpression::BETNOMINALINDIVIDUALVARIABLE)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETRULECLASSATOM)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETRULEPROPERTYATOM)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETRULEDIFFERENTINDIVIDUALSATOM)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETRULEATOMBODYLIST)) {
						mConsistencyProblematicExpressions = true;
					}
					if (hasBuildExpressions(CBuildExpression::BETRULEATOMHEADLIST)) {
						mConsistencyProblematicExpressions = true;
					}

					if (hasBuildExpressions(CBuildExpression::BETOBJECTEXACTLYCARDINALITY) || hasBuildExpressions(CBuildExpression::BETOBJECTMAXCARDINALITY)) {
						if (getMinimumMaxObjectCardinality() <= 0) {
							mConsistencyProblematicExpressions = true;
						}
					}

					if (hasBuildExpressions(CBuildExpression::BETOBJECTMINCARDINALITY) || hasBuildExpressions(CBuildExpression::BETOBJECTEXACTLYCARDINALITY)) {
						if (getMaximumMinObjectCardinality() < 0) {
							mConsistencyProblematicExpressions = true;
						}
						if (getMaximumMinObjectCardinality() > 1) {
							if (hasBuildExpressions(CBuildExpression::BETFUNCTIONALPROPERTYOF)) {
								mConsistencyProblematicExpressions = true;
							}
							if (hasBuildExpressions(CBuildExpression::BETFUNCTIONALPROPERTYOF)) {
								mConsistencyProblematicExpressions = true;
							}
						}
						if (hasBuildExpressions(CBuildExpression::BETOBJECTEXACTLYCARDINALITY) || hasBuildExpressions(CBuildExpression::BETOBJECTMAXCARDINALITY)) {
							if (getMaximumMinObjectCardinality() > getMinimumMaxObjectCardinality()) {
								mConsistencyProblematicExpressions = true;
							}
						}
					}

					if (getRefferedBottomClassCount() > 0) {
						mConsistencyProblematicExpressions = true;
					}
					if (getRefferedBottomClassCount() > 0) {
						mRefferedBottomObjectPropertyCount = true;
					}
					if (getRefferedBottomClassCount() > 0) {
						mRefferedBottomDataPropertyCount = true;
					}


					mConsistencyProblematicExpressionsChecked = true;
				}
				return mConsistencyProblematicExpressions;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "COptimizedComplexVariableIndividualBindingsCardinality.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualBindingsCardinality::COptimizedComplexVariableIndividualBindingsCardinality() {
				initCardinalities(1, 1, 0);
			}


			COptimizedComplexVariableIndividualBindingsCardinality::COptimizedComplexVariableIndividualBindingsCardinality(cint64 sameIndividualsSeparatlyConsideredCardinality, cint64 sameIndividualsJointlyConsideredCardinality, cint64 cardUpId) {
				initCardinalities(sameIndividualsSeparatlyConsideredCardinality, sameIndividualsJointlyConsideredCardinality, cardUpId);
			}


			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::addCardinalities(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				mSameIndividualsSeparatlyConsideredCardinality += cardinalities->mSameIndividualsSeparatlyConsideredCardinality;
				mSameIndividualsJointlyConsideredCardinality += cardinalities->mSameIndividualsJointlyConsideredCardinality;
				return this;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::substractCardinalities(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				mSameIndividualsSeparatlyConsideredCardinality -= cardinalities->mSameIndividualsSeparatlyConsideredCardinality;
				mSameIndividualsJointlyConsideredCardinality -= cardinalities->mSameIndividualsJointlyConsideredCardinality;
				return this;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::initCardinalities(cint64 sameIndividualsSeparatlyConsideredCardinality, cint64 sameIndividualsJointlyConsideredCardinality, cint64 cardUpId) {
				mSameIndividualsSeparatlyConsideredCardinality = sameIndividualsSeparatlyConsideredCardinality;
				mSameIndividualsJointlyConsideredCardinality = sameIndividualsJointlyConsideredCardinality;
				mCardinalityUpdateId = cardUpId;
				return this;
			}


			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::setSameIndividualsSeparatlyConsideredCardinality(cint64 cardinality) {
				mSameIndividualsSeparatlyConsideredCardinality = cardinality;
				return this;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::setSameIndividualsJointlyConsideredCardinality(cint64 cardinality) {
				mSameIndividualsJointlyConsideredCardinality = cardinality;
				return this;
			}


			cint64 COptimizedComplexVariableIndividualBindingsCardinality::getSameIndividualsSeparatlyConsideredCardinality() const {
				return mSameIndividualsSeparatlyConsideredCardinality;
			}

			cint64 COptimizedComplexVariableIndividualBindingsCardinality::getSameIndividualsJointlyConsideredCardinality() const {
				return mSameIndividualsJointlyConsideredCardinality;
			}

			cint64 COptimizedComplexVariableIndividualBindingsCardinality::getCardinalityUpdateId() {
				return mCardinalityUpdateId;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinality::setCardinalityUpdateId(cint64 cardUpId) {
				mCardinalityUpdateId = cardUpId;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

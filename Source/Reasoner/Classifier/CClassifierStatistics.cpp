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

#include "CClassifierStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassifierStatistics::CClassifierStatistics(CClassifierStatistics *nextClassificationStatistics) {
				resetValues();
				nextClassifStat = nextClassificationStatistics;
			}


			qint64 CClassifierStatistics::getPseudoModelMergedTestedSubsumptionCount() {
				return pseudoModelSubsumTests;
			}

			qint64 CClassifierStatistics::getSubclassTestedSubsumptionCount() {
				return subclassSubsumTests;
			}

			qint64 CClassifierStatistics::getSubclassIdentifierOccurTestedSubsumptionCount() {
				return subclassIdOccSubsumTests;
			}

			qint64 CClassifierStatistics::getDerivatedTestedSubsumptionCount() {
				return derivatedSubsumTests;
			}

			qint64 CClassifierStatistics::getCalculatedTestedSubsumptionCount() {
				return calculatedSubsumTests;
			}

			qint64 CClassifierStatistics::getPrunedTestedSubsumptionCount() {
				return prunedSubsumTests;
			}

			qint64 CClassifierStatistics::getToldSubsumptionCount() {
				return toldSubsumTests;
			}

			qint64 CClassifierStatistics::getTestedSubsumptionCount() {
				return getToldSubsumptionCount()+getPrunedTestedSubsumptionCount()+getDerivatedTestedSubsumptionCount()+getPseudoModelMergedTestedSubsumptionCount()+getSubclassTestedSubsumptionCount()+getSubclassIdentifierOccurTestedSubsumptionCount();
			}


			qint64 CClassifierStatistics::getPseudoModelMergedTestedSatisfiableCount() {
				return pseudoModelSatisTests;
			}

			qint64 CClassifierStatistics::getDerivatedTestedSatisfiableCount() {
				return derivatedSatisTests;
			}

			qint64 CClassifierStatistics::getCalculatedTestedSatisfiableCount() {
				return calculatedSatisTests;
			}

			qint64 CClassifierStatistics::getPrunedTestedSatisfiableCount() {
				return prunedSatisTests;
			}

			qint64 CClassifierStatistics::getTestedSatisfiableCount() {
				return getToldSatisfiableCount()+getPrunedTestedSatisfiableCount()+getDerivatedTestedSatisfiableCount()+getPseudoModelMergedTestedSatisfiableCount();
			}


			qint64 CClassifierStatistics::getTotalSubsumptionTestCount() {
				return totalSubsumTests;
			}

			qint64 CClassifierStatistics::getTotalSatisfiableCount() {
				return totalSatisTests;
			}

			qint64 CClassifierStatistics::getToldSatisfiableCount() {
				return toldSatisTests;
			}

			qint64 CClassifierStatistics::getPseudoModelMergedTestedDisjointCount() {
				return pseudoModelDisjointTests;
			}

			qint64 CClassifierStatistics::getToldDisjointCount() {
				return toldDisjointTests;
			}

			qint64 CClassifierStatistics::getCalculatedTestedDisjointCount() {
				return calculatedDisjointTests;
			}

			CClassifierStatistics *CClassifierStatistics::incPseudoModelMergedTestedSubsumptionCount(qint64 incCount) {
				pseudoModelSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incPseudoModelMergedTestedSubsumptionCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incSubclassTestedSubsumptionCount(qint64 incCount) {
				subclassSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incSubclassTestedSubsumptionCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incDerivatedTestedSubsumptionCount(qint64 incCount) {
				derivatedSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incDerivatedTestedSubsumptionCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incCalculatedTestedSubsumptionCount(qint64 incCount) {
				calculatedSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incCalculatedTestedSubsumptionCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incPrunedTestedSubsumptionCount(qint64 incCount) {
				prunedSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incPrunedTestedSubsumptionCount(incCount);
				}
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::incToldSubsumptionCount(qint64 incCount) {
				toldSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incToldSubsumptionCount(incCount);
				}
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::incPseudoModelMergedTestedSatisfiableCount(qint64 incCount) {
				pseudoModelSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incPseudoModelMergedTestedSatisfiableCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incDerivatedTestedSatisfiableCount(qint64 incCount) {
				derivatedSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incDerivatedTestedSatisfiableCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incCalculatedTestedSatisfiableCount(qint64 incCount) {
				calculatedSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incCalculatedTestedSatisfiableCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incPrunedTestedSatisfiableCount(qint64 incCount) {
				prunedSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incPrunedTestedSatisfiableCount(incCount);
				}
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::incTotalSubsumptionTestCount(qint64 incCount) {
				totalSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incTotalSubsumptionTestCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incTotalSatisfiableTestCount(qint64 incCount) {
				totalSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incTotalSatisfiableTestCount(incCount);
				}
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::incToldSatisfiableCount(qint64 incCount) {
				toldSatisTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incToldSatisfiableCount(incCount);
				}
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::incPseudoModelMergedTestedDisjointCount(qint64 incCount) {
				pseudoModelDisjointTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incPseudoModelMergedTestedDisjointCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incToldDisjointCount(qint64 incCount) {
				toldDisjointTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incToldDisjointCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incCalculatedTestedDisjointCount(qint64 incCount) {
				calculatedDisjointTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incCalculatedTestedDisjointCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::incSubclassIdentifierOccurTestedSubsumptionCount(qint64 incCount) {
				subclassIdOccSubsumTests += incCount;
				if (nextClassifStat) {
					nextClassifStat->incSubclassIdentifierOccurTestedSubsumptionCount(incCount);
				}
				return this;
			}

			CClassifierStatistics *CClassifierStatistics::resetValues() {
				pseudoModelSubsumTests = 0;
				subclassSubsumTests = 0;
				derivatedSubsumTests = 0;
				calculatedSubsumTests = 0;
				prunedSubsumTests = 0;
				pseudoModelSatisTests = 0;
				derivatedSatisTests = 0;
				calculatedSatisTests = 0;
				prunedSatisTests = 0;
				totalSubsumTests = 0;
				totalSatisTests = 0;
				toldSubsumTests = 0;
				toldSatisTests = 0;
				pseudoModelDisjointTests = 0;
				toldDisjointTests = 0;
				calculatedDisjointTests = 0;
				subclassIdOccSubsumTests = 0;
				return this;
			}


			CClassifierStatistics *CClassifierStatistics::appendStatistics(CClassifierStatistics *statistics) {
				pseudoModelSubsumTests += statistics->getPseudoModelMergedTestedSubsumptionCount();
				subclassSubsumTests += statistics->getSubclassTestedSubsumptionCount();
				derivatedSubsumTests += statistics->getDerivatedTestedSubsumptionCount();
				calculatedSubsumTests += statistics->getCalculatedTestedSubsumptionCount();
				prunedSubsumTests += statistics->getPrunedTestedSubsumptionCount();
				pseudoModelSatisTests += statistics->getPseudoModelMergedTestedSatisfiableCount();
				derivatedSatisTests += statistics->getDerivatedTestedSatisfiableCount();
				calculatedSatisTests += statistics->getCalculatedTestedSatisfiableCount();
				prunedSatisTests += statistics->getPrunedTestedSatisfiableCount();
				totalSubsumTests += statistics->getTotalSubsumptionTestCount();
				totalSatisTests += statistics->getTotalSatisfiableCount();
				toldSubsumTests += statistics->getToldSubsumptionCount();
				toldSatisTests += statistics->getToldSatisfiableCount();
				pseudoModelDisjointTests += statistics->getPseudoModelMergedTestedDisjointCount();
				toldDisjointTests += statistics->getToldDisjointCount();
				calculatedDisjointTests += statistics->getCalculatedTestedDisjointCount();
				subclassIdOccSubsumTests += statistics->getSubclassIdentifierOccurTestedSubsumptionCount();
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

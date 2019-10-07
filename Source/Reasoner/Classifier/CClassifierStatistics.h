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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIERSTATISTICS_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIERSTATISTICS_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassifierStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassifierStatistics {
				// public methods
				public:
					//! Constructor
					CClassifierStatistics(CClassifierStatistics *nextClassificationStatistics = 0);

					qint64 getPseudoModelMergedTestedSubsumptionCount();
					qint64 getSubclassTestedSubsumptionCount();
					qint64 getSubclassIdentifierOccurTestedSubsumptionCount();
					qint64 getDerivatedTestedSubsumptionCount();
					qint64 getCalculatedTestedSubsumptionCount();
					qint64 getPrunedTestedSubsumptionCount();
					qint64 getTestedSubsumptionCount();
					qint64 getToldSubsumptionCount();

					qint64 getPseudoModelMergedTestedSatisfiableCount();
					qint64 getDerivatedTestedSatisfiableCount();
					qint64 getCalculatedTestedSatisfiableCount();
					qint64 getPrunedTestedSatisfiableCount();
					qint64 getTestedSatisfiableCount();
					qint64 getToldSatisfiableCount();

					qint64 getTotalSubsumptionTestCount();
					qint64 getTotalSatisfiableCount();

					qint64 getPseudoModelMergedTestedDisjointCount();
					qint64 getToldDisjointCount();
					qint64 getCalculatedTestedDisjointCount();

					CClassifierStatistics *incPseudoModelMergedTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incSubclassTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incSubclassIdentifierOccurTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incDerivatedTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incCalculatedTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incPrunedTestedSubsumptionCount(qint64 incCount = 1);
					CClassifierStatistics *incToldSubsumptionCount(qint64 incCount = 1);

					CClassifierStatistics *incPseudoModelMergedTestedSatisfiableCount(qint64 incCount = 1);
					CClassifierStatistics *incDerivatedTestedSatisfiableCount(qint64 incCount = 1);
					CClassifierStatistics *incCalculatedTestedSatisfiableCount(qint64 incCount = 1);
					CClassifierStatistics *incPrunedTestedSatisfiableCount(qint64 incCount = 1);
					CClassifierStatistics *incToldSatisfiableCount(qint64 incCount = 1);


					CClassifierStatistics *incPseudoModelMergedTestedDisjointCount(qint64 incCount = 1);
					CClassifierStatistics *incToldDisjointCount(qint64 incCount = 1);
					CClassifierStatistics *incCalculatedTestedDisjointCount(qint64 incCount = 1);


					CClassifierStatistics *incTotalSubsumptionTestCount(qint64 incCount = 1);
					CClassifierStatistics *incTotalSatisfiableTestCount(qint64 incCount = 1);

					CClassifierStatistics *resetValues();
					CClassifierStatistics *appendStatistics(CClassifierStatistics *statistics);

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 pseudoModelSubsumTests;
					qint64 subclassSubsumTests;
					qint64 subclassIdOccSubsumTests;
					qint64 derivatedSubsumTests;
					qint64 prunedSubsumTests;
					qint64 calculatedSubsumTests;
					qint64 toldSubsumTests;

					qint64 pseudoModelSatisTests;
					qint64 derivatedSatisTests;
					qint64 calculatedSatisTests;
					qint64 prunedSatisTests;
					qint64 toldSatisTests;

					qint64 pseudoModelDisjointTests;
					qint64 calculatedDisjointTests;
					qint64 toldDisjointTests;


					qint64 totalSubsumTests;
					qint64 totalSatisTests;


					CClassifierStatistics *nextClassifStat;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIERSTATISTICS_H

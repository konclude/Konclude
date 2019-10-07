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

#include "CDisjunctionBranchingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDisjunctionBranchingStatistics::CDisjunctionBranchingStatistics() {
				mExpandedCount = 0;
				mClashesFullyInvolvedCount = 0;
				mClashesPartiallyInvolvedCount = 0;
				mSatisfiableOccurrenceCount = 0;
				mDisjunctBranchingStatistics = nullptr;
			}


			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::initDisjunctionBranchingStatistics() {
				mExpandedCount = 0;
				mClashesFullyInvolvedCount = 0;
				mClashesPartiallyInvolvedCount = 0;
				mSatisfiableOccurrenceCount = 0;
				mDisjunctBranchingStatistics = nullptr;
				return this;
			}


			cint64 CDisjunctionBranchingStatistics::getExpandedCount() {
				return mExpandedCount;
			}

			cint64 CDisjunctionBranchingStatistics::getClashFullyInvolvedCount() {
				return mClashesFullyInvolvedCount;
			}

			cint64 CDisjunctionBranchingStatistics::getClashPartiallyInvolvedCount() {
				return mClashesPartiallyInvolvedCount;
			}

			cint64 CDisjunctionBranchingStatistics::getSatisfiableOccurrenceCount() {
				return mSatisfiableOccurrenceCount;
			}



			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::setExpandedCount(cint64 count) {
				mExpandedCount = count;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::setClashFullyInvolvedCount(cint64 count) {
				mClashesFullyInvolvedCount = count;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::setClashPartiallyInvolvedCount(cint64 count) {
				mClashesPartiallyInvolvedCount = count;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::setSatisfiableOccurrenceCount(cint64 count) {
				mSatisfiableOccurrenceCount = count;
				return this;
			}


			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::incExpandedCount(cint64 incCount) {
				mExpandedCount += incCount;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::incClashFullyInvolvedCount(cint64 incCount) {
				mClashesFullyInvolvedCount += incCount;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::incClashPartiallyInvolvedCount(cint64 incCount) {
				mClashesPartiallyInvolvedCount += incCount;
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::incSatisfiableOccurrenceCount(cint64 incCount) {
				mSatisfiableOccurrenceCount += incCount;
				return this;
			}


			CDisjunctBranchingStatistics* CDisjunctionBranchingStatistics::getDisjunctBranchingStatistics() {
				return mDisjunctBranchingStatistics;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::addDisjunctBranchingStatistics(CDisjunctBranchingStatistics* disjunctBranchingStatistics) {
				if (disjunctBranchingStatistics) {
					mDisjunctBranchingStatistics = mDisjunctBranchingStatistics->append(disjunctBranchingStatistics);
				}
				return this;
			}

			CDisjunctionBranchingStatistics* CDisjunctionBranchingStatistics::setDisjunctBranchingStatistics(CDisjunctBranchingStatistics* disjunctBranchingStatistics) {
				mDisjunctBranchingStatistics = disjunctBranchingStatistics;
				return this;
			}


		}; // end namespace Ontology


	}; // end namespace Reasoner

}; // end namespace Konclude

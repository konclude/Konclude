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

#include "CDisjunctBranchingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDisjunctBranchingStatistics::CDisjunctBranchingStatistics() {
				mExpandedCount = 0;
				mClashesInvolvedCount = 0;
				mSatisfiableOccurrenceCount = 0;
				setData(nullptr);
			}


			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::initDisjunctBranchingStatistics(CSortedNegLinker<CConcept*>* disjunctLinker) {
				mExpandedCount = 0;
				mClashesInvolvedCount = 0;
				mSatisfiableOccurrenceCount = 0;
				setData(disjunctLinker);
				return this;
			}

			CSortedNegLinker<CConcept*>* CDisjunctBranchingStatistics::getDisjunctLinker() {
				return getData();
			}



			cint64 CDisjunctBranchingStatistics::getExpandedCount() {
				return mExpandedCount;
			}

			cint64 CDisjunctBranchingStatistics::getClashInvolvedCount() {
				return mClashesInvolvedCount;
			}

			cint64 CDisjunctBranchingStatistics::getSatisfiableOccurrenceCount() {
				return mSatisfiableOccurrenceCount;
			}



			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::setExpandedCount(cint64 count) {
				mExpandedCount = count;
				return this;
			}

			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::setClashInvolvedCount(cint64 count) {
				mClashesInvolvedCount = count;
				return this;
			}

			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::setSatisfiableOccurrenceCount(cint64 count) {
				mSatisfiableOccurrenceCount = count;
				return this;
			}



			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::incExpandedCount(cint64 incCount) {
				mExpandedCount += incCount;
				return this;
			}

			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::incClashInvolvedCount(cint64 incCount) {
				mClashesInvolvedCount += incCount;
				return this;
			}

			CDisjunctBranchingStatistics* CDisjunctBranchingStatistics::incSatisfiableOccurrenceCount(cint64 incCount) {
				mSatisfiableOccurrenceCount += incCount;
				return this;
			}




		}; // end namespace Ontology


	}; // end namespace Reasoner

}; // end namespace Konclude

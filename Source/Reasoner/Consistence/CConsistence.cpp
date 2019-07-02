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

#include "CConsistence.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {


			CConsistence::CConsistence() {
				mConsistent = false;
				mConsData = nullptr;
				mRepresentativelyStored = false;
				mAllRepresentativelyStored = false;
				mStatsColl = nullptr;
			}


			CConsistence::~CConsistence() {
				delete mConsData;
			}


			bool CConsistence::isOntologyConsistent() {
				return mConsistent;
			}

			CConsistence* CConsistence::setOntologyConsistent(bool consistency) {
				mConsistent = consistency;
				return this;
			}


			CConsistenceData* CConsistence::getConsistenceModelData() {
				return mConsData;
			}

			bool CConsistence::hasConsistenceModelData() {
				return mConsData != nullptr;
			}

			CConsistence* CConsistence::setConsistenceModelData(CConsistenceData* consData) {
				mConsData = consData;
				return this;
			}

			bool CConsistence::areIndividualsRepresentativelyStored() {
				return mRepresentativelyStored;
			}

			bool CConsistence::areAllIndividualsRepresentativelyStored() {
				return mAllRepresentativelyStored;
			}

			CConsistence* CConsistence::setIndividualsRepresentativelyStored(bool representativelyStored) {
				mRepresentativelyStored = representativelyStored;
				return this;
			}

			CConsistence* CConsistence::setAllIndividualsRepresentativelyStored(bool representativelyStored) {
				mAllRepresentativelyStored = representativelyStored;
				return this;
			}

			bool CConsistence::hasConsistenceStatistics() {
				return mStatsColl != nullptr;
			}

			CConsistenceStatistics* CConsistence::getConsistenceStatistics() {
				return mStatsColl;
			}

			CConsistence* CConsistence::setConsistenceStatistics(CConsistenceStatistics* statColl) {
				mStatsColl = statColl;
				return this;
			}

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#include "CConsistenceStatisticsCollectionStrings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {


			CConsistenceStatisticsCollectionStrings::CConsistenceStatisticsCollectionStrings() {
				mCalcStatCollLinker = nullptr;
				mStatStringsCollected = false;
			}


			CConsistenceStatisticsCollectionStrings::~CConsistenceStatisticsCollectionStrings() {
				CConsistenceCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					CConsistenceCalculationStatisticsCollection* tmpCalcStatCollLinker = mCalcStatCollLinkerIt;
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
					delete tmpCalcStatCollLinker;
				}
			}


			CConsistenceCalculationStatisticsCollection* CConsistenceStatisticsCollectionStrings::getCalculationStatisticsCollectorLinker() {
				return mCalcStatCollLinker;
			}

			CConsistenceCalculationStatisticsCollection* CConsistenceStatisticsCollectionStrings::createCalculationStatisticsCollection() {
				CConsistenceCalculationStatisticsCollection* tmpCalcStatCollLinker = new CConsistenceCalculationStatisticsCollection();
				mCalcStatCollLinker = tmpCalcStatCollLinker->append(mCalcStatCollLinker);
				return tmpCalcStatCollLinker;
			}


			bool CConsistenceStatisticsCollectionStrings::addProcessingStatistics(const QString& statName, cint64 statValue) {
				mStatNameStringsValueHash.insert(statName,statValue);
				return true;
			}



			QList<QString> CConsistenceStatisticsCollectionStrings::getStatisticsNameStringList() {
				if (!mStatStringsCollected) {
					CConsistenceCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
					while (mCalcStatCollLinkerIt) {
						mCalcStatCollLinkerIt->appendStatisticsStringNames(&mStatNameStrings);
						mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
					}
					foreach (QString statName, mStatNameStringsValueHash.keys()) {
						mStatNameStrings.insert(statName);
					}
					mStatStringsCollected = true;
				}
				return mStatNameStrings.toList();
			}

			cint64 CConsistenceStatisticsCollectionStrings::getStatisticIntegerValue(const QString& statName) {
				cint64 statValue = mStatNameStringsValueHash.value(statName,0);
				CConsistenceCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					statValue += mCalcStatCollLinkerIt->getStatisticIntegerValue(statName);
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
				}
				return statValue;
			}


		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

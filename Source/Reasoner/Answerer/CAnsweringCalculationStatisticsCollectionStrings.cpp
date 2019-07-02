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

#include "CAnsweringCalculationStatisticsCollectionStrings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringCalculationStatisticsCollectionStrings::CAnsweringCalculationStatisticsCollectionStrings() {
				mCalcStatCollLinker = nullptr;
				mStatStringsCollected = false;
			}


			CAnsweringCalculationStatisticsCollectionStrings::~CAnsweringCalculationStatisticsCollectionStrings() {
				CAnsweringCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					CAnsweringCalculationStatisticsCollection* tmpCalcStatCollLinker = mCalcStatCollLinkerIt;
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
					delete tmpCalcStatCollLinker;
				}
			}


			CAnsweringCalculationStatisticsCollection* CAnsweringCalculationStatisticsCollectionStrings::getCalculationStatisticsCollectorLinker() {
				return mCalcStatCollLinker;
			}

			CAnsweringCalculationStatisticsCollection* CAnsweringCalculationStatisticsCollectionStrings::createCalculationStatisticsCollection() {
				CAnsweringCalculationStatisticsCollection* tmpCalcStatCollLinker = new CAnsweringCalculationStatisticsCollection();
				mCalcStatCollLinker = tmpCalcStatCollLinker->append(mCalcStatCollLinker);
				return tmpCalcStatCollLinker;
			}


			bool CAnsweringCalculationStatisticsCollectionStrings::addProcessingStatistics(const QString& statName, cint64 statValue) {
				mStatNameStringsValueHash.insert(statName,statValue);
				return true;
			}


			QList<QString> CAnsweringCalculationStatisticsCollectionStrings::getStatisticsNameStringList() {
				if (!mStatStringsCollected) {
					CAnsweringCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
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

			cint64 CAnsweringCalculationStatisticsCollectionStrings::getStatisticIntegerValue(const QString& statName) {
				cint64 statValue = mStatNameStringsValueHash.value(statName,0);
				CAnsweringCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					statValue += mCalcStatCollLinkerIt->getStatisticIntegerValue(statName);
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
				}
				return statValue;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

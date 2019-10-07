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

#include "CClassificationStatisticsCollectionStrings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CClassificationStatisticsCollectionStrings::CClassificationStatisticsCollectionStrings() {
				mCalcStatCollLinker = nullptr;
				mStatStringsCollected = false;
			}


			CClassificationStatisticsCollectionStrings::~CClassificationStatisticsCollectionStrings() {
				CClassificationCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					CClassificationCalculationStatisticsCollection* tmpCalcStatCollLinker = mCalcStatCollLinkerIt;
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
					delete tmpCalcStatCollLinker;
				}
			}


			CClassificationCalculationStatisticsCollection* CClassificationStatisticsCollectionStrings::getCalculationStatisticsCollectorLinker() {
				return mCalcStatCollLinker;
			}

			CClassificationCalculationStatisticsCollection* CClassificationStatisticsCollectionStrings::createCalculationStatisticsCollection() {
				CClassificationCalculationStatisticsCollection* tmpCalcStatCollLinker = new CClassificationCalculationStatisticsCollection();
				mCalcStatCollLinker = tmpCalcStatCollLinker->append(mCalcStatCollLinker);
				return tmpCalcStatCollLinker;
			}


			bool CClassificationStatisticsCollectionStrings::addProcessingStatistics(const QString& statName, cint64 statValue) {
				mStatNameStringsValueHash.insert(statName,statValue);
				return true;
			}



			QList<QString> CClassificationStatisticsCollectionStrings::getStatisticsNameStringList() {
				if (!mStatStringsCollected) {
					CClassificationCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
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

			cint64 CClassificationStatisticsCollectionStrings::getStatisticIntegerValue(const QString& statName) {
				cint64 statValue = mStatNameStringsValueHash.value(statName,0);
				CClassificationCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					statValue += mCalcStatCollLinkerIt->getStatisticIntegerValue(statName);
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
				}
				return statValue;
			}


		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

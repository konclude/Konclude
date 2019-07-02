/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CQueryStatisticsCollectionStrings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CQueryStatisticsCollectionStrings::CQueryStatisticsCollectionStrings() {
				mCalcStatCollLinker = nullptr;
				mStatStringsCollected = false;
			}


			CQueryStatisticsCollectionStrings::~CQueryStatisticsCollectionStrings() {
				CQueryCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					CQueryCalculationStatisticsCollection* tmpCalcStatCollLinker = mCalcStatCollLinkerIt;
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
					delete tmpCalcStatCollLinker;
				}
			}


			CQueryCalculationStatisticsCollection* CQueryStatisticsCollectionStrings::getCalculationStatisticsCollectorLinker() {
				return mCalcStatCollLinker;
			}

			CQueryCalculationStatisticsCollection* CQueryStatisticsCollectionStrings::createCalculationStatisticsCollection() {
				CQueryCalculationStatisticsCollection* tmpCalcStatCollLinker = new CQueryCalculationStatisticsCollection();
				mCalcStatCollLinker = tmpCalcStatCollLinker->append(mCalcStatCollLinker);
				return mCalcStatCollLinker;
			}


			bool CQueryStatisticsCollectionStrings::addProcessingStatistics(const QString& statName, cint64 statValue) {
				mStatNameStringsValueHash.insert(statName,statValue);
				return true;
			}



			QList<QString> CQueryStatisticsCollectionStrings::getStatisticsNameStringList() {
				if (!mStatStringsCollected) {
					CQueryCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
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

			cint64 CQueryStatisticsCollectionStrings::getStatisticIntegerValue(const QString& statName) {
				cint64 statValue = mStatNameStringsValueHash.value(statName,0);
				CQueryCalculationStatisticsCollection* mCalcStatCollLinkerIt = mCalcStatCollLinker;
				while (mCalcStatCollLinkerIt) {
					statValue += mCalcStatCollLinkerIt->getStatisticIntegerValue(statName);
					mCalcStatCollLinkerIt = mCalcStatCollLinkerIt->getNext();
				}
				return statValue;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

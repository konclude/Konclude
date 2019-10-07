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


			bool CQueryStatisticsCollectionStrings::addProcessingStatistics(COntologyProcessingStatistics* ontProcStats) {
				QHash<QString,cint64>* statsValueHash = ontProcStats->getProcessingCountStatisticsHash();
				for (QHash<QString,cint64>::const_iterator it = statsValueHash->constBegin(), itEnd = statsValueHash->constEnd(); it != itEnd; ++it) {
					const QString& statString(it.key());
					cint64 statValue = it.value();
					incProcessingStatistics(statString,statValue);
				}
				return true;
			}


			bool CQueryStatisticsCollectionStrings::addProcessingStatistics(const QString& statName, cint64 statValue) {
				mStatNameStringsValueHash[statName] = statValue;
				return true;
			}


			bool CQueryStatisticsCollectionStrings::incProcessingStatistics(const QString& statName, cint64 incStatValue) {
				mStatNameStringsValueHash[statName] += incStatValue;
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

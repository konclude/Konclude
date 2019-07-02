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

#include "CConsistenceCalculationStatisticsCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {


			CConsistenceCalculationStatisticsCollection::CConsistenceCalculationStatisticsCollection() : CLinkerBase<CConsistenceCalculationStatisticsCollection*,CConsistenceCalculationStatisticsCollection>(this) {
			}


			CConsistenceCalculationStatisticsCollection::~CConsistenceCalculationStatisticsCollection() {
			}


			bool CConsistenceCalculationStatisticsCollection::addProcessingStatistics(const QString& statName, cint64 statValue) {
				cint64 prevValue = mStatNameStringsValueHash.value(statName,0);
				mStatNameStringsValueHash.insert(statName,statValue+prevValue);
				return true;
			}


			CConsistenceCalculationStatisticsCollection* CConsistenceCalculationStatisticsCollection::appendStatisticsStringNames(QSet<QString>* statNamesSet) {
				if (statNamesSet) {
					foreach (QString statName, mStatNameStringsValueHash.keys()) {
						statNamesSet->insert(statName);
					}
				} 
				return this;
			}

			cint64 CConsistenceCalculationStatisticsCollection::getStatisticIntegerValue(const QString& statName) {
				return mStatNameStringsValueHash.value(statName,0);
			}

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

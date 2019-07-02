/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CClassificationCalculationStatisticsCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CClassificationCalculationStatisticsCollection::CClassificationCalculationStatisticsCollection() : CLinkerBase<CClassificationCalculationStatisticsCollection*,CClassificationCalculationStatisticsCollection>(this) {
			}


			CClassificationCalculationStatisticsCollection::~CClassificationCalculationStatisticsCollection() {
			}


			bool CClassificationCalculationStatisticsCollection::addProcessingStatistics(const QString& statName, cint64 statValue) {
				cint64 prevValue = mStatNameStringsValueHash.value(statName,0);
				mStatNameStringsValueHash.insert(statName,statValue+prevValue);
				return true;
			}


			CClassificationCalculationStatisticsCollection* CClassificationCalculationStatisticsCollection::appendStatisticsStringNames(QSet<QString>* statNamesSet) {
				if (statNamesSet) {
					foreach (QString statName, mStatNameStringsValueHash.keys()) {
						statNamesSet->insert(statName);
					}
				} 
				return this;
			}

			cint64 CClassificationCalculationStatisticsCollection::getStatisticIntegerValue(const QString& statName) {
				return mStatNameStringsValueHash.value(statName,0);
			}

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

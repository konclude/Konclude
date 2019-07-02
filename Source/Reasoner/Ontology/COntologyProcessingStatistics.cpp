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

#include "COntologyProcessingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingStatistics::COntologyProcessingStatistics() {
			}

			QHash<QString,cint64>* COntologyProcessingStatistics::getProcessingCountStatisticsHash() {
				return &mProcessingCountStatisticsHash;
			}

			cint64 COntologyProcessingStatistics::getProcessingCountStatisticValue(const QString& statisticName) {
				return mProcessingCountStatisticsHash.value(statisticName);
			}

			COntologyProcessingStatistics* COntologyProcessingStatistics::setProcessingCountStatisticValue(const QString& statisticName, cint64 statisticValue) {
				mProcessingCountStatisticsHash[statisticName] = statisticValue;
				return this;
			}

			COntologyProcessingStatistics* COntologyProcessingStatistics::incProcessingCountStatisticValue(const QString& statisticName, cint64 incValue) {
				mProcessingCountStatisticsHash[statisticName] += incValue;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

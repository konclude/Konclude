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

#include "CContextRecordData.h"

namespace Konclude {

	namespace Logger {

		namespace Record {


			CContextRecordData::CContextRecordData(double recordErrorLevel, CContextRecordData *nextRecordData) {
				prevRecordData = 0;
				prevRecordLevel = 0;
				setNextRecordData(nextRecordData);
				recordLevel = recordErrorLevel;
			}

			CContextRecordData::~CContextRecordData()	{
			}


			CContextRecordData *CContextRecordData::getNextRecordData() {
				return prevRecordData;
			}

			CContextRecordData *CContextRecordData::setNextRecordData(CContextRecordData *recordData) {
				prevRecordData = recordData;
				if (prevRecordData) {
					prevRecordLevel = prevRecordData->getReflexiveTransetiveMaximumRecordErrorLevel();
				}
				return this;
			}


			double CContextRecordData::getReflexiveTransetiveMaximumRecordErrorLevel() {
				return qMax(recordLevel,prevRecordLevel);
			}

			double CContextRecordData::getRecordErrorLevel() {
				return recordLevel;
			}

			CContextRecordData *CContextRecordData::setRecordErrorLevel(double recordErrorLevel) {
				recordLevel = recordErrorLevel;
				return this;
			}


		}; // end namespace Record

	}; // end namespace Logger

}; // end namespace Konclude

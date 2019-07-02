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

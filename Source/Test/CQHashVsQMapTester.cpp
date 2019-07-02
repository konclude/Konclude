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

#include "CQHashVsQMapTester.h"


namespace Konclude {

	namespace Test {



		CQHashVsQMapTester::CQHashVsQMapTester() {
		}


		CQHashVsQMapTester::~CQHashVsQMapTester() {
		}


		void CQHashVsQMapTester::startModificationTest(cint64 modificationCount) {

			cint64 maxValue = 1000000;
			mMeasurementTimer.start();

			for (cint64 i = 0; i < modificationCount; ++i) {
				qint64 modHashKey = qrand() % maxValue;
				mTestMap.insert(modHashKey,i);
			}
			cint64 elapsedTimeMap = mMeasurementTimer.elapsed();

			cout<<"map modification time: "<<elapsedTimeMap<<" ms, modification count: "<<modificationCount<<"\n";

			mMeasurementTimer.restart();

			for (cint64 i = 0; i < modificationCount; ++i) {
				qint64 modHashKey = qrand() % maxValue;
				mTestHash.insert(modHashKey,i);
			}
			cint64 elapsedTimeHash = mMeasurementTimer.elapsed();
			cout<<"hash modification time: "<<elapsedTimeHash<<" ms, modification count: "<<modificationCount<<"\n";

		}



	}; // end namespace Test

}; // end namespace Konclude

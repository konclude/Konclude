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

#include "CCacheTestReader.h"


namespace Konclude {

	namespace Test {



		CCacheTestReader::CCacheTestReader(COccurrenceUnsatisfiableCacheReader *occUnsatCacheReader) {
			reader = occUnsatCacheReader;
			testCount = 0;
			unsatCount = 0;
			
		}


		CCacheTestReader::~CCacheTestReader() {
			quit();
			wait();
		}


		void CCacheTestReader::startReader() {
			start();
			moveToThread(this);
		}


		qint64 CCacheTestReader::getCacheReads() {
			return testCount;
		}


		void CCacheTestReader::run() {
			if (reader) {

				qint64 maxOntTag = 1;
				qint64 maxConTag = 100;
				qint64 maxCount = 20;

				while (true) {
					qint64 randOntTagNumber = rand() % maxOntTag;
					qint64 randCount = rand() % maxCount;
					QVector<qint64> conTags(randCount,0);
					for (qint64 i = 0; i < randCount; ++i) {
						qint64 randConTagNumber = rand() % maxConTag;
						conTags[i] = randConTagNumber;
					}
					qSort(conTags);
					QVector<CCacheValue> values;
					for (qint64 i = 0; i < randCount; ++i) {
						values.append(CCacheValue(conTags[i],randOntTagNumber,CCacheValue::CACHEVALCONCEPTONTOLOGYTAG));
					}
					values.append(CCacheValue(0,0,CCacheValue::CACHEVALUETERMINATOR));
					
					testCount++;
					if (reader->isUnsatisfiable(values,values.count())) {
						unsatCount++;
					}
				}
			}
		}




	}; // end namespace Test

}; // end namespace Konclude

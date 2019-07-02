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

#include "CCacheTestWriter.h"


namespace Konclude {

	namespace Test {



		CCacheTestWriter::CCacheTestWriter(COccurrenceUnsatisfiableCacheWriter *takeOccUnsatCacheWriter) {
			writer = takeOccUnsatCacheWriter;
			writeCount = 0;			
		}


		CCacheTestWriter::~CCacheTestWriter() {
			quit();
			wait();
			if (writer) {
				delete writer;
			}
		}

		void CCacheTestWriter::startWriter() {
			start();
			moveToThread(this);
		}


		qint64 CCacheTestWriter::getCacheWrites() {
			return writeCount;
		}


		void CCacheTestWriter::run() {
			if (writer) {
				qint64 count = 100;

				qint64 maxOntTag = 1;
				qint64 maxConTag = 100;
				qint64 maxCount = 5;

				while (true/*--count > 0*/) {
					qint64 randOntTagNumber = rand() % maxOntTag;
					qint64 randCount = (rand() % maxCount) + 2;
					QVector<qint64> conTags(randCount,0);
					for (qint64 i = 0; i < randCount; ++i) {
						qint64 randConTagNumber = rand() % maxConTag;
						conTags[i] = randConTagNumber;
					}
					qSort(conTags);
					QList<CCacheValue> values;
					for (qint64 i = 0; i < randCount; ++i) {
						values.append(CCacheValue(conTags[i],randOntTagNumber,CCacheValue::CACHEVALCONCEPTONTOLOGYTAG));
					}
					
					writeCount++;
					writer->setUnsatisfiable(values);

					QThread::msleep(10);
				}
			}
		}




	}; // end namespace Test

}; // end namespace Konclude

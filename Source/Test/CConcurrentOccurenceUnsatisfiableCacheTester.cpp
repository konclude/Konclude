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

#include "CConcurrentOccurenceUnsatisfiableCacheTester.h"


namespace Konclude {

	namespace Test {



		CConcurrentOccurenceUnsatisfiableCacheTester::CConcurrentOccurenceUnsatisfiableCacheTester() {
			occUnsatCache = 0;
			writer = 0;
		}


		CConcurrentOccurenceUnsatisfiableCacheTester::~CConcurrentOccurenceUnsatisfiableCacheTester() {
			qDeleteAll(readerList);
			if (writer) {
				delete writer;
			}
			if (occUnsatCache) {
				delete occUnsatCache;
			}
		}


		void CConcurrentOccurenceUnsatisfiableCacheTester::generateCacheStructure(qint64 cacheReaderCount) {
			readerCount = cacheReaderCount;
			if (occUnsatCache) {
				delete occUnsatCache;
			}
			if (writer) {
				delete writer;
			}
			qDeleteAll(readerList);
			readerList.clear();
			occUnsatCache = new COccurrenceUnsatisfiableCache(readerCount+2);
			writer = new CCacheTestWriter(new COccurrenceUnsatisfiableCacheWriter(occUnsatCache));

			for (qint64 i = 0; i < cacheReaderCount; ++i) {
				CCacheTestReader *reader = new CCacheTestReader(occUnsatCache->getCacheReader());
				readerList.append(reader);
			}
		}

		void CConcurrentOccurenceUnsatisfiableCacheTester::runCacheTest() {
			foreach (CCacheTestReader *reader, readerList) {
				reader->startReader();
			}
			if (writer) {
				writer->startWriter();
			}
		}

		qint64 CConcurrentOccurenceUnsatisfiableCacheTester::getCacheWrites() {
			if (writer) {
				return writer->getCacheWrites();
			} else {
				return 0;
			}
		}

		qint64 CConcurrentOccurenceUnsatisfiableCacheTester::getCacheReads() {
			qint64 cacheReads = 0;
			foreach (CCacheTestReader *reader, readerList) {
				cacheReads = reader->getCacheReads();
			}
			return cacheReads;
		}



	}; // end namespace Test

}; // end namespace Konclude

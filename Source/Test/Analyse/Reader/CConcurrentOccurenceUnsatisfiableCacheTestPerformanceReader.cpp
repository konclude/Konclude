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

#include "CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader::CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader(CConcurrentOccurenceUnsatisfiableCacheTester *unsatCacheTester) {
					cacheTester = unsatCacheTester;

					cacheWritesDescription = 0;
					cacheReadsDescription = 0;

					if (cacheTester) {
						cacheWritesDescription = new CAnalyseValueDescription();
						cacheWritesDescription->setValueName(QString("Test::CacheWrites"));
						cacheWritesDescription->setTemplateValue(new CIntegerAnalyseValue(0.));
						
						cacheReadsDescription = new CAnalyseValueDescription();
						cacheReadsDescription->setValueName(QString("Test::CacheReads"));
						cacheReadsDescription->setTemplateValue(new CIntegerAnalyseValue(0.));
					}
				}

				CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader::~CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader() {
					if (cacheWritesDescription) {
						delete cacheWritesDescription;
					}
					if (cacheReadsDescription) {
						delete cacheReadsDescription;
					}
				}



				CAnalyseValueDescription *CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader::getCacheWritesCountValueDescription() {
					return cacheWritesDescription;
				}


				CAnalyseValueDescription *CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader::getCacheReadsCountValueDescription() {
					return cacheReadsDescription;
				}


				CAnalyseValue *CConcurrentOccurenceUnsatisfiableCacheTestPerformanceReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == cacheWritesDescription && cacheTester) {
						val = new CDoubleAnalyseValue(cacheTester->getCacheWrites());
					} else if (valueDes == cacheReadsDescription && cacheTester) {
						val = new CDoubleAnalyseValue(cacheTester->getCacheReads());
					} 
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

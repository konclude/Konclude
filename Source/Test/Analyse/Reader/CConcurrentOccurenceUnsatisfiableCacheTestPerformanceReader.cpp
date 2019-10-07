/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
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

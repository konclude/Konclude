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

#ifndef KONCLUDE_TEST_CCONCURRENTOCCURENCEUNSATISFIABLECACHETESTER_H
#define KONCLUDE_TEST_CCONCURRENTOCCURENCEUNSATISFIABLECACHETESTER_H

// Libraries includes


// Namespace includes
#include "CCacheTestReader.h"
#include "CCacheTestWriter.h"


// Other includes
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCache.h"
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheReader.h"
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheWriter.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Reasoner::Kernel::Cache;

	namespace Test {


		/*! 
		 *
		 *		\class		CConcurrentOccurenceUnsatisfiableCacheTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConcurrentOccurenceUnsatisfiableCacheTester {
			// public methods
			public:
				//! Constructor
				CConcurrentOccurenceUnsatisfiableCacheTester();

				//! Destructor
				virtual ~CConcurrentOccurenceUnsatisfiableCacheTester();


				void generateCacheStructure(qint64 cacheReaderCount);
				void runCacheTest();


				qint64 getCacheWrites();
				qint64 getCacheReads();


			// protected methods
			protected:

			// protected variables
			protected:
				COccurrenceUnsatisfiableCache *occUnsatCache;
				qint64 readerCount;

				CCacheTestWriter *writer;
				QLinkedList<CCacheTestReader *> readerList;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CCONCURRENTOCCURENCEUNSATISFIABLECACHETESTER_H

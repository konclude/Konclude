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

#ifndef KONCLUDE_TEST_CCACHETESTREADER_H
#define KONCLUDE_TEST_CCACHETESTREADER_H

// Libraries includes
#include <QThread>


// Namespace includes


// Other includes
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheReader.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Reasoner::Kernel::Cache;

	namespace Test {


		/*! 
		 *
		 *		\class		CCacheTestReader
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CCacheTestReader : public QThread {
			// public methods
			public:
				//! Constructor
				CCacheTestReader(COccurrenceUnsatisfiableCacheReader *occUnsatCacheReader);

				//! Destructor
				virtual ~CCacheTestReader();

				virtual void startReader();

				qint64 getCacheReads();


			// protected methods
			protected:
				virtual void run();

			// protected variables
			protected:
				COccurrenceUnsatisfiableCacheReader *reader;
				qint64 testCount;
				qint64 unsatCount;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CCACHETESTREADER_H

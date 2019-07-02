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

#ifndef KONCLUDE_TEST_CQHASHVSQMAPTESTER_H
#define KONCLUDE_TEST_CQHASHVSQMAPTESTER_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "CHashModificationThread.h"


// Other includes
#include "Utilities/Container/CList.h"

#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Test {


		/*! 
		 *
		 *		\class		CQHashVsQMapTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CQHashVsQMapTester {
			// public methods
			public:
				//! Constructor
				CQHashVsQMapTester();

				//! Destructor
				virtual ~CQHashVsQMapTester();


				void startModificationTest(cint64 modificationCount);



			// protected methods
			protected:

			// protected variables
			protected:
				QMap<cint64,cint64> mTestMap;
				QHash<cint64,cint64> mTestHash;
				QTime mMeasurementTimer;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CQHASHVSQMAPTESTER_H

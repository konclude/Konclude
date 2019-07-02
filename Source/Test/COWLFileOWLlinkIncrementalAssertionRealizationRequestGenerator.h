/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREALIZATIONREQUESTGENERATOR_H
#define KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREALIZATIONREQUESTGENERATOR_H

// Libraries includes


// Namespace includes
#include "COWLFileOWLlinkIncrementalAssertionRequestGenerator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	namespace Test {


		/*! 
		 *
		 *		\class		COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator : public COWLFileOWLlinkIncrementalAssertionRequestGenerator {

			// public methods
			public:
				//! Constructor
				COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator(CConfiguration *config);


			// protected methods
			protected:
				virtual QDomElement getFirstReasoningRequestNode(QDomDocument& document);
				virtual QDomElement getSecondReasoningRequestNode(QDomDocument& document);

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREALIZATIONREQUESTGENERATOR_H

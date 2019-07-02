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

#ifndef KONCLUDE_TEST_COWLFILEOWLLINKSATISFIABILITYREQUESTGENERATOR_H
#define KONCLUDE_TEST_COWLFILEOWLLINKSATISFIABILITYREQUESTGENERATOR_H

// Libraries includes
#include <QProcess>
#include <QDir>
#include <QDomDocument>
#include <iostream>

// Namespace includes


// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Reasoner/Generator/CConcreteOntologyEntityCollectorBuilder.h"
#include "Parser/COWL2QtXMLOntologyStreamParser.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


using namespace std;


namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Reasoner::Generator;
	using namespace Parser;

	namespace Test {


		/*! 
		 *
		 *		\class		COWLFileOWLlinkSatisfiabilityRequestGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLFileOWLlinkSatisfiabilityRequestGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLFileOWLlinkSatisfiabilityRequestGenerator(CConfiguration *config);

				//! Destructor
				virtual ~COWLFileOWLlinkSatisfiabilityRequestGenerator();

				bool generateOWLlinkConsistencyRequests(const QString& requestFileName, const QString& responseFileName, cint64 testCount);
				bool generateOWLlinkConsistencyRequest(const QString& requestFileName, const QString& responseFileName, const QString& className);

			// protected methods
			protected:
				QDomDocument createRequestDocument();
				QString getClassNameRequestFileAppendix(const QString& className, cint64 testNumber);
				QSet<QString> loadClassEntities(const QString& requestFileName);

			// protected variables
			protected:
				CConfiguration* mConfig;

				QString mReasonerTestKBName;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLFILEOWLLINKSATISFIABILITYREQUESTGENERATOR_H

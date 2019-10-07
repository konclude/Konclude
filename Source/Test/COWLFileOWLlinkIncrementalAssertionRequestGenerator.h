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

#ifndef KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREQUESTGENERATOR_H
#define KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREQUESTGENERATOR_H

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
		 *		\class		COWLFileOWLlinkIncrementalAssertionRequestGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLFileOWLlinkIncrementalAssertionRequestGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLFileOWLlinkIncrementalAssertionRequestGenerator(CConfiguration *config);

				//! Destructor
				virtual ~COWLFileOWLlinkIncrementalAssertionRequestGenerator();

				bool generateOWLlinkIncrementalAssertionRequests(const QString& requestFileName, const QString& responseFileName, double removeAssertionPercentage, double addAssertionPercentage, cint64 testCount);
				bool generateOWLlinkIncrementalAssertionRequests(const QString& requestFileName, const QString& responseFileName, cint64 removeAssertionCount, cint64 addAssertionCount, cint64 testCount);
				bool generateOWLlinkIncrementalAssertionConsistencyRequest(const QString& requestFileName, const QString& responseFileName, QList<QDomElement>& removalAssertionList, QList<QDomElement>& additionAssertionList);

			// protected methods
			protected:
				QDomDocument createRequestDocument();

				QList<QDomElement> loadOntologyAssertions(const QString& requestFileName);

				virtual QDomElement getFirstReasoningRequestNode(QDomDocument& document) = 0;
				virtual QDomElement getSecondReasoningRequestNode(QDomDocument& document) = 0;


			// protected variables
			protected:
				CConfiguration* mConfig;

				QString mReasonerTestKBName;
				QString mRequestTypeName;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLFILEOWLLINKINCREMENTALASSERTIONREQUESTGENERATOR_H

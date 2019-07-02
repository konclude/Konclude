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

#ifndef KONCLUDE_TEST_COWLLINKAPIREASONERTESTFILETESTER_H
#define KONCLUDE_TEST_COWLLINKAPIREASONERTESTFILETESTER_H

// Libraries includes
#include <QProcess>
#include <QDir>
#include <QDomDocument>
#include <iostream>

// Namespace includes
#include "CReasonerTestfileEvaluator.h"

// Other includes
#include "Test/Events/CTestTestfileEvent.h"
#include "Test/Events/CEvaluateTestfileTestEvent.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Network/HTTP/CQtHttpTransactionManager.h"

#include "Reasoner/Query/CClassHierarchyResult.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


using namespace std;


namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Network::HTTP;
	using namespace Reasoner::Query;

	namespace Test {

		using namespace Test::Events;


		/*! 
		 *
		 *		\class		COWLlinkAPIReasonerTestfileTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLlinkAPIReasonerTestfileTester : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLlinkAPIReasonerTestfileTester(CConfiguration *config);

				//! Destructor
				virtual ~COWLlinkAPIReasonerTestfileTester();

				void startTestfileTest(const QString& requestFileName, const QString& responseFileName);

			// protected methods
			protected:
				bool modifyRequestDomStructure(QDomElement& el);
				void modifyResponseDomStructure(QDomElement& el, cint64 respTime);

				CClassHierarchyResult* parseClassHierarchyResult(QDomElement *node);
				CClassHierarchyResult* parseClassHierarchyResultRelocatedTOP(QDomElement *node);
				CClassSynsetResult* parseClassSynsetResult(QDomElement *node);

				bool hasClassSynsetTOP(QDomElement *node);
				QStringList parseClassNames(QDomElement *node);

				bool renameAbbreviatedClassSynset(QDomElement *node, const QString& abbIRI, const QString& replaceIRI);

			// protected variables
			protected:
				CQtHttpTransactionManager* mTransManager;
				QString mAddressString;
				bool mWriteResponseStdOut;
				bool mModifyRequest;
				bool mModifyRequestAbsolutePaths;
				bool mModifyResponseTiming;
				CConfiguration* mConfig;

				CClassHierarchyResult* mExpectedResult;

			// private methods
			private:

			// private methods
			private slots:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLLINKAPIREASONERTESTFILETESTER_H

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

#ifndef KONCLUDE_TEST_CONFORMANCE_COWLLINKBOOLEANTASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_COWLLINKBOOLEANTASKHANDLER_H

// Namespace includes
#include <QList>

#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		COWLLinkBooleanTaskHandler
			 *		\brief		Runs an OWLlink request file with 'Konclude owllinkfile' and
			 *					evaluates every BooleanResponse of the XML ResponseMessage
			 *					that the binary writes for '-o'.
			 *
			 *					An OWLlink request message may contain several requests and
			 *					therefore produces several responses; all of them are checked,
			 *					in document order. The expected verdicts are declared with one
			 *					kt:expectedResult per response, or with a single one that then
			 *					applies to every response. The responses are parsed with the
			 *					same COWLlinkQtXMLResultParser that the evaluation code uses.
			 */
			class COWLLinkBooleanTaskHandler : public CConformanceTaskHandler {
				// public methods
				public:
					COWLLinkBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~COWLLinkBooleanTaskHandler();

					virtual CConformanceTask::TYPE getTaskType() const;

				// protected methods
				protected:
					virtual QStringList buildArguments(const QString& responseFilePath);
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput);

					//! Live channel: 'owllinkserver -p <port>'.
					virtual QStringList buildServerArguments(quint16 port);

					//! Reads the result attribute of every BooleanResponse element, in
					//! document order. Returns false when the text is not parseable or
					//! contains no BooleanResponse.
					static bool parseBooleanResponses(const QString& responseText, QList<CConformanceTask::VERDICT>& verdicts, QString& errorMessage);

					//! Compares the observed answers with the declared expectations.
					CConformanceTaskResult compareVerdicts(const QList<CConformanceTask::VERDICT>& actualVerdicts) const;
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_COWLLINKBOOLEANTASKHANDLER_H

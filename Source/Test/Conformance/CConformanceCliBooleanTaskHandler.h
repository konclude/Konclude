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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECLIBOOLEANTASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECLIBOOLEANTASKHANDLER_H

// Namespace includes
#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceCliBooleanTaskHandler
			 *		\brief		Executes the command line tasks that answer with a boolean and
			 *					write it to the response file: 'consistency' and
			 *					'satisfiability'.
			 *
			 *					Konclude's CLI loaders (CCLIConsistencyBatchProcessingLoader,
			 *					CCLISatisfiabilityBatchProcessingLoader) write the plain token
			 *					"true" or "false" followed by a newline into the file passed
			 *					with '-o'. That file is the answer source; the trailing
			 *					newline is trimmed before parsing. An OWLlink/OWL-XML style
			 *					<BooleanResponse result="..."/> document is accepted as well,
			 *					so a future loader that writes XML needs no change here.
			 *
			 *					The console log is only consulted as an additional diagnostic
			 *					when the response file is missing or does not contain a
			 *					boolean, because the log wording is user-facing and not a
			 *					stable interface.
			 */
			class CConformanceCliBooleanTaskHandler : public CConformanceTaskHandler {
				// public methods
				public:
					CConformanceCliBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~CConformanceCliBooleanTaskHandler();

					virtual CConformanceTask::TYPE getTaskType() const;

				// protected methods
				protected:
					virtual QStringList buildArguments(const QString& responseFilePath);
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput);

					//! Interprets the 'true'/'false' text (or a BooleanResponse element)
					//! written by the CLI loaders. Returns false when the text is not a
					//! boolean response.
					static bool parseBooleanResponse(const QString& responseText, CConformanceTask::VERDICT& verdict);

					//! Derives the verdict from the console output as a diagnostic fallback.
					//! Returns false when the output contains no statement about the result.
					bool verdictFromLogOutput(const QString& logOutput, CConformanceTask::VERDICT& verdict) const;
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECLIBOOLEANTASKHANDLER_H

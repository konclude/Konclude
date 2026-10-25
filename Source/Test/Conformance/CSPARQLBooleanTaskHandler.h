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

#ifndef KONCLUDE_TEST_CONFORMANCE_CSPARQLBOOLEANTASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_CSPARQLBOOLEANTASKHANDLER_H

// Namespace includes
#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CSPARQLBooleanTaskHandler
			 *		\brief		Runs a SPARQL ASK test and evaluates the boolean result.
			 *
			 *					File channel: 'sparqlfile -i <dataset> -s <query> -o <response>'
			 *					Live channel: 'sparqlserver -p <port> -i <dataset>' and the query
			 *					is posted to the server.
			 *
			 *					kt:usesOntology is the query file, kt:usesDataset the ontology
			 *					loaded before the query is answered. The answer is the
			 *					SPARQL Results XML document with a boolean element.
			 */
			class CSPARQLBooleanTaskHandler : public CConformanceTaskHandler {
				// public methods
				public:
					CSPARQLBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~CSPARQLBooleanTaskHandler();

					virtual CConformanceTask::TYPE getTaskType() const;

				// protected methods
				protected:
					virtual QStringList buildArguments(const QString& responseFilePath);
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput);

					//! Live channel: 'sparqlserver -p <port>'.
					virtual QStringList buildServerArguments(quint16 port);

					//! Reads the boolean element of a SPARQL Results XML document.
					static bool parseBooleanAnswer(const QString& responseText, CConformanceTask::VERDICT& verdict, QString& errorMessage);

					//! Builds the task result from an answer document.
					CConformanceTaskResult interpretAnswer(const QString& responseText) const;
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CSPARQLBOOLEANTASKHANDLER_H

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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLER_H

// Libraries includes
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QProcess>

// Namespace includes
#include "CConformanceTestCase.h"
#include "CConformanceTaskResult.h"

// Logger includes
#include "Logger/CLogIdentifier.h"
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTaskHandler
			 *		\brief		Executes one kind of conformance task with the Konclude binary
			 *					as a subprocess and interprets the answer.
			 *
			 *					The base class owns everything that is identical for all tasks
			 *					(process start, timeout handling, log capture, configuration
			 *					argument). A concrete handler only builds the task specific
			 *					command line and reads the answer from the response file that
			 *					the binary was asked to write.
			 *
			 *					Extending the runner with a new task means adding a
			 *					%ConformanceTask::TYPE value, a handler subclass and a case in
			 *					CConformanceTaskHandlerFactory; no existing handler changes.
			 */
			class CConformanceTaskHandler : public CLogIdentifier {
				// public methods
				public:
					CConformanceTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~CConformanceTaskHandler();

					//! Task kind this handler executes.
					virtual CConformanceTask::TYPE getTaskType() const = 0;

					//! Executes the task once. When outputLogFilePath is non-empty the
					//! subprocess' merged standard output and error are captured directly
					//! into that file. The reasoner answer is read from responseFilePath.
					CConformanceTaskResult run(qint64 timeoutMs, const QString& outputLogFilePath, const QString& responseFilePath);

				// protected methods
				protected:
					//! Builds the Konclude command line. Implementations have to pass the
					//! input ontology and '-o <responseFilePath>' so that the answer can be
					//! read back from a file instead of the console output.
					virtual QStringList buildArguments(const QString& responseFilePath) = 0;

					//! Reads the reasoner answer from the response file written by the
					//! binary. logOutput is the captured console output and may be used
					//! only as an additional diagnostic.
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput) = 0;

					//! Command line of the server process for a server channel, e.g.
					//! 'owllinkserver -p <port>'. Empty for handlers that only support
					//! file channels.
					virtual QStringList buildServerArguments(quint16 port);

					//! Appends '-c <file>' when a reasoner configuration is configured.
					void appendConfigArgument(QStringList& arguments) const;

					//! Compares the actual answer file with the expected result file
					//! (kt:expectedFile) using the semantic XML comparator.
					CConformanceTaskResult compareWithExpectedFile(const QString& responseFilePath) const;

					//! Reads the complete text file. Returns false when it cannot be read.
					static bool readTextFile(const QString& filePath, QString& text);

					//! Returns the captured console output of the finished process,
					//! either from the log file or from the in-memory channel buffers.
					static QString readLogOutput(const QString& outputLogFilePath, QProcess& process);

				// protected variables
				protected:
					const CConformanceTestCase& mTestCase;
					QString mBinaryFile;
					QString mConfigFile;

				// private methods
				private:
					//! Executes the task through the command line and reads '-o'.
					CConformanceTaskResult runFileChannel(qint64 timeoutMs, const QString& outputLogFilePath, const QString& responseFilePath);

					//! Starts the server, waits for the port, posts the request, writes the
					//! answer to responseFilePath and stops the server again.
					CConformanceTaskResult runServerChannel(qint64 timeoutMs, const QString& outputLogFilePath, const QString& responseFilePath);

					//! Asks the operating system for a currently unused TCP port.
					static quint16 findFreeTcpPort();

					//! Waits until the given local port accepts connections.
					static bool waitForTcpPort(quint16 port, qint64 timeoutMs);

					//! Sends a synchronous HTTP POST and returns the response body.
					static bool postHttpRequest(const QString& url, const QByteArray& requestBody, const QString& contentType, qint64 timeoutMs, QString& responseText, QString& errorText);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLER_H

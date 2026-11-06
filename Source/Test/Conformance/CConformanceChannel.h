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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECHANNEL_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECHANNEL_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CConformanceTask.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceChannel
			 *		\brief		Transport over which a task is executed and answered.
			 *
			 *					The task kind says *what* is asked, the channel says *how* it
			 *					is asked: a command line call writing a response file, an
			 *					OWLlink request file, a live OWLlink HTTP server, a SPARQL
			 *					query file or a live SPARQL HTTP server.
			 *
			 *					The channel is declared with kt:onChannel in a test
			 *					description (kt:CommandLine, kt:OWLlink, kt:OWLlinkServer,
			 *					kt:SPARQL, kt:SPARQLServer) and defaults to the file channel
			 *					of the task kind when it is omitted.
			 */
			class CConformanceChannel {
				// public types
				public:
					enum class TYPE {
						COMMAND_LINE,			//!< Konclude CLI command, answer in a response file
						OWLLINK_FILE,			//!< OWLlink request file, answer in a response file
						OWLLINK_SERVER,			//!< live OWLlink HTTP server, answer as HTTP response
						SPARQL_FILE,			//!< SPARQL query file, answer in a response file
						SPARQL_SERVER			//!< live SPARQL HTTP server, answer as HTTP response
					};

				// public methods
				public:
					//! Canonical label of the channel, also used as interface name in
					//! reports ("CommandLine", "OWLlink", "OWLlinkServer", "SPARQL",
					//! "SPARQLServer").
					static QString typeToString(TYPE type);

					//! Parses the name used in kt:onChannel. Returns false for unknown names.
					static bool typeFromOntologyName(const QString& name, TYPE& type);

					//! Channel used for a task when the description does not declare one.
					static TYPE typeDefaultForTask(CConformanceTask::TYPE task);

					//! True when the channel can execute the task kind.
					static bool typeIsSupportedForTask(TYPE channel, CConformanceTask::TYPE task);

					//! True for channels that talk to a running server over HTTP.
					static bool typeIsServer(TYPE channel);
			};


			inline QString CConformanceChannel::typeToString(TYPE type) {
				switch (type) {
					case TYPE::COMMAND_LINE: return QString("CommandLine");
					case TYPE::OWLLINK_FILE: return QString("OWLlink");
					case TYPE::OWLLINK_SERVER: return QString("OWLlinkServer");
					case TYPE::SPARQL_FILE: return QString("SPARQL");
					case TYPE::SPARQL_SERVER: return QString("SPARQLServer");
				}
				return QString();
			}


			inline bool CConformanceChannel::typeFromOntologyName(const QString& name, TYPE& type) {
				QString localName = CConformanceTask::localName(name).trimmed();
				if (localName.compare("CommandLine", Qt::CaseInsensitive) == 0) {
					type = TYPE::COMMAND_LINE;
					return true;
				}
				if (localName.compare("OWLlink", Qt::CaseInsensitive) == 0) {
					type = TYPE::OWLLINK_FILE;
					return true;
				}
				if (localName.compare("OWLlinkServer", Qt::CaseInsensitive) == 0) {
					type = TYPE::OWLLINK_SERVER;
					return true;
				}
				if (localName.compare("SPARQL", Qt::CaseInsensitive) == 0) {
					type = TYPE::SPARQL_FILE;
					return true;
				}
				if (localName.compare("SPARQLServer", Qt::CaseInsensitive) == 0) {
					type = TYPE::SPARQL_SERVER;
					return true;
				}
				return false;
			}


			inline CConformanceChannel::TYPE CConformanceChannel::typeDefaultForTask(CConformanceTask::TYPE task) {
				switch (task) {
					case CConformanceTask::TYPE::CONSISTENCY:
					case CConformanceTask::TYPE::SATISFIABILITY:
					case CConformanceTask::TYPE::CLASS_CLASSIFICATION:
					case CConformanceTask::TYPE::CLASS_REALIZATION:
						return TYPE::COMMAND_LINE;
					case CConformanceTask::TYPE::OWLLINK:
						return TYPE::OWLLINK_FILE;
					case CConformanceTask::TYPE::SPARQL:
						return TYPE::SPARQL_FILE;
				}
				return TYPE::COMMAND_LINE;
			}


			inline bool CConformanceChannel::typeIsSupportedForTask(TYPE channel, CConformanceTask::TYPE task) {
				switch (channel) {
					case TYPE::COMMAND_LINE:
						return task == CConformanceTask::TYPE::CONSISTENCY
								|| task == CConformanceTask::TYPE::SATISFIABILITY
								|| task == CConformanceTask::TYPE::CLASS_CLASSIFICATION
								|| task == CConformanceTask::TYPE::CLASS_REALIZATION;
					case TYPE::OWLLINK_FILE:
					case TYPE::OWLLINK_SERVER:
						return task == CConformanceTask::TYPE::OWLLINK;
					case TYPE::SPARQL_FILE:
					case TYPE::SPARQL_SERVER:
						return task == CConformanceTask::TYPE::SPARQL;
				}
				return false;
			}


			inline bool CConformanceChannel::typeIsServer(TYPE channel) {
				return channel == TYPE::OWLLINK_SERVER || channel == TYPE::SPARQL_SERVER;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCECHANNEL_H

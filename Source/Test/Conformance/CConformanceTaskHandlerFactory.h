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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLERFACTORY_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLERFACTORY_H

// Namespace includes
#include "CConformanceTestCase.h"
#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTaskHandlerFactory
			 *		\brief		Creates the handler that executes a given task kind over a given
			 *					channel.
			 *
			 *					This is the single place that maps a
			 *					%ConformanceTask::TYPE plus a %ConformanceChannel::TYPE to a
			 *					concrete handler class. Supporting a new task means adding one
			 *					case here and one handler subclass; existing handlers stay
			 *					untouched.
			 */
			class CConformanceTaskHandlerFactory {
				// public methods
				public:
					//! Creates the handler for the task/channel combination. The caller
					//! owns the returned object. Returns nullptr when the combination is
					//! not supported.
					static CConformanceTaskHandler* createHandler(CConformanceTask::TYPE taskType, CConformanceChannel::TYPE channel, const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKHANDLERFACTORY_H

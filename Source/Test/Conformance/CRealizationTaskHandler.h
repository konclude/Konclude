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

#ifndef KONCLUDE_TEST_CONFORMANCE_CREALIZATIONTASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_CREALIZATIONTASKHANDLER_H

// Namespace includes
#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CRealizationTaskHandler
			 *		\brief		Runs 'Konclude realization' (class realization: the
			 *					computed individual types) for a test ontology and compares
			 *					them against the expected OWL/XML result of the test case.
			 *
			 *					The comparison is delegated to CRealizationResultComparator,
			 *					which ignores the order of the class assertions.
			 */
			class CRealizationTaskHandler : public CConformanceTaskHandler {
				// public methods
				public:
					CRealizationTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~CRealizationTaskHandler();

					virtual CConformanceTask::TYPE getTaskType() const;

				// protected methods
				protected:
					virtual QStringList buildArguments(const QString& responseFilePath);
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CREALIZATIONTASKHANDLER_H

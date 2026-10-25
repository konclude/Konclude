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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONTASKHANDLER_H
#define KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONTASKHANDLER_H

// Namespace includes
#include "CConformanceTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CClassificationTaskHandler
			 *		\brief		Runs 'Konclude classification' for a test ontology and compares
			 *					the systematically written taxonomy against the expected
			 *					OWL/XML result of the test case.
			 *
			 *					The comparison is delegated to CClassificationResultComparator,
			 *					which ignores the order of the subsumption assertions.
			 */
			class CClassificationTaskHandler : public CConformanceTaskHandler {
				// public methods
				public:
					CClassificationTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile);
					virtual ~CClassificationTaskHandler();

					virtual CConformanceTask::TYPE getTaskType() const;

				// protected methods
				protected:
					virtual QStringList buildArguments(const QString& responseFilePath);
					virtual CConformanceTaskResult interpretResponse(const QString& responseFilePath, const QString& logOutput);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCLASSIFICATIONTASKHANDLER_H

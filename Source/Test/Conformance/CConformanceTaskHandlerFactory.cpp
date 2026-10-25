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

#include "CConformanceTaskHandlerFactory.h"

#include "CConformanceCliBooleanTaskHandler.h"
#include "CClassificationTaskHandler.h"
#include "COWLLinkBooleanTaskHandler.h"
#include "CSPARQLBooleanTaskHandler.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CConformanceTaskHandler* CConformanceTaskHandlerFactory::createHandler(CConformanceTask::TYPE taskType, CConformanceChannel::TYPE channel, const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile) {
				if (!CConformanceChannel::typeIsSupportedForTask(channel, taskType)) {
					return nullptr;
				}

				switch (taskType) {
					case CConformanceTask::TYPE::CONSISTENCY:
					case CConformanceTask::TYPE::SATISFIABILITY:
						// Both tasks are command line queries that answer with a boolean
						// in the response file.
						return new CConformanceCliBooleanTaskHandler(testCase, binaryFile, configFile);

					case CConformanceTask::TYPE::CLASSIFICATION:
						return new CClassificationTaskHandler(testCase, binaryFile, configFile);

					case CConformanceTask::TYPE::OWLLINK:
						// One handler for the file and the live-server channel; the base
						// class decides how the request is transported.
						return new COWLLinkBooleanTaskHandler(testCase, binaryFile, configFile);

					case CConformanceTask::TYPE::SPARQL:
						return new CSPARQLBooleanTaskHandler(testCase, binaryFile, configFile);
				}
				return nullptr;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

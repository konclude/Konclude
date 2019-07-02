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

#ifndef KONCLUDE_Control_LOADER_CDEFAULTLOADERFACTORY_H
#define KONCLUDE_Control_LOADER_CDEFAULTLOADERFACTORY_H

// Libraries includes
#include <QString>
#include <QLinkedList>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CCommandLineLoader.h"
#include "CCoutLogObserverLoader.h"
#include "CDefaultReasonerLoader.h"
#include "COWLlinkBatchFileLoader.h"
#include "CAnalyseReasonerLoader.h"
#include "CExperimentalReasonerLoader.h"
#include "CCommandLineTranlatorHelpLoader.h"
#include "CConfigurableCoutLogObserverLoader.h"
#include "CTestHashModificationLoader.h"
#include "CTestTaskThroughputLoader.h"
#include "CTestQHashVsQMapLoader.h"
#include "CPigenholePrincipleGeneratorLoader.h"
#include "CTestOWLlinkAPIBatchFileLoader.h"
#include "CTestMemoryAccessLoader.h"
#include "COWLFilesIncompatibleAxiomChangerLoader.h"
#include "CReasonerEvaluationLoader.h"
#include "COWLFilesOWLlinkTestcaseGeneratorLoader.h"
#include "CReasonerEvaluationClassificationTimesComparerLoader.h"
#include "COWLlinkHttpServerLoader.h"
#include "CReasonerEvaluationLoadingTimesComparerLoader.h"
#include "CRecursionTestDataGeneratorLoader.h"
#include "CLargeJoinTestDataGeneratorLoader.h"
#include "CTransitiveConnectedELClassesTestDataGeneratorLoader.h"
#include "CReasonerEvaluationProgramLoader.h"
#include "COREBatchProcessingLoader.h"
#include "CORECoutLogObserverLoader.h"
#include "CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader.h"
#include "CReasonerEvaluationOntologyExtractionLoader.h"
#include "CCLIClassificationBatchProcessingLoader.h"
#include "CCLIConsistencyBatchProcessingLoader.h"
#include "CCLISatisfiabilityBatchProcessingLoader.h"
#include "CCLIRealizationBatchProcessingLoader.h"
#include "CLoggerConfigurationLoader.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Control/Command/CReasonerConfigurationGroup.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Config;

	namespace Control {

		using namespace Command;

		namespace Loader {

			/*! 
			 *
			 *		\class		CDefaultLoaderFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDefaultLoaderFactory : public CLoaderFactory {
				// public methods
				public:
					//! Constructor
					CDefaultLoaderFactory();

					//! Destructor
					virtual ~CDefaultLoaderFactory();

					virtual CLoader *createLoaderFromName(const QString &loaderName, CConfiguration *config = 0);
					virtual bool canCreateLoaderFromName(const QString &loaderName, CConfiguration *config = 0);


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_CDEFAULTLOADERFACTORY_H

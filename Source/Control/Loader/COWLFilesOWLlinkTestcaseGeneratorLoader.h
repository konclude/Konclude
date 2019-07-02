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

#ifndef KONCLUDE_CONTROL_LOADER_COWLFILESOWLLINKTESTCASEGENERATORLOADER_H
#define KONCLUDE_CONTROL_LOADER_COWLFILESOWLLINKTESTCASEGENERATORLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Test/COWLFileOWLlinkClassifyRequestGenerator.h"
#include "Test/COWLFileOWLlinkConsistencyRequestGenerator.h"
#include "Test/COWLFileOWLlinkSatisfiabilityRequestGenerator.h"
#include "Test/COWLFileOWLlinkTypesRequestGenerator.h"
#include "Test/COWLFileOWLlinkIncrementalAssertionConsistencyRequestGenerator.h"
#include "Test/COWLFileOWLlinkIncrementalAssertionClassificationRequestGenerator.h"
#include "Test/COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Test;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		COWLFilesOWLlinkTestcaseGeneratorLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COWLFilesOWLlinkTestcaseGeneratorLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					COWLFilesOWLlinkTestcaseGeneratorLoader();

					//! Destructor
					virtual ~COWLFilesOWLlinkTestcaseGeneratorLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:
					virtual CLoader *generateTestcasesForDirectory(const QString& dirString, const QString& subDirPathString);

					
				// protected variables
				protected:
					QString mInputDir;
					QString mOutputDir;
					QString mFilterDir;
					QString mOutputDirClassify;
					QString mOutputDirConsistency;
					QString mOutputDirSatisfiability;
					QString mOutputDirTypes;


					cint64 mSatisfiabilityTestCount;
					cint64 mTypesTestCount;


					cint64 mIncAssConsTestCount;
					cint64 mIncAssConsistencyAdditionCount;
					cint64 mIncAssConsistencyRemovalCount;
					cint64 mIncAssConsistencyAdditionCountRatioDenominator;
					cint64 mIncAssConsistencyRemovalCountRatioDenominator;
					bool mConfCreateIncAssConsistencyTests;
					bool mConfCreateIncAssConsistencyByRatio;
					QString mOutputDirIncAssConsistency;


					cint64 mIncAssClassificationTestCount;
					cint64 mIncAssClassificationAdditionCount;
					cint64 mIncAssClassificationRemovalCount;
					cint64 mIncAssClassificationAdditionCountRatioDenominator;
					cint64 mIncAssClassificationRemovalCountRatioDenominator;
					bool mConfCreateIncAssClassificationTests;
					bool mConfCreateIncAssClassificationByRatio;
					QString mOutputDirIncAssClassification;



					cint64 mIncAssRealizationTestCount;
					cint64 mIncAssRealizationAdditionCount;
					cint64 mIncAssRealizationRemovalCount;
					cint64 mIncAssRealizationAdditionCountRatioDenominator;
					cint64 mIncAssRealizationRemovalCountRatioDenominator;
					bool mConfCreateIncAssRealizationTests;
					bool mConfCreateIncAssRealizationByRatio;
					QString mOutputDirIncAssRealization;


					bool mConfCreateClassifyTests;
					bool mConfCreateConsistencyTests;
					bool mConfCreateSatisfiabilityTests;
					bool mConfCreateTypeTests;



					CConfiguration *loaderConfig;

					COWLFileOWLlinkClassifyRequestGenerator* mClassifyGen;
					COWLFileOWLlinkConsistencyRequestGenerator* mConsistencyGen;
					COWLFileOWLlinkSatisfiabilityRequestGenerator* mSatisfiabilityGen;
					COWLFileOWLlinkTypesRequestGenerator* mTypesGen;
					COWLFileOWLlinkIncrementalAssertionConsistencyRequestGenerator* mIncAssConsistencyGen;
					COWLFileOWLlinkIncrementalAssertionClassificationRequestGenerator* mIncAssClassificationGen;
					COWLFileOWLlinkIncrementalAssertionRealizationRequestGenerator* mIncAssRealizationGen;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_COWLFILESOWLLINKTESTCASEGENERATORLOADER_H

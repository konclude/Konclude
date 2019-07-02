/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_CONTROL_LOADER_CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader_H
#define KONCLUDE_CONTROL_LOADER_CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QDomElement>
#include <QDir>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Config;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader();

					//! Destructor
					virtual ~CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:
					CLoader *collectTestcasesStatisticsForDirectory(const QString& dirString, const QString& subDirPathString);

					CLoader *collectTestcasesStatisticsForTestcase(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString);
					QStringList collectOntologyIRIs(const QDomElement& element);
					QStringList collectOntologyIRIsStatistics(const QStringList& ontoIRIList);

				// protected variables
				protected:
					QString mInputDir;

					QString mOntologyStatisticsDir;
					QString mResponseStatisticsOutputDir;

					CConfiguration *loaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader_H

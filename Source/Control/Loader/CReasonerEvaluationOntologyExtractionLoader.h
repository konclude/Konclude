/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONONTOLOGYEXTRACTIONLOADER_H
#define KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONONTOLOGYEXTRACTIONLOADER_H

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

#include "Test/Evaluation/CReasonerEvaluationFiltering.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Test;
	using namespace Evaluation;

	namespace Control {

		namespace Loader {



			/*! 
			 *
			 *		\class		CReasonerEvaluationOntologyExtractionLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationOntologyExtractionLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationOntologyExtractionLoader();

					//! Destructor
					virtual ~CReasonerEvaluationOntologyExtractionLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:
					CLoader* extractOntologiesForDirectory(const QString& dirString, const QString& subDirPathString);

					CLoader* extractOntologiesForTestcase(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString);


					bool satisfiesExtractionCondition(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList);


					bool loadFilterConditions(const QString& fileNameString);


					CLoader* addExtractedOntologyStatistics(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList);
					CLoader* copyExtractedOntology(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString);


					CLoader* saveExtractedOntologyStatistics(const QString& fileNameString);

				// protected variables
				protected:
					QString mInputDir;
					QString mOutputDir;


					QString mExtractedOntStatOutputFileString;


					QString mOntologyStatisticsDir;
					QString mFilterFile;


					CReasonerEvaluationFiltering* mFiltering;

					bool mCopyExtractedOntologyToOutput;
					bool mCollectExtractedOntologyStatistics;

					QList< QHash<QString,QString>* > mExtractedOntStatHashList;
					QSet<QString> mExtractedStatsSet;
					QList<QString> mExtractedStatsList;


					CConfiguration *loaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONONTOLOGYEXTRACTIONLOADER_H

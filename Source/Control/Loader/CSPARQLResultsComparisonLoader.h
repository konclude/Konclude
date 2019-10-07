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

#ifndef KONCLUDE_CONTROL_LOADER_CSPARQLRESULTSCOMPARISONLOADER_H
#define KONCLUDE_CONTROL_LOADER_CSPARQLRESULTSCOMPARISONLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Reasoner/Query/CVariableBindingsAnswersSetResult.h"

#include "Parser/CSPARQLQtXMLResultParser.h"

#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Config;
	using namespace Logger;
	using namespace Parser;
	using namespace Reasoner::Query;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CSPARQLResultsComparisonLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSPARQLResultsComparisonLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CSPARQLResultsComparisonLoader();

					//! Destructor
					virtual ~CSPARQLResultsComparisonLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:
					bool isVariableBindingResultSimilarTo(const QString& responseFileString, const QString& otherResponseFileString, const QString& reportingFileName);
					bool isVariableBindingResultSimilarTo(CVariableBindingsAnswersSetResult* varBindingResult, cint64 nodeNumber, const QString& otherResponseFileString, QHash<QString, cint64>* variableNameIndexHash, const QString& reportingFileName);

					
				// protected variables
				protected:
					QString mReqFileString;
					QString mCompFileString;
					QString mResFileString;

					CLogIdentifier mLogIdentifier;

					cint64 mDifferencesLoggingLimit;

					CSPARQLQtXMLResultParser mSPARQLResultParser;

					CConfiguration *loaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CSPARQLRESULTSCOMPARISONLOADER_H

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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSERCHECKER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSERCHECKER_H

// Libraries includes
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationAnalyserChecker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationAnalyserChecker : public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationAnalyserChecker();

					//! Destructor
					virtual ~CReasonerEvaluationAnalyserChecker();


					bool checkAnalysingUpdateNecessary(const QStringList& reasonerPathList, const QString& analyserCheckFileString, cint64 maxAnalyseCount);

					bool saveAnalysingUpdateCheckFile(const QString& analyserCheckFileString);


				// protected methods
				protected:
					bool checkDirsAnalysingUpdateNecessary(const QString& directory, const QString& baseDirectory);
					bool checkFilesAnalysingUpdateNecessary(const QString& directory, const QString& requestFileString);
					bool checkAnalysingUpdateNecessary(const QString& baseDirectory);

					bool loadAnalysingUpdateCheckFile(const QString& analyserCheckFileString);

				// protected variables
				protected:

				// private methods
				private:
					QHash<QString,cint64> mAnalysedResponseCountHash;
					cint64 mMaxAnalyseCount;

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSERCHECKER_H

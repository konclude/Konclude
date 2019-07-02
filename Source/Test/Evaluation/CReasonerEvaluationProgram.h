/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROGRAM_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROGRAM_H

// Libraries includes
#include <QString>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationProgram
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationProgram {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationProgram();


					CReasonerEvaluationProgram* setConfigFileString(const QString& configFileString);
					CReasonerEvaluationProgram* setInputDirectoryString(const QString& inputString);
					CReasonerEvaluationProgram* setOutputDirectoryString(const QString& outputString);
					CReasonerEvaluationProgram* setAnalyserDirectoryString(const QString& analyserString);
					CReasonerEvaluationProgram* setProgramName(const QString& programName);


					CReasonerEvaluationProgram* appendReasonerString(const QString& reasonerString);
					CReasonerEvaluationProgram* appendAnalyserString(const QString& analyserString);
					CReasonerEvaluationProgram* appendFilterString(const QString& filterString);

					QString getConfigFileString();
					QString getInputDirectoryString();
					QString getOutputDirectoryString();

					QString getAnalyserDirectoryString();
					QString getProgramName();


					QStringList getReasonerStringList();
					QStringList getAnalyserStringList();
					QStringList getFilterStringList();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mProgramName;
					QString mConfigFileString;
					QString mInputDirString;
					QString mOutputDirString;
					QString mAnalyserDirString;
					QStringList mReasonerStringList;
					QStringList mAnalyserStringList;
					QStringList mFilterStringList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROGRAM_H

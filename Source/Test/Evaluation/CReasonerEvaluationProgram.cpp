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

#include "CReasonerEvaluationProgram.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationProgram::CReasonerEvaluationProgram() {
			}



			CReasonerEvaluationProgram* CReasonerEvaluationProgram::setConfigFileString(const QString& configFileString) {
				mConfigFileString = configFileString;
				return this;
			}


			CReasonerEvaluationProgram* CReasonerEvaluationProgram::setInputDirectoryString(const QString& inputString) {
				mInputDirString = inputString;
				return this;
			}

			CReasonerEvaluationProgram* CReasonerEvaluationProgram::setOutputDirectoryString(const QString& outputString) {
				mOutputDirString = outputString;
				return this;
			}


			CReasonerEvaluationProgram* CReasonerEvaluationProgram::setAnalyserDirectoryString(const QString& analyserString) {
				mAnalyserDirString = analyserString;
				return this;
			}

			CReasonerEvaluationProgram* CReasonerEvaluationProgram::setProgramName(const QString& programName) {
				mProgramName = programName;
				return this;
			}


			CReasonerEvaluationProgram* CReasonerEvaluationProgram::appendReasonerString(const QString& reasonerString) {
				mReasonerStringList.append(reasonerString);
				return this;
			}

			CReasonerEvaluationProgram* CReasonerEvaluationProgram::appendAnalyserString(const QString& analyserString) {
				mAnalyserStringList.append(analyserString);
				return this;
			}


			CReasonerEvaluationProgram* CReasonerEvaluationProgram::appendFilterString(const QString& filterString) {
				mFilterStringList.append(filterString);
				return this;
			}


			QString CReasonerEvaluationProgram::getConfigFileString() {
				return mConfigFileString;
			}

			QString CReasonerEvaluationProgram::getInputDirectoryString() {
				return mInputDirString;
			}

			QString CReasonerEvaluationProgram::getOutputDirectoryString() {
				return mOutputDirString;
			}


			QStringList CReasonerEvaluationProgram::getReasonerStringList() {
				return mReasonerStringList;
			}

			QStringList CReasonerEvaluationProgram::getAnalyserStringList() {
				return mAnalyserStringList;
			}

			QStringList CReasonerEvaluationProgram::getFilterStringList() {
				return mFilterStringList;
			}


			QString CReasonerEvaluationProgram::getAnalyserDirectoryString() {
				return mAnalyserDirString;
			}

			QString CReasonerEvaluationProgram::getProgramName() {
				return mProgramName;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

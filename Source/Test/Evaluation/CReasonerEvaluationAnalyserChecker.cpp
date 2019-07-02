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

#include "CReasonerEvaluationAnalyserChecker.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationAnalyserChecker::CReasonerEvaluationAnalyserChecker() : CLogIdentifier("::Konclude::Test::Evaluation::AnalyserUpdateChecker",this) {
			}


			CReasonerEvaluationAnalyserChecker::~CReasonerEvaluationAnalyserChecker() {
			}


			bool CReasonerEvaluationAnalyserChecker::loadAnalysingUpdateCheckFile(const QString& analyserCheckFileString) {
				mAnalysedResponseCountHash.clear();
				QFile analyserCheckFile(analyserCheckFileString);
				if (analyserCheckFile.open(QIODevice::ReadOnly)) {
					LOG(INFO,getLogDomain(),logTr("Loading alraedy analysed data from file '%1'.").arg(analyserCheckFileString),this);
					while (!analyserCheckFile.atEnd()) {
						QString analysedCheckLine(analyserCheckFile.readLine());
						QStringList analysedParsedList = analysedCheckLine.trimmed().split(" ");
						if (analysedParsedList.count() == 2) {
							cint64 analysdCount = analysedParsedList.first().toInt();
							QString analysedFileString = analysedParsedList.last();
							if (analysdCount > 0 && !analysedFileString.isEmpty()) {
								mAnalysedResponseCountHash.insert(analysedFileString,analysdCount);
							}
						}
					}
					analyserCheckFile.close();
					return true;
				} else {
					LOG(ERROR,getLogDomain(),logTr("Could not load alraedy analysed data from file '%1'.").arg(analyserCheckFileString),this);
				}
				return false;
			}


			bool CReasonerEvaluationAnalyserChecker::saveAnalysingUpdateCheckFile(const QString& analyserCheckFileString) {
				QFile analyserCheckFile(analyserCheckFileString);
				if (analyserCheckFile.open(QIODevice::WriteOnly)) {
					LOG(INFO,getLogDomain(),logTr("Save updated analysed data to file '%1'.").arg(analyserCheckFileString),this);
					for (QHash<QString,cint64>::const_iterator it = mAnalysedResponseCountHash.constBegin(), itEnd = mAnalysedResponseCountHash.constEnd(); it != itEnd; ++it) {
						QString writeLine = QString("%1 %2\n").arg(it.value()).arg(it.key());
						analyserCheckFile.write(writeLine.toLocal8Bit());
					}
					analyserCheckFile.close();
					return true;
				} else {
					LOG(ERROR,getLogDomain(),logTr("Could not save updated analysed data to file '%1'.").arg(analyserCheckFileString),this);
				}
				return false;
			}


			bool CReasonerEvaluationAnalyserChecker::checkAnalysingUpdateNecessary(const QStringList& reasonerPathList, const QString& analyserCheckFileString, cint64 maxAnalyseCount) {
				mMaxAnalyseCount = maxAnalyseCount;
				bool updateNecessary = false;
				loadAnalysingUpdateCheckFile(analyserCheckFileString);
				foreach (const QString& reasonerPath, reasonerPathList) {
					updateNecessary |= checkAnalysingUpdateNecessary(reasonerPath);
				}
				return updateNecessary;
			}

			bool CReasonerEvaluationAnalyserChecker::checkAnalysingUpdateNecessary(const QString& baseDirectory) {
				return checkDirsAnalysingUpdateNecessary(baseDirectory,baseDirectory);
			}

			bool CReasonerEvaluationAnalyserChecker::checkDirsAnalysingUpdateNecessary(const QString& directory, const QString& baseDirectory) {
				bool updateNecessary = false;
				QDir dir(directory);
				QStringList dirList = dir.entryList(QDir::Dirs,QDir::Name|QDir::IgnoreCase);
				foreach (QString dirString, dirList) {
					if (dirString != "." && dirString != "..") {
						LOG(INFO,getLogDomain(),logTr("Checking analysing update necessary for data from '%1'.").arg(dirString),this);
						updateNecessary |= checkFilesAnalysingUpdateNecessary(directory+dirString+"/",dirString);
					}
				}
				foreach (QString dirString, dirList) {
					if (dirString != "." && dirString != "..") {
						updateNecessary |= checkDirsAnalysingUpdateNecessary(directory+dirString+"/",baseDirectory);
					}
				}
				return updateNecessary;
			}


			bool CReasonerEvaluationAnalyserChecker::checkFilesAnalysingUpdateNecessary(const QString& directory, const QString& requestFileString) {
				bool updateNecessary = false;
				QDir dir(directory);
				QStringList fileList = dir.entryList(QDir::Files,QDir::Name|QDir::IgnoreCase);
				cint64 fileCount = fileList.count();
				cint64 analysedCount = mAnalysedResponseCountHash.value(directory,0);
				if (analysedCount < mMaxAnalyseCount) {
					if (fileCount > analysedCount) {
						analysedCount = qMin(mMaxAnalyseCount,fileCount);
						updateNecessary = true;
						mAnalysedResponseCountHash.insert(directory,analysedCount);
					}
				}
				return updateNecessary;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

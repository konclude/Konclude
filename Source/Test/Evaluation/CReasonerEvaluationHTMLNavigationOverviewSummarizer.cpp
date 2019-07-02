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

#include "CReasonerEvaluationHTMLNavigationOverviewSummarizer.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationHTMLNavigationOverviewSummarizer::CReasonerEvaluationHTMLNavigationOverviewSummarizer() {
				mDirLinConString = loadTemplateFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/DirectlyLinkedContent.html");
				mDirLinOveString = loadTemplateFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/DirectlyLinkedOverview.html");
				mDirLinSubString = loadTemplateFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/DirectlyLinkedSubsection.html");
				mDirLinSecString = loadTemplateFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/DirectlyLinkedSection.html");

				loadConditionedFiltersFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/filters-conditioned.txt",&mFilterConditionedHash);
				loadFiltersFromFile("Evaluation/Templates/Navigation/HTMLNavigationOverview/filters.txt",&mFilterSet);
			}


			CReasonerEvaluationHTMLNavigationOverviewSummarizer::~CReasonerEvaluationHTMLNavigationOverviewSummarizer() {
			}


			QString CReasonerEvaluationHTMLNavigationOverviewSummarizer::loadTemplateFromFile(const QString& fileString) {
				QString dataString;
				QFile file(fileString);
				if (file.open(QIODevice::ReadOnly)) {
					dataString = file.readAll();
				}
				return dataString;
			}


			bool CReasonerEvaluationHTMLNavigationOverviewSummarizer::loadFiltersFromFile(const QString& fileString, QSet<QString>* filterSet) {
				QFile file(fileString);
				if (filterSet && file.open(QIODevice::ReadOnly)) {
					while (!file.atEnd()) {
						QString filterLine = file.readLine();
						if (!filterLine.startsWith("#")) {
							filterLine = filterLine.trimmed();
							if (!filterLine.isEmpty()) {
								filterSet->insert(filterLine);
							}
						}
					}
					return true;
				}
				return false;
			}



			bool CReasonerEvaluationHTMLNavigationOverviewSummarizer::loadConditionedFiltersFromFile(const QString& fileString, QHash<QString,QStringList>* filterConditionedHash) {
				QFile file(fileString);
				if (filterConditionedHash && file.open(QIODevice::ReadOnly)) {
					while (!file.atEnd()) {
						QString filterLine = file.readLine();
						if (!filterLine.startsWith("#")) {
							filterLine = filterLine.trimmed();
							if (!filterLine.isEmpty()) {
								QStringList filterList = filterLine.split("\t");
								if (filterList.count() >= 2) {
									QString conditionString = filterList.takeFirst();
									filterConditionedHash->insertMulti(conditionString,filterList);
								}
							}
						}
					}
					return true;
				}
				return false;
			}


			bool CReasonerEvaluationHTMLNavigationOverviewSummarizer::isAcceptedByFilters(const QString& name) {
				bool accepted = true;
				if (!mFilterSet.isEmpty()) {
					bool oneAccepted = false;
					for (QSet<QString>::const_iterator it = mFilterSet.constBegin(), itEnd = mFilterSet.constEnd(); !oneAccepted && it != itEnd; ++it) {
						const QString& filterString(*it);
						if (name.contains(filterString)) {
							oneAccepted = true;
						}
					}
					accepted &= oneAccepted;
				}
				if (!mFilterConditionedHash.isEmpty()) {
					for (QHash<QString,QStringList>::const_iterator it = mFilterConditionedHash.constBegin(), itEnd = mFilterConditionedHash.constEnd(); accepted && it != itEnd; ++it) {
						const QString& conditionString(it.key());
						const QStringList& filterStringList(it.value());
						bool oneAccepted = false;
						bool noMatch = true;
						if (name.contains(conditionString)) {
							noMatch = false;
							foreach (const QString& filterString, filterStringList) {
								if (name.contains(filterString)) {
									oneAccepted = true;
								}
							}
						}
						if (!noMatch) {
							accepted &= oneAccepted;
						}
					}
				}
				return accepted;
			}

			QString CReasonerEvaluationHTMLNavigationOverviewSummarizer::createHTMLForDirectory(const QString& directoryString, const QString& relativePathString, const QString& groupFilterString, cint64* addContentCounter) {
				QString sectionString = mDirLinSecString;
				QDir directory(directoryString);
				QStringList subDirStringList = directory.entryList(QDir::Dirs);
				QStringList fileStringList = directory.entryList(QDir::Files);
				QString sectionContentsString;
				cint64 addedContentCount = 0;
				foreach (const QString& fileString, fileStringList) {
					if (fileString.endsWith(".html") && isAcceptedByFilters(relativePathString+fileString)) {
						QString secConString = mDirLinConString;
						QString titleString = fileString;
						titleString.replace(".html","");
						secConString.replace("$$_CONTENT_LINK_$$",relativePathString+fileString);
						secConString.replace("$$_CONTENT_TITLE_$$",titleString);
						sectionContentsString += secConString;
						++addedContentCount;
					}
				}
				sectionString.replace("$$_SECTION_CONTENT_$$",sectionContentsString);

				QString subsectionsString;
				foreach (const QString& subDirString, subDirStringList) {
					if (subDirString != "." && subDirString != "..") {

						bool groupFiltered = false;
						if (subDirString.contains("Grouped")) {
							groupFiltered = true;
							if (subDirString.contains(groupFilterString)) {
								groupFiltered = false;
							}
						}

						if (!groupFiltered) {
							cint64 recursiveAddedContentCount = 0;
							QString subSecSectionString = createHTMLForDirectory(directoryString+subDirString+"/",relativePathString+subDirString+"/",groupFilterString,&recursiveAddedContentCount);
							if (recursiveAddedContentCount > 0) {
								addedContentCount += recursiveAddedContentCount;
								QString subSecString = mDirLinSubString;
								QString subSecTitleString = subDirString;
								subSecString.replace("$$_SECTION_TITLE_$$",subSecTitleString);
								subSecString.replace("$$_SECTIONS_$$",subSecSectionString);
								subsectionsString += subSecString;
							}
						}
					}
				}
				if (addContentCounter) {
					*addContentCounter = addedContentCount;
				}
				sectionString.replace("$$_SUBSECTIONS_$$",subsectionsString);
				return sectionString;
			}



			CReasonerEvaluationHTMLNavigationOverviewSummarizer* CReasonerEvaluationHTMLNavigationOverviewSummarizer::createHTMLNavigationOverview(const QString& directory, const QString& evaluationProgramName, const QStringList& reasonerNameList, const QStringList& filterNameList, cint64 testTimeout, cint64 testCount) {


				QString outputDir = directory;
				if (!outputDir.endsWith("/")) {
					outputDir.append("/");
				}

				QStringList groupFilterList;
				groupFilterList.append("");
				groupFilterList.append("Directory");
				groupFilterList.append("Expressiveness");
				groupFilterList.append("Profile");


				foreach (const QString& groupString, groupFilterList) {
					QString outputFileString;
					if (!groupString.isEmpty()) {
						outputFileString = outputDir+QString("Only%1Grouped-DirectlyLinkedNavigationOverview.html").arg(groupString);
					} else {
						outputFileString = outputDir+QString("DirectlyLinkedNavigationOverview.html");
					}
					QFile outputNavFile(outputFileString);
					if (outputNavFile.open(QIODevice::WriteOnly)) {
						outputNavFile.close();
					}
				}


				foreach (const QString& groupString, groupFilterList) {
					QString outputFileString;
					if (!groupString.isEmpty()) {
						outputFileString = outputDir+QString("Only%1Grouped-DirectlyLinkedNavigationOverview.html").arg(groupString);
					} else {
						outputFileString = outputDir+QString("DirectlyLinkedNavigationOverview.html");
					}
					QFile outputNavFile(outputFileString);
					if (outputNavFile.open(QIODevice::WriteOnly)) {

						QString overviewNavString = mDirLinOveString;


						overviewNavString.replace("$$_TEST_COUNT_$$",QString::number(testCount));
						overviewNavString.replace("$$_TEST_TIMEOUT_$$",QString::number(testTimeout));
						overviewNavString.replace("$$_EVALUATION_PROGRAM_$$",evaluationProgramName);
						overviewNavString.replace("$$_TEST_REASONERS_$$",reasonerNameList.join(", "));
						overviewNavString.replace("$$_TEST_DATE_$$",QDateTime::currentDateTime().toString());
						overviewNavString.replace("$$_TEST_VERSION_$$",CKoncludeInfo::getKoncludeVersionString());
						overviewNavString.replace("$$_TEST_FILTERS_$$",filterNameList.join(", "));



						QString sectionString = createHTMLForDirectory(outputDir,"",groupString);
						overviewNavString.replace("$$_SECTIONS_$$",sectionString);

						outputNavFile.write(overviewNavString.toLocal8Bit());

						outputNavFile.close();
					}
				}


				return this;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

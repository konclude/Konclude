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

#include "CReasonerEvaluationGNUPlotPlotter.h"
#include "CReasonerEvaluationAnalyseContext.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGNUPlotPlotter::CReasonerEvaluationGNUPlotPlotter(CReasonerEvaluationAnalyseContext* context) {
				mContext = context;


				//mReasCompTemplateString = 
				//	"set terminal svg size 1280,800 dynamic enhanced fname 'arial'  fsize 10 mousing name \"$$_PLOT_NAME_$$\" butt solid\r\n"
				//	"set output '$$_PLOT_FILE_$$'\r\n"
				//	"set style fill solid 1.00 border lt -1\r\n"
				//	"set style histogram clustered gap 5 title  offset character 0, 0, 0\r\n"
				//	"set style data histograms\r\n"
				//	"set xtics border in scale 0,00 nomirror rotate by -45  offset character 0, 0, 0 autojustify\r\n"
				//	"set xtics norangelimit font \",8\"\r\n"
				//	"set title \"$$_PLOT_TITLE_$$\"\r\n"
				//	"set bmargin 12\r\n"
				//	"set grid ytics lt 0 lw 0.2 lc rgb \"#000000\"\r\n"
				//	"plot '$$_PLOT_DATA_$$' ";


				mReasCompTemplateString = 
					"set terminal svg size 1280,800\r\n"
					"set output '$$_PLOT_FILE_$$'\r\n"
					"set style fill solid 1.00 border lt -1\r\n"
					"set style histogram clustered gap 5 title  offset character 0, 0, 0\r\n"
					"set style data histograms\r\n"
					"set xtics border in scale 0,00 nomirror rotate by -45  offset character 0, 0, 0 autojustify\r\n"
					"set xtics norangelimit font \",8\"\r\n"
					"set title \"$$_PLOT_TITLE_$$\"\r\n"
					"set bmargin 10\r\n"
					"set grid ytics lt 0 lw 0.2 lc rgb \"#000000\"\r\n"
					"plot '$$_PLOT_DATA_$$' ";

				mReasOverTemplateString = 
					"set terminal svg size 1280,800\r\n"
					"set output '$$_PLOT_FILE_$$'\r\n"
					"set title \"$$_PLOT_TITLE_$$\"\r\n"
					"set log y\r\n"
					"set grid ytics lt 0 lw 0.2 lc rgb \"#000000\"\r\n"
					"plot '$$_PLOT_DATA_$$' ";


				mReasColorHash.insert("FaCT++","#E4FECA");
				mReasColorHash.insert("HermiT","#BBD2FF");
				mReasColorHash.insert("Pellet","#404040");
				mReasColorHash.insert("Konclude","#3784A0");
				mReasColorHash.insert("Average","#F7C298");
				mReasColorHash.insert("Min","#D5803D");
				mReasColorHash.insert("Max","#603326");
			}


			CReasonerEvaluationGNUPlotPlotter::~CReasonerEvaluationGNUPlotPlotter() {
			}

			CReasonerEvaluationGNUPlotPlotter* CReasonerEvaluationGNUPlotPlotter::createReasonerComparisonPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile) {
				QString outputScriptFileString(outputFile+".gpl");
				QString outputFileString(outputFile+".svg");
				QFile dataFile(dataFileString);
				QMap<QString,cint64> reasonerStringIndexMap;
				if (dataFile.open(QIODevice::ReadOnly)) {
					QString titleLineString = dataFile.readLine();
					cint64 nextIndex = 0;
					cint64 nextStartPos = 0;
					cint64 nextEndPos = 0;

					bool hasNextTitle = true;
					while (hasNextTitle) {
						nextEndPos = titleLineString.indexOf("\t",nextStartPos);
						if (nextEndPos <= -1) {
							nextEndPos = titleLineString.length();
							hasNextTitle = false;
						}

						QString nextTitle = titleLineString.mid(nextStartPos,nextEndPos-nextStartPos);
						nextTitle.replace("\"","");
						if (!nextTitle.isEmpty()) {
							reasonerStringIndexMap.insert(nextTitle,nextIndex++);
						}
						nextStartPos = nextEndPos+1;

					}
					dataFile.close();
				}


				QFile outputScriptFile(outputScriptFileString);
				if (outputScriptFile.open(QIODevice::WriteOnly)) {

					QString plotScriptString = mReasCompTemplateString;
					QString plotNameString = outputFile;
					cint64 lastSlashPos = plotNameString.lastIndexOf("/");
					if (lastSlashPos >= 0) {
						plotNameString = plotNameString.mid(lastSlashPos+1);
					}
					plotNameString.replace(" ","_");
					plotNameString.replace("-table","");
					plotNameString.replace("-","_");
					plotScriptString.replace("$$_PLOT_NAME_$$",plotNameString);
					plotScriptString.replace("$$_PLOT_FILE_$$",outputFileString);
					plotScriptString.replace("$$_PLOT_TITLE_$$",plotTitle);
					plotScriptString.replace("$$_PLOT_DATA_$$",dataFileString);


					bool validColorsForAllReasoners = true;
					QSet<QString> usedReasonerColorSet;
					foreach (QString reasonerString, reasonerNameList) {
						QString colorString = getColorString(reasonerString,&usedReasonerColorSet);
						if (colorString.isEmpty()) {
							validColorsForAllReasoners = false;
						}
					}
					QString colorString = getColorString("Average",&usedReasonerColorSet);
					if (colorString.isEmpty()) {
						validColorsForAllReasoners = false;
					}

					cint64 reasonerIndex = 0;
					foreach (QString reasonerString, reasonerNameList) {
						QString plotReasonerString;
						if (reasonerIndex++ == 0) {
							plotReasonerString = QString("using %1:xtic(%2) ti col ").arg(reasonerStringIndexMap.value(reasonerString)+1).arg(reasonerStringIndexMap.value("Requests")+1);
						} else {
							plotReasonerString = QString(", '' u %1 ti col ").arg(reasonerStringIndexMap.value(reasonerString)+1);
						}
						if (validColorsForAllReasoners) {
							plotReasonerString += QString("lc rgb \"%1\"").arg(getColorString(reasonerString));
						}
						plotScriptString += plotReasonerString;
					}

					if (reasonerStringIndexMap.contains("Index") && reasonerStringIndexMap.contains("Average")) {
						QString plotAverageString = QString(", '' using %1:%2 with lines title \"Average\" ").arg(reasonerStringIndexMap.value("Index")+1).arg(reasonerStringIndexMap.value("Average")+1);
						if (validColorsForAllReasoners) {
							plotAverageString += QString("lc rgb \"%1\" ").arg(getColorString("Average"));
						}
						plotAverageString += QString("lw 4\r\n");
						plotScriptString += plotAverageString;
					}

					outputScriptFile.write(plotScriptString.toLocal8Bit());

					outputScriptFile.close();

					plotScriptFile(outputScriptFileString);
				}

				return this;
			}


			CReasonerEvaluationGNUPlotPlotter* CReasonerEvaluationGNUPlotPlotter::createReasonerOverviewPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile) {


				QString outputScriptFileString(outputFile+".gpl");
				QString outputFileString(outputFile+".svg");
				QFile dataFile(dataFileString);
				QMap<QString,cint64> reasonerStringIndexMap;
				if (dataFile.open(QIODevice::ReadOnly)) {
					QString titleLineString = dataFile.readLine();
					cint64 nextIndex = 0;
					cint64 nextStartPos = 0;
					cint64 nextEndPos = 0;

					bool hasNextTitle = true;
					while (hasNextTitle) {
						nextEndPos = titleLineString.indexOf("\t",nextStartPos);
						if (nextEndPos <= -1) {
							nextEndPos = titleLineString.length();
							hasNextTitle = false;
						}

						QString nextTitle = titleLineString.mid(nextStartPos,nextEndPos-nextStartPos);
						nextTitle.replace("\"","");
						nextTitle = nextTitle.trimmed();
						if (!nextTitle.isEmpty()) {
							reasonerStringIndexMap.insert(nextTitle,nextIndex++);
						}
						nextStartPos = nextEndPos+1;

					}
					dataFile.close();
				}


				QFile outputScriptFile(outputScriptFileString);
				if (outputScriptFile.open(QIODevice::WriteOnly)) {

					QString plotScriptString = mReasOverTemplateString;
					QString plotNameString = outputFile;
					cint64 lastSlashPos = plotNameString.lastIndexOf("/");
					if (lastSlashPos >= 0) {
						plotNameString = plotNameString.mid(lastSlashPos+1);
					}
					plotNameString.replace(" ","_");
					plotNameString.replace("-table","");
					plotNameString.replace("-","_");
					plotScriptString.replace("$$_PLOT_NAME_$$",plotNameString);
					plotScriptString.replace("$$_PLOT_FILE_$$",outputFileString);
					plotScriptString.replace("$$_PLOT_TITLE_$$",plotTitle);
					plotScriptString.replace("$$_PLOT_DATA_$$",dataFileString);


					bool validColorsForAllReasoners = true;
					QSet<QString> usedReasonerColorSet;
					foreach (QString reasonerString, reasonerNameList) {
						QString colorString = getColorString(reasonerString,&usedReasonerColorSet);
						if (colorString.isEmpty()) {
							validColorsForAllReasoners = false;
						}
					}

					cint64 reasonerIndex = 0;
					foreach (QString reasonerString, reasonerNameList) {
						QString plotReasonerString;
						cint64 reasonerIndexValu = reasonerStringIndexMap.value(reasonerString);
						if (reasonerIndex++ == 0) {
							plotReasonerString = QString("using %1:%2 with lines ti col ").arg(reasonerStringIndexMap.value("Index")+1).arg(reasonerStringIndexMap.value(reasonerString)+1);
						} else {
							plotReasonerString = QString(", '' u %1 with lines ti col ").arg(reasonerStringIndexMap.value(reasonerString)+1);
						}
						if (validColorsForAllReasoners) {
							plotReasonerString += QString("lc rgb \"%1\"").arg(getColorString(reasonerString));
						}
						plotReasonerString += " lw 3";
						plotScriptString += plotReasonerString;
					}

					outputScriptFile.write(plotScriptString.toLocal8Bit());

					outputScriptFile.close();

					plotScriptFile(outputScriptFileString);
				}

				return this;
			}



			CReasonerEvaluationGNUPlotPlotter* CReasonerEvaluationGNUPlotPlotter::plotScriptFile(const QString& scriptFileString) {
				QProcess gnuPlotProcess;
				gnuPlotProcess.start(QString("gnuplot < \"%1\"").arg(scriptFileString));
				gnuPlotProcess.waitForFinished(300000);
				gnuPlotProcess.terminate();
				gnuPlotProcess.kill();				
				return this;
			}

			QString CReasonerEvaluationGNUPlotPlotter::getColorString(const QString& reasonerString, QSet<QString>* usedReasonerColorSet) {
				QString reasonerColorString(reasonerString);
				QString colorString;
				if (mReasColorHash.contains(reasonerColorString)) {
					colorString = mReasColorHash.value(reasonerColorString);
				} else {
					cint64 versionSplitSpacePos = reasonerColorString.indexOf(" ");
					if (versionSplitSpacePos >= 0) {
						reasonerColorString = reasonerColorString.mid(0,versionSplitSpacePos);
						if (mReasColorHash.contains(reasonerColorString)) {
							colorString = mReasColorHash.value(reasonerColorString);
						}
					}
				}
				if (usedReasonerColorSet && !colorString.isEmpty()) {
					if (usedReasonerColorSet->contains(reasonerColorString)) {
						colorString.clear();
					} else {
						usedReasonerColorSet->insert(reasonerColorString);
					}
				}
				return colorString;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

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

#include "CReasonerEvaluationLoadingTimesComparerLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CReasonerEvaluationLoadingTimesComparerLoader::CReasonerEvaluationLoadingTimesComparerLoader() {
				loaderConfig = nullptr;
			}



			CReasonerEvaluationLoadingTimesComparerLoader::~CReasonerEvaluationLoadingTimesComparerLoader() {
			}


			CLoader *CReasonerEvaluationLoadingTimesComparerLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				CConfigData *data = config->getLastConfigChange("Konclude.Test.Generator.InputDirectory");
				if (data) {
					mInputDir = data->getString();
				}
				if (!mInputDir.endsWith("/")) {
					mInputDir += QString("/");
				}

				data = config->getLastConfigChange("Konclude.Test.Generator.OutputDirectory");
				if (data) {
					mOutputDir = data->getString();
				}
				if (!mOutputDir.endsWith("/")) {
					mOutputDir += QString("/");
				}
				mOutputDirClassify = mOutputDir+"Classification/";

				return this;
			}



			CLoader *CReasonerEvaluationLoadingTimesComparerLoader::load() {

				CReasonerEvaluationLoadingTimeExtractor* loadTimeExtractor = new CReasonerEvaluationLoadingTimeExtractor(60000);
				CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();

				QStringList reasonerNameStringList = QStringList()
						<<"HermiT 1.3.6"
						<<"FaCT++ 1.5.3"
						<<"Pellet 2.3.0"
						<<"Konclude 0.3.7-620";
				QStringList reasonerPathStringList = QStringList()
						<<CReasonerEvaluationPathConverter::getCorrectedPathString("Evaluation/Responses/R-000/HermiT/$$_LATEST_STRING_$$/Config001/")
						<<CReasonerEvaluationPathConverter::getCorrectedPathString("Evaluation/Responses/R-000/FaCT++/$$_LATEST_STRING_$$/Config001/")
						<<CReasonerEvaluationPathConverter::getCorrectedPathString("Evaluation/Responses/R-000/Pellet/$$_LATEST_STRING_$$/Config001/")
						<<CReasonerEvaluationPathConverter::getCorrectedPathString("Evaluation/Responses/R-000/Konclude/$$_LATEST_STRING_$$/Config001/");

				QString outputDirectory = CReasonerEvaluationPathConverter::getCorrectedPathString("Evaluation/Analyses/R-000/A-$$_LATEST+1_$$/ReasonerLoadingComparison/");
				QDir currDir(".");
				currDir.mkpath(outputDirectory);


				CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext();
				//reasonerEvaluationContext->getGNUPlotPlotter()->createReasonerOverviewPlot("Evaluation/Analyse/R-000/A-003/ReasonerClassificationComparison/TONES Repository/Incompatible Axioms Removed/Classification/classification-time-separate-sorted-descending-table.dat","xyz",QStringList()<<"Min"<<"Max","Evaluation/Analyse/R-000/A-003/ReasonerComparison/TONES Repository/Incompatible Axioms Removed/Classification/xyz");
				//reasonerEvaluationContext->getHighchartPlotter()->createReasonerComparisonPlot("Evaluation/Analyses/R-000/A-004/ReasonerClassificationComparison/TONES Repository/Incompatible Axioms Removed/Classification/classification-time-sorted-descending-by-Average-split-from-0-to-16-table.csv","xyz",QStringList()<<"Min"<<"Max","Evaluation/Analyses/R-000/A-004/ReasonerClassificationComparison/TONES Repository/Incompatible Axioms Removed/Classification/xyz");


				CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(loadTimeExtractor,avarageSummarize);

				CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(reasonerNameStringList);
				
				for (QStringList::const_iterator it1 = reasonerNameStringList.constBegin(), it2 = reasonerPathStringList.constBegin(), it1End = reasonerNameStringList.constEnd(), it2End = reasonerPathStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
					QString reasonerName(*it1);
					QString reasonerPath(*it2);
					LOG(INFO,"::Konclude::Test::Evaluation::ReasonerLoadingTimeComparer",logTr("Extracting loading times for '%1' reasoner from responses in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
					collector->collectReasonerEvaluationDataValues(reasonerComp,reasonerName,reasonerPath);
				}


				LOG(INFO,"::Konclude::Test::Evaluation::ReasonerLoadingTimeComparer",logTr("Comparing loading times for '%1'.").arg(reasonerNameStringList.join(", ")),this);

				CReasonerEvaluationGroupRequestSelector* selectors = nullptr;
				CReasonerEvaluationExpressivitySelectorFactory expEvalSelFactory("Expressivity Grouped","Expressivity Grouped/","Evaluation/Infos/Expressivity/");
				selectors = expEvalSelFactory.getSelectors(selectors);
				CReasonerEvaluationDirectoryStructureSelectorFactory dirEvalSelFactory("Evaluation/Requests/","Evaluation/Requests/","Directory Grouped","Directory Grouped/");
				selectors = dirEvalSelFactory.getSelectors(selectors);


				CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser();
				analyser1->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

				CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser* analyser2 = new CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser();
				analyser2->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

				CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser* analyser3 = new CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser();
				analyser3->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

				LOG(INFO,"::Konclude::Test::Evaluation::ReasonerLoadingTimeComparer",logTr("Loading times for '%1' compared to '%2'.").arg(reasonerNameStringList.join(", ")).arg(outputDirectory),this);


				return this;
			}




			CLoader *CReasonerEvaluationLoadingTimesComparerLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

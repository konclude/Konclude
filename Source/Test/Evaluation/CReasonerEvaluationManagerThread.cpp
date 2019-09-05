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

#include "CReasonerEvaluationManagerThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationManagerThread::CReasonerEvaluationManagerThread(CConfiguration *config) : CThread("ReasonerEvaluationManagerThread"), CLogIdentifier("::Konclude::Test::Evaluation::ReasonerEvaluationManagerThread",this) {
				mConfig = config;
				mSatisticBasePath = "Evaluation/Infos/Statistics/Requests/";
				startThread();
			}


			CReasonerEvaluationManagerThread::~CReasonerEvaluationManagerThread() {
			}


			CReasonerEvaluationManager* CReasonerEvaluationManagerThread::realizeEvaluationCommand(CEvaluateTestsuiteCommand* evalTestsuiteComm) {
				postEvent(new CReasonerEvaluationProgramEvent(evalTestsuiteComm));
				return this;
			}


			bool CReasonerEvaluationManagerThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else {
					if (type == CReasonerEvaluationProgramEvent::EVENTTYPE) {
						CReasonerEvaluationProgramEvent* repe = (CReasonerEvaluationProgramEvent*)event;
						CEvaluateTestsuiteCommand* evalTestComm = repe->getEvaluationTestsuiteCommand();

						CCommandRecordRouter commandRecordRouter(evalTestComm,this);
						CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

						initEvaluationProgram(evalTestComm);
						evaluationProgram(evalTestComm);
						analysingProgram(evalTestComm);

						CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
						CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

						return true;
					}
				}
				return false;
			}


			CReasonerEvaluationManager* CReasonerEvaluationManagerThread::initEvaluationProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm) {
				CReasonerEvaluationProgram* evalProg = evalTestsuiteComm->getEvaluationTestsuite();
				mReasonerNameStringList.clear();
				mReasonerOutputDirStringList.clear();
				mTestTimeout = 0;
				mTestCount = 0;
				mOneReasonerFailed = false;
				mAllReasonerFailed = true;
				mFiltering = new CReasonerEvaluationFiltering(mSatisticBasePath);
				if (!evalProg->getFilterStringList().isEmpty()) {
					mFitlerStringList = evalProg->getFilterStringList();
					mFiltering->loadFiltersFromFile(mFitlerStringList);
				} else {
					mFiltering->loadEmptyFilter();
				}				
				return this;
			}



			bool CReasonerEvaluationManagerThread::evaluationProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm) {
				CReasonerEvaluationProgram* evalProg = evalTestsuiteComm->getEvaluationTestsuite();

				QStringList reasonerConfigList = evalProg->getReasonerStringList();

				QString testInputDirectory = evalProg->getInputDirectoryString();
				QString testOutputDirectory = evalProg->getOutputDirectoryString();

				QString plattform = CConfigDataReader::readConfigString(mConfig,"Konclude.Evaluation.Platform");

				QString globalConfig = evalProg->getConfigFileString();

				bool evaluationRunned = false;





				if (plattform != "UNSPECIFIED") {


					QTime totalTimer;
					totalTimer.start();
					LOG(INFO,getLogDomain(),logTr("Starting evaluation."),this);

					foreach (QString reasonerConfig, reasonerConfigList) {

						QTime timer;
						timer.start();

						LOG(INFO,getLogDomain(),logTr("Starting evaluation for reasoner '%1'.").arg(reasonerConfig),this);


						CReasonerEvaluationTestsuite* reasonerEvalTestsuite = new CReasonerEvaluationTestsuite();
						reasonerEvalTestsuite->initEvaluationTestsuite(globalConfig,reasonerConfig,testInputDirectory,testOutputDirectory,plattform,mFiltering);

						CBlockingCallbackData blockCallData;

						CReasonerEvaluationExecutorThread* reasonerExecutorThread = new CReasonerEvaluationExecutorThread(mConfig);
						reasonerExecutorThread->executeEvaluation(reasonerEvalTestsuite,&blockCallData);
						blockCallData.waitForCallback();
						CReasonerEvaluationEvaluatedCallbackContextData* evaluatedCallbackContextData = (CReasonerEvaluationEvaluatedCallbackContextData*)blockCallData.getCallbackDataContext();
						if (!evaluatedCallbackContextData->getEvaluationFailed()) {
							evaluationRunned |= evaluatedCallbackContextData->getEvaluationRunned();
							mReasonerNameStringList.append(evaluatedCallbackContextData->getReasonerNameString());
							mReasonerOutputDirStringList.append(evaluatedCallbackContextData->getReasonerOutputDirectoryString());
							mTestTimeout = evaluatedCallbackContextData->getTestTimeout();
							mTestCount = evaluatedCallbackContextData->getTestCount();
							mAllReasonerFailed = false;
						} else {
							mOneReasonerFailed = true;
							mFailedReasonerStringList.append(reasonerConfig);
						}
						delete reasonerExecutorThread;

						cint64 elapsedTime = timer.elapsed();
						LOG(INFO,getLogDomain(),logTr("Finished evaluation for reasoner '%1' in '%2' seconds.").arg(reasonerConfig).arg(elapsedTime/1000),this);

					}

					cint64 totalElapsedTime = totalTimer.elapsed();
					LOG(INFO,getLogDomain(),logTr("Finished evaluation in '%1' seconds.").arg(totalElapsedTime/1000),this);
				} else {
					LOG(ERROR,getLogDomain(),logTr("Evaluation not possible, platform not specified."),this);
				}

				return evaluationRunned;
			}




			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationManagerThread::getSelectors(const QString& requestDirectory) {
				CReasonerEvaluationGroupRequestSelector* selectors = nullptr;
				CReasonerEvaluationExpressivitySelectorFactory expEvalSelFactory("ExpressivenessGrouped","ExpressivenessGrouped/",mSatisticBasePath);
				selectors = expEvalSelFactory.getSelectors(selectors);
				CReasonerEvaluationDirectoryStructureSelectorFactory dirEvalSelFactory(requestDirectory,requestDirectory,"DirectoryGrouped","DirectoryGrouped/");
				selectors = dirEvalSelFactory.getSelectors(selectors);
				CReasonerEvaluationProfileSelectorFactory profileEvalSelFactory("ProfileGrouped","ProfileGrouped/",mSatisticBasePath);
				selectors = profileEvalSelFactory.getSelectors(selectors);
				return selectors;
			}



			QString CReasonerEvaluationManagerThread::getAnalyserOutputDirectory(const QString& baseDirectory, const QString& evaluationProgramName, const QString& platformName, const QString& analyserName) {
				QString outputDirectory = baseDirectory;
				if (outputDirectory.contains("$$_ANALYSER_NAME_$$")) {
					outputDirectory = outputDirectory.replace("$$_ANALYSER_NAME_$$",analyserName);
					outputDirectory = outputDirectory.replace("$$_EVALUATION_PROGRAM_NAME_$$",evaluationProgramName);
					outputDirectory = outputDirectory.replace("$$_PLATFORM_$$",platformName);
				} else {
					outputDirectory = outputDirectory+evaluationProgramName+"/"+platformName+"/"+analyserName+"/";
				}
				QDir currDir(".");
				currDir.mkpath(outputDirectory);
				return outputDirectory;
			}




			bool CReasonerEvaluationManagerThread::analyseTimes(const QString& analysingDirectoryString, const QString& evaluationProgramName, const QString& plattform, const QString& analyserOutputDirectoryName, const QString& requestDirectory, cint64 testCountCut, cint64 timeoutCut, cint64 errorPunishmentTime, CReasonerEvaluationDataValueCacher* dataValueCacher, CReasonerEvaluationExtractor::EXTRACTORTYPE extractorType, const QString& extractionName, const QString& contextSpecificationName) {
				bool analysed = false;
				QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,analyserOutputDirectoryName);

				CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
				if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {

					LOG(INFO,getLogDomain(),logTr("Analysing %1 times for '%2' into directory '%3'.").arg(extractionName).arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);

					CReasonerEvaluationSpecifiedTimeExtractor* timeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(extractorType,timeoutCut,errorPunishmentTime,dataValueCacher);
					CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();

					CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,contextSpecificationName);

					CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(timeExtractor,avarageSummarize,mFiltering);

					CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

					for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
						QString reasonerName(*it1);
						QString reasonerPath(*it2);
						LOG(INFO,getLogDomain(),logTr("Extracting %1 times for reasoner '%2' from responses in directory '%3'.").arg(extractionName).arg(reasonerName).arg(reasonerPath),this);
						collector->collectReasonerEvaluationDataValues(reasonerComp,reasonerName,reasonerPath);
					}


					LOG(INFO,getLogDomain(),logTr("Comparing %1 times for '%2'.").arg(extractionName).arg(mReasonerNameStringList.join(", ")),this);


					CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


					CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser();
					analyser1->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

					CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser* analyser2 = new CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser();
					analyser2->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

					CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser* analyser3 = new CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser();
					analyser3->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

					CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser* analyser4 = new CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser(100.,timeoutCut,qSqrt(10.),timeoutCut);
					analyser4->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);
					CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser* analyser5 = new CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser(100.,timeoutCut,10.,timeoutCut);
					analyser5->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

					LOG(INFO,getLogDomain(),logTr("Compared %1 times for '%2' into directory '%3'.").arg(extractionName).arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);

					analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");
					analysed = true;
				} else {
					LOG(WARNING,getLogDomain(),logTr("Compared %1 times for '%2' in directory '%3' are already up to date.").arg(extractionName).arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
				}
				return analysed;
			}



			CReasonerEvaluationManager* CReasonerEvaluationManagerThread::analysingProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm) {
				CReasonerEvaluationProgram* evalProg = evalTestsuiteComm->getEvaluationTestsuite();

				bool analysed = false;

				QString plattform = CConfigDataReader::readConfigString(mConfig,"Konclude.Evaluation.Platform");

				QString analysingDirectoryString = CReasonerEvaluationPathConverter::getCorrectedPathString(evalProg->getAnalyserDirectoryString());
				if (!analysingDirectoryString.endsWith("/")) {
					analysingDirectoryString += "/";
				}

				QString requestDirectory = evalProg->getInputDirectoryString();
				if (!requestDirectory.endsWith("/")) {
					requestDirectory += "/";
				}

				QString evaluationProgramName = evalProg->getProgramName();
				cint64 testCountCut = mTestCount;
				cint64 timeoutCut = mTestTimeout;
				cint64 errorPunishmentTime = 1;


				CReasonerEvaluationDataValueCacher* dataValueCacher = new CReasonerEvaluationDataValueCacher();

				if (!mAllReasonerFailed) {

					QString dataString = QDateTime::currentDateTime().date().toString(Qt::ISODate);

					if (plattform != "UNSPECIFIED") {

						QTime totalTimer;
						totalTimer.start();
						LOG(INFO,getLogDomain(),logTr("Starting analysing."),this);


						QStringList analysingStringList = evalProg->getAnalyserStringList();

						foreach (QString ananysingString, analysingStringList) {


							QTime timer;
							timer.start();

							LOG(INFO,getLogDomain(),logTr("Starting analysing for '%1'.").arg(ananysingString),this);


							if (ananysingString == "ReasonerEvaluationClassificationTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"ClassificationTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::CLASSIFICATIONTIMEEXTRACTOR,"classification","Classification ");
							}

							if (ananysingString == "ReasonerEvaluationComplexQueryingTimesComparer") {
								analyseTimes(analysingDirectoryString, evaluationProgramName, plattform, "ComplexQueryingTimeComparison", requestDirectory, testCountCut, timeoutCut, errorPunishmentTime, dataValueCacher, CReasonerEvaluationExtractor::COMPLEXQUERYINGTIMEEXTRACTOR, "complex-querying", "Complex Querying ");
							}

							if (ananysingString == "ReasonerEvaluationRealizationTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"RealizationTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::REALIZATIONTIMEEXTRACTOR,"realization","Realization ");
							}

							if (ananysingString == "ReasonerEvaluationConsistencyTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"ConsistencyTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::CONSISTENCYTIMEEXTRACTOR,"consistency","Consistency ");					
							}

							if (ananysingString == "ReasonerEvaluationSatisfiabilityTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"SatisfiabilityTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::SATISFIABILITYTIMEEXTRACTOR,"satisfiability","Satisfiability ");					
							}

							if (ananysingString == "ReasonerEvaluationLoadingTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"LoadingTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::LOADINGTIMEEXTRACTOR,"loading","Loading ");
							}

							if (ananysingString == "ReasonerEvaluationResponseTimesComparer") {
								analyseTimes(analysingDirectoryString,evaluationProgramName,plattform,"ResponseTimeComparison",requestDirectory,testCountCut,timeoutCut,errorPunishmentTime,dataValueCacher,CReasonerEvaluationExtractor::TOTALRESPONSETIMEEXTRACTOR,"response","Response ");					
							}


							if (ananysingString == "ReasonerEvaluationAccumulatedTimesComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,"AccumulatedTimeComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {

									LOG(INFO,getLogDomain(),logTr("Analysing accumulated times for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);


									CReasonerEvaluationSpecifiedTimeExtractor* classTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::CLASSIFICATIONTIMEEXTRACTOR, timeoutCut, errorPunishmentTime, dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* complexQueryingTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::COMPLEXQUERYINGTIMEEXTRACTOR, timeoutCut, errorPunishmentTime, dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* consTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::CONSISTENCYTIMEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* satTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::SATISFIABILITYTIMEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* realTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::REALIZATIONTIMEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* loadTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::LOADINGTIMEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* reponseTimeExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::TOTALRESPONSETIMEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* errorExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::ERROREXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);
									CReasonerEvaluationSpecifiedTimeExtractor* timeoutExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::TIMEOUTEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);

									CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();


									CReasonerEvaluationCollector* responseCollector = new CReasonerEvaluationCollector(reponseTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* responceReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* loadingCollector = new CReasonerEvaluationCollector(loadTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* loadingReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* classificationCollector = new CReasonerEvaluationCollector(classTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* classificationReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									CReasonerEvaluationCollector* complexQueryingCollector = new CReasonerEvaluationCollector(complexQueryingTimeExtractor, avarageSummarize, mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* complexQueryingReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									CReasonerEvaluationCollector* realizationCollector = new CReasonerEvaluationCollector(realTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* realizationReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* consistencyCollector = new CReasonerEvaluationCollector(consTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* consistencyReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* satisfiabilityCollector = new CReasonerEvaluationCollector(satTimeExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* satisfiabilityReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* errorCollector = new CReasonerEvaluationCollector(errorExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* errorReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);
									CReasonerEvaluationCollector* timeoutCollector = new CReasonerEvaluationCollector(timeoutExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* timeoutReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);


									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO,getLogDomain(),logTr("Extracting accumulated times for '%1' reasoner from responses in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
										responseCollector->collectReasonerEvaluationDataValues(responceReasonerComp,reasonerName,reasonerPath);
										loadingCollector->collectReasonerEvaluationDataValues(loadingReasonerComp,reasonerName,reasonerPath);
										classificationCollector->collectReasonerEvaluationDataValues(classificationReasonerComp, reasonerName, reasonerPath);
										complexQueryingCollector->collectReasonerEvaluationDataValues(complexQueryingReasonerComp, reasonerName, reasonerPath);
										realizationCollector->collectReasonerEvaluationDataValues(realizationReasonerComp,reasonerName,reasonerPath);
										consistencyCollector->collectReasonerEvaluationDataValues(consistencyReasonerComp,reasonerName,reasonerPath);
										satisfiabilityCollector->collectReasonerEvaluationDataValues(satisfiabilityReasonerComp,reasonerName,reasonerPath);
										errorCollector->collectReasonerEvaluationDataValues(errorReasonerComp,reasonerName,reasonerPath);
										timeoutCollector->collectReasonerEvaluationDataValues(timeoutReasonerComp,reasonerName,reasonerPath);
									}


									LOG(INFO,getLogDomain(),logTr("Comparing accumulated times for '%1'.").arg(mReasonerNameStringList.join(", ")),this);

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									CReasonerEvaluationAnalyseContext* responseReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Response ","response-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* responseAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									responseAnalyser1->analyseEvaluationData(responceReasonerComp,responseReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* loadingReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Loading ","loading-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* loadingAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									loadingAnalyser1->analyseEvaluationData(loadingReasonerComp,loadingReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* classificationReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Classification ","classification-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* classificationAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									classificationAnalyser1->analyseEvaluationData(classificationReasonerComp,classificationReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* complexQueryingReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig, "Complex Querying ", "complex-querying-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* complexQueryingAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									complexQueryingAnalyser1->analyseEvaluationData(complexQueryingReasonerComp, complexQueryingReasonerEvaluationContext, outputDirectory, selectors);

									CReasonerEvaluationAnalyseContext* realizationReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Realization ","realization-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* realizationAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									realizationAnalyser1->analyseEvaluationData(realizationReasonerComp,realizationReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* consistencyReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Consistency ","consistency-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* consistencyAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									consistencyAnalyser1->analyseEvaluationData(consistencyReasonerComp,consistencyReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* satisfiabilityReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Satisfiability ","satisfiability-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser* satisfiabilityAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser();
									satisfiabilityAnalyser1->analyseEvaluationData(satisfiabilityReasonerComp,satisfiabilityReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* errorReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Error ","error-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser* errorAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser();
									errorAnalyser1->analyseEvaluationData(errorReasonerComp,errorReasonerEvaluationContext,outputDirectory,selectors);

									CReasonerEvaluationAnalyseContext* timeoutReasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,"Timeout ","timeout-");
									CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser* timeoutAnalyser1 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser();
									timeoutAnalyser1->analyseEvaluationData(timeoutReasonerComp,timeoutReasonerEvaluationContext,outputDirectory,selectors);

									LOG(INFO,getLogDomain(),logTr("Accumulated times for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING,getLogDomain(),logTr("Accumulated times for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
								}						
							}








							if (ananysingString == "ReasonerEvaluationStatisticValuesComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,"StatisticValuesComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {



									LOG(INFO,getLogDomain(),logTr("Collecting statistic names for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);


									QSet<QString> staticticNamesSet;
									CReasonerEvaluationStatisticsNameCollectingExtractor* statNameCollectingExtractor = new CReasonerEvaluationStatisticsNameCollectingExtractor(&staticticNamesSet,dataValueCacher);

									CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();

									CReasonerEvaluationCollector* statNameCollector = new CReasonerEvaluationCollector(statNameCollectingExtractor,avarageSummarize,mFiltering);
									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* statNameReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);


									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO,getLogDomain(),logTr("Extracting statistic names for '%1' reasoner from responses in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
										statNameCollector->collectReasonerEvaluationDataValues(statNameReasonerComp,reasonerName,reasonerPath);
									}


									LOG(INFO,getLogDomain(),logTr("Analysing statistic values for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);


									CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser* analyser4 = new CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser();
									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									for (QSet<QString>::const_iterator it = staticticNamesSet.constBegin(), itEnd = staticticNamesSet.constEnd(); it != itEnd; ++it) {
										QString statName(*it);

										analyser4->addNextStatisticName(statName);

										CReasonerEvaluationStatisticsValueExtractor* statValueExtractor = new CReasonerEvaluationStatisticsValueExtractor(statName,&staticticNamesSet,timeoutCut,errorPunishmentTime,dataValueCacher);

										CReasonerEvaluationCollector* statValueCollector = new CReasonerEvaluationCollector(statValueExtractor,avarageSummarize,mFiltering);
										CReasonerEvaluationDataValueGroupCollectionReasonerComparison* statValueReasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);


										for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
											QString reasonerName(*it1);
											QString reasonerPath(*it2);
											LOG(INFO,getLogDomain(),logTr("Extracting statistic values for '%1' reasoner from responses in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
											statValueCollector->collectReasonerEvaluationDataValues(statValueReasonerComp,reasonerName,reasonerPath);
										}




										LOG(INFO,getLogDomain(),logTr("Comparing statistic values for '%1'.").arg(mReasonerNameStringList.join(", ")),this);


										CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig,QString("%1 Statistics").arg(statName.toUpper()),statName+"-");


										CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser();
										analyser1->analyseEvaluationData(statValueReasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

										CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser* analyser2 = new CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser();
										analyser2->analyseEvaluationData(statValueReasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

										CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser* analyser3 = new CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser();
										analyser3->analyseEvaluationData(statValueReasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

										analyser4->analyseEvaluationData(statValueReasonerComp,reasonerEvaluationContext,outputDirectory,selectors);

										LOG(INFO,getLogDomain(),logTr("Statistics values for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
										analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");


										analysed = true;
									}



								} else {
									LOG(WARNING,getLogDomain(),logTr("Statistic values for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
								}						
							}









							if (ananysingString == "ReasonerEvaluationResultDifferencesComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,"ResultDifferenceComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {

									LOG(INFO,getLogDomain(),logTr("Analysing result differences for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);

									CReasonerEvaluationSpecifiedTimeExtractor* respFileExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::RESPONSEFILEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);

									CReasonerEvaluationResponseFileListSummarizer* respFileListSummarizer = new CReasonerEvaluationResponseFileListSummarizer();

									CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig);

									CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(respFileExtractor,respFileListSummarizer,mFiltering);

									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO,getLogDomain(),logTr("Collecting response files for '%1' reasoner in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
										collector->collectReasonerEvaluationDataValues(reasonerComp,reasonerName,reasonerPath);
									}


									LOG(INFO,getLogDomain(),logTr("Comparing result differences for '%1'.").arg(mReasonerNameStringList.join(", ")),this);

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser();
									analyser1->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);


									LOG(INFO,getLogDomain(),logTr("Result differences for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING,getLogDomain(),logTr("Result differences for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
								}
							}









							if (ananysingString == "ReasonerEvaluationResultSubsumptionComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString, evaluationProgramName, plattform, "ResultSubsumptionComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList, outputDirectory + "AnalysingUpdateCheckingData.dat", testCountCut)) {

									LOG(INFO, getLogDomain(), logTr("Analysing result subsumptions for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);

									CReasonerEvaluationSpecifiedTimeExtractor* respFileExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::RESPONSEFILEEXTRACTOR, timeoutCut, errorPunishmentTime, dataValueCacher);

									CReasonerEvaluationResponseFileListSummarizer* respFileListSummarizer = new CReasonerEvaluationResponseFileListSummarizer();

									CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig);

									CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(respFileExtractor, respFileListSummarizer, mFiltering);

									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO, getLogDomain(), logTr("Collecting response files for '%1' reasoner in directory '%2'.").arg(reasonerName).arg(reasonerPath), this);
										collector->collectReasonerEvaluationDataValues(reasonerComp, reasonerName, reasonerPath);
									}


									LOG(INFO, getLogDomain(), logTr("Comparing result subsumptions for '%1'.").arg(mReasonerNameStringList.join(", ")), this);

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser();
									analyser1->analyseEvaluationData(reasonerComp, reasonerEvaluationContext, outputDirectory, selectors);


									LOG(INFO, getLogDomain(), logTr("Result subsumptions for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory + "AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING, getLogDomain(), logTr("Result subsumptions for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);
								}
							}







							if (ananysingString == "ReasonerEvaluationErrorTimeoutComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,"ErrorTimeoutComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {

									LOG(INFO,getLogDomain(),logTr("Analysing errors and timeouts for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);

									CReasonerEvaluationSpecifiedTimeExtractor* errorExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::TIMEOUTERROREXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);

									CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();

									CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig);

									CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(errorExtractor,avarageSummarize,mFiltering);

									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO,getLogDomain(),logTr("Collecting errors/timeouts for '%1' reasoner in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
										collector->collectReasonerEvaluationDataValues(reasonerComp,reasonerName,reasonerPath);
									}


									LOG(INFO,getLogDomain(),logTr("Comparing errors/timeouts for '%1'.").arg(mReasonerNameStringList.join(", ")),this);

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									CReasonerEvaluationGroupRequestReasonerErrorAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerErrorAnalyser();
									analyser1->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);


									LOG(INFO,getLogDomain(),logTr("Errors/timeouts for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING,getLogDomain(),logTr("Errors/timeouts for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory),this);
								}
							}



							if (ananysingString == "ReasonerEvaluationErrorComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString, evaluationProgramName, plattform, "ErrorComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList, outputDirectory + "AnalysingUpdateCheckingData.dat", testCountCut)) {

									LOG(INFO, getLogDomain(), logTr("Analysing errors for '%1' to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);

									CReasonerEvaluationSpecifiedTimeExtractor* errorExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::ERROREXTRACTOR, timeoutCut, errorPunishmentTime, dataValueCacher);

									CReasonerEvaluationAvaragerSummarizer* avarageSummarize = new CReasonerEvaluationAvaragerSummarizer();

									CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig);

									CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(errorExtractor, avarageSummarize, mFiltering);

									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO, getLogDomain(), logTr("Collecting errors for '%1' reasoner in directory '%2'.").arg(reasonerName).arg(reasonerPath), this);
										collector->collectReasonerEvaluationDataValues(reasonerComp, reasonerName, reasonerPath);
									}


									LOG(INFO, getLogDomain(), logTr("Comparing errors for '%1'.").arg(mReasonerNameStringList.join(", ")), this);

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);


									CReasonerEvaluationGroupRequestReasonerErrorAnalyser* analyser1 = new CReasonerEvaluationGroupRequestReasonerErrorAnalyser();
									analyser1->analyseEvaluationData(reasonerComp, reasonerEvaluationContext, outputDirectory, selectors);


									LOG(INFO, getLogDomain(), logTr("Errors for '%1' compared to '%2'.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory + "AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING, getLogDomain(), logTr("Errors for '%1' in '%2' already up to date.").arg(mReasonerNameStringList.join(", ")).arg(outputDirectory), this);
								}
							}




							if (ananysingString == "RequestStatisticsComparer") {

								QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,"RequestStatisticsComparison");

								CReasonerEvaluationAnalyserChecker analysingUpdateChecker;
								if (analysingUpdateChecker.checkAnalysingUpdateNecessary(mReasonerOutputDirStringList,outputDirectory+"AnalysingUpdateCheckingData.dat",testCountCut)) {

									LOG(INFO,getLogDomain(),logTr("Analysing request files of '%1'.").arg(outputDirectory),this);

									CReasonerEvaluationSpecifiedTimeExtractor* requestFileExtractor = new CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::REQUESTFILEEXTRACTOR,timeoutCut,errorPunishmentTime,dataValueCacher);

									CReasonerEvaluationResponseFileListSummarizer* respFileListSummarizer = new CReasonerEvaluationResponseFileListSummarizer();

									CReasonerEvaluationAnalyseContext* reasonerEvaluationContext = new CReasonerEvaluationAnalyseContext(mConfig);

									CReasonerEvaluationCollector* collector = new CReasonerEvaluationCollector(requestFileExtractor,respFileListSummarizer,mFiltering);

									CReasonerEvaluationDataValueGroupCollectionReasonerComparison* reasonerComp = new CReasonerEvaluationDataValueGroupCollectionReasonerComparison(mReasonerNameStringList);

									for (QStringList::const_iterator it1 = mReasonerNameStringList.constBegin(), it2 = mReasonerOutputDirStringList.constBegin(), it1End = mReasonerNameStringList.constEnd(), it2End = mReasonerOutputDirStringList.constEnd(); it1 != it1End && it2 != it2End; ++it1, ++it2) {
										QString reasonerName(*it1);
										QString reasonerPath(*it2);
										LOG(INFO,getLogDomain(),logTr("Collecting request files for '%1' reasoner in directory '%2'.").arg(reasonerName).arg(reasonerPath),this);
										collector->collectReasonerEvaluationDataValues(reasonerComp,reasonerName,reasonerPath);
									}

									CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(requestDirectory);

									CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser* analyser1 = new CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser(mFiltering);
									analyser1->analyseEvaluationData(reasonerComp,reasonerEvaluationContext,outputDirectory,selectors);


									LOG(INFO,getLogDomain(),logTr("Request statistics evaluated to '%1'.").arg(outputDirectory),this);
									analysingUpdateChecker.saveAnalysingUpdateCheckFile(outputDirectory+"AnalysingUpdateCheckingData.dat");
									analysed = true;
								} else {
									LOG(WARNING,getLogDomain(),logTr("Request statistics evaluation of '%1' already up to date.").arg(outputDirectory),this);
								}
							}





							cint64 elapsedTime = timer.elapsed();
							LOG(INFO,getLogDomain(),logTr("Finished analysing for '%1' in '%2' seconds.").arg(ananysingString).arg(elapsedTime/1000),this);


						}

						cint64 totalElapsedTime = totalTimer.elapsed();
						LOG(INFO,getLogDomain(),logTr("Finished analysing in '%1' seconds.").arg(totalElapsedTime/1000),this);
					} else {
						LOG(ERROR,getLogDomain(),logTr("Analysing not possible, platform not specified."),this);
					}




					if (analysed) {
						LOG(INFO,getLogDomain(),logTr("Creation HTML navigation overview site."),this);
						CReasonerEvaluationHTMLNavigationOverviewSummarizer* htmlNavOvSumm = new CReasonerEvaluationHTMLNavigationOverviewSummarizer();
						QString outputDirectory = getAnalyserOutputDirectory(analysingDirectoryString,evaluationProgramName,plattform,".");
						htmlNavOvSumm->createHTMLNavigationOverview(outputDirectory,evaluationProgramName,mReasonerNameStringList,mFitlerStringList,mTestTimeout,mTestCount);
						delete htmlNavOvSumm;
					}
				}

				if (!mFailedReasonerStringList.isEmpty()) {
					LOG(WARNING,getLogDomain(),logTr("Evaluation failed for the following reasoner configuration files:\r\n%1.").arg(mFailedReasonerStringList.join(",\r\n")),this);
				}

				return this;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

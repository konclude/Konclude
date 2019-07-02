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

#include "COntologyQueryExtendedConfigDependedPreProcesser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			COntologyQueryExtendedConfigDependedPreProcesser::COntologyQueryExtendedConfigDependedPreProcesser() 
					: CLogIdentifier("::Konclude::Preprocess::QueryExtensionPreprocessor",this) {

				mNegNorm = nullptr;
				mSubRoleNorm = nullptr;
				mDatatypeNorm = nullptr;
				mDataLitNorm = nullptr;
				mTriggImplNorm = nullptr;
				mRoleChainAutomataTran = nullptr;
				mBraTrigTran = nullptr;
				mLegNorm = nullptr;
				mCommDisConExt = nullptr;
				mSortDisjPre = nullptr;
				mDataExtender = nullptr;
				mBranchStatExtener = nullptr;
			}

			COntologyQueryExtendedConfigDependedPreProcesser::~COntologyQueryExtendedConfigDependedPreProcesser() {
				delete mNegNorm;
				delete mSubRoleNorm;
				delete mDatatypeNorm;
				delete mDataLitNorm;
				delete mTriggImplNorm;
				delete mRoleChainAutomataTran;
				delete mLegNorm;
				delete mBraTrigTran;
				delete mCommDisConExt;
				delete mSortDisjPre;
				delete mDataExtender;
				delete mBranchStatExtener;
			}

			
			COntologyQueryExtendedConfigDependedPreProcesser* COntologyQueryExtendedConfigDependedPreProcesser::reportLog(const QString& string) {
				LOG(INFO,getLogDomain(),string,this);
				return this;
			}


			CConcreteOntology *COntologyQueryExtendedConfigDependedPreProcesser::preprocess(CConcreteOntology *ontology, CConfiguration *config) {

				CPreProcessContextBase preprocessingContext(ontology,config);

				bool writeDebugginData = CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config);

				if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config,"Konclude.Debugging.WritePreprocessingDebuggingData.BuildedTBox",false)) {
					COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.BuildedTBox", "Konclude.Debugging.QueryPreprocessingBaseWritingPathExtension"));
				}


				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing",true)) {

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NegationNormalization",true)) {
						reportLog("Starting negation transformation preprocessing.");
						if (!mNegNorm) {
							mNegNorm = new CNegationTransformationPreProcess();
							mNegNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mNegNorm->continuePreprocessing();
						}
						reportLog("Stopped negation transformation preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.SubroleTransformation",true)) {
						reportLog("Starting subrole transformation preprocessing.");
						if (!mSubRoleNorm) {
							mSubRoleNorm = new CSubroleTransformationPreProcess();
							mSubRoleNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mSubRoleNorm->continuePreprocessing();
						}
						reportLog("Stopped subrole transformation preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DatatypeNormalizer",true)) {
						reportLog("Starting datatype normalization preprocessing.");
						if (!mDatatypeNorm) {
							mDatatypeNorm = new CDatatypeNormalizerPreProcess();
							mDatatypeNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mDatatypeNorm->continuePreprocessing();
						}
						reportLog("Stopped datatype normalization preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DataLiteralValueNormalizer",true)) {
						reportLog("Starting data literal values normalization preprocessing.");
						if (!mDataLitNorm) {
							mDataLitNorm = new CDataLiteralNormalizerPreProcess();
							mDataLitNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mDataLitNorm->continuePreprocessing();
						}
						reportLog("Stopped data literal values normalization preprocessing.");
					}

					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.NormalizedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.NormalizedTBox", "Konclude.Debugging.QueryPreprocessingBaseWritingPathExtension"));
					}


					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.GCIAbsorption.TriggeredImplicationBinaryGCIAbsorption",true)) {
						reportLog("Starting triggered implication binary GCI absorption preprocessing.");
						if (!mTriggImplNorm) {
							mTriggImplNorm = new CTriggeredImplicationBinaryAbsorberPreProcess();
							mTriggImplNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mTriggImplNorm->continuePreprocessing();
						}
						reportLog("Stopped triggered implication binary GCI absorption preprocessing.");
					}

					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.AbsorbedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.AbsorbedTBox", "Konclude.Debugging.QueryPreprocessingBaseWritingPathExtension"));
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.RoleChainAutomataTransformation",true)) {
						reportLog("Starting role chain automata transformation preprocessing.");
						if (!mRoleChainAutomataTran) {
							mRoleChainAutomataTran = new CRoleChainAutomataTransformationPreProcess();
							mRoleChainAutomataTran->preprocess(ontology, &preprocessingContext);
						} else {
							mRoleChainAutomataTran->continuePreprocessing();
						}
						reportLog("Stopped role chain automata transformation preprocessing.");
					}



					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.LexicalNormalization",true)) {
						reportLog("Starting lexical normalization preprocessing.");
						if (!mLegNorm) {
							mLegNorm = new CLexicalNormalisationPreProcess();
							mLegNorm->preprocess(ontology, &preprocessingContext);
						} else {
							mLegNorm->continuePreprocessing();
						}
						reportLog("Stopped lexical normalization preprocessing.");
					}

					if (CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Preprocessing.BranchTriggerExtraction", true)) {
						reportLog("Starting branch trigger generation preprocessing.");
						if (!mBraTrigTran) {
							mBraTrigTran = new CBranchTriggerPreProcess();
							mBraTrigTran->preprocess(ontology, &preprocessingContext);
						}
						else {
							mBraTrigTran->continuePreprocessing();
						}
						reportLog("Stopped branch trigger generation preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CommonDisjunctConceptExtraction",true)) {
						reportLog("Starting common disjunct concept extraction preprocessing.");
						if (!mCommDisConExt) {
							mCommDisConExt = new CCommonDisjunctConceptExtractionPreProcess();
							mCommDisConExt->preprocess(ontology, &preprocessingContext);
						} else {
							mCommDisConExt->continuePreprocessing();
						}
						reportLog("Stopped common disjunct concept extraction preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctSorting",true)) {
						reportLog("Starting disjunct sorting preprocessing.");
						if (!mSortDisjPre) {
							mSortDisjPre = new CDisjunctSortingPreProcess();
							mSortDisjPre->preprocess(ontology, &preprocessingContext);
						} else {
							mSortDisjPre->continuePreprocessing();
						}
						reportLog("Stopped disjunct sorting preprocessing.");
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.ProcessingDataExtender",true)) {
						reportLog("Starting processing data extender preprocessing.");
						if (!mDataExtender) {
							mDataExtender = new CDataExtenderPreProcess();
							mDataExtender->preprocess(ontology, &preprocessingContext);
						} else {
							mDataExtender->continuePreprocessing();
						}
						reportLog("Stopped processing data extender preprocessing.");
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.BranchingStatisticsExtender",true)) {
						reportLog("Starting processing branching statistics extender preprocessing.");
						if (!mBranchStatExtener) {
							mBranchStatExtener = new CBranchStatisticsExtenderPreProcess();
							mBranchStatExtener->preprocess(ontology, &preprocessingContext);
						} else {
							mBranchStatExtener->continuePreprocessing();
						}
						reportLog("Stopped processing branching statistics extender preprocessing.");
					}




					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.ProcessedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.ProcessedTBox", "Konclude.Debugging.QueryPreprocessingBaseWritingPathExtension"));
					}



				}


				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

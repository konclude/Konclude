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

#include "COntologyConfigDependedPreProcesser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			COntologyConfigDependedPreProcesser::COntologyConfigDependedPreProcesser(CCommandRecordRouter &commandRecRouter) 
					: CLogIdentifier("::Konclude::Preprocess::Preprocessor",this) {

				commandRecordRouter = new CCommandRecordRouter(commandRecRouter,this);
			}

			COntologyConfigDependedPreProcesser::COntologyConfigDependedPreProcesser() 
					: CLogIdentifier("::Konclude::Preprocess::Preprocessor",this) {

				commandRecordRouter = nullptr;
			}

			COntologyConfigDependedPreProcesser::~COntologyConfigDependedPreProcesser() {
				delete commandRecordRouter;
			}

			
			COntologyConfigDependedPreProcesser* COntologyConfigDependedPreProcesser::reportLog(const QString& string) {
				if (commandRecordRouter) {
					CUnspecifiedMessageInformationRecord::makeRecord(string,commandRecordRouter);
				} else {
					LOG(INFO,getLogDomain(),string,this);
				}
				return this;
			}


			CConcreteOntology *COntologyConfigDependedPreProcesser::preprocess(CConcreteOntology *ontology, CConfiguration *config) {

				CPreProcessContextBase preprocessingContext(ontology,config);

				bool writeDebugginData = CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config);

				if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config,"Konclude.Debugging.WritePreprocessingDebuggingData.BuildedTBox",false)) {
					COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.BuildedTBox"));
				}


				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing",true)) {

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NegationNormalization",true)) {
						COntologyPreProcess *negNorm = new CNegationTransformationPreProcess();
						reportLog("Starting negation transformation preprocessing.");
						negNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped negation transformation preprocessing.");
						delete negNorm;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.SubroleTransformation",true)) {
						COntologyPreProcess *subRoleNorm = new CSubroleTransformationPreProcess();
						reportLog("Starting subrole transformation preprocessing.");
						subRoleNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped subrole transformation preprocessing.");
						delete subRoleNorm;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DatatypeNormalizer",true)) {
						COntologyPreProcess* datatypeNorm = new CDatatypeNormalizerPreProcess();
						reportLog("Starting datatype normalization preprocessing.");
						datatypeNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped datatype normalization preprocessing.");
						delete datatypeNorm;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DataLiteralValueNormalizer",true)) {
						COntologyPreProcess* dataLitNorm = new CDataLiteralNormalizerPreProcess();
						reportLog("Starting data literal values normalization preprocessing.");
						dataLitNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped data literal values normalization preprocessing.");
						delete dataLitNorm;
					}

					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.NormalizedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.NormalizedTBox"));
					}


					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NominalSchemaTemplateExtraction",true)) {
						COntologyPreProcess *onPreProc = new CNominalSchemaTemplateExtractionPreProcess();
						reportLog("Starting nominal schema template extraction preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped nominal schema template extraction preprocessing.");
						delete onPreProc;
					} 
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.NominalSchemaGrounding",true)) {
						COntologyPreProcess *onPreProc = new CFullNominalSchemaGroundingPreProcess();
						reportLog("Starting full nominal schema grounding preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped full nominal schema grounding preprocessing.");
						delete onPreProc;
					} 

					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.GroundedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.GroundedTBox"));
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.GCIAbsorption.TriggeredImplicationBinaryGCIAbsorption",true)) {
						COntologyPreProcess *triggImplNorm = new CTriggeredImplicationBinaryAbsorberPreProcess();
						reportLog("Starting triggered implication binary GCI absorption preprocessing.");
						triggImplNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped triggered implication binary GCI absorption preprocessing.");
						delete triggImplNorm;
					}

					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.AbsorbedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.AbsorbedTBox"));
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.RoleChainAutomataTransformation",true)) {
						COntologyPreProcess *onPreProc = new CRoleChainAutomataTransformationPreProcess();
						reportLog("Starting role chain automata transformation preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped role chain automata transformation preprocessing.");
						delete onPreProc;
					}



					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.LexicalNormalization",true)) {
						COntologyPreProcess *legNorm = new CLexicalNormalisationPreProcess();
						reportLog("Starting lexical normalization preprocessing.");
						legNorm->preprocess(ontology,&preprocessingContext);		
						reportLog("Stopped lexical normalization preprocessing.");
						delete legNorm;
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.BranchTriggerExtraction",true)) {
						COntologyPreProcess *braTrigTran = new CBranchTriggerPreProcess();
						reportLog("Starting branch trigger generation preprocessing.");
						braTrigTran->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped branch trigger generation preprocessing.");
						delete braTrigTran;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CommonDisjunctConceptExtraction",true)) {
						COntologyPreProcess *commDisConExt = new CCommonDisjunctConceptExtractionPreProcess();
						reportLog("Starting common disjunct concept extraction preprocessing.");
						commDisConExt->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped common disjunct concept extraction preprocessing.");
						delete commDisConExt;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.DisjunctSorting",true)) {
						COntologyPreProcess *sortDisjPre = new CDisjunctSortingPreProcess();
						reportLog("Starting disjunct sorting preprocessing.");
						sortDisjPre->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped disjunct sorting preprocessing.");
						delete sortDisjPre;
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.OntologyAssignmentTransformation",true)) {
						COntologyPreProcess *ontAssignNorm = new COntologyAssignmentTransformationPreProcess(ontology);
						reportLog("Starting ontology revision assignment preprocessing.");
						ontAssignNorm->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped ontology revision assignment preprocessing.");
						delete ontAssignNorm;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.OntologyMappingUpdate",true)) {
						COntologyPreProcess *onPreProc = new CMappingUpdaterPreProcess();
						reportLog("Starting ontology mapping update preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped ontology mapping update preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.ProcessingDataExtender",true)) {
						COntologyPreProcess *onPreProc = new CDataExtenderPreProcess();
						reportLog("Starting processing data extender preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped processing data extender preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.BranchingStatisticsExtender",true)) {
						COntologyPreProcess *onPreProc = new CBranchStatisticsExtenderPreProcess();
						reportLog("Starting processing branching statistics extender preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped processing branching statistics extender preprocessing.");
						delete onPreProc;
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.ReverseRoleAssertionGeneration",true)) {
						COntologyPreProcess *onPreProc = new CReverseRoleAssertionGeneratorPreProcess();
						reportLog("Starting reverse role assertion generation.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped reverse role assertion generation.");
						delete onPreProc;
					}

					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.OntologyStructureInspection",true)) {
						COntologyPreProcess *onPreProc = new COntologyStructureInspectionPreProcess();
						reportLog("Starting ontology structure inspection preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped ontology structure inspection preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.ExtractPropagationIntoCreationDirection",true)) {
						COntologyPreProcess *onPreProc = new CExtractPropagationIntoCreationDirectionPreProcess();
						reportLog("Starting extraction for propagations into creation direction preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped extraction for propagations into creation direction preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CoreBlockingConceptMarking",true)) {
						COntologyPreProcess *onPreProc = new CCoreBlockingConceptMarkerPreProcess();
						reportLog("Starting marking core blocking concept preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped marking core blocking concept preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.RelevantConceptRoleMarker",false)) {
						COntologyPreProcess *onPreProc = new CExtractRelevantConceptRolePreProcess();
						reportLog("Starting marking relevant concepts and roles preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped marking relevant concepts and roles preprocessing.");
						delete onPreProc;
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.CoreConceptCyclesExtraction",true)) {
						COntologyPreProcess *onPreProc = new CCoreOntologyCyclesExtractorPreProcess();
						reportLog("Starting extracting core concept cycles preprocessing.");
						onPreProc->preprocess(ontology,&preprocessingContext);
						reportLog("Stopped extracting core concept cycles preprocessing.");
						delete onPreProc;
					}



					if (writeDebugginData && CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData.ProcessedTBox", false)) {
						COntologyTextFormater::writeOntologyToFile(ontology, CConfigDebugDataReader::readConfigDebuggingWritingFileString(config, "Konclude.Debugging.WritingPath.Preprocessing.ProcessedTBox"));
					}



				}


				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

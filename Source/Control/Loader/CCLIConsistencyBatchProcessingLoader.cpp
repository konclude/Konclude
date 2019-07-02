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

#include "CCLIConsistencyBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLIConsistencyBatchProcessingLoader::CCLIConsistencyBatchProcessingLoader() {
			}



			CCLIConsistencyBatchProcessingLoader::~CCLIConsistencyBatchProcessingLoader() {
			}


			
			
			void CCLIConsistencyBatchProcessingLoader::createTestingCommands() {
				createConsistencyTestingCommands();
			}


			void CCLIConsistencyBatchProcessingLoader::createConsistencyTestingCommands() {
				logOutputMessage(QString("Starting consistency checking for '%1'.").arg(mRequestFileString));
				QString testKB = QString("http://konclude.com/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(testKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mRequestFileString);
				//CLoadKnowledgeBaseOWLXMLOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLXMLOntologyCommand(testKB,ontoIRIList);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(testKB,ontoIRIList);
				CIsConsistentQueryCommand* consistencyKBCommand = new CIsConsistentQueryCommand(testKB);
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(consistencyKBCommand,false,"",true,mResponseFileString);
				processNextCommand();
			}


			void CCLIConsistencyBatchProcessingLoader::writeCommandOutput(const QString& outputFileName, CCommand* processedCommand) {
				bool outputWritten = false;
				CKnowledgeBaseQueryCommand* kbQueryCommand = dynamic_cast<CKnowledgeBaseQueryCommand*>(processedCommand);
				if (kbQueryCommand) {
					CQuery* query = kbQueryCommand->getCalculateQueryCommand()->getQuery();
					if (query) {
						CQueryResult* queryResult = query->getQueryResult();
						if (queryResult) {
							CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(queryResult);
							if (boolQueryResult) {
								outputWritten = true;
								if (boolQueryResult->getResult() == true) {
									logOutputMessage(QString("Ontology '%1' is consistent.").arg(mRequestFileString));
								} else {
									logOutputMessage(QString("Ontology '%1' is not consistent.").arg(mRequestFileString));
								}

								if (!outputFileName.isEmpty()) {
									forcedPathCreated(outputFileName);
									QFile outputFile(outputFileName);
									if (outputFile.open(QIODevice::WriteOnly)) {
										QString outputData;
										if (boolQueryResult->getResult() == true) {
											outputData = QString("true\n");
										} else {
											outputData = QString("false\n");
										}
										outputFile.write(outputData.toUtf8());
										outputFile.close();
									} else {
										logOutputError(QString("Failed writing output to file '%1'.").arg(outputFileName));
									}
								}
							}
						}
					}
				}
				if (!outputWritten) {
					logOutputError("Consistency checking failed.");
				}
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

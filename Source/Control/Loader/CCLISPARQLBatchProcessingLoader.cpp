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

#include "CCLISPARQLBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLISPARQLBatchProcessingLoader::CCLISPARQLBatchProcessingLoader() {
			}



			CCLISPARQLBatchProcessingLoader::~CCLISPARQLBatchProcessingLoader() {
			}


			CLoader *CCLISPARQLBatchProcessingLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				CCLIBatchProcessingLoader::init(loaderFactory, config);

				mSPARQLFileNameString = CConfigDataReader::readConfigString(config, "Konclude.SPARQL.RequestFile");
				return this;
			}
			
			
			void CCLISPARQLBatchProcessingLoader::createTestingCommands() {
				createRealizationTesingCommands();
			}


			void CCLISPARQLBatchProcessingLoader::createRealizationTesingCommands() {
				logOutputMessage(QString("Starting SPARQL query processing for ontology '%1'.").arg(mRequestFileString));
				QString testKB = QString("http://konclude.com/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(testKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mRequestFileString);
				//CLoadKnowledgeBaseOWLXMLOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLXMLOntologyCommand(testKB,ontoIRIList);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(testKB,ontoIRIList);
				//CRealizeQueryCommand* realizeKBCommand = new CRealizeQueryCommand(testKB);
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				//addProcessingCommand(realizeKBCommand);
				if (!mResponseFileString.isEmpty()) {
					//CWriteCustomQueryCommand* writeTypesCommand = new CWriteCustomQueryCommand(testKB,CWriteQuery::WRITEINDIVIDUALFLATTENEDTYPES,new CWriteQueryFileOWL2XMLSerializer(mResponseFileString));
					//addProcessingCommand(writeTypesCommand);
				}
				processNextCommand();
			}





		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

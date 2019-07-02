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

#include "CCLIObjectPropertyClassificationBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLIObjectPropertyClassificationBatchProcessingLoader::CCLIObjectPropertyClassificationBatchProcessingLoader() {
			}



			CCLIObjectPropertyClassificationBatchProcessingLoader::~CCLIObjectPropertyClassificationBatchProcessingLoader() {
			}


			
			
			void CCLIObjectPropertyClassificationBatchProcessingLoader::createTestingCommands() {
				createClassificationTestingCommands();
			}


			void CCLIObjectPropertyClassificationBatchProcessingLoader::createClassificationTestingCommands() {
				logOutputMessage(QString("Starting object property classification for '%1'.").arg(mRequestFileString));
				QString testKB = QString("http://konclude.com/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(testKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mRequestFileString);
				//CLoadKnowledgeBaseOWLXMLOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLXMLOntologyCommand(testKB,ontoIRIList);
				//CLoadKnowledgeBaseOWLFunctionalOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLFunctionalOntologyCommand(testKB,ontoIRIList);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(testKB,ontoIRIList);
				//CClassifyQueryCommand* classifyKBCommand = new CClassifyQueryCommand(testKB);
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				//addProcessingCommand(classifyKBCommand);
				if (!mResponseFileString.isEmpty()) {
					CWriteCustomQueryCommand* writeHierarchyCommand = new CWriteCustomQueryCommand(testKB,CWriteQuery::WRITESUBOBJECTPROPERTYHIERARCHY,new CWriteQueryFileOWL2XMLSerializer(mResponseFileString));
					addProcessingCommand(writeHierarchyCommand);
				}
				processNextCommand();
			}





		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

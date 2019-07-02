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

#include "CCLIClassificationBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLIClassificationBatchProcessingLoader::CCLIClassificationBatchProcessingLoader() {
			}



			CCLIClassificationBatchProcessingLoader::~CCLIClassificationBatchProcessingLoader() {
			}


			
			
			void CCLIClassificationBatchProcessingLoader::createTestingCommands() {
				createClassificationTestingCommands();
			}


			void CCLIClassificationBatchProcessingLoader::createClassificationTestingCommands() {
				logOutputMessage(QString("Starting classification for '%1'.").arg(mRequestFileString));
				QString testKB = QString("http://konclude.com/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(testKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mRequestFileString);
				//CLoadKnowledgeBaseOWLXMLOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLXMLOntologyCommand(testKB,ontoIRIList);
				//CLoadKnowledgeBaseOWLFunctionalOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLFunctionalOntologyCommand(testKB,ontoIRIList);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(testKB,ontoIRIList);
				CClassifyQueryCommand* classifyKBCommand = new CClassifyQueryCommand(testKB);
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(classifyKBCommand);
				if (!mResponseFileString.isEmpty()) {
					CWriteXMLSubClassHierarchyQueryCommand* writeHierarchyCommand = new CWriteXMLSubClassHierarchyQueryCommand(testKB,mResponseFileString);
					addProcessingCommand(writeHierarchyCommand);
				}
				processNextCommand();
			}





		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

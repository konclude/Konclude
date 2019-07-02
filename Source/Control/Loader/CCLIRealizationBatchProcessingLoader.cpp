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

#include "CCLIRealizationBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLIRealizationBatchProcessingLoader::CCLIRealizationBatchProcessingLoader() {
			}



			CCLIRealizationBatchProcessingLoader::~CCLIRealizationBatchProcessingLoader() {
			}


			
			
			void CCLIRealizationBatchProcessingLoader::createTestingCommands() {
				createRealizationTesingCommands();
			}


			void CCLIRealizationBatchProcessingLoader::createRealizationTesingCommands() {
				if (mIRINameString.isEmpty()) {
					logOutputMessage(QString("Starting realization processing for ontology '%1'.").arg(mRequestFileString));
				} else {
					logOutputMessage(QString("Starting realization processing of individual '%1' for ontology '%2'.").arg(mIRINameString).arg(mRequestFileString));
				}
				QString testKB = QString("http://konclude.com/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(testKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mRequestFileString);
				//CLoadKnowledgeBaseOWLXMLOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLXMLOntologyCommand(testKB,ontoIRIList);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(testKB,ontoIRIList);
				CRealizeQueryCommand* realizeKBCommand = new CRealizeQueryCommand(testKB);
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(realizeKBCommand);
				if (!mResponseFileString.isEmpty()) {
					CWriteXMLIndividualTypesQueryCommand* realizeKBCommand = new CWriteXMLIndividualTypesQueryCommand(testKB,mResponseFileString,mIRINameString);
					addProcessingCommand(realizeKBCommand);
				}
				processNextCommand();
			}





		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

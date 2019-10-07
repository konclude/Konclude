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

#include "CConstructWriteXMLSubClassHirarchyQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructWriteXMLSubClassHirarchyQueryCommand::CConstructWriteXMLSubClassHirarchyQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, const QString& outputFileString, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mOutputFileName = outputFileString;
					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructWriteXMLSubClassHirarchyQueryCommand::~CConstructWriteXMLSubClassHirarchyQueryCommand() {
					delete query;
				}

				qint64 CConstructWriteXMLSubClassHirarchyQueryCommand::getCommandTag() {
					return CONSTRUCTWRITEXMLSUBCLASSHIERARCHYQUERYCOMMAND;
				}


				QString CConstructWriteXMLSubClassHirarchyQueryCommand::getBriefCommandDescription() {
					return QString("Construct-WriteXMLSubClassHierarchy-Query Command");
				}


				CQuery *CConstructWriteXMLSubClassHirarchyQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructWriteXMLSubClassHirarchyQueryCommand::getCommand() {
					return this;
				}


				CConstructWriteXMLSubClassHirarchyQueryCommand *CConstructWriteXMLSubClassHirarchyQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructWriteXMLSubClassHirarchyQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				QString CConstructWriteXMLSubClassHirarchyQueryCommand::getOutputFileName() {
					return mOutputFileName;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

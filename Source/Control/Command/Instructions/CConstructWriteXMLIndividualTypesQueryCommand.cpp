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

#include "CConstructWriteXMLIndividualTypesQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructWriteXMLIndividualTypesQueryCommand::CConstructWriteXMLIndividualTypesQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, const QString& outputFileString, const QString& individualName, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mOutputFileName = outputFileString;
					mIndividualName = individualName;
					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructWriteXMLIndividualTypesQueryCommand::~CConstructWriteXMLIndividualTypesQueryCommand() {
					delete query;
				}

				qint64 CConstructWriteXMLIndividualTypesQueryCommand::getCommandTag() {
					return CONSTRUCTWRITEXMLINDIVIDUALTYPESQUERYCOMMAND;
				}


				QString CConstructWriteXMLIndividualTypesQueryCommand::getBriefCommandDescription() {
					return QString("Construct-WriteXMLIndividualTypes-Query Command");
				}


				CQuery *CConstructWriteXMLIndividualTypesQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructWriteXMLIndividualTypesQueryCommand::getCommand() {
					return this;
				}


				CConstructWriteXMLIndividualTypesQueryCommand *CConstructWriteXMLIndividualTypesQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructWriteXMLIndividualTypesQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				QString CConstructWriteXMLIndividualTypesQueryCommand::getOutputFileName() {
					return mOutputFileName;
				}

				QString CConstructWriteXMLIndividualTypesQueryCommand::getIndividualName() {
					return mIndividualName;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

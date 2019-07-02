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

#include "CConstructWriteCustomQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructWriteCustomQueryCommand::CConstructWriteCustomQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand,  CWriteQuery::WRITEQUERYTYPE queryType, CWriteQuerySerializer* serializer, const QString& entityRestrictionString, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					mQueryType = queryType;
					mSerializer = serializer;
					mEntityRestrictionString = entityRestrictionString;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructWriteCustomQueryCommand::~CConstructWriteCustomQueryCommand() {
					delete query;
				}

				qint64 CConstructWriteCustomQueryCommand::getCommandTag() {
					return CONSTRUCTWRITECUSTOMQUERYCOMMAND;
				}


				QString CConstructWriteCustomQueryCommand::getBriefCommandDescription() {
					return QString("Construct-Write-Custom-Query Command");
				}


				CQuery *CConstructWriteCustomQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructWriteCustomQueryCommand::getCommand() {
					return this;
				}


				CConstructWriteCustomQueryCommand *CConstructWriteCustomQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructWriteCustomQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}


				CWriteQuerySerializer* CConstructWriteCustomQueryCommand::getWriteQuerySerializer() {
					return mSerializer;
				}

				CWriteQuery::WRITEQUERYTYPE CConstructWriteCustomQueryCommand::getWriteQueryType() {
					return mQueryType;
				}

				const QString CConstructWriteCustomQueryCommand::getEntityRestrictionString() {
					return mEntityRestrictionString;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

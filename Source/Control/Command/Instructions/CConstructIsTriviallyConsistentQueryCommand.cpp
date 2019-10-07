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

#include "CConstructIsTriviallyConsistentQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructIsTriviallyConsistentQueryCommand::CConstructIsTriviallyConsistentQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructIsTriviallyConsistentQueryCommand::~CConstructIsTriviallyConsistentQueryCommand() {
					delete query;
				}

				qint64 CConstructIsTriviallyConsistentQueryCommand::getCommandTag() {
					return CONSTRUCTISTRIVIALLYCONSISTENTQUERYCOMMAND;
				}


				QString CConstructIsTriviallyConsistentQueryCommand::getBriefCommandDescription() {
					return QString("Construct-IsTriviallyConsistent-Query Command");
				}


				CQuery *CConstructIsTriviallyConsistentQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructIsTriviallyConsistentQueryCommand::getCommand() {
					return this;
				}


				CConstructIsTriviallyConsistentQueryCommand *CConstructIsTriviallyConsistentQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructIsTriviallyConsistentQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

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

#include "CConstructClassSatisfiableQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructClassSatisfiableQueryCommand::CConstructClassSatisfiableQueryCommand(const QString& className, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mQuery = nullptr;
					mClassName = className;
					revProvider = ontologyRevisionProviderCommand;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructClassSatisfiableQueryCommand::~CConstructClassSatisfiableQueryCommand() {
					delete mQuery;
				}

				qint64 CConstructClassSatisfiableQueryCommand::getCommandTag() {
					return CONSTRUCTCLASSSATISFIABLEQUERYCOMMAND;
				}


				QString CConstructClassSatisfiableQueryCommand::getBriefCommandDescription() {
					return QString("Construct-Class-Satisfiable-Query Command");
				}


				CCommand *CConstructClassSatisfiableQueryCommand::getCommand() {
					return this;
				}

				COntologyRevision *CConstructClassSatisfiableQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				const QString& CConstructClassSatisfiableQueryCommand::getClassName() {
					return mClassName;
				}


				CQuery* CConstructClassSatisfiableQueryCommand::getQuery() {
					return mQuery;
				}

				CConstructClassSatisfiableQueryCommand* CConstructClassSatisfiableQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					mQuery = knowledgeBaseQuery;
					return this;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

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

#include "CConstructSubClassHirarchyQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructSubClassHirarchyQueryCommand::CConstructSubClassHirarchyQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, const QDomElement &queryNode, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					node = queryNode;
					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructSubClassHirarchyQueryCommand::~CConstructSubClassHirarchyQueryCommand() {
					delete query;
				}

				qint64 CConstructSubClassHirarchyQueryCommand::getCommandTag() {
					return CONSTRUCTSUBCLASSHIERARCHYQUERYCOMMAND;
				}


				QString CConstructSubClassHirarchyQueryCommand::getBriefCommandDescription() {
					return QString("Construct-SubClassHierarchy-Query Command");
				}


				CQuery *CConstructSubClassHirarchyQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructSubClassHirarchyQueryCommand::getCommand() {
					return this;
				}


				CConstructSubClassHirarchyQueryCommand *CConstructSubClassHirarchyQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructSubClassHirarchyQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				QDomElement *CConstructSubClassHirarchyQueryCommand::getQueryNode() {
					return &node;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

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

#include "CParseSPARQLQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseSPARQLQueryCommand::CParseSPARQLQueryCommand(const QStringList& queryTextList, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand)
						: CParseOntologyCommand(parentSuperCommand) {

					mQueryTextList = queryTextList;
					revProvider = ontologyRevisionProviderCommand;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}



				CParseSPARQLQueryCommand::~CParseSPARQLQueryCommand() {
				}

				qint64 CParseSPARQLQueryCommand::getCommandTag() {
					return PARSESPARQLQUERYCOMMAND;
				}


				QString CParseSPARQLQueryCommand::getBriefCommandDescription() {
					return QString("Parse-SPARQL-Query Command");
				}

				QStringList CParseSPARQLQueryCommand::getSPARQLQueryTextList() {
					return mQueryTextList;
				}


				CCommand *CParseSPARQLQueryCommand::getCommand() {
					return this;
				}


				COntologyRevision *CParseSPARQLQueryCommand::getOntologyRevision() {
					return revProvider->getOntologyRevision();
				}

				QList<CQuery*> CParseSPARQLQueryCommand::getQueryList() {
					return mQueryList;
				}

				CParseSPARQLQueryCommand* CParseSPARQLQueryCommand::setQueryList(const QList<CQuery*>& queryList) {
					mQueryList = queryList;
					return this;
				}

				CParseSPARQLQueryCommand* CParseSPARQLQueryCommand::addQueryText(const QStringList& queryTextList) {
					mQueryTextList += queryTextList;
					return this;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

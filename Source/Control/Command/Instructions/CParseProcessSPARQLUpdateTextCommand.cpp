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

#include "CParseProcessSPARQLUpdateTextCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseProcessSPARQLUpdateTextCommand::CParseProcessSPARQLUpdateTextCommand(const QString& updateText, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand)
						: CParseOntologyCommand(parentSuperCommand) {
					mUpdateText = updateText;
					mRevProvider = ontologyRevisionProviderCommand;
					mUpdateTextSplitted = false;
					addCommandPrecondition(new CCommandProcessedPrecondition(mRevProvider->getCommand()));
				}


				CParseProcessSPARQLUpdateTextCommand::CParseProcessSPARQLUpdateTextCommand(const QStringList& updateTextList, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand)
					: CParseOntologyCommand(parentSuperCommand) {
					mUpdateTextList = updateTextList;
					mRevProvider = ontologyRevisionProviderCommand;
					mUpdateTextSplitted = true;
					addCommandPrecondition(new CCommandProcessedPrecondition(mRevProvider->getCommand()));
				}

				CParseProcessSPARQLUpdateTextCommand::~CParseProcessSPARQLUpdateTextCommand() {
				}

				qint64 CParseProcessSPARQLUpdateTextCommand::getCommandTag() {
					return PROCESSPARSESPARQLUPDATETEXTCOMMAND;
				}


				QString CParseProcessSPARQLUpdateTextCommand::getBriefCommandDescription() {
					return QString("Process-Parse-SPARQL-Update Command");
				}

				QString CParseProcessSPARQLUpdateTextCommand::getSPARQLUpdateText() {
					return mUpdateText;
				}

				QStringList CParseProcessSPARQLUpdateTextCommand::getSPARQLUpdateTextList() {
					return mUpdateTextList;
				}


				bool CParseProcessSPARQLUpdateTextCommand::isUpdateTextSplitted() {
					return mUpdateTextSplitted;
				}


				CCommand *CParseProcessSPARQLUpdateTextCommand::getCommand() {
					return this;
				}


				COntologyRevision *CParseProcessSPARQLUpdateTextCommand::getOntologyRevision() {
					return mRevProvider->getOntologyRevision();
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

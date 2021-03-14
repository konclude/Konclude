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

#include "CParseProcessSPARQLManageTextCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseProcessSPARQLManageTextCommand::CParseProcessSPARQLManageTextCommand(const QString& updateText, const QString &knowlegdeBaseName, CCommand *parentSuperCommand)
						: CParseOntologyCommand(parentSuperCommand) {
					mUpdateText = updateText;
					mUpdateTextSplitted = false;
					mKnowledgeBaseName = knowlegdeBaseName;
				}


				CParseProcessSPARQLManageTextCommand::CParseProcessSPARQLManageTextCommand(const QStringList& updateTextList, const QString &knowlegdeBaseName, CCommand *parentSuperCommand)
					: CParseOntologyCommand(parentSuperCommand) {
					mUpdateTextList = updateTextList;
					mUpdateTextSplitted = true;
					mKnowledgeBaseName = knowlegdeBaseName;
				}

				CParseProcessSPARQLManageTextCommand::~CParseProcessSPARQLManageTextCommand() {
				}

				qint64 CParseProcessSPARQLManageTextCommand::getCommandTag() {
					return PROCESSPARSESPARQLMANAGETEXTCOMMAND;
				}


				QString CParseProcessSPARQLManageTextCommand::getBriefCommandDescription() {
					return QString("Process-Parse-SPARQL-Manage Command");
				}

				QString CParseProcessSPARQLManageTextCommand::getSPARQLUpdateText() {
					return mUpdateText;
				}

				QStringList CParseProcessSPARQLManageTextCommand::getSPARQLUpdateTextList() {
					return mUpdateTextList;
				}


				bool CParseProcessSPARQLManageTextCommand::isUpdateTextSplitted() {
					return mUpdateTextSplitted;
				}

				CParseProcessSPARQLManageTextCommand *CParseProcessSPARQLManageTextCommand::setKnowlegeBaseName(const QString &knowlegdeBaseName) {
					mKnowledgeBaseName = knowlegdeBaseName;
					return this;
				}

				QString CParseProcessSPARQLManageTextCommand::getKnowlegeBaseString() {
					return mKnowledgeBaseName;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

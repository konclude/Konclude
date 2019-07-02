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

#include "CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand(const QString &knowledgeBaseNameString, const QList<CLoadKnowledgeBaseData*>& loadDataList, QMap<QString,QString> ontoIRIMapping, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CLoadKnowledgeBaseCommand(knowledgeBaseNameString,parentSuperCommand) {
					mLoadDataList = loadDataList;
					mOntoIRIMapping = ontoIRIMapping;
					mOntologyRevisionProviderCommand = ontologyRevisionProviderCommand;
				}


				CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::~CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand() {
				}


				qint64 CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getCommandTag() {
					return CHOOSEPARSEINSTALLKNOWLEDGEBASEOWLAUTOONTOLOGYCOMMAND;
				}


				COntologyRevision* CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getOntologyRevision() {
					return mOntologyRevisionProviderCommand->getOntologyRevision();
				}


				QString CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getBriefCommandDescription() {
					return QString("Choose-Parse-Install-'%1'-Knowledge-Base-OWL-Auto-Ontology Command").arg(kbString);
				}



				bool CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getLoadImports() {
					return mLoadImports;
				}


				CCommand* CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getCommand() {
					return this;
				}


				QList<CLoadKnowledgeBaseData*> CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getLoadDataList() {
					return mLoadDataList;
				}

				QMap<QString,QString> CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand::getOntologieIRIMappings() {
					return mOntoIRIMapping;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

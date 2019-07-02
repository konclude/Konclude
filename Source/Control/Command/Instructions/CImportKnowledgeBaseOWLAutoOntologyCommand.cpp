/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CImportKnowledgeBaseOWLAutoOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CImportKnowledgeBaseOWLAutoOntologyCommand::CImportKnowledgeBaseOWLAutoOntologyCommand(const QString &knowledgeBaseNameString, const QStringList& loadIRIs, const QMap<QString,QString>& ontoIRIMap, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CLoadKnowledgeBaseCommand(knowledgeBaseNameString,parentSuperCommand) {
					mOntoIRIMapping = ontoIRIMap;
					mLoadIRIs = loadIRIs;
					mOntologyRevisionProviderCommand = ontologyRevisionProviderCommand;
				}


				CImportKnowledgeBaseOWLAutoOntologyCommand::~CImportKnowledgeBaseOWLAutoOntologyCommand() {
				}


				qint64 CImportKnowledgeBaseOWLAutoOntologyCommand::getCommandTag() {
					return IMPORTKNOWLEDGEBASEOWLAUTOONTOLOGYCOMMAND;
				}


				COntologyRevision* CImportKnowledgeBaseOWLAutoOntologyCommand::getOntologyRevision() {
					return mOntologyRevisionProviderCommand->getOntologyRevision();
				}


				QString CImportKnowledgeBaseOWLAutoOntologyCommand::getBriefCommandDescription() {
					return QString("Load-'%1'-Knowledge-Base-Imports-OWL-Auto-Ontology Command").arg(kbString);
				}



				bool CImportKnowledgeBaseOWLAutoOntologyCommand::getLoadImports() {
					return mLoadImports;
				}


				CCommand* CImportKnowledgeBaseOWLAutoOntologyCommand::getCommand() {
					return this;
				}

				QStringList CImportKnowledgeBaseOWLAutoOntologyCommand::getLoadIRIStrings() {
					return mLoadIRIs;
				}

				QMap<QString,QString> CImportKnowledgeBaseOWLAutoOntologyCommand::getOntologieIRIMappings() {
					return mOntoIRIMapping;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

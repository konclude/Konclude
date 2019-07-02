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

#include "CInstallKnowledgeBaseRevisionUpdateCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CInstallKnowledgeBaseRevisionUpdateCommand::CInstallKnowledgeBaseRevisionUpdateCommand(const QString &knowledgeBaseNameString, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CKnowledgeBaseCommand(parentSuperCommand) {
					kbString = knowledgeBaseNameString;
					onRevProvider = ontologyRevisionProviderCommand;
					mOntRevision = nullptr;
					//addCommandPrecondition(new CCommandProcessedPrecondition(onRevProvider->getCommand()));
				}


				CInstallKnowledgeBaseRevisionUpdateCommand::CInstallKnowledgeBaseRevisionUpdateCommand(const QString &knowledgeBaseNameString, COntologyRevision* ontRevision, CCommand *parentSuperCommand) {
					kbString = knowledgeBaseNameString;
					mOntRevision = ontRevision;
					onRevProvider = nullptr;
				}


				CInstallKnowledgeBaseRevisionUpdateCommand::~CInstallKnowledgeBaseRevisionUpdateCommand() {
				}


				qint64 CInstallKnowledgeBaseRevisionUpdateCommand::getCommandTag() {
					return INSTALLKNOWLEDGEBASEREVISIONCOMMAND;
				}

				QString CInstallKnowledgeBaseRevisionUpdateCommand::getKnowledgeBaseName() {
					return kbString;
				}


				QString CInstallKnowledgeBaseRevisionUpdateCommand::getBriefCommandDescription() {
					return QString("Install-'%1'-Knowledge-Base-Revision-Update Command").arg(kbString);
				}


				COntologyRevision *CInstallKnowledgeBaseRevisionUpdateCommand::getOntologyRevision() {
					if (mOntRevision) {
						return mOntRevision;
					} else if (onRevProvider) {
						return onRevProvider->getOntologyRevision();
					}
					return nullptr;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

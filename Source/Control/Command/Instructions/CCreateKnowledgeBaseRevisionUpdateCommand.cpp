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

#include "CCreateKnowledgeBaseRevisionUpdateCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CCreateKnowledgeBaseRevisionUpdateCommand::CCreateKnowledgeBaseRevisionUpdateCommand(const QString &knowledgeBaseNameString, CCommand *parentSuperCommand) 
						: CKnowledgeBaseCommand(parentSuperCommand) {
					kbString = knowledgeBaseNameString;
					onRev = nullptr;
					mCreateIfNotExist = false;
				}

				CCreateKnowledgeBaseRevisionUpdateCommand::CCreateKnowledgeBaseRevisionUpdateCommand(const QString &knowledgeBaseNameString, bool createIfNotExist, CCommand *parentSuperCommand) : CKnowledgeBaseCommand(parentSuperCommand) {
					kbString = knowledgeBaseNameString;
					onRev = nullptr;
					mCreateIfNotExist = createIfNotExist;
				}


				CCreateKnowledgeBaseRevisionUpdateCommand::~CCreateKnowledgeBaseRevisionUpdateCommand() {
				}


				qint64 CCreateKnowledgeBaseRevisionUpdateCommand::getCommandTag() {
					return CREATEKNOWLEDGEBASEREVISIONCOMMAND;
				}

				QString CCreateKnowledgeBaseRevisionUpdateCommand::getKnowledgeBaseName() {
					return kbString;
				}


				QString CCreateKnowledgeBaseRevisionUpdateCommand::getBriefCommandDescription() {
					return QString("Create-'%1'-Knowledge-Base-Revision-Update Command").arg(kbString);
				}


				COntologyRevision *CCreateKnowledgeBaseRevisionUpdateCommand::getOntologyRevision() {
					return onRev;
				}

				CCreateKnowledgeBaseRevisionUpdateCommand *CCreateKnowledgeBaseRevisionUpdateCommand::setOntologyRevision(COntologyRevision *ontologyRevision) {
					onRev = ontologyRevision;
					return this;
				}

				CCommand *CCreateKnowledgeBaseRevisionUpdateCommand::getCommand() {
					return this;
				}


				bool CCreateKnowledgeBaseRevisionUpdateCommand::requireCreateIfNotExist() {
					return mCreateIfNotExist;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

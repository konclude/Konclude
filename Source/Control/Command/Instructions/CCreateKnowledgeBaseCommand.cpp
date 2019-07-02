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

#include "CCreateKnowledgeBaseCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CCreateKnowledgeBaseCommand::CCreateKnowledgeBaseCommand(const QString &knowledgeBaseNameString, CCommand *parentSuperCommand) 
						: CKnowledgeBaseCommand(parentSuperCommand) {
					kbString = knowledgeBaseNameString;
				}


				CCreateKnowledgeBaseCommand::~CCreateKnowledgeBaseCommand() {
					cDeleteAll(mPrefixList);
				}


				qint64 CCreateKnowledgeBaseCommand::getCommandTag() {
					return CREATEKNOWLEDGEBASECOMMAND;
				}

				QString CCreateKnowledgeBaseCommand::getKnowledgeBaseName() {
					return kbString;
				}


				QString CCreateKnowledgeBaseCommand::getBriefCommandDescription() {
					return QString("Create-'%1'-Knowledge-Base Command").arg(kbString);
				}



				QList<CNamePrefix*>* CCreateKnowledgeBaseCommand::getPrefixList() {
					return &mPrefixList;
				}

				CCreateKnowledgeBaseCommand* CCreateKnowledgeBaseCommand::addNamePrefix(CNamePrefix* prefix) {
					mPrefixList.append(prefix);
					return this;
				}

				bool CCreateKnowledgeBaseCommand::hasPrefixes() {
					return !mPrefixList.isEmpty();
				}


				CCreateKnowledgeBaseCommand* CCreateKnowledgeBaseCommand::setKnowledgeBaseName(const QString& kbNameString) {
					kbString = kbNameString;
					return this;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

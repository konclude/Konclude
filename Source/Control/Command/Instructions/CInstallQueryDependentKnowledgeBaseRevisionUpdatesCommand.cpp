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

#include "CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand(QList<CKnowledgeBaseRevisionCommandProvider*> kbProviderList, CQuery* query, CCommand *parentSuperCommand)
						: CPreconditionCommand(parentSuperCommand) {

					mQuery = query;
					mKBProviderList.append(kbProviderList);
					for (CKnowledgeBaseRevisionCommandProvider* kbCommProv : kbProviderList) {
						addCommandPrecondition(new CCommandProcessedPrecondition(kbCommProv->getCommand()));
					}
				}


				CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::~CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand() {
				}

				qint64 CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::getCommandTag() {
					return INSTALLQUERYDEPENDENTKNOWLEDGEBASEREVISONUPDATESCOMMAND;
				}


				QString CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::getBriefCommandDescription() {
					return QString("Install-Query-Dependent-Knowledge-Base-Revision-Updates Command");
				}


				CCommand *CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::getCommand() {
					return this;
				}

				CQuery* CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::getQuery() {
					return mQuery;
				}


				QList<CKnowledgeBaseRevisionCommandProvider*>* CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand::getDependentKnowledgeBasesProviderCommandList() {
					return &mKBProviderList;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

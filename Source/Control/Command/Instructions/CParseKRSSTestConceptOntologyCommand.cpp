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

#include "CParseKRSSTestConceptOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseKRSSTestConceptOntologyCommand::CParseKRSSTestConceptOntologyCommand(CReadTextCommandProvider* textCommandProvider, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, const QString& testConName, CCommand *parentSuperCommand) 
						: CParseOntologyCommand(parentSuperCommand) {

					mTestConName = testConName;
					mTextCommandProvider = textCommandProvider;
					revProvider = ontologyRevisionProviderCommand;
					addCommandPrecondition(new CCommandProcessedPrecondition(mTextCommandProvider->getCommand()));
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CParseKRSSTestConceptOntologyCommand::~CParseKRSSTestConceptOntologyCommand() {
				}

				qint64 CParseKRSSTestConceptOntologyCommand::getCommandTag() {
					return READPARSEKRSSTESTCONCEPTONTOLOGYCOMMAND;
				}


				QString CParseKRSSTestConceptOntologyCommand::getBriefCommandDescription() {
					return QString("Parse-KRSS-Test-Ontology Command");
				}

				QString CParseKRSSTestConceptOntologyCommand::getOntologyKRSSText() {
					return mTextCommandProvider->getReadText();
				}


				QString CParseKRSSTestConceptOntologyCommand::getTestConceptNameString() {
					return mTestConName;
				}


				COntologyRevision *CParseKRSSTestConceptOntologyCommand::getOntologyRevision() {
					return revProvider->getOntologyRevision();
				}

				CCommand *CParseKRSSTestConceptOntologyCommand::getCommand() {
					return this;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

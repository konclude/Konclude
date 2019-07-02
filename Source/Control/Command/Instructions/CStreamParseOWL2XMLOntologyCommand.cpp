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

#include "CStreamParseOWL2XMLOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CStreamParseOWL2XMLOntologyCommand::CStreamParseOWL2XMLOntologyCommand(const QString& sourceStringName, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CParseOntologyCommand(parentSuperCommand) {

					mSourceString = sourceStringName;
					revProvider = ontologyRevisionProviderCommand;
					//addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CStreamParseOWL2XMLOntologyCommand::~CStreamParseOWL2XMLOntologyCommand() {
				}

				qint64 CStreamParseOWL2XMLOntologyCommand::getCommandTag() {
					return STREAMPARSEOWL2XMLONTOLOGYCOMMAND;
				}


				QString CStreamParseOWL2XMLOntologyCommand::getBriefCommandDescription() {
					return QString("Stream-Parse-OWL2-XML-Ontology Command");
				}

				const QString& CStreamParseOWL2XMLOntologyCommand::getSourceString() {
					return mSourceString;
				}


				COntologyRevision* CStreamParseOWL2XMLOntologyCommand::getOntologyRevision() {
					return revProvider->getOntologyRevision();
				}

				CCommand *CStreamParseOWL2XMLOntologyCommand::getCommand() {
					return this;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

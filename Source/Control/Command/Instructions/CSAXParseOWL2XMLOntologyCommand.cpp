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

#include "CSAXParseOWL2XMLOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CSAXParseOWL2XMLOntologyCommand::CSAXParseOWL2XMLOntologyCommand(const QString& sourceStringName, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CParseOntologyCommand(parentSuperCommand) {

					mSourceString = sourceStringName;
					revProvider = ontologyRevisionProviderCommand;
					//addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CSAXParseOWL2XMLOntologyCommand::~CSAXParseOWL2XMLOntologyCommand() {
				}

				qint64 CSAXParseOWL2XMLOntologyCommand::getCommandTag() {
					return SAXPARSEOWL2XMLONTOLOGYCOMMAND;
				}


				QString CSAXParseOWL2XMLOntologyCommand::getBriefCommandDescription() {
					return QString("SAX-Parse-OWL2-XML-Ontology Command");
				}

				const QString& CSAXParseOWL2XMLOntologyCommand::getSourceString() {
					return mSourceString;
				}


				COntologyRevision* CSAXParseOWL2XMLOntologyCommand::getOntologyRevision() {
					return revProvider->getOntologyRevision();
				}

				CCommand *CSAXParseOWL2XMLOntologyCommand::getCommand() {
					return this;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

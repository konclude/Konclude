/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CParseOWL2XMLOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseOWL2XMLOntologyCommand::CParseOWL2XMLOntologyCommand(const QDomElement &ontologyNode, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CParseOntologyCommand(parentSuperCommand) {

					node = ontologyNode;
					revProvider = ontologyRevisionProviderCommand;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CParseOWL2XMLOntologyCommand::CParseOWL2XMLOntologyCommand(CReadXMLCommandProvider* xmlCommandProvider, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
					: CParseOntologyCommand(parentSuperCommand) {

					mXmlCommandProvider = xmlCommandProvider;
					revProvider = ontologyRevisionProviderCommand;
					//addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
					addCommandPrecondition(new CCommandProcessedPrecondition(xmlCommandProvider->getCommand()));
				}

				CParseOWL2XMLOntologyCommand::~CParseOWL2XMLOntologyCommand() {
				}

				qint64 CParseOWL2XMLOntologyCommand::getCommandTag() {
					return PARSEOWL2XMLONTOLOGYCOMMAND;
				}


				QString CParseOWL2XMLOntologyCommand::getBriefCommandDescription() {
					return QString("Parse-OWL2-XML-Ontology Command");
				}

				QDomElement CParseOWL2XMLOntologyCommand::getOntologyNode() {
					if (node.isNull() && mXmlCommandProvider) {
						node = mXmlCommandProvider->getReadXMLNode();
					}
					return node;
				}


				COntologyRevision *CParseOWL2XMLOntologyCommand::getOntologyRevision() {
					return revProvider->getOntologyRevision();
				}

				CCommand *CParseOWL2XMLOntologyCommand::getCommand() {
					return this;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

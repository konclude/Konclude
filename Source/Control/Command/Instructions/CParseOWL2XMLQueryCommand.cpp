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

#include "CParseOWL2XMLQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseOWL2XMLQueryCommand::CParseOWL2XMLQueryCommand(const QDomElement &queryNode, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CParseOntologyCommand(parentSuperCommand) {

					node = queryNode;
					revProvider = ontologyRevisionProviderCommand;
					mQueryGenerator = 0;
					// default complex
					mComplex = true;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CParseOWL2XMLQueryCommand::~CParseOWL2XMLQueryCommand() {
					delete mQueryGenerator;
				}

				qint64 CParseOWL2XMLQueryCommand::getCommandTag() {
					return PARSEOWL2XMLQUERYCOMMAND;
				}


				QString CParseOWL2XMLQueryCommand::getBriefCommandDescription() {
					return QString("Parse-OWL2-XML-Query Command");
				}

				QDomElement CParseOWL2XMLQueryCommand::getQueryNode() {
					return node;
				}

				CCommand *CParseOWL2XMLQueryCommand::getCommand() {
					return this;
				}

				COntologyRevision *CParseOWL2XMLQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				bool CParseOWL2XMLQueryCommand::isComplexQuery() {
					return mComplex;
				}

				CParseOWL2XMLQueryCommand* CParseOWL2XMLQueryCommand::setQueryComplexity(bool complex) {
					mComplex = complex;
					return this;
				}

				CQueryBuilderGenerator* CParseOWL2XMLQueryCommand::getQueryGenerator() {
					return mQueryGenerator;
				}

				CParseOWL2XMLQueryCommand* CParseOWL2XMLQueryCommand::setQueryGenerator(CQueryBuilderGenerator* queryGenerator) {
					mQueryGenerator = queryGenerator;
					return this;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

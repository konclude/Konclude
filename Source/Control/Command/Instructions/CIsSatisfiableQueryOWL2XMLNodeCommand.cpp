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

#include "CIsSatisfiableQueryOWL2XMLNodeCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CIsSatisfiableQueryOWL2XMLNodeCommand::CIsSatisfiableQueryOWL2XMLNodeCommand(const QString &knowledgeBaseNameString, const QDomElement &queryNode, CCommand *parentSuperCommand) 
						: CIsSatisfiableQueryCommand(knowledgeBaseNameString,parentSuperCommand) {

					node = queryNode;
				}


				CIsSatisfiableQueryOWL2XMLNodeCommand::~CIsSatisfiableQueryOWL2XMLNodeCommand() {
				}


				QString CIsSatisfiableQueryOWL2XMLNodeCommand::getBriefCommandDescription() {
					return QString("Knowledge-Base-SatisfiableQuery Command");
				}


				QDomElement CIsSatisfiableQueryOWL2XMLNodeCommand::getQueryNode() {
					return node;
				}


				qint64 CIsSatisfiableQueryOWL2XMLNodeCommand::getCommandTag() {
					return SATISFIABLEQUERYKNOWLEDGEBASEOWL2XMLCOMMAND;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

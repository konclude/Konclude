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

#include "CIsClassSubsumedByQueryOWL2XMLNodeCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CIsClassSubsumedByQueryOWL2XMLNodeCommand::CIsClassSubsumedByQueryOWL2XMLNodeCommand(const QString &knowledgeBaseNameString, const QDomElement &queryNode, CCommand *parentSuperCommand) 
						: CIsClassSubsumedByQueryCommand(knowledgeBaseNameString,parentSuperCommand) {

					node = queryNode;
				}


				CIsClassSubsumedByQueryOWL2XMLNodeCommand::~CIsClassSubsumedByQueryOWL2XMLNodeCommand() {
				}


				QString CIsClassSubsumedByQueryOWL2XMLNodeCommand::getBriefCommandDescription() {
					return QString("Knowledge-Base-ClassSubsumtionQuery Command");
				}


				QDomElement CIsClassSubsumedByQueryOWL2XMLNodeCommand::getQueryNode() {
					return node;
				}


				qint64 CIsClassSubsumedByQueryOWL2XMLNodeCommand::getCommandTag() {
					return SUBSUMEDQUERYKNOWLEDGEBASEOWL2XMLCOMMAND;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

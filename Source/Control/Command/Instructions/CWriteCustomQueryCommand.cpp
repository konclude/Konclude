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

#include "CWriteCustomQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CWriteCustomQueryCommand::CWriteCustomQueryCommand(const QString& knowledgeBaseNameString, CWriteQuery::WRITEQUERYTYPE queryType, CWriteQuerySerializer* serializer, const QString& entityRestrictionString, CCommand *parentSuperCommand) 
						: CKnowledgeBaseQueryCommand(knowledgeBaseNameString,parentSuperCommand) {

					mQueryType = queryType;
					mSerializer = serializer;
					mEntityRestrictionString = entityRestrictionString;					

				}


				CWriteCustomQueryCommand::~CWriteCustomQueryCommand() {
				}

				qint64 CWriteCustomQueryCommand::getCommandTag() {
					return WRITECUSTOMQUERYCOMMAND;
				}



				CWriteQuerySerializer* CWriteCustomQueryCommand::getWriteQuerySerializer() {
					return mSerializer;
				}

				CWriteQuery::WRITEQUERYTYPE CWriteCustomQueryCommand::getWriteQueryType() {
					return mQueryType;
				}

				const QString CWriteCustomQueryCommand::getEntityRestrictionString() {
					return mEntityRestrictionString;
				}

				QString CWriteCustomQueryCommand::getBriefCommandDescription() {
					return QString("Knowledge-Base-Custom-Write-Query Command");
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

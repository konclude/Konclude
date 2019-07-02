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

#include "CWriteXMLIndividualTypesQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CWriteXMLIndividualTypesQueryCommand::CWriteXMLIndividualTypesQueryCommand(const QString& knowledgeBaseNameString, const QString& outputFileString, const QString& individualName, CCommand *parentSuperCommand) 
						: CKnowledgeBaseQueryCommand(knowledgeBaseNameString,parentSuperCommand) {
					

					mIndividualName = individualName;
					mOutputFileString = outputFileString;
				}


				CWriteXMLIndividualTypesQueryCommand::~CWriteXMLIndividualTypesQueryCommand() {
				}

				qint64 CWriteXMLIndividualTypesQueryCommand::getCommandTag() {
					return WRITEXMLINDIVIDUALTYPESQUERYCOMMAND;
				}


				QString CWriteXMLIndividualTypesQueryCommand::getBriefCommandDescription() {
					return QString("Knowledge-Base-WriteXMLIndividualTypes Command");
				}

				QString CWriteXMLIndividualTypesQueryCommand::getOutputFileString() {
					return mOutputFileString;
				}

				QString CWriteXMLIndividualTypesQueryCommand::getIndividualName() {
					return mIndividualName;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

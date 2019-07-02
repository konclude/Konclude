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

#include "CGetConfigCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CGetConfigCommand::CGetConfigCommand(const QString &propertyName, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {
					propName = propertyName;
					data = 0;
				}


				CGetConfigCommand::~CGetConfigCommand() {
				}

				CGetConfigCommand *CGetConfigCommand::setConfigName(const QString &propertyName) {
					propName = propertyName;
					return this;
				}

				QString CGetConfigCommand::getConfigNameString() {
					return propName;
				}

				qint64 CGetConfigCommand::getCommandTag() {
					return GETCONFIGURATIONCOMMAND;
				}

				QString CGetConfigCommand::getBriefCommandDescription() {					
					return QString("Get-'%1'-Config Command").arg(propName);
				}

				CGetConfigCommand *CGetConfigCommand::setConfigData(CConfigData *configData) {
					data = configData;
					return this;
				}

				CConfigData *CGetConfigCommand::getConfigData() {
					return data;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

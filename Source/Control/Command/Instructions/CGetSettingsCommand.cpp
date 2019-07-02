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

#include "CGetSettingsCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CGetSettingsCommand::CGetSettingsCommand(CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {
					config = 0;
				}


				CGetSettingsCommand::~CGetSettingsCommand() {
				}

				qint64 CGetSettingsCommand::getCommandTag() {
					return GETSETTINGSCOMMAND;
				}

				QString CGetSettingsCommand::getBriefCommandDescription() {					
					return QString("Get-Settings Command");
				}

				CGetSettingsCommand *CGetSettingsCommand::setConfiguration(CConfigurationBase *configuration) {
					config = configuration;
					return this;
				}

				CConfigurationBase *CGetSettingsCommand::getConfiguration() {
					return config;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

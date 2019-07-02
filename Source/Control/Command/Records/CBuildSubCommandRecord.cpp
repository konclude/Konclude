/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CBuildSubCommandRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CBuildSubCommandRecord::CBuildSubCommandRecord(CCommand *subCommand, CCommand *command, const QString &recordDomain, CCommandRecordData *nextRecordData) 
						: CCommandRecordData(command,recordDomain,0,nextRecordData) {
					subComm = subCommand;
				}


				CBuildSubCommandRecord::~CBuildSubCommandRecord() {
				}


				CCommand *CBuildSubCommandRecord::getBuildSubCommand() {
					return subComm;
				}

				CBuildSubCommandRecord *CBuildSubCommandRecord::setBuildSubCommand(CCommand *subCommand) {
					subComm = subCommand;
					return this;
				}


				QString CBuildSubCommandRecord::getRecordMessage() {
					return QString("Build '%1' as Sub-Command from '%2'.").arg(comm->getBriefCommandDescription()).arg(subComm->getBriefCommandDescription());
				}

				void CBuildSubCommandRecord::makeRecord(CCommandRecorder *recorder, const QString &domain, CCommand *command, CCommand *subCommand) {
					CBuildSubCommandRecord *record = new CBuildSubCommandRecord(subCommand,command,domain,command->getRecordData());
					if (recorder) {
						recorder->recordData(record);
					} else {
						CCommandRecorder::recordToCommand(command,record);
					}
				}



			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

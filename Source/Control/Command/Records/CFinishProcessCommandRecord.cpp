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

#include "CFinishProcessCommandRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CFinishProcessCommandRecord::CFinishProcessCommandRecord(CCommand *command, const QString &recordDomain, CCommandRecordData *recordData) 
						: CCommandRecordData(command,recordDomain,0,recordData) {
				}


				CFinishProcessCommandRecord::~CFinishProcessCommandRecord() {
				}

				QString CFinishProcessCommandRecord::getRecordMessage() {
					return QString("Finish processing '%1'.").arg(comm->getBriefCommandDescription());
				}


				void CFinishProcessCommandRecord::makeRecord(CCommandRecorder *recorder, const QString &domain, CCommand *command) {
					if (!command->isProcessed()) {
						CFinishProcessCommandRecord *record = new CFinishProcessCommandRecord(command,domain,command->getRecordData());
						if (recorder) {
							recorder->recordData(record);
						} else {
							CCommandRecorder::recordToCommand(command,record);
						}
						command->setProcessed(true);
					}
				}

				void CFinishProcessCommandRecord::makeRecord(CCommandRecordRouter *commandRecordRouter) {
					if (commandRecordRouter) {
						makeRecord(commandRecordRouter->getCommandRecorder(),commandRecordRouter->getLogDomain(),commandRecordRouter->getCommand());
					}
				}



			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

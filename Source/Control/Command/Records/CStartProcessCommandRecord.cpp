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

#include "CStartProcessCommandRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CStartProcessCommandRecord::CStartProcessCommandRecord(CCommand *command, const QString &recordDomain, CCommandRecordData *recordData) 
						: CCommandRecordData(command,recordDomain,0,recordData) {
				}


				CStartProcessCommandRecord::~CStartProcessCommandRecord() {
				}

				QString CStartProcessCommandRecord::getRecordMessage() {
					return QString("Start processing '%1'.").arg(comm->getBriefCommandDescription());
				}


				void CStartProcessCommandRecord::makeRecord(CCommandRecorder *recorder, const QString &domain, CCommand *command) {
					CStartProcessCommandRecord *record = new CStartProcessCommandRecord(command,domain,command->getRecordData());
					command->setProcessing(true);
					if (recorder) {
						recorder->recordData(record);
					} else {
						CCommandRecorder::recordToCommand(command,record);
					}
				}

				void CStartProcessCommandRecord::makeRecord(CCommandRecordRouter *commandRecordRouter) {
					if (commandRecordRouter) {
						makeRecord(commandRecordRouter->getCommandRecorder(),commandRecordRouter->getLogDomain(),commandRecordRouter->getCommand());
					}
				}



			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

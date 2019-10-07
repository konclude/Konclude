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

#include "CUnspecifiedMessageInformationRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CUnspecifiedMessageInformationRecord::CUnspecifiedMessageInformationRecord(CCommand *command, const QString &informationMessage, const QString &recordDomain, CCommandRecordData *recordData) 
						: CCommandRecordData(command,recordDomain,0,recordData) {

					message = informationMessage;
				}


				CUnspecifiedMessageInformationRecord::~CUnspecifiedMessageInformationRecord() {
				}


				QString CUnspecifiedMessageInformationRecord::getRecordMessage() {
					return message;
				}


				void CUnspecifiedMessageInformationRecord::makeRecord(CCommandRecorder *recorder, const QString &message, const QString &domain, CCommand *command) {
					CUnspecifiedMessageInformationRecord *record = new CUnspecifiedMessageInformationRecord(command,message,domain,command->getRecordData());
					if (recorder) {
						recorder->recordData(record);
					}  else {
						CCommandRecorder::recordToCommand(command,record);
					}
				}


				void CUnspecifiedMessageInformationRecord::makeRecord(const QString &message, CLogIdentifier *logIdentifier, CCommand *command) {
					if (logIdentifier && command && command->getRecorder()) {
						makeRecord(command->getRecorder(),message,logIdentifier->getLogDomain(),command);
					}
				}


				void CUnspecifiedMessageInformationRecord::makeRecord(const QString &message, CCommandRecordRouter *commandRecordRouter) {
					if (commandRecordRouter) {
						makeRecord(commandRecordRouter->getCommandRecorder(),message,commandRecordRouter->getLogDomain(),commandRecordRouter->getCommand());
					}
				}


			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

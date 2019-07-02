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

#include "CNotSupportedNodeErrorRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CNotSupportedNodeErrorRecord::CNotSupportedNodeErrorRecord(const QString &node, CCommand *command, const QString &recordDomain, CCommandRecordData *recordData) 
						: CParseSyntaxErrorRecord(command,recordDomain,70,recordData) {
					nodeString = node;
				}


				CNotSupportedNodeErrorRecord::~CNotSupportedNodeErrorRecord() {
				}


				QString CNotSupportedNodeErrorRecord::getRecordMessage() {
					return QString("Node '%1' is not supported and could not be processed.").arg(nodeString);
				}


				void CNotSupportedNodeErrorRecord::makeRecord(const QString &node, CCommandRecorder *recorder, const QString &domain, CCommand *command) {
					CCommandRecordData *recordData = 0;
					if (command) {
						recordData = command->getRecordData();
					}
					CNotSupportedNodeErrorRecord *record = new CNotSupportedNodeErrorRecord(node,command,domain,recordData);
					if (recorder) {
						recorder->recordData(record);
					} else {
						CCommandRecorder::recordToCommand(command,record);
					}
				}


				void CNotSupportedNodeErrorRecord::makeRecord(const QString &node, CCommandRecordRouter *commandRecordRouter) {
					if (commandRecordRouter) {
						makeRecord(node,commandRecordRouter->getCommandRecorder(),commandRecordRouter->getLogDomain(),commandRecordRouter->getCommand());
					}
				}




			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

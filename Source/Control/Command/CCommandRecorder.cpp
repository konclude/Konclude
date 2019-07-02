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

#include "CCommandRecorder.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CCommandRecorder::CCommandRecorder() {
			}


			CCommandRecorder::~CCommandRecorder() {
			}


			CContextRecorder *CCommandRecorder::recordData(CContextRecordData *recData) {
				if (recData) {
					CCommandRecordData *commRecData = dynamic_cast<CCommandRecordData *>(recData);
					if (commRecData) {
						recordData(commRecData);
					} else {
						delete recData;
					}
				}
				return this;
			}

			CCommandRecorder *CCommandRecorder::recordData(CCommandRecordData *recData) {
				if (recData) {
					LOGC(recData->getRecordErrorLevel(),recData->getRecordDomain(),recData->getRecordMessage(),recData->getCommand());
					CCommand *command = recData->getCommand();
					if (command) {
						command->reportError(recData->getReflexiveTransetiveMaximumRecordErrorLevel());
						command->appendRecordData(recData);
					}
				}
				return this;
			}


			void CCommandRecorder::recordToCommand(CCommand *command, CCommandRecordData *recData) {
				if (recData) {
					LOGC(recData->getRecordErrorLevel(),recData->getRecordDomain(),recData->getRecordMessage(),recData->getCommand());
					if (command) {
						command->reportError(recData->getReflexiveTransetiveMaximumRecordErrorLevel());
						command->appendRecordData(recData);
					}
				}			
			}


		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

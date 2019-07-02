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

#include "CTestTestfileCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CTestTestfileCommand::CTestTestfileCommand(CReasonerTestfileTest *reasonerTest, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {
					test = reasonerTest;
				}


				CTestTestfileCommand::~CTestTestfileCommand() {
				}


				qint64 CTestTestfileCommand::getCommandTag() {
					return TESTTESTFILECOMMAND;
				}

				CReasonerTestfileTest *CTestTestfileCommand::getReasonerTestfileTest() {
					return test;
				}


				QString CTestTestfileCommand::getBriefCommandDescription() {
					return QString("Test-Testfile Command");
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

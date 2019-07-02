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

#include "CPreconditionCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CPreconditionCommand::CPreconditionCommand(CCommand *parentSuperCommand) 
					: CCommand(parentSuperCommand) {
				commandPreconditionLinker = 0;
			}


			CPreconditionCommand::~CPreconditionCommand() {
			}


			CLinker<CCommandPrecondition *> *CPreconditionCommand::getCommandPreconditionLinker() {
				return commandPreconditionLinker;
			}

			CPreconditionCommand *CPreconditionCommand::addCommandPrecondition(CCommandPrecondition *takeCommandPrecondition) {
				commandPreconditionLinker = new CLinker<CCommandPrecondition *>(takeCommandPrecondition,commandPreconditionLinker);
				return this;
			}


			bool CPreconditionCommand::isProcessable() {
				if (isProcessed()) {
					return false;
				}
				CLinker<CCommandPrecondition *> *preconIt = commandPreconditionLinker;
				while (preconIt) {
					if (!preconIt->getData()->isPreconditionFullfilled()) {
						return false;
					}
					preconIt = preconIt->getNext();
				}
				return true;
			}


			CCommand *CPreconditionCommand::deleteDependencies() {
				deletePreconditions();
				return this;
			}

			CPreconditionCommand *CPreconditionCommand::deletePreconditions() {
				while (commandPreconditionLinker) {
					CCommandPrecondition *pre = commandPreconditionLinker->getData();
					delete pre;
					CLinker<CCommandPrecondition *> *tmpPreIt = commandPreconditionLinker;
					commandPreconditionLinker = (CLinker<CCommandPrecondition *> *)commandPreconditionLinker->getNext();
					delete tmpPreIt;
				}
				return this;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

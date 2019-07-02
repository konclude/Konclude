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

#include "CGenerateQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CGenerateQueryCommand::CGenerateQueryCommand(CQueryGeneratorCommandProvider* queryCommandProvider, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mQuery = nullptr;
					mQueryCommandProvider = queryCommandProvider;
					if (queryCommandProvider) {
						addCommandPrecondition(new CCommandProcessedPrecondition(queryCommandProvider->getCommand()));
					}
				}


				CGenerateQueryCommand::~CGenerateQueryCommand() {
					delete mQuery;
				}

				qint64 CGenerateQueryCommand::getCommandTag() {
					return GENERATEQUERYCOMMANDCOMMAND;
				}


				QString CGenerateQueryCommand::getBriefCommandDescription() {
					return QString("Prepare-Knowledge-Base-For-Query Command");
				}


				CQueryBuilderGenerator* CGenerateQueryCommand::getQueryGenerator() {
					CQueryBuilderGenerator* queryGenerator = nullptr;
					if (mQueryCommandProvider) {
						queryGenerator = mQueryCommandProvider->getQueryGenerator();
					}
					return queryGenerator;
				}

				CQuery* CGenerateQueryCommand::getQuery() {
					return mQuery;
				}

				CCommand* CGenerateQueryCommand::getCommand() {
					return this;
				}

				CGenerateQueryCommand* CGenerateQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					mQuery = knowledgeBaseQuery;
					return this;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

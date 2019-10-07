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

#include "CCalculateQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CCalculateQueryCommand::CCalculateQueryCommand(CQueryCommandProvider *queryCommandProvider, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					queryProvider = queryCommandProvider;
					mQuery = nullptr;
					addCommandPrecondition(new CCommandProcessedPrecondition(queryProvider->getCommand()));
				}


				CCalculateQueryCommand::CCalculateQueryCommand(CQuery* query, CCommand *parentSuperCommand): CPreconditionCommand(parentSuperCommand) {
					queryProvider = nullptr;
					mQuery = query;
				}


				CCalculateQueryCommand::~CCalculateQueryCommand() {
				}

				qint64 CCalculateQueryCommand::getCommandTag() {
					return CALCULATEQUERYCOMMAND;
				}


				QString CCalculateQueryCommand::getBriefCommandDescription() {
					return QString("Calculate-Query Command");
				}


				CCommand *CCalculateQueryCommand::getCommand() {
					return this;
				}

				CQuery *CCalculateQueryCommand::getQuery() {
					if (queryProvider) {
						return queryProvider->getQuery();
					}
					return mQuery;
				}

				CQueryCommandProvider* CCalculateQueryCommand::getQueryCommandProvider() {
					return queryProvider;
				}

				CCalculateQueryCommand* CCalculateQueryCommand::setQueryCommandProvider(CQueryCommandProvider* queryCommandProvider) {
					queryProvider = queryCommandProvider;
					return this;
				}

				CCalculateQueryCommand* CCalculateQueryCommand::setQuery(CQuery* query) {
					mQuery = query;
					return this;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

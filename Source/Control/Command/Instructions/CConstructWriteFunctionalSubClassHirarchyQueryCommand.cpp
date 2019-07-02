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

#include "CConstructWriteFunctionalSubClassHirarchyQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CConstructWriteFunctionalSubClassHirarchyQueryCommand::CConstructWriteFunctionalSubClassHirarchyQueryCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, const QString& outputFileString, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mOutputFileName = outputFileString;
					revProvider = ontologyRevisionProviderCommand; 
					query = 0;
					addCommandPrecondition(new CCommandProcessedPrecondition(revProvider->getCommand()));
				}


				CConstructWriteFunctionalSubClassHirarchyQueryCommand::~CConstructWriteFunctionalSubClassHirarchyQueryCommand() {
					delete query;
				}

				qint64 CConstructWriteFunctionalSubClassHirarchyQueryCommand::getCommandTag() {
					return CONSTRUCTWRITEFUNCTIONALSUBCLASSHIERARCHYQUERYCOMMAND;
				}


				QString CConstructWriteFunctionalSubClassHirarchyQueryCommand::getBriefCommandDescription() {
					return QString("Construct-WriteFunctionalSubClassHierarchy-Query Command");
				}


				CQuery *CConstructWriteFunctionalSubClassHirarchyQueryCommand::getQuery() {
					return query;
				}

				CCommand *CConstructWriteFunctionalSubClassHirarchyQueryCommand::getCommand() {
					return this;
				}


				CConstructWriteFunctionalSubClassHirarchyQueryCommand *CConstructWriteFunctionalSubClassHirarchyQueryCommand::setQuery(CQuery *knowledgeBaseQuery) {
					query = knowledgeBaseQuery;
					return this;
				}

				COntologyRevision *CConstructWriteFunctionalSubClassHirarchyQueryCommand::getOntologyRevision() {
					COntologyRevision *rev = 0;
					if (revProvider) {
						rev = revProvider->getOntologyRevision();
					}
					return rev;
				}

				QString CConstructWriteFunctionalSubClassHirarchyQueryCommand::getOutputFileName() {
					return mOutputFileName;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

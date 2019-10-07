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

#include "CKnowledgeBaseQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CKnowledgeBaseQueryCommand::CKnowledgeBaseQueryCommand(const QString &knowledgeBaseNameString, CCommand *parentSuperCommand) 
						: CKnowledgeBaseCommand(parentSuperCommand) {

					kbString = knowledgeBaseNameString;
					buildCommand = 0;
					calcQueryCommand = 0;
					mComplex = false;
				}


				CKnowledgeBaseQueryCommand::~CKnowledgeBaseQueryCommand() {
				}


				QString CKnowledgeBaseQueryCommand::getBriefCommandDescription() {
					return QString("Knowledge-Base-Query Command").arg(kbString);
				}

				QString CKnowledgeBaseQueryCommand::getKnowledgeBaseName() {
					return kbString;
				}

				CCalculateQueryCommand *CKnowledgeBaseQueryCommand::getCalculateQueryCommand() {
					return calcQueryCommand;
				}

				CKnowledgeBaseQueryCommand *CKnowledgeBaseQueryCommand::setCalculateQueryCommand(CCalculateQueryCommand *calculateQueryCommand) {
					calcQueryCommand = calculateQueryCommand;
					return this;
				}

				CBuildQueryCommand *CKnowledgeBaseQueryCommand::getBuildQueryCommand() {
					return buildCommand;
				}

				CKnowledgeBaseQueryCommand *CKnowledgeBaseQueryCommand::setBuildQueryCommand(CBuildQueryCommand *buildQueryCommand) {
					buildCommand = buildQueryCommand;
					return this;
				}


				bool CKnowledgeBaseQueryCommand::isComplexQuery() {
					return mComplex;
				}

				CKnowledgeBaseQueryCommand* CKnowledgeBaseQueryCommand::setQueryComplexity(bool complex) {
					mComplex = complex;
					return this;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

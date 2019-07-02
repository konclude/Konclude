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

#include "CPrepareKnowledgeBaseForQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CPrepareKnowledgeBaseForQueryCommand::CPrepareKnowledgeBaseForQueryCommand(CKnowledgeBaseRevisionCommandProvider* ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CPrepareKnowledgeBaseCommand(ontologyRevisionProviderCommand,parentSuperCommand) {


					COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
					mOntProReqList.append( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
				}


				CPrepareKnowledgeBaseForQueryCommand::~CPrepareKnowledgeBaseForQueryCommand() {
				}

				qint64 CPrepareKnowledgeBaseForQueryCommand::getCommandTag() {
					return PREPAREKNOWLEDGEBASEFORQUERYCOMMANDCOMMAND;
				}


				QString CPrepareKnowledgeBaseForQueryCommand::getBriefCommandDescription() {
					return QString("Prepare-Knowledge-Base-For-Query Command");
				}

				


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

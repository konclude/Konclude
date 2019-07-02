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

#include "CPrepareKnowledgeBaseCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CPrepareKnowledgeBaseCommand::CPrepareKnowledgeBaseCommand(CKnowledgeBaseRevisionCommandProvider* ontologyRevisionProviderCommand, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {


					//COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
					//mOntProReqList.append( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSBUILD), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
					//mOntProReqList.append( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
					mRevProvider = ontologyRevisionProviderCommand;
				}


				CPrepareKnowledgeBaseCommand::~CPrepareKnowledgeBaseCommand() {
					qDeleteAll(mOntProReqList);
				}



				QString CPrepareKnowledgeBaseCommand::getBriefCommandDescription() {
					return QString("Prepare-Knowledge-Base Command");
				}

				
				QList<COntologyProcessingRequirement*>* CPrepareKnowledgeBaseCommand::getOntologyProcessingRequirementList() {
					return &mOntProReqList;
				}


				CConcreteOntology* CPrepareKnowledgeBaseCommand::getOntology() {
					CConcreteOntology* ontology = nullptr;
					COntologyRevision* ontoRev = nullptr;
					if (mRevProvider) {
						ontoRev = mRevProvider->getOntologyRevision();
					}
					if (ontoRev) {
						ontology = ontoRev->getOntology();
					}
					return ontology;
				}

				COntologyRevision* CPrepareKnowledgeBaseCommand::getOntologyRevision() {
					return mRevProvider->getOntologyRevision();
				}

				CPrepareKnowledgeBaseCommand* CPrepareKnowledgeBaseCommand::addRequirement(COntologyProcessingStepRequirement* requirement) {
					mOntProReqList.append(requirement);
					return this;
				}

				qint64 CPrepareKnowledgeBaseCommand::getCommandTag() {
					return PREPAREKNOWLEDGEBASEFORQUERYCOMMANDCOMMAND;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

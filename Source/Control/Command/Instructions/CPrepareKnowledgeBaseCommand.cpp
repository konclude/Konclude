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
					mOntology = nullptr;
				}

				CPrepareKnowledgeBaseCommand::CPrepareKnowledgeBaseCommand(CConcreteOntology *ontology, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					mOntology = ontology;
					mRevProvider = nullptr;
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
					CConcreteOntology* ontology = mOntology;
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
					return PREPROCESSKNOWLEDGEBASEREQUIREMENTSFORQUERYCOMMANDCOMMAND;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

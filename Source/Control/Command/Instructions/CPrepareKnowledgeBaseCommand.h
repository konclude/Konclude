/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_CONTROL_COMMAND_CPREPAREKNOWLEDGEBASECOMMAND_H
#define KONCLUDE_CONTROL_COMMAND_CPREPAREKNOWLEDGEBASECOMMAND_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CKnowledgeBaseRevisionCommandProvider.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"
#include "Control/Command/CCommandProcessedPrecondition.h"

#include "Reasoner/Ontology/COntologyProcessingRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepVector.h"
#include "Reasoner/Ontology/COntologyProcessingRequirementExpander.h"
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Ontology;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CPrepareKnowledgeBaseCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPrepareKnowledgeBaseCommand : public CPreconditionCommand {
					// public methods
					public:
						//! Constructor
						CPrepareKnowledgeBaseCommand(CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CPrepareKnowledgeBaseCommand();

						virtual QString getBriefCommandDescription();

						virtual qint64 getCommandTag();

						virtual COntologyRevision* getOntologyRevision();
						virtual CConcreteOntology* getOntology();
						virtual QList<COntologyProcessingRequirement*>* getOntologyProcessingRequirementList();

						CPrepareKnowledgeBaseCommand* addRequirement(COntologyProcessingStepRequirement* requirement);

					// protected methods
					protected:

					// protected variables
					protected:
						CKnowledgeBaseRevisionCommandProvider* mRevProvider;
						QList<COntologyProcessingRequirement*> mOntProReqList;
						CConcreteOntology* mOntology;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CPREPAREKNOWLEDGEBASECOMMAND_H

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

#ifndef KONCLUDE_Control_COMMAND_CKNOWLEDGEBASEQUERYCOMMAND_H
#define KONCLUDE_Control_COMMAND_CKNOWLEDGEBASEQUERYCOMMAND_H

// Libraries includes
#include <QString>
#include <QDomElement>

// Namespace includes
#include "CKnowledgeBaseCommand.h"
#include "CCalculateQueryCommand.h"
#include "CBuildQueryCommand.h"

// Other includes

#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CKnowledgeBaseQueryCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CKnowledgeBaseQueryCommand : public CKnowledgeBaseCommand {
					// public methods
					public:
						//! Constructor
						CKnowledgeBaseQueryCommand(const QString &knowledgeBaseNameString, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CKnowledgeBaseQueryCommand();

						virtual QString getBriefCommandDescription();
						virtual QString getKnowledgeBaseName();

						virtual CCalculateQueryCommand *getCalculateQueryCommand();
						virtual CKnowledgeBaseQueryCommand *setCalculateQueryCommand(CCalculateQueryCommand *calculateQueryCommand);

						virtual bool isComplexQuery();
						virtual CKnowledgeBaseQueryCommand* setQueryComplexity(bool complex);


						virtual CBuildQueryCommand *getBuildQueryCommand();
						virtual CKnowledgeBaseQueryCommand *setBuildQueryCommand(CBuildQueryCommand *buildQueryCommand);


					// protected methods
					protected:

					// protected variables
					protected:
						CCalculateQueryCommand *calcQueryCommand;
						CBuildQueryCommand *buildCommand;
						QString kbString;
						bool mComplex;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CKNOWLEDGEBASEQUERYCOMMAND_H

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

#ifndef KONCLUDE_CONTROL_COMMAND_CSAVEOWL2XMLEXTRACTEDKNOWLEDGEBASECOMMAND_H
#define KONCLUDE_CONTROL_COMMAND_CSAVEOWL2XMLEXTRACTEDKNOWLEDGEBASECOMMAND_H

// Libraries includes
#include <QString>
#include <QDomElement>

// Namespace includes
#include "CSaveKnowledgeBaseCommand.h"

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
				 *		\class		CSaveOWL2XMLExtractedKnowledgeBaseCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaveOWL2XMLExtractedKnowledgeBaseCommand : public CSaveKnowledgeBaseCommand {
					// public methods
					public:
						//! Constructor
						CSaveOWL2XMLExtractedKnowledgeBaseCommand(const QString &knowledgeBaseNameString, const QString& loadIRI, const QStringList& extractEntityNames, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CSaveOWL2XMLExtractedKnowledgeBaseCommand();

						virtual qint64 getCommandTag();
						virtual QString getBriefCommandDescription();


						virtual QString getSaveIRIString();
						virtual QStringList getExtractEntitiyList();

					// protected methods
					protected:

					// protected variables
					protected:
						QString mSaveIRI;
						QStringList mEntityNames;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CSAVEOWL2XMLEXTRACTEDKNOWLEDGEBASECOMMAND_H

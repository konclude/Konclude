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

#ifndef KONCLUDE_Control_COMMAND_CPARSEOWL2XMLONTOLOGYCOMMAND_H
#define KONCLUDE_Control_COMMAND_CPARSEOWL2XMLONTOLOGYCOMMAND_H

// Libraries includes
#include <QString>
#include <QDomElement>

// Namespace includes
#include "CParseOntologyCommand.h"
#include "CKnowledgeBaseRevisionCommandProvider.h"
#include "CKnowledgeBaseRevisionCommandProvider.h"
#include "CReadXMLCommandProvider.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"
#include "Control/Command/CCommandProcessedPrecondition.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CParseOWL2XMLOntologyCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CParseOWL2XMLOntologyCommand : public CParseOntologyCommand, public CKnowledgeBaseRevisionCommandProvider {
					// public methods
					public:
						//! Constructor
						CParseOWL2XMLOntologyCommand(const QDomElement &ontologyNode, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand = 0);
						CParseOWL2XMLOntologyCommand(CReadXMLCommandProvider* xmlCommandProvider, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CParseOWL2XMLOntologyCommand();

						virtual qint64 getCommandTag();

						virtual QString getBriefCommandDescription();

						virtual QDomElement getOntologyNode();

						virtual COntologyRevision *getOntologyRevision();
						virtual CCommand *getCommand();


					// protected methods
					protected:

					// protected variables
					protected:
						QDomElement node;
						CKnowledgeBaseRevisionCommandProvider *revProvider;
						CReadXMLCommandProvider* mXmlCommandProvider;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CPARSEOWL2XMLONTOLOGYCOMMAND_H

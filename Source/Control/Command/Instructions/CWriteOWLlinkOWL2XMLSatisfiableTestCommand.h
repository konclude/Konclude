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

#ifndef KONCLUDE_CONTROL_COMMAND_CWRITEOWLLINKOWL2XMLSATISFIABLETESTCOMMAND_H
#define KONCLUDE_CONTROL_COMMAND_CWRITEOWLLINKOWL2XMLSATISFIABLETESTCOMMAND_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CWriteOntologyCommand.h"
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"

// Other includes
#include "Renderer/COntologyOWL2QtXMLRenderer.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Renderer;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CWriteOWLlinkOWL2XMLSatisfiableTestCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CWriteOWLlinkOWL2XMLSatisfiableTestCommand : public CWriteOntologyCommand {
					// public methods
					public:
						//! Constructor
						CWriteOWLlinkOWL2XMLSatisfiableTestCommand(COntologyOWL2QtXMLRenderer* renderer, const QString& fileName, const QStringList& satTestClasses, const QString& testKBName, CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CWriteOWLlinkOWL2XMLSatisfiableTestCommand();


						virtual qint64 getCommandTag();
						virtual QString getBriefCommandDescription();

						virtual COntologyRenderer* getRenderer();
						virtual CCommand *getCommand();

						COntologyOWL2QtXMLRenderer* getOWL2XMLRenderer();
						QString getWriteFileName();
						QStringList getSatisfiableTestClassesList();
						QString getTestKBName();

					// protected methods
					protected:

					// protected variables
					protected:
						QString mWriteFileName;
						QString mTestKBName;
						COntologyOWL2QtXMLRenderer* mRenderer;
						QStringList mSatTestClasses;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CWRITEOWLLINKOWL2XMLSATISFIABLETESTCOMMAND_H

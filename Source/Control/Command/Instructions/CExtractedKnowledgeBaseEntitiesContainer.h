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

#ifndef KONCLUDE_CONTROL_COMMAND_CEXTRACTEDKNOWLEDGEBASEENTITIESCONTAINER_H
#define KONCLUDE_CONTROL_COMMAND_CEXTRACTEDKNOWLEDGEBASEENTITIESCONTAINER_H

// Libraries includes
#include <QString>

// Namespace includes

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"

#include "Renderer/COntologyRenderer.h"


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
				 *		\class		CExtractedKnowledgeBaseEntitiesContainer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CExtractedKnowledgeBaseEntitiesContainer  {
					// public methods
					public:
						//! Constructor
						CExtractedKnowledgeBaseEntitiesContainer();

						QStringList* getExtractedEntityList();
						CExtractedKnowledgeBaseEntitiesContainer* addExtractedEntity(const QString& entityName);

					// protected methods
					protected:

					// protected variables
					protected:
						QStringList mExtractedEntityList;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CEXTRACTEDKNOWLEDGEBASEENTITIESCONTAINER_H

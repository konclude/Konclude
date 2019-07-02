/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_CONTROL_COMMAND_CKNOWLEDGEBASEENTITIESEXTRACTIONSPECIFICATION_H
#define KONCLUDE_CONTROL_COMMAND_CKNOWLEDGEBASEENTITIESEXTRACTIONSPECIFICATION_H

// Libraries includes

// Namespace includes

// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Utilities;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CKnowledgeBaseEntitiesExtractionSpecification
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CKnowledgeBaseEntitiesExtractionSpecification  {
					// public methods
					public:
						//! Constructor
						CKnowledgeBaseEntitiesExtractionSpecification(cint64 flags = 0);

						const static cint64 EXTRACTCLASSENTITIES			= 0x01;
						const static cint64 EXTRACTOBJECTPROPERTYENTITIES	= 0x02;
						const static cint64 EXTRACTINDIVIDUALSENTITIES		= 0x04;


						bool hasPartialEntityExtractionFlags(cint64 extractionFlag);

						cint64 getEntityExtractionFlags();
						CKnowledgeBaseEntitiesExtractionSpecification* setEntityExtractionFlags(cint64 flags);

						CKnowledgeBaseEntitiesExtractionSpecification* clearEntityExtractionFlags();

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mEntityExtractionFlags;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_COMMAND_CKNOWLEDGEBASEENTITIESEXTRACTIONSPECIFICATION_H

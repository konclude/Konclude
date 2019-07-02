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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CBoxContext.h"
#include "CDatatypeValueSpaceConceptTriggeringData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpaceTriggers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceTriggers {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceTriggers(CBoxContext* boxContext);

					CDatatypeValueSpaceTriggers* initValueSpaceTriggers(CDatatypeValueSpaceTriggers* data);

					CDatatypeValueSpaceConceptTriggeringData* getValueSpaceConceptTriggeringData();

					CDatatypeValueSpaceTriggers* incConceptTriggerCount(cint64 incCount = 1);
					cint64 getConceptTriggerCount();


					CDatatypeValueSpaceTriggers* addValueSpaceConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);


				// protected methods
				protected:

				// protected variables
				protected:
					CBoxContext* mBoxContext;
					cint64 mConceptTriggerCount;
					CDatatypeValueSpaceConceptTriggeringData mValueSpateTriggerData;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERS_H

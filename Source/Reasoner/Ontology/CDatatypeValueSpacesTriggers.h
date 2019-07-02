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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACESTRIGGERS_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACESTRIGGERS_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CBoxContext.h"
#include "CDatatypeValueSpaceRealTriggers.h"
#include "CDatatypeValueSpaceStringTriggers.h"
#include "CDatatypeValueSpaceBooleanTriggers.h"
#include "CDatatypeValueSpaceDoubleTriggers.h"
#include "CDatatypeValueSpaceFloatTriggers.h"
#include "CDatatypeValueSpaceIRITriggers.h"
#include "CDatatypeValueSpaceBinaryHexDataTriggers.h"
#include "CDatatypeValueSpaceBinaryBase64DataTriggers.h"
#include "CDatatypeValueSpaceXMLTriggers.h"
#include "CDatatypeValueSpaceDateTimeTriggers.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpacesTriggers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpacesTriggers {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpacesTriggers(CBoxContext* boxContext);

					CDatatypeValueSpacesTriggers* initValueSpacesTriggers(CDatatypeValueSpacesTriggers* data);


					CDatatypeValueSpaceTriggers* getValueSpaceTriggers(CDatatypeValueSpaceType* valueSpaceType);

					CDatatypeValueSpaceRealTriggers* getRealValueSpaceTriggers();
					CDatatypeValueSpaceStringTriggers* getStringValueSpaceTriggers();
					CDatatypeValueSpaceBooleanTriggers* getBooleanValueSpaceTriggers();
					CDatatypeValueSpaceDoubleTriggers* getDoubleValueSpaceTriggers();
					CDatatypeValueSpaceFloatTriggers* getFloatValueSpaceTriggers();
					CDatatypeValueSpaceIRITriggers* getIRIValueSpaceTriggers();
					CDatatypeValueSpaceBinaryHexDataTriggers* getBinaryHexDataValueSpaceTriggers();
					CDatatypeValueSpaceBinaryBase64DataTriggers* getBinaryBase64DataValueSpaceTriggers();
					CDatatypeValueSpaceXMLTriggers* getXMLValueSpaceTriggers();
					CDatatypeValueSpaceDateTimeTriggers* getDateTimeValueSpaceTriggers();


				// protected methods
				protected:

				// protected variables
				protected:
					CBoxContext* mBoxContext;

					CDatatypeValueSpaceRealTriggers mRealValueSpaceTriggers;
					CDatatypeValueSpaceStringTriggers mStringValueSpaceTriggers;
					CDatatypeValueSpaceBooleanTriggers mBooleanValueSpaceTriggers;
					CDatatypeValueSpaceDoubleTriggers mDoubleValueSpaceTriggers;
					CDatatypeValueSpaceFloatTriggers mFloatValueSpaceTriggers;
					CDatatypeValueSpaceIRITriggers mIRIValueSpaceTriggers;
					CDatatypeValueSpaceBinaryHexDataTriggers mBinaryHexDataValueSpaceTriggers;
					CDatatypeValueSpaceBinaryBase64DataTriggers mBinaryBase64DataValueSpaceTriggers;
					CDatatypeValueSpaceXMLTriggers mXMLValueSpaceTriggers;
					CDatatypeValueSpaceDateTimeTriggers mDateTimeValueSpaceTriggers;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACESTRIGGERS_H

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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPE_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPE_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CDatatypeValueSpaceType
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceType : public CLinkerBase<CDatatypeValueSpaceType*,CDatatypeValueSpaceType> {
				// public methods
				public:

					enum VALUESPACETYPE { VALUESPACEUNKNOWNTYPE = 0, VALUESPACEREALTYPE = 1, VALUESPACESTRINGTYPE = 2, VALUESPACEBOOLEANTYPE = 3, VALUESPACEDOUBLETYPE = 4, VALUESPACEFLOATTYPE = 5, VALUESPACEIRITYPE = 6, VALUESPACEHEXDATATYPE = 7, VALUESPACEBASE64DATATYPE = 8, VALUESPACEXMLTYPE = 9, VALUESPACEDATETIMETYPE = 10 };

					//! Constructor
					CDatatypeValueSpaceType(VALUESPACETYPE type);

					VALUESPACETYPE getValueSpaceType();

					CDatatypeValueSpaceType* getNextValueSpaceType();

					cint64 getValueSpaceTypeIndex();
					static cint64 getValueSpaceTypeCount();

					const static cint64 VALUESPACETYPECOUNT = 11;

				// protected methods
				protected:

				// protected variables
				protected:
					VALUESPACETYPE mType;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETYPE_H

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

#ifndef KONCLUDE_REASONER_TRIPLES_CTRIPLESDATA_H
#define KONCLUDE_REASONER_TRIPLES_CTRIPLESDATA_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Triples {


			enum CTRIPLES_DATA_UPDATE_TYPE {
				TRIPLES_DATA_NOTHING,
				TRIPLES_DATA_ADDITION,
				TRIPLES_DATA_REMOVAL,
				TRIPLES_DATA_NEW
			};


			/*! 
			 *
			 *		\class		CTriplesData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTriplesData {
				// public methods
				public:
					//! Constructor
					CTriplesData();

					virtual CTRIPLES_DATA_UPDATE_TYPE getUpdateType() = 0;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CTRIPLESDATA_H

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

#ifndef KONCLUDE_REASONER_ANSWERER_CMAPPINGSINSTANCESCOMPUTATIONMODE_H
#define KONCLUDE_REASONER_ANSWERER_CMAPPINGSINSTANCESCOMPUTATIONMODE_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"



// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CMappingsInstancesComputationMode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMappingsInstancesComputationMode {
				// public methods
				public:
					const static cint64 COMPUTATION_TYPE_NO_REALIZATION = 0x01;
					const static cint64 COMPUTATION_TYPEF_FULL_REALIZATION = 0x02;
					const static cint64 COMPUTATION_TYPEF_ALL = COMPUTATION_TYPE_NO_REALIZATION | COMPUTATION_TYPEF_FULL_REALIZATION;



					//! Constructor
					CMappingsInstancesComputationMode(cint64 mode = COMPUTATION_TYPE_NO_REALIZATION, cint64 sizeLimit = 1000);


					bool hasComputationMode(CMappingsInstancesComputationMode* mode);
					CMappingsInstancesComputationMode* setComputationMode(CMappingsInstancesComputationMode* mode);
					CMappingsInstancesComputationMode* addComputationMode(CMappingsInstancesComputationMode* mode);

					bool hasALLComputationMode();
					bool hasComputationMode(cint64 mode, cint64 limit);
					bool hasComputationMode(cint64 mode);
					cint64 getVariableMappingsComputationMode();
					CMappingsInstancesComputationMode* setComputationMode(cint64 mode);
					CMappingsInstancesComputationMode* addComputationMode(cint64 mode);



					cint64 getComputationSizeLimit();
					CMappingsInstancesComputationMode* setComputationSizeLimit(cint64 limit);

				// protected methods
				protected:

				// protected variables
				protected:

					cint64 mComputationMode;
					cint64 mComputationSizeLimit;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CMAPPINGSINSTANCESCOMPUTATIONMODE_H

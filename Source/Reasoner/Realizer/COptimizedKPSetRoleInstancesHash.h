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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESHASH_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESHASH_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesHashData.h"
#include "COptimizedKPSetRoleInstancesData.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetRoleInstancesHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleInstancesHash : public QHash<cint64,COptimizedKPSetRoleInstancesHashData> {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleInstancesHash();


					COptimizedKPSetRoleInstancesData* getInstanceItemData(COptimizedKPSetIndividualItem* item);
					COptimizedKPSetRoleInstancesData* getInstanceItemData(cint64 indiId, bool create = true);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLEINSTANCESHASH_H

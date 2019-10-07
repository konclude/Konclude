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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLENEIGHBOURINSTANCESHASHDATA_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLENEIGHBOURINSTANCESHASHDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesHash.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetRoleNeighbourInstancesHashData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleNeighbourInstancesHashData {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleNeighbourInstancesHashData();



					COptimizedKPSetRoleInstancesData* getRoleNeighbourInstanceItemData(bool inversed, cint64 indiId, bool forceCreated = true);

					COptimizedKPSetRoleInstancesData* getRoleSuccessorInstanceItemData(cint64 indiId, bool forceCreated = true);
					COptimizedKPSetRoleInstancesData* getRolePredecessorInstanceItemData(cint64 indiId, bool forceCreated = true);


					COptimizedKPSetRoleInstancesData* getRoleSuccessorInstanceItemData(COptimizedKPSetIndividualItem* item);
					COptimizedKPSetRoleInstancesData* getRolePredecessorInstanceItemData(COptimizedKPSetIndividualItem* item);


					COptimizedKPSetRoleInstancesHash* getRoleSuccessorInstancesHash(bool forceCreated = true);
					COptimizedKPSetRoleInstancesHash* getRolePredecessorInstancesHash(bool forceCreated = true);
					COptimizedKPSetRoleInstancesHash* getRoleNeighbourInstancesHash(bool inversed, bool forceCreated = true);


					COptimizedKPSetRoleInstancesHash* mRoleSuccessorInstancesHash;
					COptimizedKPSetRoleInstancesHash* mRolePredecessorInstancesHash;

					bool mRoleSuccessorInstancesInitialized;
					bool mRolePredecessorInstancesInitialized;
					bool mRoleSuccessorInstancesInitializing;
					bool mRolePredecessorInstancesInitializing;


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

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETROLENEIGHBOURINSTANCESHASHDATA_H

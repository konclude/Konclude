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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCondensedReapplyConceptDescriptor_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCondensedReapplyConceptDescriptor_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CReapplyConceptDescriptor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CCondensedReapplyConceptDescriptor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCondensedReapplyConceptDescriptor : public CReapplyConceptDescriptor {
					// public methods
					public:
						//! Constructor
						CCondensedReapplyConceptDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint = 0, bool isPositiveDes = true);
						CCondensedReapplyConceptDescriptor();

						CCondensedReapplyConceptDescriptor* getNext();

						CCondensedReapplyConceptDescriptor *initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint = 0, bool isPositiveDes = true);
						CCondensedReapplyConceptDescriptor *initReapllyDescriptor(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint *depTrackPoint, bool isPositiveDes, CProcessingRestrictionSpecification* procRest);

						bool isPositiveDescriptor();

						bool isExtended();

					// protected methods
					protected:

					// protected variables
					protected:
						bool mPositive;
						bool mExtended;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCondensedReapplyConceptDescriptor_H

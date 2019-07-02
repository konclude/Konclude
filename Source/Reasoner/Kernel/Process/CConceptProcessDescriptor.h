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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTPROCESSDESCRIPTOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTPROCESSDESCRIPTOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CConceptProcessPriority.h"
#include "CProcessingRestrictionSpecification.h"

// Other includes
#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Utilities/Container/CSortedLinker.h"

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
				 *		\class		CConceptProcessDescriptor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptProcessDescriptor : public CSortedLinkerBase<CConceptProcessDescriptor*,CConceptProcessDescriptor> {
					// public methods
					public:
						//! Constructor
						CConceptProcessDescriptor(CConceptDescriptor *conceptDescriptor, CConceptProcessPriority processPriority, bool conceptReapplied = false, CDependencyTrackPoint *dependencyTrackPoint = nullptr);

						CConceptProcessDescriptor();

						CConceptProcessDescriptor *init(CConceptDescriptor *conceptDescriptor, CConceptProcessPriority processPriority, bool conceptReapplied = false, CDependencyTrackPoint *dependencyTrackPoint = nullptr, CProcessingRestrictionSpecification* procSpec = nullptr);
						CConceptProcessDescriptor *initCopy(CConceptProcessDescriptor* conceptProcessDes);

						CConceptDescriptor *getConceptDescriptor();
						CConceptProcessPriority getProcessPriority();
						CDependencyTrackPoint *getDependencyTrackPoint();
						bool isConceptReapplied();
						CProcessingRestrictionSpecification* getProcessingRestrictionSpecification();

						bool operator<=(const CConceptProcessDescriptor &descriptor);


					// protected methods
					protected:

					// protected variables
					protected:
						CConceptDescriptor *conceptDes;
						CConceptProcessPriority priority;
						CDependencyTrackPoint *depTrackPoint;
						CProcessingRestrictionSpecification* mProcSpec;
						bool mReapplied;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTPROCESSDESCRIPTOR_H

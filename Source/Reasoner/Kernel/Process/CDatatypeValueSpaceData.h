/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CClashedDependencyDescriptor.h"
#include "CDatatypeValueSpaceValuesCounter.h"
#include "CDatatypeDependencyTrackPointCollection.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeValueSpaceData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueSpaceData {
					// public methods
					public:
						//! Constructor
						CDatatypeValueSpaceData(CProcessContext* processContext);

						CDatatypeValueSpaceData* initValueSpaceData(CDatatypeValueSpaceData* spaceData);


						bool isValueSpaceClashCheckingRequired();
						CDatatypeValueSpaceData* setValueSpaceClashCheckingRequired(bool clashCheckingRequired);


						CDatatypeDependencyTrackPointCollection* getClashDependencyTrackPointCollection();
						CClashedDependencyDescriptor* getClashDependencyDescriptorLinker();


						bool isValueSpaceClashed();
						CDatatypeValueSpaceData* setValueSpaceClashed(bool clashed);

						CDependencyTrackPoint* getValueSpaceInitialisationDependencyTrackPoint();
						CDatatypeValueSpaceData* setValueSpaceInitialisationDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint);
						bool setValueSpaceInitialisationDependencyTrackPointCandidate(CDependencyTrackPoint* depTrackPoint);


						bool isValueSpaceTriggeringCompleted();
						CDatatypeValueSpaceData* setValueSpaceTriggeringCompleted(bool triggeringCompleted);

						bool isValueSpaceTriggeringRequired();
						CDatatypeValueSpaceData* setValueSpaceTriggeringRequired(bool triggeringRequired);

						bool isValueSpaceTriggeringStarted();
						CDatatypeValueSpaceData* setValueSpaceTriggeringStarted(bool triggeringStarted);

						bool isValueSpaceCountingRequired();
						CDatatypeValueSpaceData* setValueSpaceCountingRequired(bool recountingRequired);

						bool isValueSpaceCounted();
						CDatatypeValueSpaceData* setValueSpaceCounted(bool counted);

						CDatatypeValueSpaceValuesCounter* getValuesCounter();


						CDatatypeValueSpaceData* setValueSpaceModification();

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CDependencyTrackPoint* mValueSpaceInitialisationDepTrackPoint;

						bool mValueSpaceClashCheckingRequired;
						bool mValueSpaceClashed;
						CDatatypeDependencyTrackPointCollection mClashDepTrackPointCollection;

						bool mValueSpaceTriggeringCompleted;
						bool mValueSpaceTriggeringStarted;
						bool mValueSpaceTriggeringRequired;

						bool mValueSpaceCounted;
						bool mValueSpaceCountingRequired;
						CDatatypeValueSpaceValuesCounter mValuesCounter;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEVALUESPACEDATA_H

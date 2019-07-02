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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEPROPAGATIONSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEPROPAGATIONSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CRepresentativePropagationMap.h"
#include "CRepresentativePropagationMapData.h"
#include "CRepresentativePropagationDescriptor.h"
#include "CRepresentativeVariableBindingPathSetData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CRepresentativePropagationSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CRepresentativePropagationSet {
					// public methods
					public:
						//! Constructor
						CRepresentativePropagationSet(CProcessContext* processContext);

						CRepresentativePropagationSet* initRepresentativePropagationSet(CRepresentativePropagationSet* prevSet);

						CRepresentativePropagationMap* getRepresentativePropagationMap();
						bool containsRepresentativePropagation(CRepresentativeVariableBindingPathSetData* repRepPropSetData);
						bool containsRepresentativePropagation(cint64 repID);
						CRepresentativePropagationDescriptor* getRepresentativePropagationDescriptor(CRepresentativeVariableBindingPathSetData* repRepPropSetData);

						CRepresentativePropagationSet* addIncomingRepresentativePropagation(CRepresentativePropagationDescriptor* repPropDes);
						CRepresentativePropagationSet* copyRepresentativePropagations(CRepresentativePropagationMap* repPropMap);

						CConceptDescriptor* getConceptDescriptor();
						CRepresentativePropagationSet* setConceptDescriptor(CConceptDescriptor* conDes);


						CRepresentativePropagationSet* addIncomingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker);
						CRepresentativePropagationDescriptor* getIncomingRepresentativePropagationDescriptorLinker();

						CRepresentativePropagationSet* setOutgoingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker);
						CRepresentativePropagationSet* addOutgoingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker);
						CRepresentativePropagationDescriptor* getOutgoingRepresentativePropagationDescriptorLinker();

						cint64 getIncomingRepresentativePropagationSignatureKey();


						CRepresentativePropagationDescriptor* getLastProcessedIncomingRepresentativePropagationDescriptorLinker();
						CRepresentativePropagationSet* setLastProcessedIncomingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* descriptor);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CRepresentativePropagationMap mRepPropMap;
						CConceptDescriptor* mConceptDescriptor;
						CRepresentativePropagationDescriptor* mIncomingRepPropDesLinker;
						CRepresentativePropagationDescriptor* mOutgoingRepPropDesLinker;
						CRepresentativeVariableBindingPathSetDataSignature mIncomingRepPropSignature;

						CRepresentativePropagationDescriptor* mLastProcessedIncomingRepPropDesLinker;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREPRESENTATIVEPROPAGATIONSET_H

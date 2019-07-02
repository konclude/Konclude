/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CPropagationBindingMap.h"
#include "CPropagationBindingDescriptor.h"
#include "CPropagationBindingReapplyConceptDescriptor.h"
#include "CPropagationBindingReapplyConceptHash.h"
#include "CPropagationVariableBindingTransitionExtension.h"
#include "CPropagationRepresentativeTransitionExtension.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CPropagationBindingSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPropagationBindingSet {
					// public methods
					public:
						//! Constructor
						CPropagationBindingSet(CProcessContext* processContext);

						CPropagationBindingSet* initPropagationBindingSet(CPropagationBindingSet* prevSet);

						CPropagationBindingMap* getPropagationBindingMap();
						bool containsPropagationBinding(CPropagationBinding* propagationBinding);
						bool containsPropagationBinding(cint64 bindingID);
						CPropagationBindingDescriptor* getPropagationBindingDescriptor(CPropagationBinding* propagationBinding);

						CPropagationBindingDescriptor* getNewSepcialPropagationBindingDescriptor();

						CPropagationBindingSet* addPropagationBinding(CPropagationBindingDescriptor* propBindDes, bool newSpecial);
						CPropagationBindingReapplyConceptDescriptor* addPropagationBindingReturnReapplyLinker(CPropagationBindingDescriptor* propBindDes, bool newSpecial);
						CPropagationBindingSet* copyPropagationBindings(CPropagationBindingMap* propBindMap);

						CConceptDescriptor* getConceptDescriptor();
						CPropagationBindingSet* setConceptDescriptor(CConceptDescriptor* conDes);


						CPropagationBindingSet* addPropagationBindingDescriptorLinker(CPropagationBindingDescriptor* propBindDesLinker);
						CPropagationBindingDescriptor* getPropagationBindingDescriptorLinker();
						
						CPropagationBindingReapplyConceptHash* getPropagationBindingReapplyConceptHash(bool create = true);
						CPropagationBindingSet* addPropagationBindingReapplyConceptDescriptor(CPropagationBindingReapplyConceptDescriptor* propBindReapplyConDesLinker);

						CPropagationVariableBindingTransitionExtension* getPropagationVariableBindingTransitionExtension(bool create = true);
						CPropagationRepresentativeTransitionExtension* getPropagationRepresentativeTransitionExtension(bool create = true);

						bool hasPropagateAllFlag();
						bool getPropagateAllFlag();
						CPropagationBindingSet* setPropagateAllFlag(bool propAllFlag);
						bool adoptPropagateAllFlag(CPropagationBindingSet* propBindSet);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CPropagationBindingMap mPropMap;
						bool mPropagateAllFlag;
						CConceptDescriptor* mConceptDescriptor;
						CPropagationBindingDescriptor* mPropBindDesLinker;

						CPropagationBindingDescriptor* mSpecialNewPropBindDes;
						CPropagationBindingReapplyConceptHash* mReapplyHash;

						CPropagationVariableBindingTransitionExtension* mPropVarBindTransExtension;
						CPropagationRepresentativeTransitionExtension* mPropRepTransExtension;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGSET_H

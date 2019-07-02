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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGREAPPLYCONCEPTHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGREAPPLYCONCEPTHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CPropagationBindingReapplyConceptIterator.h"
#include "CPropagationBindingReapplyConceptHashData.h"
#include "CPropagationBindingReapplyConceptDescriptor.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CPropagationBindingReapplyConceptHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPropagationBindingReapplyConceptHash : public CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData> {
					// public methods
					public:
						//! Constructor
						CPropagationBindingReapplyConceptHash(CProcessContext* processContext);

						CPropagationBindingReapplyConceptHash* initPropagationBindingReapplyConceptHash(CPropagationBindingReapplyConceptHash* prevHash);

						CPropagationBindingReapplyConceptHash* addPropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair, CPropagationBindingReapplyConceptDescriptor* reapplyConDes);
						CPropagationBindingReapplyConceptHash* addPropagationBindingReapplyConceptDescriptor(CIndividualProcessNode* indi, CConcept* concept, CPropagationBindingReapplyConceptDescriptor* reapplyConDes);

						CPropagationBindingReapplyConceptDescriptor* takePropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair);
						bool hasPropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair);

						CPropagationBindingReapplyConceptIterator getPropagationBindingReapplyConceptDescriptorIterator();

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDINGREAPPLYCONCEPTHASH_H

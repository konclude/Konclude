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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGTRIGGERLINKER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGTRIGGERLINKER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CVariableBindingPath.h"
#include "CVariableBindingPathDescriptor.h"
#include "CVariableBindingDescriptor.h"


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
				 *		\class		CVariableBindingTriggerLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CVariableBindingTriggerLinker : public CLinkerBase<CVariableBindingPathDescriptor*,CVariableBindingTriggerLinker> {
					// public methods
					public:
						//! Constructor
						CVariableBindingTriggerLinker(CVariableBindingPathDescriptor* varBindPath = nullptr);

						CVariableBindingTriggerLinker* initTriggerLinker(CVariableBindingPathDescriptor* varBindPathDes, CVariableBindingDescriptor* varBindDes, bool leftTriggered); 

						CVariableBindingPathDescriptor* getVariableBindingPathDescriptor(); 
						CVariableBindingTriggerLinker* setVariableBindingPathDescriptor(CVariableBindingPathDescriptor* varBindPathDes); 
						
						CVariableBindingDescriptor* getNextTriggerVariableBindingDescriptor(); 
						CVariableBindingTriggerLinker* setNextTriggerVariableBindingDescriptor(CVariableBindingDescriptor* varBindDes); 
						
						bool isLeftTriggered(); 
						CVariableBindingTriggerLinker* setLeftTriggered(bool leftTriggered); 

					// protected methods
					protected:

					// protected variables
					protected:
						CVariableBindingDescriptor* mNextTriggerVariableBinding;
						bool mLeftTriggered;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGTRIGGERLINKER_H

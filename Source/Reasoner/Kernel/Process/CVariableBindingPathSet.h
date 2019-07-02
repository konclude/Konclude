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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHSET_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHSET_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CVariableBindingPathMap.h"
#include "CVariableBindingPathMapData.h"
#include "CVariableBindingPathDescriptor.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CVariableBindingPathSet
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CVariableBindingPathSet {
					// public methods
					public:
						//! Constructor
						CVariableBindingPathSet(CProcessContext* processContext);

						CVariableBindingPathSet* initVariableBindingPathSet(CVariableBindingPathSet* prevSet);

						CVariableBindingPathMap* getVariableBindingPathMap();
						bool containsVariableBindingPath(CVariableBindingPath* variableBindingPath);
						bool containsVariableBindingPath(cint64 bindingID);
						CVariableBindingPathDescriptor* getVariableBindingPathDescriptor(CVariableBindingPath* variableBindingPath);

						CVariableBindingPathSet* addVariableBindingPath(CVariableBindingPathDescriptor* varBindPathDes);
						CVariableBindingPathSet* copyVariableBindingPaths(CVariableBindingPathMap* varBindPathMap);

						CConceptDescriptor* getConceptDescriptor();
						CVariableBindingPathSet* setConceptDescriptor(CConceptDescriptor* conDes);


						CVariableBindingPathSet* addVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* varBindPathDesLinker);
						CVariableBindingPathDescriptor* getVariableBindingPathDescriptorLinker();

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CVariableBindingPathMap mVarBindPathMap;
						CConceptDescriptor* mConceptDescriptor;
						CVariableBindingPathDescriptor* mVarBindPathDesLinker;


					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CVARIABLEBINDINGPATHSET_H

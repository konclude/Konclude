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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREPRESENTATIVESELECTDEPENDENCYNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREPRESENTATIVESELECTDEPENDENCYNODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CDeterministicDependencyNode.h"


// Other includes
#include "Reasoner/Kernel/Process/CVariableBindingPath.h"
#include "Reasoner/Kernel/Process/CConceptDescriptor.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CRepresentativeSelectDependencyNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CRepresentativeSelectDependencyNode : public CDeterministicDependencyNode {
						// public methods
						public:
							//! Constructor
							CRepresentativeSelectDependencyNode(CProcessContext* processContext = nullptr);


							CRepresentativeSelectDependencyNode* initRepresentativeSelectDependencyNode(DEPENDENCNODEYTYPE depType, CConceptDescriptor* conceptDescriptor = nullptr, CVariableBindingPath* selectedVarBindPath = nullptr);
							
							virtual bool isRepresentativeSelectDependencyNode();
							CVariableBindingPath* getSelectedVariableBindingPath();

						// protected methods
						protected:


						// protected variables
						protected:
							CVariableBindingPath* mSelectedVarBindPath;

						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREPRESENTATIVESELECTDEPENDENCYNODE_H

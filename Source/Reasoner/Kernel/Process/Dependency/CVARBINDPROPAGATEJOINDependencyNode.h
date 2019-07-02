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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CVARBINDPROPAGATEJOINDEPENDENCYNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CVARBINDPROPAGATEJOINDEPENDENCYNODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CDeterministicDependencyNode.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CVARBINDPROPAGATEJOINDependencyNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CVARBINDPROPAGATEJOINDependencyNode : public CDeterministicDependencyNode {
						// public methods
						public:
							//! Constructor
							CVARBINDPROPAGATEJOINDependencyNode(CProcessContext* processContext = nullptr);


							CVARBINDPROPAGATEJOINDependencyNode* initVARBINDPROPAGATEJOINDependencyNode(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* prevConceptDependencyTrackPoint, CDependencyTrackPoint* prevOtherDependencyTrackPoint);
							

						// protected methods
						protected:


						// protected variables
						protected:
							CDependency mPrevOtherDep;

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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CVARBINDPROPAGATEJOINDEPENDENCYNODE_H

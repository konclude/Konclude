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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREUSEBACKENDEXPANSIONMODESDEPENDENCYNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREUSEBACKENDEXPANSIONMODESDEPENDENCYNODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CNonDeterministicDependencyNode.h"


// Other includes
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CREUSEBACKENDEXPANSIONMODESDependencyNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CREUSEBACKENDEXPANSIONMODESDependencyNode : public CNonDeterministicDependencyNode {
						// public methods
						public:
							//! Constructor
							CREUSEBACKENDEXPANSIONMODESDependencyNode(CProcessContext* processContext = nullptr);


							CREUSEBACKENDEXPANSIONMODESDependencyNode* initREUSEBACKENDEXPANSIONMODESDependencyNode(CBranchTreeNode* branchNode, CDependencyTrackPoint* depTrackPoint);
							
							CNonDeterministicDependencyTrackPoint* getFixedReuseDependencyTrackPoint();
							CREUSEBACKENDEXPANSIONMODESDependencyNode* setFixedReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);

							CNonDeterministicDependencyTrackPoint* getPriorizedReuseDependencyTrackPoint();
							CREUSEBACKENDEXPANSIONMODESDependencyNode* setPriorizedReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint);

							CXLinker<cint64>* getAffectedIndividualIdLinker();
							bool addAffectedIndividualIdLinker(CXLinker<cint64>* expectedLinker, CXLinker<cint64>* newLinker);


							CXLinker<cint64>* getInvolvedIndividualIdLinker();
							bool setInvolvedIndividualIdLinker(CXLinker<cint64>* newLinker);


						// protected methods
						protected:

						// protected variables
						protected:
							CNonDeterministicDependencyTrackPoint* mFixedReuseDepTrackPoint;
							CNonDeterministicDependencyTrackPoint* mPriorizedReuseDepTrackPoint;

							QAtomicPointer<CXLinker<cint64>> mAffectedIndividualIdLinker;
							CXLinker<cint64>* mInvolvedIndividualIdLinker;

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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CREUSEBACKENDEXPANSIONMODESDEPENDENCYNODE_H

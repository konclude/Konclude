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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CORDISJUNCTDEPENDENCYTRACKPOINT_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CORDISJUNCTDEPENDENCYTRACKPOINT_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CNonDeterministicDependencyTrackPoint.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CDisjunctBranchingStatistics.h"

#include "Utilities/Container/CLinker.h"
#include "Utilities/CSortedNegLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CORDisjunctDependencyTrackPoint
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CORDisjunctDependencyTrackPoint : public CNonDeterministicDependencyTrackPoint {
						// public methods
						public:
							//! Constructor
							CORDisjunctDependencyTrackPoint(CDependencyNode* depNode = nullptr);

							CSortedNegLinker<CConcept*>* getDisjunctConceptLinker();
							CDisjunctBranchingStatistics* getDisjunctBranchingStatistics();


							CORDisjunctDependencyTrackPoint* setDisjunctConceptLinker(CSortedNegLinker<CConcept*>* disjunctConLinker);
							CORDisjunctDependencyTrackPoint* setDisjunctBranchingStatistics(CDisjunctBranchingStatistics* disjunctBranchStats);


						// protected methods
						protected:

						// protected variables
						protected:
							CSortedNegLinker<CConcept*>* mDisjunctConceptLinker;
							CDisjunctBranchingStatistics* mDisjunctBranchStats;

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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CORDISJUNCTDEPENDENCYTRACKPOINT_H

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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGORPROCESSINGRESTRICTIONSPECIFICATION_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGORPROCESSINGRESTRICTIONSPECIFICATION_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessingRestrictionSpecification.h"
#include "CClashedConceptDescriptor.h"

// Other includes
#include "Reasoner/Ontology/CConceptRoleBranchingTrigger.h"
#include "Reasoner/Ontology/CDisjunctBranchingStatistics.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBranchingORProcessingRestrictionSpecification
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBranchingORProcessingRestrictionSpecification : public CProcessingRestrictionSpecification {
					// public methods
					public:
						//! Constructor
						CBranchingORProcessingRestrictionSpecification();

						CBranchingORProcessingRestrictionSpecification* initBranchingORProcessingRestriction(CBranchingORProcessingRestrictionSpecification* prevRest = nullptr);
					
						CBranchingORProcessingRestrictionSpecification* setConceptRoleBranchingTrigger(CConceptRoleBranchingTrigger* conRoleBranchTrigger);

						CConceptRoleBranchingTrigger* getConceptRoleBranchingTrigger();

						CSortedNegLinker<CConcept*>* getFirstNotPosAndNegContainedOperand();
						CSortedNegLinker<CConcept*>* getSecondNotPosAndNegContainedOperand();
						CSortedNegLinker<CConcept*>* getContainedOperand();


						CDisjunctBranchingStatistics* getFirstNotPosAndNegContainedOperandBranchingStatistics();
						CDisjunctBranchingStatistics* getSecondNotPosAndNegContainedOperandBranchingStatistics();

						CBranchingORProcessingRestrictionSpecification* setFirstNotPosAndNegContainedOperandBranchingStatistics(CDisjunctBranchingStatistics* branchStats);
						CBranchingORProcessingRestrictionSpecification* setSecondNotPosAndNegContainedOperandBranchingStatistics(CDisjunctBranchingStatistics* branchStats);

						CBranchingORProcessingRestrictionSpecification* setFirstNotPosAndNegContainedOperand(CSortedNegLinker<CConcept*>* firstOp);
						CBranchingORProcessingRestrictionSpecification* setSecondNotPosAndNegContainedOperand(CSortedNegLinker<CConcept*>* secondOp);

						CBranchingORProcessingRestrictionSpecification* setContainedOperand(CSortedNegLinker<CConcept*>* containedOp);



						CBranchingORProcessingRestrictionSpecification* addClashedConceptDescriptors(CClashedDependencyDescriptor* clashes);
						CClashedDependencyDescriptor* getClashedConceptDescriptors();


					// protected methods
					protected:

					// protected variables
					protected:
						CConceptRoleBranchingTrigger* mConRoleBranchTrigg;
						CSortedNegLinker<CConcept*>* mFirstNotPosAndNegContainedOperand;
						CSortedNegLinker<CConcept*>* mSecondNotPosAndNegContainedOperand;
						CSortedNegLinker<CConcept*>* mContainedOperand;
						CClashedDependencyDescriptor* mClashes;

						CDisjunctBranchingStatistics* mFirstNotPosAndNegContainedOperandBranchStats;
						CDisjunctBranchingStatistics* mSecondNotPosAndNegContainedOperandBranchStats;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBRANCHINGORPROCESSINGRESTRICTIONSPECIFICATION_H

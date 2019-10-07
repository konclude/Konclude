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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCLASHDESCRIPTORFACTORY_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCLASHDESCRIPTORFACTORY_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"


// Other includes
#include "Reasoner/Kernel/Process/CClashedIndividualLinkDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedIndividualDistinctDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedNegationDisjointLinkDescriptor.h"

// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CClashDescriptorFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CClashDescriptorFactory {
					// public methods
					public:
						//! Constructor
						CClashDescriptorFactory();

						//! Destructor
						virtual ~CClashDescriptorFactory();


						CClashedDependencyDescriptor* createClashedConceptDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext = nullptr);
						CClashedDependencyDescriptor* createClashedIndividualLinkDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualLinkEdge* link, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext = nullptr);
						CClashedDependencyDescriptor* createClashedIndividualDistinctDescriptor(CClashedDependencyDescriptor* prevClashes, CDistinctEdge* distinct, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext = nullptr);
						CClashedDependencyDescriptor* createClashedNegationDisjointDescriptor(CClashedDependencyDescriptor* prevClashes, CNegationDisjointEdge* disjointNegLink, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext = nullptr);

					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCLASHDESCRIPTORFACTORY_H

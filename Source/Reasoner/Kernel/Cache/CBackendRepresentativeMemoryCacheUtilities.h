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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEUTILITIES_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEUTILITIES_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

#include "Reasoner/Kernel/Process/CConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CConceptSaturationDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualMergingHash.h"
#include "Reasoner/Kernel/Process/CDistinctHash.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Process;

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheUtilities
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheUtilities {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheUtilities();

						static cint64 getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, bool deterministic, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts);
						static cint64 getConceptDescriptorSignature(CConceptSaturationDescriptor* conDesLinker, cint64 count, CConcept* exclusionConcept);
						static cint64 getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction);
						static cint64 getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction, function<bool(CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)> deterministicDetermineFunction);

						static cint64 getRoleInversedLinkerSignature(CSortedNegLinker<CRole*>* roleLinker, bool inversed, cint64 count);
						static cint64 getNeighbourRoleInstantiatedSetLinkerSignature(CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neighbourRoleSetLinker, cint64 count);

						static cint64 getIndividualSetSignature(cint64 indiId, CIndividualMergingHash* indiMerginghash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag);
						static cint64 getIndividualSetSignature(CPROCESSSET<cint64>* individualSet, cint64& count);
						static cint64 getIndividualSetSignature(cint64 indiId, CDistinctHash* indiDistinctHash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag);

						static cint64 getSignatureExtensionByCacheValue(cint64 signature, CCacheValue& cacheValue);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEUTILITIES_H

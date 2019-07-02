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

#include "CBackendRepresentativeMemoryCacheUtilities.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheUtilities::CBackendRepresentativeMemoryCacheUtilities() {
				}


				cint64 CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(CConceptSaturationDescriptor* conDesLinker, cint64 count, CConcept* exclusionConcept) {
					cint64 sigValue = 0;
					for (CConceptSaturationDescriptor* conDesLinkerIt = conDesLinker; conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNext()) {
						CConcept* concept = conDesLinkerIt->getConcept();
						bool negated = conDesLinkerIt->isNegated();

						if (negated || concept != exclusionConcept) {

							cint64 tag = concept->getConceptTag();
							if (negated) {
								tag = -tag;
							}
							sigValue += qHash((cint64)tag);
						}
					}
					return sigValue;
				}

				cint64 CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, bool deterministic, cint64 maxDeterministicBranchTag, bool excludePositiveNominalConcepts) {
					cint64 sigValue = 0;
					count = 0;
					for (CConceptDescriptor* conDesLinkerIt = conDesLinker; conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNext()) {
						CConcept* concept = conDesLinkerIt->getConcept();
						bool negated = conDesLinkerIt->isNegated();


						bool deterministicallyDerived = true;
						if (!conDesLinkerIt->getDependencyTrackPoint() || conDesLinkerIt->getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
							deterministicallyDerived = false;
						}
						bool considerConcept = deterministicallyDerived == deterministic;
						if (excludePositiveNominalConcepts && concept->getOperatorCode() == CCNOMINAL && !negated) {
							considerConcept = false;
						}

						if (considerConcept) {
							cint64 tag = concept->getConceptTag();
							if (negated) {
								tag = -tag;
							}
							sigValue += qHash((cint64)tag);
							++count;
						}
					}
					return sigValue;
				}


				cint64 CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(cint64 indiId, CIndividualMergingHash* indiMerginghash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag) {
					cint64 sigValue = 0;
					count = 0;
					for (CIndividualMergingHash::const_iterator it = indiMerginghash->constBegin(), itEnd = indiMerginghash->constEnd(); it != itEnd; ++it) {
						cint64 mergedIndiId = it.key();
						if (mergedIndiId != indiId && it.value().isMergedWithIndividual()) {
							CIndividualMergingHashData mergedData = it.value();

							bool deterministicallyDerived = true;
							if (!mergedData.getDependencyTrackPoint() || mergedData.getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
								deterministicallyDerived = false;
							}
							bool considerIndividual = deterministicallyDerived || !onlyDeterministic;

							if (considerIndividual) {
								cint64 tag = mergedIndiId;
								sigValue += qHash((cint64)tag);
								++count;
							}
						}
					}
					if (count > 0 && indiId >= 0) {
						sigValue += qHash((cint64)indiId);
						++count;
					}
					return sigValue;
				}

				cint64 CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(cint64 indiId, CDistinctHash* indiDistinctHash, cint64& count, bool onlyDeterministic, cint64 maxDeterministicBranchTag) {
					cint64 sigValue = 0;
					count = 0;
					for (CDistinctHash::const_iterator it = indiDistinctHash->constBegin(), itEnd = indiDistinctHash->constEnd(); it != itEnd; ++it) {
						cint64 distinctIndiId = -it.key();
						if (distinctIndiId >= 0 && distinctIndiId != indiId) {
							CDistinctEdge* distinctEdge = it.value();

							bool deterministicallyDerived = true;
							if (!distinctEdge->getDependencyTrackPoint() || distinctEdge->getDependencyTrackPoint()->getBranchingTag() > maxDeterministicBranchTag) {
								deterministicallyDerived = false;
							}
							bool considerIndividual = deterministicallyDerived || !onlyDeterministic;

							if (considerIndividual) {
								cint64 tag = distinctIndiId;
								sigValue += qHash((cint64)tag);
								++count;
							}
						}
					}
					if (count > 0 && indiId >= 0) {
						sigValue += qHash((cint64)indiId);
						++count;
					}
					return sigValue;
				}

				cint64 CBackendRepresentativeMemoryCacheUtilities::getIndividualSetSignature(CPROCESSSET<cint64>* individualSet, cint64& count) {
					cint64 sigValue = 0;
					count = 0;
					for (CPROCESSSET<cint64>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
						cint64 indiId = *it;
						sigValue += qHash((cint64)indiId);
						++count;
					}
					return sigValue;
				}

				cint64 CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction) {
					cint64 sigValue = 0;
					count = 0;
					for (CConceptDescriptor* conDesLinkerIt = conDesLinker; conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNext()) {
						CConcept* concept = conDesLinkerIt->getConcept();
						bool negated = conDesLinkerIt->isNegated();

						bool considerConcept = exclusionDetermineFunction(concept, negated);

						if (considerConcept) {
							cint64 tag = concept->getConceptTag();
							if (negated) {
								tag = -tag;
							}
							sigValue += qHash((cint64)tag);
							++count;
						}
					}
					return sigValue;
				}


				cint64 CBackendRepresentativeMemoryCacheUtilities::getConceptDescriptorSignature(CConceptDescriptor* conDesLinker, cint64& count, function<bool(CConcept* concept, bool negation)> exclusionDetermineFunction, function<bool(CConcept* concept, bool negation, CDependencyTrackPoint* depTrackPoint)> deterministicDetermineFunction) {
					cint64 sigValue = 0;
					count = 0;
					for (CConceptDescriptor* conDesLinkerIt = conDesLinker; conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNext()) {
						CConcept* concept = conDesLinkerIt->getConcept();
						bool negated = conDesLinkerIt->isNegated();

						bool considerConcept = exclusionDetermineFunction(concept, negated);
						if (considerConcept) {
							bool deterministic = deterministicDetermineFunction(concept, negated, conDesLinkerIt->getDependencyTrackPoint());
						
							cint64 tag = concept->getConceptTag();
							if (negated) {
								tag = -tag;
							}
							sigValue += qHash((cint64)tag);
							++count;
						}
					}
					return sigValue;
				}

				cint64 CBackendRepresentativeMemoryCacheUtilities::getRoleInversedLinkerSignature(CSortedNegLinker<CRole*>* roleLinker, bool inversed, cint64 count) {
					cint64 sigValue = 0;
					for (CSortedNegLinker<CRole*>* roleLinkerIt = roleLinker; roleLinkerIt; roleLinkerIt = roleLinkerIt->getNext()) {
						CRole* role = roleLinkerIt->getData();
						bool roleInversed = roleLinkerIt->isNegated() ^ inversed;

						cint64 tag = role->getRoleTag();
						if (roleInversed) {
							tag = -tag;
						}
						sigValue += qHash((cint64)tag);
					}
					return sigValue;
				}


				cint64 CBackendRepresentativeMemoryCacheUtilities::getNeighbourRoleInstantiatedSetLinkerSignature(CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neighbourRoleSetLinker, cint64 count) {
					cint64 signature = 0;
					for (CBackendRepresentativeMemoryCacheTemporaryLabelReferenceDataLinker* neighbourRoleSetLinkerIt = neighbourRoleSetLinker; neighbourRoleSetLinkerIt; neighbourRoleSetLinkerIt = neighbourRoleSetLinkerIt->getNext()) {
						CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = neighbourRoleSetLinkerIt->getReferredLabelData();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpLabelWriteData = neighbourRoleSetLinkerIt->getReferredTemporaryLabelData();

						cint64 value = 0;
						if (tmpLabelWriteData) {
							value = (cint64)tmpLabelWriteData;
						} else {
							value = (cint64)labelCacheItem->getCacheEntryID();
						}
						signature += qHash((qint64)value);
					}
					return signature;
				}


				cint64 CBackendRepresentativeMemoryCacheUtilities::getSignatureExtensionByCacheValue(cint64 signature, CCacheValue& cacheValue) {
					signature += qHash((qint64)cacheValue.getTag());
					return signature;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

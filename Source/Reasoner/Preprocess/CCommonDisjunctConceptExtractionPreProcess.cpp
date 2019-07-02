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

#include "CCommonDisjunctConceptExtractionPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CCommonDisjunctConceptExtractionPreProcess::CCommonDisjunctConceptExtractionPreProcess() {
				mOntology = nullptr;
				mLastConceptId = 0;
			}


			CCommonDisjunctConceptExtractionPreProcess::~CCommonDisjunctConceptExtractionPreProcess() {
				cDeleteAll(mSetContainer);
			}


			CConcreteOntology *CCommonDisjunctConceptExtractionPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.CommonDisjunctConceptExtraction.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				mOntology = nullptr;
				mLastConceptId = 0;

				if (nonELConstructsUsed || !skipForELOntologies) {

					mOntology = ontology;

					CMBox *mBox = ontology->getDataBoxes()->getMBox();
					CTBox *tBox = ontology->getDataBoxes()->getTBox();
					CRBox *rBox = ontology->getDataBoxes()->getRBox();
					CABox *aBox = ontology->getDataBoxes()->getABox();

					mConceptVector = tBox->getConceptVector();

					mTopConcept = mConceptVector->getLocalData(1);

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();

					mDisjunctionCommDisConHash.clear();

					mStatExtractedCommDisCon = 0;
					mStatExtractedConceptForDisjunctionCount = 0;

					extractCommonDisjuncts();

					LOG(INFO,"::Konclude::Reasoner::Preprocess::CommonDisjunctConceptExtractor",logTr("Extracted %1 concepts for %2 disjunctions.").arg(mStatExtractedCommDisCon).arg(mStatExtractedConceptForDisjunctionCount),this);
				}
				return ontology;
			}


			CCommonDisjunctConceptExtractionPreProcess* CCommonDisjunctConceptExtractionPreProcess::extractCommonDisjuncts() {
				CMBox *mBox = mOntology->getDataBoxes()->getMBox();
				CImplicationReplacementVector* repVector = mBox->getImplicationReplacementVector(true);
				cint64 conCount = mConceptVector->getItemCount();
				for (cint64 conIdx = mLastConceptId; conIdx < conCount; ++conIdx) {
					CConcept* concept = mConceptVector->getData(conIdx);
					if (concept) {
						cint64 conOpCode = concept->getOperatorCode();
						cint64 opCount = concept->getOperandCount();
						if (concept->getOperandCount() >= 1 && (conOpCode == CCAND || conOpCode == CCEQ || conOpCode == CCOR)) {
							bool negateConcept = conOpCode == CCAND || conOpCode == CCEQ;
							QSet<TConNegPair> consideredDisConConceptSet;
							QSet<TConNegPair> disConConceptSet;
							getDisjunctConcepts(concept, negateConcept, &disConConceptSet, &consideredDisConConceptSet);
							disConConceptSet.remove(TConNegPair(concept, negateConcept));
							bool localRepData = false;
							CReplacementData* prevReplData = repVector->getData(conIdx, &localRepData);
							if (!disConConceptSet.isEmpty() || prevReplData) {
								CReplacementData* replData = prevReplData;
								if (!localRepData) {
									replData = CObjectAllocator<CReplacementData>::allocateAndConstruct(mMemMan);
									repVector->setData(conIdx, replData);
									replData->initReplacementData(prevReplData);
								}
								++mStatExtractedConceptForDisjunctionCount;
								replData->clearCommonDisjunctConceptLinker();
								for (QSet<TConNegPair>::const_iterator it = disConConceptSet.constBegin(), itEnd = disConConceptSet.constEnd(); it != itEnd; ++it) {
									TConNegPair conNegPair(*it);
									CConcept* commCon = conNegPair.first;
									bool commNeg = conNegPair.second;
									if (commCon != concept || negateConcept != commNeg) {
										CXNegLinker<CConcept*>* commNegConLinker = CObjectAllocator< CXNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
										commNegConLinker->initNegLinker(commCon, commNeg);
										replData->addCommonDisjunctConceptLinker(commNegConLinker);
										++mStatExtractedCommDisCon;
									}
								}
							}
						}
					}
				}
				mLastConceptId = conCount;
				return this;
			}

			CConcreteOntology* CCommonDisjunctConceptExtractionPreProcess::continuePreprocessing() {
				if (mOntology) {
					extractCommonDisjuncts();
				}
				return mOntology;
			}



			bool CCommonDisjunctConceptExtractionPreProcess::getDisjunctConcepts(CConcept* concept, bool negated, QSet<TConNegPair>* collectConSet, QSet<TConNegPair>* consideredConSet) {
				if (!consideredConSet->contains(TConNegPair(concept,negated))) {
					consideredConSet->insert(TConNegPair(concept,negated));
					collectConSet->insert(TConNegPair(concept,negated));
					cint64 opCode = concept->getOperatorCode();
					if (!negated && (opCode == CCSUB || opCode == CCEQ || opCode == CCAND || (concept->getOperandCount() == 1 && opCode == CCOR)) || negated && (opCode == CCOR || (concept->getOperandCount() == 1 && (opCode == CCAND || opCode == CCEQ)))) {
						for (CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList(); opLinker; opLinker = opLinker->getNext()) {
							CConcept* opConcept = opLinker->getData();
							bool opConNegation = opLinker->isNegated() ^ negated;
							getDisjunctConcepts(opConcept,opConNegation,collectConSet,consideredConSet);
						}
					} else if (!negated && (opCode == CCOR) || negated && (opCode == CCAND || opCode == CCEQ)) {

						QSet<TConNegPair>* firstDisjunctConSet = mDisjunctionCommDisConHash.value(TConNegPair(concept,negated));
						if (!firstDisjunctConSet) {
							firstDisjunctConSet = new QSet<TConNegPair>();
							QSet<TConNegPair> nextDisjunctConSet;
							CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
							if (opLinker) {
								CConcept* firstDisjunctConcept = opLinker->getData();
								bool firstDisConNegation = opLinker->isNegated() ^ negated;
								opLinker = opLinker->getNext();
								QSet<TConNegPair> consideredConSet1(*consideredConSet);
								getDisjunctConcepts(firstDisjunctConcept,firstDisConNegation,firstDisjunctConSet,&consideredConSet1);
								while (opLinker && !firstDisjunctConSet->isEmpty()) {
									CConcept* nextDisjunctConcept = opLinker->getData();
									bool nextDisConNegation = opLinker->isNegated() ^ negated;
									nextDisjunctConSet.clear();
									QSet<TConNegPair> consideredConSet2(*consideredConSet);
									getDisjunctConcepts(nextDisjunctConcept,nextDisConNegation,&nextDisjunctConSet,&consideredConSet2);

									QSet<TConNegPair>::iterator it = firstDisjunctConSet->begin();
									while (it != firstDisjunctConSet->end()) {
										TConNegPair conNegPair(*it);
										if (!nextDisjunctConSet.contains(conNegPair)) {
											it = firstDisjunctConSet->erase(it);
										} else {
											++it;
										}
									}

									opLinker = opLinker->getNext();
								}
							}

							mSetContainer.append(firstDisjunctConSet);
							mDisjunctionCommDisConHash.insert(TConNegPair(concept,negated),firstDisjunctConSet);
						}

						for (QSet<TConNegPair>::iterator it = firstDisjunctConSet->begin(); it != firstDisjunctConSet->end(); ++it) {
							TConNegPair conNegPair(*it);
							collectConSet->insert(conNegPair);
						}
					}
					return true;
				}
				return false;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

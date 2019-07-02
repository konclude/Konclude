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

#include "CDisjunctSortingPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CDisjunctSortingPreProcess::CDisjunctSortingPreProcess() {
			}


			CDisjunctSortingPreProcess::~CDisjunctSortingPreProcess() {
			}


			CConcreteOntology *CDisjunctSortingPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.DisjunctSorting.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				if (nonELConstructsUsed || !skipForELOntologies) {

					mTBox = ontology->getDataBoxes()->getTBox();

					mConceptVec = mTBox->getConceptVector();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOnto = ontology;

					mSortedDisjunctCount = 0;
					mPotentialDisjunctionCount = 0;

					cint64 conCount = mTBox->getConceptCount();
					for (cint64 conIdx = 0; conIdx < conCount; ++conIdx) {
						CConcept* concept = mConceptVec->getLocalData(conIdx);
						if (concept) {
							cint64 conOpCode = concept->getOperatorCode();
							cint64 opCount = concept->getOperandCount();
							if (opCount != 1) {
								bool sortOperands = false;
								bool disjunctionNegation = false;
								if (conOpCode == CCAND || conOpCode == CCEQ) {
									sortOperands = true;
									disjunctionNegation = true;
								} else if (conOpCode == CCOR) {
									sortOperands = true;
								}
								if (sortOperands) {
									++mPotentialDisjunctionCount;
									sortDisjuncts(concept,disjunctionNegation);
								}
							}
						}
					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::DisjunctSorting",logTr("Sorted %1 disjuncts for %2 possible disjunctions.").arg(mSortedDisjunctCount).arg(mPotentialDisjunctionCount),this);
				}
				return ontology;
			}


			void CDisjunctSortingPreProcess::addDisjunctToSort(CSortedNegLinker<CConcept*>* conLinker, bool negated) {
				CConcept* concept = conLinker->getData();
				bool conNegation = conLinker->isNegated() ^ negated;
				cint64 conCode = concept->getOperatorCode();
				CConceptOperator* conceptOperator = concept->getConceptOperator();
				if (conCode == CCATOM) {
					mAtomicDisjList.append(conLinker);
				} else if (conNegation && conCode == CCSUB) {
					mPrimNegDisjList.append(conLinker);
				} else if (!conNegation && (conCode == CCSELF || conCode == CCVALUE)) {
					mSimpleExistDisjList.append(conLinker);
				} else if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME || conCode == CCAQCHOOCE || conCode == CCATLEAST) || conNegation && (conCode == CCALL || conCode == CCATMOST)) {
					mExistDisjList.append(conLinker);
				} else if (!conNegation && conCode == CCNOMINAL) {
					mPosNominalDisjList.append(conLinker);
				} else if (conNegation && conCode == CCNOMINAL) {
					mNegNominalDisjList.append(conLinker);
				} else {
					if (!conNegation && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE) || conNegation && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_SOME_TYPE)) {
						mPropDisjList.append(conLinker);
					} else if (!conNegation && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_AND_AQAND_TYPE)) {
						mTriggerDisjList.append(conLinker);
					} else if (!conNegation && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE)) {
						mImplDisjList.append(conLinker);
					} else {
						mOtherDisjList.append(conLinker);
					}
				}
			}


			void CDisjunctSortingPreProcess::sortDisjuncts(CConcept* concept, bool negated) {
				CSortedNegLinker<CConcept*>* opConcepts = concept->getOperandList();
				mAtomicDisjList.clear();
				mExistDisjList.clear();
				mNegNominalDisjList.clear();
				mPosNominalDisjList.clear();
				mPrimNegDisjList.clear();
				mSimpleExistDisjList.clear();
				mPropDisjList.clear();
				mTriggerDisjList.clear();
				mImplDisjList.clear();
				mOtherDisjList.clear();
				for (CSortedNegLinker<CConcept*>* opConceptIt = concept->getOperandList(); opConceptIt; opConceptIt = opConceptIt->getNext()) {
					CSortedNegLinker<CConcept*>* opConcept = opConceptIt;
					addDisjunctToSort(opConcept,negated);
					++mSortedDisjunctCount;
				}

				CSortedNegLinker<CConcept*>* newOperandLinker = nullptr;
				CSortedNegLinker<CConcept*>* lastNewOperandLinker = nullptr;
				addDisjunctFromList(mAtomicDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mPrimNegDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mSimpleExistDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mNegNominalDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mImplDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mTriggerDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mPropDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mOtherDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mExistDisjList,newOperandLinker,lastNewOperandLinker);
				addDisjunctFromList(mPosNominalDisjList,newOperandLinker,lastNewOperandLinker);
				concept->setOperandList(newOperandLinker);
			}


			void CDisjunctSortingPreProcess::addDisjunctFromList(QList< CSortedNegLinker<CConcept*>* >& disjunctList, CSortedNegLinker<CConcept*>*& newOperandLinker, CSortedNegLinker<CConcept*>*& lastNewOperandLinker) {
				for (QList< CSortedNegLinker<CConcept*>* >::const_iterator it = disjunctList.constBegin(), itEnd = disjunctList.constEnd(); it != itEnd; ++it) {
					CSortedNegLinker<CConcept*>* opConLinker(*it);
					opConLinker->setNext(nullptr);
					if (!newOperandLinker) {
						newOperandLinker = opConLinker;
					} else {
						lastNewOperandLinker->setNext(opConLinker);
					}
					lastNewOperandLinker = opConLinker;
				}
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

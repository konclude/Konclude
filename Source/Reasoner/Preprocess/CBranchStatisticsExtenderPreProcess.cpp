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

#include "CBranchStatisticsExtenderPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CBranchStatisticsExtenderPreProcess::CBranchStatisticsExtenderPreProcess() {
				mOnto = nullptr;
			}


			CBranchStatisticsExtenderPreProcess::~CBranchStatisticsExtenderPreProcess() {
			}


			CConcreteOntology *CBranchStatisticsExtenderPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				mOnto = nullptr;
				mLastConceptId = 0;

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.BranchingStatisticsExtender.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				if (nonELConstructsUsed || !skipForELOntologies) {

					mTBox = ontology->getDataBoxes()->getTBox();

					mConceptVec = mTBox->getConceptVector();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOnto = ontology;

					mExtendedDisjunctionsBranchingStatistics = 0;
					mExtendedDisjunctsBranchingStatistics = 0;

					extendDisjunctions();


					LOG(INFO,"::Konclude::Reasoner::Preprocess::BranchStatisticsExtender",logTr("Extended %1 potential disjunctions with branching statistics for %2 disjuncts.").arg(mExtendedDisjunctionsBranchingStatistics).arg(mExtendedDisjunctsBranchingStatistics),this);
				}
				return ontology;
			}


			CConcreteOntology* CBranchStatisticsExtenderPreProcess::continuePreprocessing() {
				if (mOnto) {
					extendDisjunctions();
				}
				return mOnto;
			}

			CBranchStatisticsExtenderPreProcess* CBranchStatisticsExtenderPreProcess::extendDisjunctions() {
				cint64 conCount = mTBox->getConceptCount();
				for (cint64 conIdx = mLastConceptId; conIdx < conCount; ++conIdx) {
					CConcept* concept = mConceptVec->getLocalData(conIdx);
					if (concept) {
						cint64 conOpCode = concept->getOperatorCode();
						cint64 opCount = concept->getOperandCount();
						if (opCount > 1) {
							bool extendBranchStats = false;
							bool disjunctionNegation = false;
							if (conOpCode == CCAND || conOpCode == CCEQ) {
								extendBranchStats = true;
								disjunctionNegation = true;
							} else if (conOpCode == CCOR) {
								extendBranchStats = true;
							}
							if (extendBranchStats) {
								++mExtendedDisjunctionsBranchingStatistics;
								extendDisjunction(concept, disjunctionNegation);
							}
						}
					}
				}
				mLastConceptId = conCount;
				return this;
			}


			CBranchStatisticsExtenderPreProcess* CBranchStatisticsExtenderPreProcess::extendDisjunction(CConcept* concept, bool disjunctionNegation) {
				CConceptProcessData* conData = (CConceptProcessData*)concept->getConceptData();
				if (conData) {
					if (!conData->getBranchingStatistics()) {
						CDisjunctionBranchingStatistics* disjunctionBranchingStats = CObjectAllocator<CDisjunctionBranchingStatistics>::allocateAndConstruct(mMemMan);
						disjunctionBranchingStats->initDisjunctionBranchingStatistics();
						CDisjunctBranchingStatistics* firstDisjunctBranchStats = nullptr;
						CDisjunctBranchingStatistics* lastDisjunctBranchStats = nullptr;
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							CDisjunctBranchingStatistics* disjunctBranchingStats = CObjectAllocator<CDisjunctBranchingStatistics>::allocateAndConstruct(mMemMan);
							disjunctBranchingStats->initDisjunctBranchingStatistics(opConLinkerIt);
							if (!firstDisjunctBranchStats) {
								firstDisjunctBranchStats = disjunctBranchingStats;
							}
							if (lastDisjunctBranchStats) {
								lastDisjunctBranchStats->setNext(disjunctBranchingStats);
							}
							lastDisjunctBranchStats = disjunctBranchingStats;
						}
						disjunctionBranchingStats->setDisjunctBranchingStatistics(firstDisjunctBranchStats);
						conData->setBranchingStatistics(disjunctionBranchingStats);
					}
				}
				return this;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

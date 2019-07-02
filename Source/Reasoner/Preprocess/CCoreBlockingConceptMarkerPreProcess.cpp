/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CCoreBlockingConceptMarkerPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CCoreBlockingConceptMarkerPreProcess::CCoreBlockingConceptMarkerPreProcess() {
			}


			CCoreBlockingConceptMarkerPreProcess::~CCoreBlockingConceptMarkerPreProcess() {
			}



			CConcreteOntology* CCoreBlockingConceptMarkerPreProcess::preprocess(CConcreteOntology* ontology, CPreProcessContext* context) {
				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.CoreBlockingConceptMarking.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				if (nonELConstructsUsed || !skipForELOntologies) {

					CTBox* tbox = ontology->getDataBoxes()->getTBox();
					CABox* abox = ontology->getDataBoxes()->getABox();
					CRBox* rbox = ontology->getDataBoxes()->getRBox();

					CConceptVector* conVec = tbox->getConceptVector();
					qint64 itemConCounts = conVec->getItemCount();

					QSet<TConceptNegPair> visitedConceptSet;
					COntologyContext* ontoContext = ontology->getOntologyContext();
					CMemoryAllocationManager* mMemMan = ontoContext->getMemoryAllocationManager();
					COntologyCoreConceptCyclesData* coreConCyclesData = ontology->getCoreConceptCyclesData();

					cint64 statMarkedCoreBlockingConceptsCount = 0;

					cint64 nextCycleID = 0;

					for (qint64 i = 0; i < itemConCounts; ++i) {
						CConcept* concept = conVec->getLocalData(i);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							if (opCode == CCSOME || opCode == CCALL || opCode == CCATLEAST || opCode == CCAQSOME || opCode == CCATMOST) {
								bool opNegated = opCode == CCALL;

								CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
								while (opLinkerIt) {

									CConcept* opConcept = concept;
									bool opNegate = opNegated^opLinkerIt->isNegated();
									CConceptProcessData* conProDes = (CConceptProcessData*)opConcept->getConceptData();

									if (conProDes) {
										bool negated = opNegated^opNegate;
										if (!conProDes->isCoreBlockingConcept(negated)) {
											++statMarkedCoreBlockingConceptsCount;
											conProDes->setCoreBlockingConcept(negated);
										}
									}

									opLinkerIt = opLinkerIt->getNext();
								}
							}
						}
					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::CoreBlockingConceptMarker",logTr("Marked %1 concept as core blocking concepts.").arg(statMarkedCoreBlockingConceptsCount),this);
				}
				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

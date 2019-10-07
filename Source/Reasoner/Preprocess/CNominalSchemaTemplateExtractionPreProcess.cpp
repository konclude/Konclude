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

#include "CNominalSchemaTemplateExtractionPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {



			CNominalSchemaTemplateExtractionPreProcess::CNominalSchemaTemplateExtractionPreProcess() {
			}


			CNominalSchemaTemplateExtractionPreProcess::~CNominalSchemaTemplateExtractionPreProcess() {
			}


			CConcreteOntology *CNominalSchemaTemplateExtractionPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				
				bool nominalSchemaConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNominalSchemaUsed();
				if (nominalSchemaConstructsUsed) {

					mStatCreatedTemplateCount = 0;
					mStatReplacedConceptCount = 0;
					mNomSchTemplVec = nullptr;

					mTBox = ontology->getDataBoxes()->getTBox();
					CRBox *rBox = ontology->getDataBoxes()->getRBox();
					CABox *aBox = ontology->getDataBoxes()->getABox();
					mMBox = ontology->getDataBoxes()->getMBox();

					mConceptVec = mTBox->getConceptVector();
					mConceptCount = mConceptVec->getItemCount();

					mRolesVec = rBox->getRoleVector();
					mRoleCount = mRolesVec->getItemCount();

					mTopConcept = mTBox->getTopConcept();
					mBottomConcept = mTBox->getBottomConcept();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOnto = ontology;

					mIndividualVector = aBox->getIndividualVector(false);
					mIndividualCount = 0;
					if (mIndividualVector) {
						mIndividualCount = mIndividualVector->getItemCount();
					}

					bool nominalSchemaConcepts = false;
					for (cint64 i = 0; !nominalSchemaConcepts && i < mConceptCount; ++i) {
						CConcept* concept = mConceptVec->getData(i);
						if (concept) {
							cint64 conOpCode = concept->getOperatorCode();
							if (conOpCode == CCNOMVAR) {
								nominalSchemaConcepts = true;
							}
						}
					}


					if (nominalSchemaConcepts) {
						for (cint64 i = 0; i < mConceptCount; ++i) {
							CConcept* concept = mConceptVec->getLocalData(i);
							if (concept) {
								cint64 conOpCode = concept->getOperatorCode();
								if (conOpCode == CCEQ) {

									bool foundNominalSchemaConcept = false;
									for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && !foundNominalSchemaConcept; opConLinkerIt = opConLinkerIt->getNext()) {
										CConcept* opConcept = opConLinkerIt->getData();
										bool opConNegation = opConLinkerIt->isNegated();

										foundNominalSchemaConcept = hasNominalSchemaConcept(opConcept);
									}

									if (foundNominalSchemaConcept) {
										transformEquivalentConceptToGCI(concept);
									}
								}
							}
						}

						for (cint64 i = 0; i < mConceptCount; ++i) {
							CConcept* concept = mConceptVec->getLocalData(i);
							if (concept) {
								cint64 conOpCode = concept->getOperatorCode();
								if (conOpCode == CCSUB) {

									for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
										CConcept* opConcept = opConLinkerIt->getData();
										bool opConNegation = opConLinkerIt->isNegated();

										if (hasNominalSchemaConcept(opConcept)) {
											++mStatReplacedConceptCount;
											CNominalSchemaTemplate* nomSchTempl = getNominalSchemaTemplate(opConcept,opConNegation);
											CConcept* templRefConcept = nomSchTempl->getReferenceConcept();
											opConLinkerIt->setData(templRefConcept);
										}
									}

								}
							}
						}


						for (cint64 i = 0; i < mRoleCount; ++i) {
							CRole* role = mRolesVec->getLocalData(i);
							if (role) {
								for (CSortedNegLinker<CConcept*>* domConLinkerIt = role->getDomainConceptList(); domConLinkerIt; domConLinkerIt = domConLinkerIt->getNext()) {
									CConcept* opConcept = domConLinkerIt->getData();
									bool opConNegation = domConLinkerIt->isNegated();

									if (hasNominalSchemaConcept(opConcept)) {
										++mStatReplacedConceptCount;
										CNominalSchemaTemplate* nomSchTempl = getNominalSchemaTemplate(opConcept,opConNegation);
										CConcept* templRefConcept = nomSchTempl->getReferenceConcept();
										domConLinkerIt->setData(templRefConcept);
									}
								}
								for (CSortedNegLinker<CConcept*>* rangConLinkerIt = role->getRangeConceptList(); rangConLinkerIt; rangConLinkerIt = rangConLinkerIt->getNext()) {
									CConcept* opConcept = rangConLinkerIt->getData();
									bool opConNegation = rangConLinkerIt->isNegated();

									if (hasNominalSchemaConcept(opConcept)) {
										++mStatReplacedConceptCount;
										CNominalSchemaTemplate* nomSchTempl = getNominalSchemaTemplate(opConcept,opConNegation);
										CConcept* templRefConcept = nomSchTempl->getReferenceConcept();
										rangConLinkerIt->setData(templRefConcept);
									}
								}

							}
						}

						for (CSortedNegLinker<CConcept*>* gciConLinkerIt = mTopConcept->getOperandList(); gciConLinkerIt; gciConLinkerIt = gciConLinkerIt->getNext()) {
							CConcept* opConcept = gciConLinkerIt->getData();
							bool opConNegation = gciConLinkerIt->isNegated();

							if (hasNominalSchemaConcept(opConcept)) {
								if (!removeSingleAbsorbableNominalSchemaConcepts(opConcept,opConNegation)) {
									++mStatReplacedConceptCount;
									CNominalSchemaTemplate* nomSchTempl = getNominalSchemaTemplate(opConcept,opConNegation);
									CConcept* templRefConcept = nomSchTempl->getReferenceConcept();
									gciConLinkerIt->setData(templRefConcept);
								}
							}
						}


						if (mIndividualVector) {
							for (cint64 i = 0; i < mIndividualCount; ++i) {
								CIndividual* individual = mIndividualVector->getLocalData(i);
								if (individual) {

									for (CConceptAssertionLinker* assConLinkerIt = individual->getAssertionConceptLinker(); assConLinkerIt; assConLinkerIt = assConLinkerIt->getNext()) {
										CConcept* assCon = assConLinkerIt->getData();
										bool assConNegation = assConLinkerIt->isNegated();
										if (hasNominalSchemaConcept(assCon)) {
											++mStatReplacedConceptCount;
											CNominalSchemaTemplate* nomSchTempl = getNominalSchemaTemplate(assCon,assConNegation);
											CConcept* templRefConcept = nomSchTempl->getReferenceConcept();
											assConLinkerIt->setData(templRefConcept);
										}
									}

								}
							}
						}
					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::NominalSchemaTemplateExtractor",logTr("Extracted %1 nominal schema templates, replaced %2 concepts.").arg(mStatCreatedTemplateCount).arg(mStatReplacedConceptCount),this);

				}
				return ontology;
			}




			bool CNominalSchemaTemplateExtractionPreProcess::removeSingleAbsorbableNominalSchemaConcepts(CConcept* concept, bool negation) {
				bool allRemoved = true;
				QHash<CConcept*,cint64> nominalSchemaConceptCountHash;
				countNominalSchemaConcepts(concept,&nominalSchemaConceptCountHash);
				QSet<CConcept*> absorbableNominalVariableSet;
				if (findAbsorbableNominalSchemas(concept,negation,&absorbableNominalVariableSet)) {
					QSet<CConcept*> removeNominalVariableSet;
					for (QHash<CConcept*,cint64>::const_iterator it = nominalSchemaConceptCountHash.constBegin(), itEnd = nominalSchemaConceptCountHash.constEnd(); it != itEnd; ++it) {
						cint64 nomSchCount = it.value();
						CConcept* concept = it.key();
						if (nomSchCount <= 1 && absorbableNominalVariableSet.contains(concept)) {
							removeNominalVariableSet.insert(concept);
						} else {
							allRemoved = false;
						}
					}
					if (!removeNominalVariableSet.isEmpty()) {
						CConcept* replaceConcept = mTBox->getIndividualTriggerConcept();
						replaceNominalSchemasConcepts(concept,replaceConcept,removeNominalVariableSet);
					}
				} else {
					allRemoved = false;
				}
				return allRemoved;
			}


			void CNominalSchemaTemplateExtractionPreProcess::replaceNominalSchemasConcepts(CConcept* concept, CConcept* replacingConcept, const QSet<CConcept*>& removeNominalVariableSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						if (removeNominalVariableSet.contains(opConcept)) {
							opConLinkerIt->setData(replacingConcept);
						} else {
							replaceNominalSchemasConcepts(opConcept,replacingConcept,removeNominalVariableSet);
						}
					}
				}
			}


			void CNominalSchemaTemplateExtractionPreProcess::countNominalSchemaConcepts(CConcept* concept, QHash<CConcept*,cint64>* nominalSchemaConceptCountHash) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
				} else if (conOpCode == CCNOMVAR) {
					cint64 prevCount = nominalSchemaConceptCountHash->value(concept,0);
					nominalSchemaConceptCountHash->insert(concept,prevCount+1);
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						countNominalSchemaConcepts(opConcept,nominalSchemaConceptCountHash);
					}
				}
			}



			bool CNominalSchemaTemplateExtractionPreProcess::findAbsorbableNominalSchemas(CConcept* concept, bool negation, QSet<CConcept*>* absorbableNominalVariableSet) {
				cint64 opCode = concept->getOperatorCode();
				cint64 param = concept->getParameter();
				if (negation && (opCode == CCSOME) || !negation && (opCode == CCALL || opCode == CCSOME)) {
					bool oneAbsorbable = false;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConNegation = opLinkerIt->isNegated() ^ negation;
						oneAbsorbable |= findAbsorbableNominalSchemas(opConcept,opConNegation,absorbableNominalVariableSet);
					}
					return oneAbsorbable;
				} else if (negation && (opCode == CCOR || opCode == CCAND) || !negation && (opCode == CCAND || opCode == CCOR)) {
					bool oneAbsorbable = true;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConNegation = opLinkerIt->isNegated() ^ negation;
						oneAbsorbable |= findAbsorbableNominalSchemas(opConcept,opConNegation,absorbableNominalVariableSet);
					}
					return oneAbsorbable;
				} else if (negation && (opCode == CCNOMVAR)) {
					absorbableNominalVariableSet->insert(concept);
					return true;
				}
				return false;
			}



			void CNominalSchemaTemplateExtractionPreProcess::addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				opConLinker->init(opConcept,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
			}


			CConcept* CNominalSchemaTemplateExtractionPreProcess::transformEquivalentConceptToGCI(CConcept* eqConcept) {
				eqConcept->setOperatorCode(CCSUB);
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCOR);
				mConceptVec->setData(concept->getConceptTag(),concept);
				addConceptOperand(eqConcept,concept,false);
				for (CSortedNegLinker<CConcept*>* opConLinkerIt = eqConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
					CConcept* opConcept = opConLinkerIt->getData();
					bool opConNegation = opConLinkerIt->isNegated();
					addConceptOperand(eqConcept,opConcept,!opConNegation);
				}
				mTopConcept = CConceptRoleIndividualLocator::getLocatedConcept(mTopConcept,mOnto);
				addConceptOperand(mTopConcept,concept,false);
				return concept;
			}

			CConcept* CNominalSchemaTemplateExtractionPreProcess::createNewConcept() {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mTBox->getNextConceptID();
				concept->initTag(nextConTag);
				return concept;
			}


			CConcept* CNominalSchemaTemplateExtractionPreProcess::createNominalSchemaTemplateReferenceConcept(CNominalSchemaTemplate* nsTemplate) {
				CConcept* concept = createNewConcept();
				concept->setOperatorCode(CCNOMTEMPLREF);
				concept->setParameter(nsTemplate->getNominalSchemaTemplateTag());
				mConceptVec->setData(concept->getConceptTag(),concept);
				nsTemplate->setReferenceConcept(concept);
				return concept;
			}


			CNominalSchemaTemplate* CNominalSchemaTemplateExtractionPreProcess::getNominalSchemaTemplate(CConcept* concept, bool negation) {
				CNominalSchemaTemplate* nsTemplate = mConceptNominalSchemaTemplateHash.value(concept);
				if (!nsTemplate) {
					++mStatCreatedTemplateCount;

					nsTemplate = CObjectAllocator< CNominalSchemaTemplate >::allocateAndConstruct(mMemMan);
					CBOXSET<CConcept*>* nominalSchemaConceptSet = CObjectAllocator< CBOXSET<CConcept*> >::allocateAndConstruct(mMemMan);
					CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash = CObjectAllocator< CBOXHASH<CConcept*,CConcept*> >::allocateAndConstruct(mMemMan);
					CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash = CObjectAllocator< CBOXHASH<CConcept*,CConcept*> >::allocateAndConstruct(mMemMan);
					nsTemplate->initNominalSchemaTemplate(nominalSchemaConceptSet,conceptNominalSchemaConceptHash,absorbableConceptNominalSchemaConceptHash);
					if (!mNomSchTemplVec) {
						mNomSchTemplVec = mMBox->getNominalSchemaTemplateVector();
					}
					cint64 nextNomSchTemplID = mNomSchTemplVec->getItemCount();
					nsTemplate->setNominalSchemaTemplateTag(nextNomSchTemplID);
					mNomSchTemplVec->setData(nextNomSchTemplID,nsTemplate);
					nsTemplate->setTemplateConcept(concept);
					collectNominalSchemaConcepts(concept,nominalSchemaConceptSet);
					createNominalSchemaTemplateReferenceConcept(nsTemplate);

					FOREACHIT (CConcept* nomSchConcept, *nominalSchemaConceptSet) {
						QSet<CConcept*> testedNomSchemConSet;
						hasNominalSchemaConceptFillHash(concept,conceptNominalSchemaConceptHash,nomSchConcept,&testedNomSchemConSet);
						QSet<CConcept*> testedNomSchemAbsorbedConSet;
						hasNominalSchemaAbsorbableConceptFillHash(concept,negation,absorbableConceptNominalSchemaConceptHash,nomSchConcept,&testedNomSchemAbsorbedConSet);
					}

					mConceptNominalSchemaTemplateHash.insert(concept,nsTemplate);
				}
				return nsTemplate;
			}


			bool CNominalSchemaTemplateExtractionPreProcess::hasNominalSchemaAbsorbableConceptFillHash(CConcept* concept, bool negation, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
					return false;
				}
				bool foundAbsorbableNominalSchemaConcept = false;

				if (negation && concept == nominalSchemaConcept) {
					foundAbsorbableNominalSchemaConcept = true;
				} else {
					cint64 conCode = concept->getOperatorCode();
					if (negation && (conCode == CCSOME) || !negation && (conCode == CCALL)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							foundAbsorbableNominalSchemaConcept |= hasNominalSchemaAbsorbableConceptFillHash(opConcept,negation^opConLinkerIt->isNegated(),absorbableConceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
						}
					} else if (!negation && (conCode == CCAND || conCode == CCOR) || negation && (conCode == CCOR || conCode == CCAND)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							foundAbsorbableNominalSchemaConcept |= hasNominalSchemaAbsorbableConceptFillHash(opConcept,negation^opConLinkerIt->isNegated(),absorbableConceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
						}
					}
				}

				if (foundAbsorbableNominalSchemaConcept) {
					if (!testedConceptSet->contains(concept)) {
						testedConceptSet->insert(concept);
						absorbableConceptNominalSchemaConceptHash->insertMulti(concept,nominalSchemaConcept);
					}
				}
				return foundAbsorbableNominalSchemaConcept;
			}


			bool CNominalSchemaTemplateExtractionPreProcess::hasNominalSchemaConceptFillHash(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
					return false;
				}
				bool foundNominalSchemaConcept = false;

				if (concept == nominalSchemaConcept) {
					foundNominalSchemaConcept = true;
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						foundNominalSchemaConcept |= hasNominalSchemaConceptFillHash(opConcept,conceptNominalSchemaConceptHash,nominalSchemaConcept,testedConceptSet);
					}
				}

				if (foundNominalSchemaConcept) {
					if (!testedConceptSet->contains(concept)) {
						testedConceptSet->insert(concept);
						conceptNominalSchemaConceptHash->insertMulti(concept,nominalSchemaConcept);
					}
				}
				return foundNominalSchemaConcept;
			}


			void CNominalSchemaTemplateExtractionPreProcess::collectNominalSchemaConcepts(CConcept* concept, CBOXSET<CConcept*>* nominalSchemaConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
				} else if (conOpCode == CCNOMVAR) {
					nominalSchemaConceptSet->insert(concept);
				} else {
					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						collectNominalSchemaConcepts(opConcept,nominalSchemaConceptSet);
					}
				}
			}


			bool CNominalSchemaTemplateExtractionPreProcess::hasNominalSchemaConcept(CConcept* concept) {
				QSet<CConcept*> testedConceptSet;
				return hasNominalSchemaConcept(concept,&testedConceptSet);
			}

			bool CNominalSchemaTemplateExtractionPreProcess::hasNominalSchemaConcept(CConcept* concept, QSet<CConcept*>* testedConceptSet) {
				cint64 conOpCode = concept->getOperatorCode();
				if ((conOpCode == CCSUB || conOpCode == CCEQ || conOpCode == CCTOP || conOpCode == CCBOTTOM)) {
					return false;
				} else if (conOpCode == CCNOMVAR) {
					return true;
				}

				if (mConceptNominalSchemaHash.contains(concept)) {
					return mConceptNominalSchemaHash.value(concept);
				}

				bool foundNominalSchemaConcept = false;
				for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && !foundNominalSchemaConcept; opConLinkerIt = opConLinkerIt->getNext()) {
					CConcept* opConcept = opConLinkerIt->getData();
					foundNominalSchemaConcept = hasNominalSchemaConcept(opConcept);
				}

				mConceptNominalSchemaHash.insert(concept,foundNominalSchemaConcept);
				return foundNominalSchemaConcept;
			}

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

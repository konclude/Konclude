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

#include "CFullNominalSchemaGroundingPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {



			CFullNominalSchemaGroundingPreProcess::CFullNominalSchemaGroundingPreProcess() {
				mConfReuseGroundedNominalSchemaConcepts = true;
				mConfGroundNominalSchemaAbsorbableConcepts = false;
				mConfFullyGroundNominalSchemaConcepts = false;
			}


			CFullNominalSchemaGroundingPreProcess::~CFullNominalSchemaGroundingPreProcess() {
			}


			CConcreteOntology *CFullNominalSchemaGroundingPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				bool nominalSchemaConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNominalSchemaUsed();
			
				if (nominalSchemaConstructsUsed) {
					mStatReplacedConceptCount = 0;
					mStatCreatedConceptCount = 0;
					mStatGroundedTemplateCount = 0;
					mStatInstantiatedAxiomsCount = 0;

					mTBox = ontology->getDataBoxes()->getTBox();
					CRBox *rBox = ontology->getDataBoxes()->getRBox();
					CABox *aBox = ontology->getDataBoxes()->getABox();
					mMBox = ontology->getDataBoxes()->getMBox();

					CConfiguration* config = context->getConfiguration();
					mConfGroundNominalSchemaAbsorbableConcepts = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.AbsorbableNominalSchemaGrounding",false);
					mConfFullyGroundNominalSchemaConcepts = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Preprocessing.FullNominalSchemaGrounding",false);
					mConfGroundNominalSchemaAbsorbableConcepts |= mConfFullyGroundNominalSchemaConcepts;

					mConceptVec = mTBox->getConceptVector();
					mConceptCount = mConceptVec->getItemCount();

					mRolesVec = rBox->getRoleVector();
					mRoleCount = mRolesVec->getItemCount();

					mTopConcept = mTBox->getTopConcept();
					mBottomConcept = mTBox->getBottomConcept();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOnto = ontology;

					mNomSchTemplVec = mMBox->getNominalSchemaTemplateVector(false);

					mIndividualVector = aBox->getIndividualVector(false);
					mIndividualCount = 0;
					if (mIndividualVector) {
						mIndividualCount = mIndividualVector->getItemCount();
						// TODO: only active individuals
						for (cint64 i = 0; i < mIndividualCount; ++i) {
							CIndividual* individual = mIndividualVector->getData(i);
							for (CConceptAssertionLinker* assConLinkerIt = individual->getAssertionConceptLinker(); assConLinkerIt; assConLinkerIt = assConLinkerIt->getNext()) {
								CConcept* assCon = assConLinkerIt->getData();
								bool assConNegation = assConLinkerIt->isNegated();
								cint64 assConOpCode = assCon->getOperatorCode();
								if (!assConNegation && assConOpCode == CCNOMINAL) {
									mNominalConceptSet.insert(assCon);
								}
							}
						}
					}

					if (mNomSchTemplVec) {
						for (cint64 i = 0; i < mConceptCount; ++i) {
							CConcept* concept = mConceptVec->getLocalData(i);
							if (concept) {
								cint64 conOpCode = concept->getOperatorCode();
								if (conOpCode == CCSUB) {

									for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
										CConcept* opConcept = opConLinkerIt->getData();
										bool opConNegation = opConLinkerIt->isNegated();

										cint64 opConOpCode = opConcept->getOperatorCode();
										if (opConOpCode == CCNOMTEMPLREF) {
											cint64 templNumber = opConcept->getParameter();
											CNominalSchemaTemplate* nsTemplate = mNomSchTemplVec->getData(templNumber);
											if (nsTemplate) {
												CConcept* nomSchAxiomCollConcept = createNominalSchemaAxiomCollectionConcept(opConcept,opConNegation,nsTemplate);
												opConLinkerIt->setData(nomSchAxiomCollConcept);
												opConLinkerIt->setNegated(false);
											}
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

									cint64 opConOpCode = opConcept->getOperatorCode();
									if (opConOpCode == CCNOMTEMPLREF) {
										cint64 templNumber = opConcept->getParameter();
										CNominalSchemaTemplate* nsTemplate = mNomSchTemplVec->getData(templNumber);
										if (nsTemplate) {
											CConcept* nomSchAxiomCollConcept = createNominalSchemaAxiomCollectionConcept(opConcept,opConNegation,nsTemplate);
											domConLinkerIt->setData(nomSchAxiomCollConcept);
											domConLinkerIt->setNegated(false);
										}
									}
								}
								for (CSortedNegLinker<CConcept*>* rangConLinkerIt = role->getRangeConceptList(); rangConLinkerIt; rangConLinkerIt = rangConLinkerIt->getNext()) {
									CConcept* opConcept = rangConLinkerIt->getData();
									bool opConNegation = rangConLinkerIt->isNegated();

									cint64 opConOpCode = opConcept->getOperatorCode();
									if (opConOpCode == CCNOMTEMPLREF) {
										cint64 templNumber = opConcept->getParameter();
										CNominalSchemaTemplate* nsTemplate = mNomSchTemplVec->getData(templNumber);
										if (nsTemplate) {
											CConcept* nomSchAxiomCollConcept = createNominalSchemaAxiomCollectionConcept(opConcept,opConNegation,nsTemplate);
											rangConLinkerIt->setData(nomSchAxiomCollConcept);
											rangConLinkerIt->setNegated(false);
										}
									}
								}
							}
						}


						for (CSortedNegLinker<CConcept*>* gciConLinkerIt = mTopConcept->getOperandList(); gciConLinkerIt; gciConLinkerIt = gciConLinkerIt->getNext()) {
							CConcept* opConcept = gciConLinkerIt->getData();
							bool opConNegation = gciConLinkerIt->isNegated();

							cint64 opConOpCode = opConcept->getOperatorCode();
							if (opConOpCode == CCNOMTEMPLREF) {
								cint64 templNumber = opConcept->getParameter();
								CNominalSchemaTemplate* nsTemplate = mNomSchTemplVec->getData(templNumber);
								if (nsTemplate && requiresGCIGrounding(nsTemplate,opConNegation)) {
									CConcept* nomSchAxiomCollConcept = createNominalSchemaAxiomCollectionConcept(opConcept,opConNegation,nsTemplate);
									gciConLinkerIt->setData(nomSchAxiomCollConcept);
									gciConLinkerIt->setNegated(false);
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

										cint64 opConOpCode = assCon->getOperatorCode();
										if (opConOpCode == CCNOMTEMPLREF) {
											cint64 templNumber = assCon->getParameter();
											CNominalSchemaTemplate* nsTemplate = mNomSchTemplVec->getData(templNumber);
											if (nsTemplate) {
												CConcept* nomSchAxiomCollConcept = createNominalSchemaAxiomCollectionConcept(assCon,assConNegation,nsTemplate);
												assConLinkerIt->setData(nomSchAxiomCollConcept);
												assConLinkerIt->setNegation(false);
											}
										}
									}

								}
							}
						}


					}

					LOG(INFO,"::Konclude::Reasoner::Preprocess::FullNominalSchemaGrounder",logTr("Grounded %1 nominal schema templates, added %2 axioms with %3 created concepts.").arg(mStatGroundedTemplateCount).arg(mStatInstantiatedAxiomsCount).arg(mStatCreatedConceptCount),this);
				}
				return ontology;
			}


			bool CFullNominalSchemaGroundingPreProcess::requiresGCIGrounding(CNominalSchemaTemplate* nsTemplate, bool negated) {
				CConcept* templConcept = nsTemplate->getTemplateConcept();

				if (mReqGroundConHash.contains(QPair<CConcept*,bool>(templConcept,negated))) {
					return mReqGroundConHash.value(QPair<CConcept*,bool>(templConcept,negated));
				}

				bool requiresGrounding = true;

				if (!mConfGroundNominalSchemaAbsorbableConcepts) {
					cint64 opCode = templConcept->getOperatorCode();
					if (!negated && opCode == CCOR || negated && opCode == CCAND) {
						QSet<CConcept*> absorbableNominalVariableSet;
						for (CSortedNegLinker<CConcept*>* opLinkerIt = templConcept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opConNegation = opLinkerIt->isNegated() ^ negated;
							findAbsorbableNominalSchemas(opConcept,opConNegation,&absorbableNominalVariableSet);
						}
						bool absorbable = absorbableNominalVariableSet.count() > 0;
						requiresGrounding = !absorbable;
					}
				}

				mReqGroundConHash.insert(QPair<CConcept*,bool>(templConcept,negated),requiresGrounding);
				return requiresGrounding;
			}

			bool CFullNominalSchemaGroundingPreProcess::findAbsorbableNominalSchemas(CConcept* concept, bool negation, QSet<CConcept*>* absorbableNominalVariableSet) {
				cint64 opCode = concept->getOperatorCode();
				cint64 param = concept->getParameter();
				if (negation && (opCode == CCSOME || opCode == CCAND) || !negation && (opCode == CCALL || opCode == CCSOME)) {
					bool oneAbsorbable = false;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConNegation = opLinkerIt->isNegated() ^ negation;
						oneAbsorbable |= findAbsorbableNominalSchemas(opConcept,opConNegation,absorbableNominalVariableSet);
					}
					return oneAbsorbable;
				} else if (negation && (opCode == CCOR) || !negation && (opCode == CCAND)) {
					QSet<CConcept*> allAbsorbableNominalVariableSet;
					bool allAbsorbableNominalVariableSetInitialized = false;
					bool allAbsorbable = true;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConNegation = opLinkerIt->isNegated() ^ negation;
						QSet<CConcept*> tmpAbsorbableNominalVariableSet;
						allAbsorbable &= findAbsorbableNominalSchemas(opConcept,opConNegation,&tmpAbsorbableNominalVariableSet);
						if (!allAbsorbableNominalVariableSetInitialized) {
							allAbsorbableNominalVariableSetInitialized = true;
							allAbsorbableNominalVariableSet = tmpAbsorbableNominalVariableSet;
						} else {
							QSet<CConcept*>::iterator it = allAbsorbableNominalVariableSet.begin();
							while (it != allAbsorbableNominalVariableSet.end()) {
								if (tmpAbsorbableNominalVariableSet.contains(*it)) {
									++it;
								} else {
									it = allAbsorbableNominalVariableSet.erase(it);
								}
							}
						}
					}
					*absorbableNominalVariableSet += allAbsorbableNominalVariableSet;
					return allAbsorbable;
				} else if (negation && (opCode == CCATLEAST && param > 0) || !negation && (opCode == CCATMOST && param >= 0)) {
					bool oneAbsorbable = false;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConNegation = opLinkerIt->isNegated();
						oneAbsorbable |= findAbsorbableNominalSchemas(opConcept,opConNegation,absorbableNominalVariableSet);
					}
					return oneAbsorbable;
				} else if (negation && (opCode == CCNOMVAR)) {
					absorbableNominalVariableSet->insert(concept);
					return true;
				}
				return false;
			}

			void CFullNominalSchemaGroundingPreProcess::addConceptOperand(CConcept* concept, CConcept* opConcept, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				opConLinker->init(opConcept,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
			}


			CConcept* CFullNominalSchemaGroundingPreProcess::createNominalSchemaConceptCopy(CConcept* concept) {
				CConcept* conceptCopy = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				conceptCopy->initConceptCopy(concept);
				conceptCopy->setOperandCount(0);
				conceptCopy->setConceptTag(mTBox->getNextConceptID());
				mConceptVec->setData(conceptCopy->getConceptTag(),conceptCopy);
				++mStatCreatedConceptCount;
				return conceptCopy;
			}


			CConcept* CFullNominalSchemaGroundingPreProcess::createGroundedNominalSchemaConcept(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash, QHash<CConcept*, QSet<CConcept*>::const_iterator >* nomSchConIndividualItHash) {
				
				if (templConNomSchConHash->contains(concept)) {

					cint64 opCode = concept->getOperatorCode();
					if (opCode == CCNOMVAR) {
						QSet<CConcept*>::const_iterator nomVarIt = nomSchConIndividualItHash->value(concept);
						CConcept* nominalConcept = *nomVarIt;
						return nominalConcept;
					}

					QByteArray conceptNomSchemIndiConceptByteArray;
					if (mConfReuseGroundedNominalSchemaConcepts) {
						conceptNomSchemIndiConceptByteArray.append(QByteArray::number((cint64)concept));
						FOREACHIT (CConcept* nomSchemConcept, templConNomSchConHash->values(concept)) {
							QSet<CConcept*>::const_iterator nomVarIt = nomSchConIndividualItHash->value(nomSchemConcept);
							CConcept* indiConcept = *nomVarIt;
							conceptNomSchemIndiConceptByteArray.append(QByteArray::number((cint64)indiConcept));
						}
						CConcept* replaceCon = mConceptNomSchemIndiReplaceHash.value(conceptNomSchemIndiConceptByteArray,nullptr);
						if (replaceCon) {
							return replaceCon;
						}
					}

					CConcept* copiedConcept = createNominalSchemaConceptCopy(concept);

					for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
						CConcept* opConcept = opConLinkerIt->getData();
						bool opConNegation = opConLinkerIt->isNegated();

						CConcept* newOpConcept = createGroundedNominalSchemaConcept(opConcept,templConNomSchConHash,nomSchConIndividualItHash);
						addConceptOperand(copiedConcept,newOpConcept,opConNegation);
					}

					if (mConfReuseGroundedNominalSchemaConcepts) {
						mConceptNomSchemIndiReplaceHash.insert(conceptNomSchemIndiConceptByteArray,copiedConcept);
					}

					return copiedConcept;
				} else {
					return concept;
				}

			}


			CConcept* CFullNominalSchemaGroundingPreProcess::createNominalSchemaAxiomCollectionConcept(CConcept* concept, bool negated, CNominalSchemaTemplate* nsTemplate) {
				++mStatReplacedConceptCount;
				if (mCollConHash.contains(QPair<CConcept*,bool>(concept,negated))) {
					return mCollConHash.value(QPair<CConcept*,bool>(concept,negated));
				}

				++mStatGroundedTemplateCount;

				CConcept* collConcept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				collConcept->initConcept();
				collConcept->setConceptTag(mTBox->getNextConceptID());
				collConcept->setOperatorCode(CCAND);
				mConceptVec->setData(collConcept->getConceptTag(),collConcept);


				CBOXHASH<CConcept*,CConcept*>* templConNomSchConHash = nsTemplate->getTemplateConceptNominalSchemaConceptHash();
				CBOXSET<CConcept*>* nomSchConSet = nsTemplate->getNominalSchemaConceptSet();

				QHash<CConcept*, QSet<CConcept*>::const_iterator > nomSchConIndividualItHash;
				QSet<CConcept*>::const_iterator indiIt = mNominalConceptSet.constBegin();
				QSet<CConcept*>::const_iterator indiItEnd = mNominalConceptSet.constEnd();
				FOREACHIT (CConcept* nomSchCon, *nomSchConSet) {
					nomSchConIndividualItHash.insert(nomSchCon,indiIt);
				}



				bool conVarItFinished = false;
				while (!conVarItFinished) {
					++mStatInstantiatedAxiomsCount;


					CConcept* templateConcept = nsTemplate->getTemplateConcept();
					CConcept* groundedConcept = createGroundedNominalSchemaConcept(templateConcept,templConNomSchConHash,&nomSchConIndividualItHash);
					addConceptOperand(collConcept,groundedConcept,negated);





					QHash<CConcept*, QSet<CConcept*>::const_iterator >::const_iterator variableIt = nomSchConIndividualItHash.constBegin();
					QHash<CConcept*, QSet<CConcept*>::const_iterator >::const_iterator variableItEnd = nomSchConIndividualItHash.constEnd();


					QSet<CConcept*>::const_iterator varIndiIt = variableIt.value();
					CConcept* varConcept = variableIt.key();

					++varIndiIt;
					bool conVarIteration = true;
					while (varIndiIt == indiItEnd && conVarIteration) {
						conVarIteration = false;
						nomSchConIndividualItHash.insert(varConcept,indiIt);

						++variableIt;
						if (variableIt == variableItEnd) {
							conVarItFinished = true;
						} else {
							varIndiIt = variableIt.value();
							varConcept = variableIt.key();
							++varIndiIt;

							if (varIndiIt == indiItEnd) {
								conVarIteration = true;
							}
						}
					} 
					nomSchConIndividualItHash.insert(varConcept,varIndiIt);
				}


				mCollConHash.insert(QPair<CConcept*,bool>(concept,negated),collConcept);
				return collConcept;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

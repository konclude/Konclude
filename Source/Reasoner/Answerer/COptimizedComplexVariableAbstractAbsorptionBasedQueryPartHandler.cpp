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

#include "COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem) {
				mQueryPart = queryPart;
				mOntoAnsweringItem = ontoAnsweringItem;

				mMemMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
				mTBox = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox();
				mConceptVec = mTBox->getConceptVector();
			}



			COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler* COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::readConfig(CCalculationConfigurationExtension* config) {
				return this;
			}




			bool COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::checkHasTopObjectPropertyPropagation() {
				CObjectPropertyTermExpression* topObjPropExp = mOntoAnsweringItem->getTestingOntology()->getBuildData()->getTopObjectPropertyExpression();
				QSet<CExpressionVariable*>* absorbtionVariableSet = mQueryPart->getAbsorptionVariableSet();
				for (CExpressionVariable* var : absorbtionVariableSet->toList()) {
					QList<CObjectPropertyAssertionExpression*> neighPropAssList = mQueryPart->getNeighbouringPropertyAssertions(var);
					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(var);
					for (CObjectPropertyAssertionExpression* propAss : neighPropAssList) {
						if (propAss->getObjectPropertyTermExpression() == topObjPropExp) {
							return true;
						}
					}
					for (CObjectPropertyAssertionExpression* propAss : selfPropAssList) {
						if (propAss->getObjectPropertyTermExpression() == topObjPropExp) {
							return true;
						}
					}
					QList<CBuildExpression*> classExpressions = mQueryPart->getVariableClassExpressions(var);
					if (classExpressions.size() > 0) {
						for (CBuildExpression* buildExpression : classExpressions) {
							CClassTermExpression* classExpression = (CClassTermExpression*)buildExpression;
							CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExpression);
							bool testingNegation = testingConcept->hasMappingNegation();
							if (checkHasNewTopRolePropagation(testingConcept, testingNegation)) {
								return true;
							}
						}
						
					}

				}
				return false;
			}


			bool COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::checkHasNewTopRolePropagation(CConcept* concept, bool conceptNegation) {
				CRole* topRole = mOntoAnsweringItem->getTestingOntology()->getRBox()->getTopObjectRole();
				QList<CConcept*> testingList;
				QSet<CConcept*> testedSet;
				testingList.append(concept);
				testedSet.insert(concept);
				while (!testingList.isEmpty()) {
					CConcept* testingConcept = testingList.takeFirst();
					cint64 opCode = testingConcept->getOperatorCode();

					CConceptOperator* conceptOperator = testingConcept->getConceptOperator();
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE | CConceptOperator::CCFS_SOME_TYPE)) {
						CRole* role = testingConcept->getRole();
						if (role == topRole) {
							return true;
						}
					}

					if (opCode != CCEQ && opCode != CCSUB) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = testingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							if (!testedSet.contains(opConcept)) {
								testedSet.insert(opConcept);
								testingList.append(opConcept);
							}
						}
					}

				}
				return false;
			}









			CRole* COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::getInverseRole(CRole* role) {
				return role->getInverseRole();
			}


			void COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::addConceptOperand(CConcept* concept, CConcept* operandConcept, bool operandNegation) {
				CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon->init(operandConcept, operandNegation, concept->getOperandList());
				concept->setOperandList(newBaseOpCon);
				concept->incOperandCount();
			}




		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

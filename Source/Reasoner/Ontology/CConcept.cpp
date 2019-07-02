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

#include "CConcept.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConcept::CConcept() {
				initConcept();
			}



			CConcept* CConcept::initConcept() {
				mTag = 0;
				mOperatorCode = CConceptOperator::getConceptOperator(CCNONE);
				operandCount = 0;
				operands = 0;
				opParameter = 0;
				operatorRole = 0;
				tax = 0;
				mNameLinker = 0;
				mNominalIndiviual = nullptr;
				mConceptData = nullptr;
				mVariableLinker = nullptr;
				mMappingNegated = false;
				mDataLiteral = nullptr;
				mDatatype = nullptr;
				return this;
			}



			CConcept *CConcept::init(CConcept *concept) {
				mNameLinker = concept->getClassNameLinker();
				mTag = concept->mTag;
				mOperatorCode = concept->mOperatorCode;
				operandCount = concept->operandCount;
				operands = 0;
				opParameter =  concept->opParameter;
				operatorRole =  concept->operatorRole;
				tax =  0;
				mNominalIndiviual = concept->mNominalIndiviual;
				mVariableLinker = concept->mVariableLinker;
				mMappingNegated = concept->mMappingNegated;
				mConceptData = nullptr;
				mDataLiteral = concept->mDataLiteral;
				mDatatype = concept->mDatatype;
				return this;
			}



			CConcept *CConcept::init(qint64 conTag) {
				initTag(conTag);
				mTag = conTag;
				mOperatorCode = CConceptOperator::getConceptOperator(CCNONE);
				operandCount = 0;
				operands = 0;
				opParameter = 0;
				operatorRole = 0;
				tax = 0;
				mNominalIndiviual = nullptr;
				mConceptData = nullptr;
				mVariableLinker = nullptr;
				mMappingNegated = false;
				mDataLiteral = nullptr;
				mDatatype = nullptr;
				return this;
			}

			CConcept *CConcept::initConceptCopy(CConcept *concept, CMemoryAllocationManager* memMan) {
				mNameLinker = concept->getClassNameLinker();
				mTag = concept->mTag;
				mOperatorCode = concept->mOperatorCode;
				operandCount = concept->operandCount;
				opParameter =  concept->opParameter;
				operatorRole =  concept->operatorRole;
				tax =  0;
				operands = 0;
				mNominalIndiviual = concept->mNominalIndiviual;
				mMappingNegated = concept->mMappingNegated;
				mDataLiteral = concept->mDataLiteral;
				mDatatype = concept->mDatatype;
				mVariableLinker = nullptr;
				mConceptData = nullptr;


				CSortedNegLinker<CConcept *> *opIt = concept->operands;
				while (memMan && opIt) {
					CConcept *con = opIt->getData();
					bool negated = opIt->isNegated();
					CSortedNegLinker<CConcept *> *opLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
					opLinker->init(con,negated);
					addOperandLinker(opLinker);
					opIt = (CSortedNegLinker<CConcept *> *)opIt->getNext();
				}

				CSortedLinker<CVariable*>* varIt = concept->mVariableLinker;
				while (memMan && varIt) {
					CVariable* var = varIt->getData();
					CSortedLinker<CVariable*>* varLinker = CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(memMan);
					varLinker->init(var);
					addVariableLinker(varLinker);
					varIt = (CSortedLinker<CVariable *> *)varIt->getNext();
				}

				return this;
			}


			CVariable* CConcept::getVariable() {
				if (mVariableLinker) {
					return mVariableLinker->getData();
				} 
				return nullptr;
			}

			CSortedLinker<CVariable*>* CConcept::getVariableLinker() {
				return mVariableLinker;
			}

			CConcept* CConcept::setVariableLinker(CSortedLinker<CVariable*>* variableLinker) {
				mVariableLinker = variableLinker;
				return this;
			}


			CConcept* CConcept::addVariableLinker(CSortedLinker<CVariable*>* variableLinker) {
				mVariableLinker = variableLinker->insertSortedNextSorted(mVariableLinker);
				return this;
			}


			CConcept *CConcept::setConceptTag(qint64 conTag) {
				mTag = conTag;
				return this;
			}

			qint64 CConcept::getConceptTag() const {
				return mTag;
			}


			CConcept *CConcept::setOperatorCode(qint64 opCode) {
				mOperatorCode = CConceptOperator::getConceptOperator(opCode);
				return this;
			}

			qint64 CConcept::getOperatorCode() {
				return mOperatorCode->getOperatorCode();
			}

			CConceptOperator* CConcept::getConceptOperator() {
				return mOperatorCode;
			}

			CConcept *CConcept::setOperatorTag(qint64 opTag) {
				mOperatorCode = CConceptOperator::getConceptOperator(opTag);
				return this;
			}

			qint64 CConcept::getDefinitionOperatorTag() {
				return getOperatorCode();
			}


			qint64 CConcept::getProcessingOperatorTag() {
				qint64 tag = getOperatorCode();
				if (tag == CCEQ || tag == CCSUB) {
					tag = CCAND;
				}
				return tag;
			}



			CConcept *CConcept::setTerminology(CTerminology *terminology) {
				tax = terminology;
				return this;
			}

			CTerminology *CConcept::getTerminology() {
				return tax;
			}

			qint64 CConcept::getTerminologyTag() const {
				qint64 taxTag = 0;
				if (tax) {
					taxTag = tax->getTerminologyID();
				}
				return taxTag;
			}

			QPair<qint64,qint64> CConcept::getTerminologyConceptTagPair() {
				return QPair<qint64,qint64>(getTerminologyTag(),mTag);
			}



			CConcept *CConcept::setParameter(qint64 parameter) {
				opParameter = parameter;
				return this;
			}

			qint64 CConcept::getParameter() {
				return opParameter;
			}


			CConcept *CConcept::setOperandCount(qint64 count) {
				operandCount = count;
				return this;
			}

			CConcept* CConcept::incOperandCount(cint64 incCount) {
				operandCount += incCount;
				return this;
			}


			qint64 CConcept::getOperandCount() {
				return operandCount;
			}


			CConcept *CConcept::setOperandList(CSortedNegLinker<CConcept *> *operandList) {
				operands = operandList;
				return this;
			}


			CConcept *CConcept::addOperandLinker(CSortedNegLinker<CConcept *> *operandLinker) {
				if (operands) {
					operands = operands->insertSortedNextSorted(operandLinker);
				} else {
					operands = operandLinker;
				}
				return this;
			}


			CSortedNegLinker<CConcept *> *CConcept::getOperandList() {
				return operands;
			}

			bool CConcept::operator<=(const CConcept &concept) {
				if (mTag < concept.mTag) {
					return true;
				} else if (mTag > concept.mTag) {
					return false;
				} else {
					return getTerminologyTag() <= concept.getTerminologyTag();
				}
			}

			bool CConcept::operator==(const CConcept &concept) {
				return mTag == concept.mTag && tax == concept.tax;
			}

			CConcept *CConcept::setRole(CRole *role) {
				operatorRole = role;
				return this;
			}

			CRole *CConcept::getRole() {
				return operatorRole;
			}

			CIndividual* CConcept::getNominalIndividual() {
				return mNominalIndiviual;
			}


			bool CConcept::hasNominalIndividual() {
				return mNominalIndiviual != nullptr;
			}



			CConcept* CConcept::setNominalIndividual(CIndividual* nominalIndividual) {
				mNominalIndiviual = nominalIndividual;
				return this;
			}


			bool CConcept::isGeneratingOperator(bool negated) {
				cint64 operatorTag = mOperatorCode->getOperatorCode();
				if (!negated) {
					return (operatorTag == CCSOME || operatorTag == CCATLEAST);
				} else {
					return (operatorTag == CCALL || operatorTag == CCATMOST);
				}
			}

			bool CConcept::isNegatedGeneratingOperator() {
				return isGeneratingOperator(true);
			}


			bool CConcept::isNonGeneratingOperator() {
				return !isGeneratingOperator();
			}


			bool CConcept::hasClassName() {
				return hasName();
			}


			CConcept *CConcept::setClassNameLinker(CLinker<CName *> *conceptNameLinker) {
				setNameLinker(conceptNameLinker);
				return this;
			}

			CConcept *CConcept::addClassNameLinker(CLinker<CName *> *conceptNameLinker) {
				addNameLinker(conceptNameLinker);
				return this;
			}

			CLinker<CName *> *CConcept::getClassNameLinker() {
				return getNameLinker();
			}



			bool CConcept::isEqualsToBOTTOM(bool negated) {
				cint64 operatorTag = mOperatorCode->getOperatorCode();
				return (!negated && operatorTag == CCBOTTOM || negated && operatorTag == CCTOP);
			}

			bool CConcept::isEqualsToTOP(bool negated) {
				cint64 operatorTag = mOperatorCode->getOperatorCode();
				return (negated && operatorTag == CCBOTTOM || !negated && operatorTag == CCTOP);
			}


			bool CConcept::hasOperandConceptTag(CConcept *concept) {
				return hasOperandConceptTag(concept->getConceptTag());
			}


			bool CConcept::hasOperandConceptTag(qint64 conTag) {
				CSortedNegLinker<CConcept *> *opIt = operands;
				while (opIt) {
					qint64 opConTag = opIt->getData()->getConceptTag();
					if (opConTag == conTag) {
						return true;
					} else if (opConTag > conTag) {
						return false;
					}
					opIt = (CSortedNegLinker<CConcept *> *)opIt->getNext();
				}
				return false;
			}


			bool CConcept::hasOperandConceptTag(CConcept *concept, bool negation) {
				return hasOperandConceptTag(concept->getConceptTag(),negation);
			}


			bool CConcept::hasOperandConceptTag(qint64 conTag, bool negation) {
				CSortedNegLinker<CConcept *> *opIt = operands;
				while (opIt) {
					qint64 opConTag = opIt->getData()->getConceptTag();
					bool opNeg = opIt->isNegated();
					if (opConTag == conTag && negation == opNeg) {
						return true;
					} else if (opConTag > conTag) {
						return false;
					}
					opIt = (CSortedNegLinker<CConcept *> *)opIt->getNext();
				}
				return false;
			}

			bool CConcept::hasOperandConcept(CConcept *concept) {
				CSortedNegLinker<CConcept *> *opIt = operands;
				while (opIt) {
					CConcept* opConcept = opIt->getData();
					if (opConcept == concept) {
						return true;
					}
					opIt = (CSortedNegLinker<CConcept *> *)opIt->getNext();
				}
				return false;
			}

			bool CConcept::hasOperandConcept(CConcept *concept, bool negation) {
				CSortedNegLinker<CConcept *> *opIt = operands;
				while (opIt) {
					CConcept* opConcept = opIt->getData();
					bool opNeg = opIt->isNegated();
					if (opConcept == concept && negation == opNeg) {
						return true;
					}
					opIt = (CSortedNegLinker<CConcept *> *)opIt->getNext();
				}
				return false;
			}


			CConceptData* CConcept::getConceptData() {
				return mConceptData;
			}

			CConcept* CConcept::setConceptData(CConceptData* conceptData) {
				mConceptData = conceptData;
				return this;
			}

			bool CConcept::hasConceptData() {
				return mConceptData != nullptr;
			}

			bool CConcept::hasMappingNegation() {
				return mMappingNegated;
			}

			CConcept* CConcept::setMappingNegation(bool negation) {
				mMappingNegated = negation;
				return this;
			}

			CDataLiteral* CConcept::getDataLiteral() {
				return mDataLiteral;
			}

			CConcept* CConcept::setDataLiteral(CDataLiteral* dataLiteral) {
				mDataLiteral = dataLiteral;
				return this;
			}

			CDatatype* CConcept::getDatatype() {
				return mDatatype;
			}

			CConcept* CConcept::setDatatype(CDatatype* datatype) {
				mDatatype = datatype;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

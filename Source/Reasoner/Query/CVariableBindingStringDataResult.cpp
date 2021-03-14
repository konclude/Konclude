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

#include "CVariableBindingStringDataResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingStringDataResult::CVariableBindingStringDataResult() {
				mData = nullptr;
			}

			CVariableBindingStringDataResult::CVariableBindingStringDataResult(const QString& bindingString, VariableBindingType type) {
				mBindingString = bindingString;
				mType = type;
				mData = nullptr;
			}



			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(const CIndividualReference& indiRef, CIndividualNameResolver* indiNameResolver) {
				mBindingString = indiNameResolver->getIndividualName(indiRef, false);
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (indiRef.getIndividual() && indiRef.getIndividual()->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				mData = (void*)indiRef.getIndividualID();
				return this;
			}



			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(const CIndividualReference& indiRef, const QString& resolvedIndiName) {
				mBindingString = resolvedIndiName;
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (indiRef.getIndividual() && indiRef.getIndividual()->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				mData = (void*)indiRef.getIndividualID();
				return this;
			}

			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(CIndividual* individual) {
				mBindingString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (individual && individual->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				mData = (void*)individual->getIndividualID();
				return this;
			}


			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(CConcept* concept) {
				mBindingString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
				mType = CVariableBindingResult::VBTCLASS;
				mData = (void*)concept->getConceptTag();
				return this;
			}

			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(CRole* role) {
				mBindingString = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
				mType = CVariableBindingResult::VBTPROPERTY;
				mData = (void*)role->getRoleTag();
				return this;
			}

			CVariableBindingResult* CVariableBindingStringDataResult::initVariableBinding(CDataLiteral* dataLiteral) {
				mType = CVariableBindingResult::VBTLITERAL;
				mData = dataLiteral;
				return this;
			}

			void* CVariableBindingStringDataResult::getData() {
				return mData;
			}

			CDataLiteral* CVariableBindingStringDataResult::getDataLiteral() {
				return (CDataLiteral*)mData;
			}

			cint64 CVariableBindingStringDataResult::getDataIndividualId() {
				return (cint64)mData;
			}


			bool CVariableBindingStringDataResult::enfoceBindingString() {
				if (mType == CVariableBindingResult::VBTLITERAL && mBindingString.isEmpty()) {
					CDataLiteral* dataLiteral = (CDataLiteral*)mData;
					QString literalString = "\"" + dataLiteral->getDataLiteralValue()->getValueString() + "\"";
					if (dataLiteral->getDatatype()->getDatatypeType() != CDatatype::DT_PLAINLITERAL) {
						literalString += "\"^^" + dataLiteral->getDatatype()->getDatatypeIRI();
					} else {
						literalString += "\"";
					}
					mBindingString = literalString;
					return true;
				}
				return false;
			}



			QPair<QString, QString> CVariableBindingStringDataResult::getLiteralDatatypeDatavalueBindingStringPair() {
				QString datatypeString;
				QString datatvalueString;
				if (mType == CVariableBindingResult::VBTLITERAL) {
					CDataLiteral* dataLiteral = (CDataLiteral*)mData;
					datatypeString = dataLiteral->getDatatype()->getDatatypeIRI();
					datatvalueString = dataLiteral->getDataLiteralValue()->getValueString();
				}
				return QPair<QString, QString>(datatypeString, datatvalueString);
			}



			CVariableBindingStringDataResult::~CVariableBindingStringDataResult() {
			}

			CVariableBindingResult* CVariableBindingStringDataResult::createCopy() {
				CVariableBindingStringDataResult* copy = new CVariableBindingStringDataResult(*this);
				return copy;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

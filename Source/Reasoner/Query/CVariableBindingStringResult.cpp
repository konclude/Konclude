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

#include "CVariableBindingStringResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CVariableBindingStringResult::CVariableBindingStringResult() {
			}

			CVariableBindingStringResult::CVariableBindingStringResult(const QString& bindingString, VariableBindingType type) {
				mBindingString = bindingString;
				mType = type;
			}


			CVariableBindingResult* CVariableBindingStringResult::initVariableBinding(const CIndividualReference& indiRef, CIndividualNameResolver* indiNameResolver) {
				mBindingString = indiNameResolver->getIndividualName(indiRef, false);
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (indiRef.getIndividual() && indiRef.getIndividual()->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				return this;
			}

			CVariableBindingResult* CVariableBindingStringResult::initVariableBinding(const CIndividualReference& indiRef, const QString& resolvedIndiName) {
				mBindingString = resolvedIndiName;
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (indiRef.getIndividual() && indiRef.getIndividual()->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				return this;
			}



			CVariableBindingResult* CVariableBindingStringResult::initVariableBinding(CIndividual* individual) {
				mBindingString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				mType = CVariableBindingResult::VBTNAMEDINDIVIDUAL;
				if (individual && individual->isAnonymousIndividual()) {
					mType = CVariableBindingResult::VBTANONYMOUSINDIVIDUAL;
				}
				return this;
			}

			CVariableBindingResult* CVariableBindingStringResult::initVariableBinding(CDataLiteral* dataLiteral) {
				QString literalString = "\"" + dataLiteral->getLexicalDataLiteralValueString();
				if (dataLiteral->getDatatype()->getDatatypeType() != CDatatype::DT_PLAINLITERAL) {
					literalString += "\"^^" + dataLiteral->getDatatype()->getDatatypeIRI();
				} else {
					literalString += "\"";
				}
				mBindingString = literalString;
				mType = CVariableBindingResult::VBTLITERAL;
				return this;
			}


			CVariableBindingStringResult::~CVariableBindingStringResult() {
			}


			const QString& CVariableBindingStringResult::getBindingString() {
				return mBindingString;
			}

			CVariableBindingStringResult* CVariableBindingStringResult::setBindingString(const QString& bindingString) {
				mBindingString = bindingString;
				return this;
			}

			CVariableBindingStringResult::VariableBindingType CVariableBindingStringResult::getVariableBindingType() {
				return mType;
			}


			CVariableBindingStringResult* CVariableBindingStringResult::setVariableBindingType(VariableBindingType type) {
				mType = type;
				return this;
			}

			QString CVariableBindingStringResult::getQueryResultString() {
				return mBindingString;
			}

			bool CVariableBindingStringResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				CVariableBindingStringResult* otherStringBinRes = dynamic_cast<CVariableBindingStringResult*>(otherQueryResult);
				if (!otherStringBinRes) {
					return false;
				}
				if (otherStringBinRes->getVariableBindingType() != mType) {
					return false;
				}
				if (otherStringBinRes->getBindingString() != mBindingString) {
					return false;
				}
				return true;
			}

			QString CVariableBindingStringResult::getNamedIndividualBindingString() {
				if (mType == CVariableBindingResult::VBTNAMEDINDIVIDUAL) {
					return mBindingString;
				}
				return QString();
			}

			QString CVariableBindingStringResult::getLiteralDatatypeBindingString() {
				if (mType == CVariableBindingResult::VBTLITERAL) {
					if (mBindingString.endsWith("\"")) {
						return QString(PREFIX_XML_PLAINLITERAL_DATATYPE);
					} else {
						cint64 separator = mBindingString.lastIndexOf("\"^^");
						if (separator < 0) {
							return QString(PREFIX_XML_PLAINLITERAL_DATATYPE);
						}
						else {
							return mBindingString.mid(separator + 3);
						}
					}
				}
				return QString();
			}

			QString CVariableBindingStringResult::getLiteralDatavalueBindingString() {
				if (mType == CVariableBindingResult::VBTLITERAL) {
					if (mBindingString.endsWith("\"")) {
						return mBindingString.mid(1, mBindingString.length() - 2);
					} else {
						cint64 separator = mBindingString.lastIndexOf("\"^^");
						if (separator < 0) {
							return mBindingString.mid(1, mBindingString.length() - 2);
						}
						else {
							return mBindingString.mid(1, separator - 1);
						}
					}
				}
				return QString();
			}

			CVariableBindingResult* CVariableBindingStringResult::createCopy() {
				CVariableBindingStringResult* copy = new CVariableBindingStringResult(*this);
				return copy;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

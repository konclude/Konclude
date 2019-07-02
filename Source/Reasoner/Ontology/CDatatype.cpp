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

#include "CDatatype.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatype::CDatatype() {
				initDatatype(QString(),false);
			}

			CDatatype* CDatatype::initDatatype(const QString& datatypeIRI, bool basic) {
				setDatatypeTag(0);
				mDatatypeIRI = datatypeIRI;
				mBasicDatatype = basic;
				mDatatpyeBasic = nullptr;
				mDatatypeConcept = nullptr;
				mDefinitionConcept = nullptr;
				mValueSpaceType = nullptr;
				mNormalized = false;
				mBasicDatatpyeType = DBT_NON;
				mDatatpyeType = DT_NON;
				return this;
			}

			CDatatype* CDatatype::initDatatype(const QString& datatypeIRI, cint64 datatypeTag, bool basic) {
				initDatatype(datatypeIRI,basic);
				setDatatypeTag(datatypeTag);
				mDatatypeConcept = nullptr;
				mDefinitionConcept = nullptr;
				mValueSpaceType = nullptr;
				mNormalized = false;
				mBasicDatatpyeType = DBT_NON;
				mDatatpyeType = DT_NON;
				return this;
			}


			CDatatype::DATATYPE_BASIC_TYPE CDatatype::getBasicDatatypeType() {
				return mBasicDatatpyeType;
			}

			CDatatype* CDatatype::setBasicDatatypeType(CDatatype::DATATYPE_BASIC_TYPE basicDatatpyeType) {
				mBasicDatatpyeType = basicDatatpyeType;
				return this;
			}

			CDatatype::DATATYPE_TYPE CDatatype::getDatatypeType() {
				return mDatatpyeType;
			}

			CDatatype* CDatatype::setDatatypeType(CDatatype::DATATYPE_TYPE datatpyeType) {
				mDatatpyeType = datatpyeType;
				return this;
			}

			CDatatype* CDatatype::setDatatypeTag(qint64 datatypeTag) {
				CTagItem::setTag(datatypeTag);
				return this;
			}

			cint64 CDatatype::getDatatypeTag() const {
				return CTagItem::getTag();
			}

			bool CDatatype::isBasicDatatype() {
				return mBasicDatatype;
			}

			CDatatype* CDatatype::setBasicDatatype(bool basic) {
				mBasicDatatype = basic;
				return this;
			}


			CDatatype* CDatatype::getBasicDatatype() {
				return mDatatpyeBasic;
			}

			CDatatype* CDatatype::setBasicDatatype(CDatatype* basicDatatpye) {
				mDatatpyeBasic = basicDatatpye;
				return this;
			}


			const QString& CDatatype::getDatatypeIRI() {
				return mDatatypeIRI;
			}

			CConcept* CDatatype::getDatatypeConcept() {
				return mDatatypeConcept;
			}

			bool CDatatype::hasDatatypeConcept() {
				return mDatatypeConcept != nullptr;
			}

			CDatatype* CDatatype::setDatatypeConcept(CConcept* concept) {
				mDatatypeConcept = concept;
				return this;
			}


			CDatatypeValueSpaceType* CDatatype::getValueSpaceType() {
				return mValueSpaceType;
			}

			CDatatype* CDatatype::setValueSpaceType(CDatatypeValueSpaceType* valueSpaceType) {
				mValueSpaceType = valueSpaceType;
				return this;
			}

			CConcept* CDatatype::getDefinitionConcept() {
				return mDefinitionConcept;
			}

			CDatatype* CDatatype::setDefinitionConcept(CConcept* concept) {
				mDefinitionConcept = concept;
				return this;
			}


			bool CDatatype::isNormalized() {
				return mNormalized;
			}

			CDatatype* CDatatype::setNormalized(bool normalized) {
				mNormalized = normalized;
				return this;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

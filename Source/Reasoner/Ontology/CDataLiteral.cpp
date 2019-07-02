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

#include "CDataLiteral.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDataLiteral::CDataLiteral(CContext* context) {
				mContext = context;
				mDatatype = nullptr;
				mDataValue = nullptr;
				mLexicalLiteralStringByteDataArray = nullptr;
				mLexicalLiteralStringByteDataArraySize = 0;
			}

			CDataLiteral::~CDataLiteral() {
				CObjectAllocator<QChar>::destroyAndReleaseArray(mLexicalLiteralStringByteDataArray, CContext::getMemoryAllocationManager(mContext));
			}


			CDataLiteral* CDataLiteral::initDataLiteral(const QString& lexicalDataLiteralValueString, CDatatype* datatype) {
				mLexicalLiteralStringByteDataArray = createStringByteDataArray(lexicalDataLiteralValueString);
				mLexicalLiteralStringByteDataArraySize = lexicalDataLiteralValueString.length();
				mDatatype = datatype;
				mDataValue = nullptr;
				return this;
			}

			QString CDataLiteral::getLexicalDataLiteralValueString() {
				return QString(mLexicalLiteralStringByteDataArray, mLexicalLiteralStringByteDataArraySize);
			}

			CDatatype* CDataLiteral::getDatatype() {
				return mDatatype;
			}

			CDataLiteralValue* CDataLiteral::getDataLiteralValue() {
				return mDataValue;
			}

			CDataLiteral* CDataLiteral::setDataLiteralValue(CDataLiteralValue* dataValue) {
				mDataValue = dataValue;
				return this;
			}

			QChar* CDataLiteral::createStringByteDataArray(const QString& string) {
				QChar* stringByteDataArrayCopy = CObjectAllocator<QChar>::allocateArray(CContext::getMemoryAllocationManager(mContext), string.length());
				const QChar* stringData = string.constData();
				for (cint64 i = 0; i < string.length(); ++i) {
					stringByteDataArrayCopy[i] = stringData[i];
				}
				return stringByteDataArrayCopy;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

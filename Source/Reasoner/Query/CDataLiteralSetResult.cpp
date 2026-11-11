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
 */#include "CDataLiteralSetResult.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {


			CDataLiteralSetResult::CDataLiteralSetResult() {
			}


			CDataLiteralSetResult::~CDataLiteralSetResult() {
			}


			QString CDataLiteralSetResult::getQueryResultString() {
				QStringList literalStringList;
				for (CDataLiteral* dataLiteral : mDataLiteralList) {
					literalStringList.append(dataLiteral->getLexicalDataLiteralValueString());
				}
				literalStringList.sort();
				return QString("Set of Literals: {%1}").arg(literalStringList.join(", "));
			}


			bool CDataLiteralSetResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				CDataLiteralSetResult* otherDataLiteralSetResult = dynamic_cast<CDataLiteralSetResult*>(otherQueryResult);
				if (!otherDataLiteralSetResult) {
					return false;
				}
				QSet<QString> literalStringSet;
				for (CDataLiteral* dataLiteral : mDataLiteralList) {
					literalStringSet.insert(dataLiteral->getLexicalDataLiteralValueString());
				}
				QSet<QString> otherLiteralStringSet;
				for (CDataLiteral* dataLiteral : *otherDataLiteralSetResult->getDataLiteralList()) {
					otherLiteralStringSet.insert(dataLiteral->getLexicalDataLiteralValueString());
				}
				return literalStringSet == otherLiteralStringSet;
			}


			CDataLiteralSetResult* CDataLiteralSetResult::addDataLiteral(CDataLiteral* dataLiteral) {
				if (!mDataLiteralList.contains(dataLiteral)) {
					mDataLiteralList.append(dataLiteral);
				}
				return this;
			}


			QList<CDataLiteral*>* CDataLiteralSetResult::getDataLiteralList() {
				return &mDataLiteralList;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
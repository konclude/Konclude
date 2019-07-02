/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CQuery.h"
#include "CQueryUnspecifiedStringError.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CQuery::CQuery() {
				mErrorLinker = nullptr;
			}


			CQuery::~CQuery() {
				CQueryError* errorLinkerIt = mErrorLinker;
				while (errorLinkerIt) {
					CQueryError* tmpErrorLinker = errorLinkerIt;
					errorLinkerIt = errorLinkerIt->getNext();
					tmpErrorLinker->clearNext();
					delete tmpErrorLinker;
				}
			}


			bool CQuery::hasError() {
				return mErrorLinker != nullptr;
			}


			CQuerySupport* CQuery::getQuerySupport() {
				return nullptr;
			}

			CQueryStatistics* CQuery::getQueryStatistics() {
				return nullptr;
			}

			QStringList CQuery::getErrorStringList() {
				QStringList errorStringList;
				for (CQueryError* errorLinkerIt = mErrorLinker; errorLinkerIt; errorLinkerIt = errorLinkerIt->getNext()) {
					errorStringList.append(errorLinkerIt->getErrorString());
				}
				return errorStringList;
			}

			CQuery* CQuery::addErrorString(const QString& errorString) {
				CQueryError* qError = new CQueryUnspecifiedStringError(errorString);
				mErrorLinker = qError->append(mErrorLinker);
				return this;
			}

			CQuery* CQuery::addQueryError(CQueryError* queryError) {
				if (queryError) {
					mErrorLinker = queryError->append(mErrorLinker);
				}
				return this;
			}


			CQueryError* CQuery::getQueryErrorLinker() {
				return mErrorLinker;
			}


			
		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

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

#ifndef KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONPARSER_H
#define KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONPARSER_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "CRDFSimpleTurtleParser.h"
#include "CSPARQLKnowledgeBaseSplittingOperationData.h"

// Other includes
#include "Utilities/CMath.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Utilities;

	namespace Parser {



		/*! 
		 *
		 *		\class		CSPARQLKnowledgeBaseSplittingParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLKnowledgeBaseSplittingOperationParser : public CRDFSimpleTurtleParser {
			// public methods
			public:
				//! Constructor
				CSPARQLKnowledgeBaseSplittingOperationParser();

				//! Destructor
				virtual ~CSPARQLKnowledgeBaseSplittingOperationParser();


				virtual bool parseQueryFile(const QString& filename);
				virtual bool parseQueryText(const QString& filetext);

				QStringList getKnowlegdeBaseList();
				QList<CSPARQLKnowledgeBaseSplittingOperationData*> getKnowlegdeBaseSPARQLOperationList(const QString& knowledgeBase);
				QList<CSPARQLKnowledgeBaseSplittingOperationData*> getSPARQLOperationList();


			// protected methods
			protected:
				bool isKeyword(const QString& part);
				bool processParts(QStringList& parsedParts);


				QString getGraphName(const QString& currentPartString, QStringList& parsedParts, QStringList& updateStringList, bool directGraphReference);


				CSPARQLKnowledgeBaseSplittingOperationParser* addKnowledgeBaseOperation(CSPARQLKnowledgeBaseSplittingOperationData* kbOperationData);

			// protected variables
			protected:
				QList<QString> mCommonPartStringList;

				QSet<QString> mKeywordSet;

				QStringList mKnowledgeBaseList;
				QList<CSPARQLKnowledgeBaseSplittingOperationData*> mDataContainer;
				QHash<QString, CSPARQLKnowledgeBaseSplittingOperationData*> mKnowledgeBaseQueryStringListHash;




			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLKNOWLEDGEBASESPLITTINGOPERATIONPARSER_H

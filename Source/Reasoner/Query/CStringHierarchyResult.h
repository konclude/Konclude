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

#ifndef KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_H
#define KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CStringSubStringsRelationResult.h"
#include "CQueryResult.h"
#include "CStringSynsetResult.h"

// Other includes
#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CStringHierarchyResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class R, class E>
			class CStringHierarchyResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CStringHierarchyResult();
					CStringHierarchyResult(const QString &bottomHierarchyString, const QString &topHierarchyString);
					CStringHierarchyResult(const QStringList &bottomHierarchyStrings, const QStringList &topHierarchyStrings);
					CStringHierarchyResult(E *takeBottomStringSynset, E *takeTopStringSynset);
					CStringHierarchyResult(E *takeBottomStringSynset);

					//! Destructor
					virtual ~CStringHierarchyResult();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					E* getStringSynset(const QString &stringName, bool create = true);
					E* getStringSynset(const QStringList &stringNames, bool create = true);
					E* addStringSynset(E *takeStringSynset);
					  
					E* setTopStringSynset(E *stringSynset);
					  
					  
					E* getBottomStringSynset();
					E* getTopStringSynset();
					  
					R* addSubStringRelation(E *superStringSynset, E *anySubStringSynset);
					R* getSubStringRelation(E *superStringSynset, bool create = true);

					virtual CTaxonomy *fillTaxonomy(CConcreteOntology *ontology, Reasoner::Taxonomy::CTaxonomy *taxonomy);

				// protected methods
				protected:
					void collectSubsumerStringSynsets(CStringHierarchyResult *otherStringHierarchy, QHash<QString,QString>* subsumHash, QSet< QPair<QString,QString> >* subsumSet = nullptr);
					void compareSubsumersCount(QHash<QString,QString>* subsumHash1, QHash<QString,QString>* subsumHash2);

				// protected variables
				protected:
					E *mBottomString;
					E *mTopString;

					QList<R *> mSubStringRelationContainer;
					QList<E *> mStringSynsetContainer;

					QHash<QString,E *> mStringStringSynsetHash;
					QHash<E *,R *> mStringSynsetSubStringHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CStringHierarchyResult.cpp"

#endif // KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_H

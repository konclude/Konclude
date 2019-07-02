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

#ifndef KONCLUDE_REASONER_QUERY_CCLASSHIERARCHYRESULT_H
#define KONCLUDE_REASONER_QUERY_CCLASSHIERARCHYRESULT_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CClassSubClassesRelationResult.h"
#include "CQueryResult.h"
#include "CClassSynsetResult.h"

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
			 *		\class		CClassHierarchyResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassHierarchyResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CClassHierarchyResult();
					CClassHierarchyResult(const QString &bottomHierarchyClass, const QString &topHierarchyClass);
					CClassHierarchyResult(const QStringList &bottomHierarchyClasses, const QStringList &topHierarchyClasses);
					CClassHierarchyResult(CClassSynsetResult *takeBottomClassSynset, CClassSynsetResult *takeTopClassSynset);
					CClassHierarchyResult(CClassSynsetResult *takeBottomClassSynset);

					//! Destructor
					virtual ~CClassHierarchyResult();

					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					CClassSynsetResult *getClassSynset(const QString &className, bool create = true);
					CClassSynsetResult *getClassSynset(const QStringList &classNames, bool create = true);
					CClassSynsetResult *addClassSynset(CClassSynsetResult *takeClassSynset);

					CClassSynsetResult *setTopClassSynset(CClassSynsetResult *classSynset);


					CClassSynsetResult *getBottomClassSynset();
					CClassSynsetResult *getTopClassSynset();

					CClassSubClassesRelationResult *addSubClassRelation(CClassSynsetResult *superClassSynset, CClassSynsetResult *anySubClassSynset);
					CClassSubClassesRelationResult *getSubClassRelation(CClassSynsetResult *superClassSynset, bool create = true);

					virtual CTaxonomy *fillTaxonomy(CConcreteOntology *ontology, Reasoner::Taxonomy::CTaxonomy *taxonomy);

				// protected methods
				protected:
					void collectSubsumerClassSynsets(CClassHierarchyResult *otherClassHierarchy, QHash<QString,QString>* subsumHash, QSet< QPair<QString,QString> >* subsumSet = nullptr);
					void compareSubsumersCount(QHash<QString,QString>* subsumHash1, QHash<QString,QString>* subsumHash2);

				// protected variables
				protected:
					CClassSynsetResult *bottomClass;
					CClassSynsetResult *topClass;

					QList<CClassSubClassesRelationResult *> subClassRelationContainer;
					QList<CClassSynsetResult *> classSynsetContainer;

					QHash<QString,CClassSynsetResult *> classClassSynsetHash;
					QHash<CClassSynsetResult *,CClassSubClassesRelationResult *> classSynsetSubClassHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CCLASSHIERARCHYRESULT_H

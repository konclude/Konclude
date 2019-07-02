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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_H

// Libraries includes
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CHierarchyNodeData.h"
#include "CAbstractHierarchyNode.h"

// Other includes
#include "KoncludeSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CParentChildPredecessorHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class E>
			class CParentChildPredecessorHierarchyNode : public CAbstractHierarchyNode<T,E> {
				// public methods
				public:
					//! Constructor
					CParentChildPredecessorHierarchyNode(const E &representiveElement);

					//! Destructor
					~CParentChildPredecessorHierarchyNode();

					T* addParentNode(CParentChildPredecessorHierarchyNode* parentNode);
					T* addChildNode(CParentChildPredecessorHierarchyNode* childNode);

					T* makeParent(CParentChildPredecessorHierarchyNode* parentNode);
					T* makeChild(CParentChildPredecessorHierarchyNode* childNode);

					bool hasChildNode(CParentChildPredecessorHierarchyNode* node);
					bool hasParentNode(CParentChildPredecessorHierarchyNode* node);

					T* addPredecessorNode(CParentChildPredecessorHierarchyNode* preNode);
					T* makePredecessor(CParentChildPredecessorHierarchyNode* parentNode);

					bool hasPredecessorNode(CParentChildPredecessorHierarchyNode* node);

					T* removeParent(CParentChildPredecessorHierarchyNode* parentNode);
					T* removeChild(CParentChildPredecessorHierarchyNode* childNode);
					T* removePredecessor(CParentChildPredecessorHierarchyNode* predNode);

					QSet<T*>* getParentNodeSet();
					QSet<T*>* getChildNodeSet();
					QSet<T*>* getPredecessorNodeSet();

					QList<T*> getParentNodeList();
					QList<T*> getChildNodeList();
					QList<T*> getPredecessorNodeList();


				// protected methods
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					virtual void generateNodeRelationStrings();
#endif

				// protected variables
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					QStringList parentNodeStrings;
					QStringList childNodeStrings;
					QStringList precNodeStrings;
#endif


					QSet<CParentChildPredecessorHierarchyNode*> parentNodeSet;
					QSet<CParentChildPredecessorHierarchyNode*> childNodeSet;
					QSet<CParentChildPredecessorHierarchyNode*> predNodeSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CParentChildPredecessorHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_H

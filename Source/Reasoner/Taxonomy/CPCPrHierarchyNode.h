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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_H

// Libraries includes
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CHierarchyNodeData.h"
#include "CAHierarchyNode.h"

// Other includes
#include "KoncludeSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CPCPrHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class E>
			class CPCPrHierarchyNode : public CAHierarchyNode<T,E> {
				// public methods
				public:
					//! Constructor
					CPCPrHierarchyNode(const E &representiveElement);

					//! Destructor
					~CPCPrHierarchyNode();

					T* addParentNode(CPCPrHierarchyNode* parentNode);
					T* addChildNode(CPCPrHierarchyNode* childNode);

					T* makeParent(CPCPrHierarchyNode* parentNode);
					T* makeChild(CPCPrHierarchyNode* childNode);

					bool hasChildNode(CPCPrHierarchyNode* node);
					bool hasParentNode(CPCPrHierarchyNode* node);

					T* addPredecessorNode(CPCPrHierarchyNode* preNode);
					T* makePredecessor(CPCPrHierarchyNode* parentNode);

					bool hasPredecessorNode(CPCPrHierarchyNode* node);

					T* removeParent(CPCPrHierarchyNode* parentNode);
					T* removeChild(CPCPrHierarchyNode* childNode);
					T* removePredecessor(CPCPrHierarchyNode* predNode);

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


					QSet<CPCPrHierarchyNode*> parentNodeSet;
					QSet<CPCPrHierarchyNode*> childNodeSet;
					QSet<CPCPrHierarchyNode*> predNodeSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CPCPrHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_H

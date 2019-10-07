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

#ifndef KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_H

// Libraries includes
#include <QStringList>
#include <QList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CHierarchyNodeData.h"

// Other includes
#include "KoncludeSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

	//#define DEBUGHIERARCHYNODEELEMENTSTRINGS

#endif

			/*! 
			 *
			 *		\class		CAbstractHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class E>
			class CAbstractHierarchyNode {
				// public methods
				public:
					//! Constructor
					CAbstractHierarchyNode(const E &representiveElement);

					//! Destructor
					~CAbstractHierarchyNode();

					T* addEquivalentElement(const E &element);

					QList<E>* getEquivalentElementList();

					E getOneEquivalentElement(const E &defaultValue);
					E getOneEquivalentElement();
					qint64 getEquivalentElementCount();

					bool hasNodeData();
					CHierarchyNodeData* getNodeData();
					CHierarchyNodeData* setNodeData(CHierarchyNodeData* nodeData);

				// protected methods
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					virtual void generateNodeElementStrings();
#endif

				// protected variables
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					QStringList nodeStrings;
#endif

					QList<E> eqElementList;
					qint64 eqElemCount;

					CHierarchyNodeData* mNodeData;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CAbstractHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_H

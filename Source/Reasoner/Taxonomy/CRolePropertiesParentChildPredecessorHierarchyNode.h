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

#ifndef KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_H
#define KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_H

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "TaxonomySettings.h"
#include "CParentChildPredecessorHierarchyNode.h"

// Other includes
#include "KoncludeSettings.h"

#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CRolePropertiesParentChildPredecessorHierarchyNode
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CRolePropertiesParentChildPredecessorHierarchyNode : public CParentChildPredecessorHierarchyNode<T,CRole*> {
				// public methods
				public:
					//! Constructor
					CRolePropertiesParentChildPredecessorHierarchyNode(CRole* representiveRole);

					T* addEquivalentRole(CRole* role);

					QList<CRole*>* getEquivalentRoleList();

					CRole* getOneEquivalentRole();
					QString getEquivalentRoleNodeString();
					QString getHierarchyRolesNodeString(qint64 depth);

					QStringList getEquivalentRoleStringList(bool preferAbbreviatedNames);
					qint64 getEquivalentRoleCount();


				// protected methods
				protected:
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
					virtual void generateNodeRelationStrings();
					virtual void generateNodeElementStrings();
#endif

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CRolePropertiesParentChildPredecessorHierarchyNode.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_H

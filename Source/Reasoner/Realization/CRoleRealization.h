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

#ifndef KONCLUDE_REASONER_REALIZATION_CROLEREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CROLEREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CRoleRealizationIndividualVisitor.h"
#include "CRoleRealizationInstanceVisitor.h"
#include "CRoleRealizationInstantiatedVisitor.h"
#include "CRoleRealizationRoleVisitor.h"
#include "CRealizationIndividualInstanceItemReference.h"
#include "CRealizationIndividualInstanceItemReferenceIterator.h"
#include "CRealizationIndividualSorting.h"
#include "CRoleRealizationHierarchyNodeVisitor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CRoleRealization
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRoleRealization {
				// public methods
				public:
					//! Constructor
					CRoleRealization();

					virtual CRealizationIndividualInstanceItemReference getRoleInstanceItemReference(CIndividual* individual);

					virtual bool visitTargetIndividuals(CIndividual* individual, CRole* role, CRoleRealizationInstanceVisitor* visitor);
					virtual bool visitSourceIndividuals(CIndividual* individual, CRole* role, CRoleRealizationInstanceVisitor* visitor);
					virtual bool isRoleInstance(CRole* role, CIndividual* sourceIndividual, CIndividual* targetIndividual);
					virtual bool visitSourceIndividualRoles(CIndividual* individual, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitTargetIndividualRoles(CIndividual* individual, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitBetweeenIndividualRoles(CIndividual* sourceIndividual, CIndividual* targetIndividual, CRoleRealizationInstantiatedVisitor* visitor);


					virtual bool visitTargetIndividuals(const CIndividualReference& indiRef, CRole* role, CRoleRealizationInstanceVisitor* visitor);
					virtual bool visitSourceIndividuals(const CIndividualReference& indiRef, CRole* role, CRoleRealizationInstanceVisitor* visitor);
					virtual bool isRoleInstance(CRole* role, const CIndividualReference& sourceIndiRef, const CIndividualReference& targetIndiRef);
					virtual bool visitSourceIndividualRoles(const CIndividualReference& indiRef, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitTargetIndividualRoles(const CIndividualReference& indiRef, CRoleRealizationInstantiatedVisitor* visitor);
					virtual bool visitBetweeenIndividualRoles(const CIndividualReference& sourceIndiRef, const CIndividualReference& targetIndiRef, CRoleRealizationInstantiatedVisitor* visitor);


					virtual bool isRoleInstance(CRole* role, const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef) = 0;
					virtual bool visitTargetIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitSourceIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitSourceIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitTargetIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitBetweeenIndividualRoles(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) = 0;


					virtual bool requiresSourceIndividualRolesRealization(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, bool onlyOnePerRole = false) = 0;
					virtual bool requiresSourceIndividualRolesRealization(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, CRoleInstantiatedItem* roleItem, bool onlyOne = false) = 0;
					virtual bool requiresSourceIndividualRolesRealization(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, CRole* role, bool onlyOne = false);

					virtual bool isSourceIndividualRole(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem) = 0;
					virtual bool requiresSourceIndividualRoleRealization(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem) = 0;
					virtual bool isSourceIndividualRole(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRole* role);
					virtual bool requiresSourceIndividualRoleRealization(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRole* role);


					virtual bool visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationIndividualVisitor* visitor) = 0;
					virtual bool visitRoles(CRoleInstantiatedItem* roleInstItem, CRoleRealizationRoleVisitor* visitor) = 0;
					virtual bool visitRoleHierarchyNode(CRoleInstantiatedItem* roleInstItem, CRoleRealizationHierarchyNodeVisitor* visitor) = 0;

					virtual CRealizationIndividualInstanceItemReference getRoleInstanceItemReference(const CIndividualReference& indiRef) = 0;
					virtual CSameInstanceItem* getSameInstanceItem(const CRealizationIndividualInstanceItemReference& indiRealItemRef) = 0;

					virtual CRoleInstantiatedItem* getRoleInstantiatedItem(CRole* role) = 0;
					virtual CRoleInstanceItem* getRoleInstanceItem(CIndividual* individual) = 0;
					virtual CRoleInstanceItem* getRoleInstanceItem(CSameInstanceItem* instanceItem) = 0;
					virtual CSameInstanceItem* getSameInstanceItem(CRoleInstanceItem* instanceItem) = 0;

					virtual CRealizationIndividualInstanceItemReference getRoleInstanceItemReference(CSameInstanceItem* instanceItem) = 0;

					virtual bool getPreferredRolePropagationDirection(CRole* role, bool& inversedPreferred) = 0;

					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleNeighbouringInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRoleInstantiatedItem* roleInstancesItem, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationIntersectionIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> takeIteratorList) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationCandidateIndividualIdIterator(const QMap<cint64, IndividualIdCandidateData>& candidateIndiIdsMap, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleExistentialInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getRoleDataInstancesIterator(CRole* role, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;


					virtual bool hasOnlyDeterministicRoleDataInstances(CRole* role) = 0;
					virtual bool hasExistentiallyLinkedRoleInstances(CRole* role, bool inversed) = 0;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CROLEREALIZATION_H

/*
 *		Copyright (C) 2013-2015, 2019, 2026 by the Konclude Developer Team.
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

#include "CNonSimpleRoleRestrictionCheckPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CNonSimpleRoleRestrictionCheckPreProcess::CNonSimpleRoleRestrictionCheckPreProcess() {
			}


			CNonSimpleRoleRestrictionCheckPreProcess::~CNonSimpleRoleRestrictionCheckPreProcess() {
			}


			void CNonSimpleRoleRestrictionCheckPreProcess::markNonSimpleRole(CRole* role, QSet<CRole*>& nonSimpleRoleSet) {
				if (role && !nonSimpleRoleSet.contains(role)) {
					nonSimpleRoleSet.insert(role);
					if (role->getInverseRole()) {
						nonSimpleRoleSet.insert(role->getInverseRole());
					}
				}
			}


			// the name of the property, for an inverse the name of the property it is the inverse of
			QString CNonSimpleRoleRestrictionCheckPreProcess::getRoleName(CRole* role) {
				if (role->getPropertyNameLinker()) {
					return CIRIName::getRecentIRIName(role->getPropertyNameLinker());
				}
				if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
					return CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
				}
				return QString("role %1").arg(role->getRoleTag());
			}


			// the count and the first ten names, sorted
			QString CNonSimpleRoleRestrictionCheckPreProcess::getRoleNameListString(const QSet<QString>& roleNameSet) {
				QStringList roleNameList = roleNameSet.values();
				roleNameList.sort();
				cint64 roleNameCount = roleNameList.size();
				if (roleNameCount > 10) {
					roleNameList = roleNameList.mid(0, 10);
					roleNameList.append(QString("and %1 more").arg(roleNameCount - 10));
				}
				return QString("%1 %2 (%3)").arg(roleNameCount).arg(roleNameCount == 1 ? "property" : "properties").arg(roleNameList.join(", "));
			}


			CConcreteOntology* CNonSimpleRoleRestrictionCheckPreProcess::preprocess(CConcreteOntology* ontology, CPreProcessContext* context) {
				CRBox* rBox = ontology->getDataBoxes()->getRBox();
				CTBox* tBox = ontology->getDataBoxes()->getTBox();
				CRoleVector* roleVec = rBox->getRoleVector();
				CConceptVector* conceptVec = tBox->getConceptVector();
				CRole* topRole = rBox->getTopObjectRole();

				// the transitive roles and the super roles of property chains, which the subrole transformation
				// marks complex, and everything above them, over inverses as well
				QSet<CRole*> nonSimpleRoleSet;
				cint64 roleCount = roleVec->getItemCount();
				for (cint64 roleIdx = 0; roleIdx < roleCount; ++roleIdx) {
					CRole* role = roleVec->getData(roleIdx);
					if (role && !role->isDataRole() && (role->isTransitive() || role->isComplexRole())) {
						markNonSimpleRole(role, nonSimpleRoleSet);
						for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
							markNonSimpleRole(superRoleIt->getData(), nonSimpleRoleSet);
						}
					}
				}
				// the universal role lies above every role, a restriction on it is not what the check is about
				nonSimpleRoleSet.remove(topRole);
				if (topRole && topRole->getInverseRole()) {
					nonSimpleRoleSet.remove(topRole->getInverseRole());
				}

				if (!nonSimpleRoleSet.isEmpty()) {
					// the normalisation makes several concepts of one restriction, so the properties are counted,
					// an inverse under its named property
					QSet<QString> restrictionRoleNameSet;
					cint64 conceptCount = tBox->getConceptCount();
					for (cint64 conIdx = 0; conIdx < conceptCount; ++conIdx) {
						CConcept* concept = conceptVec->getData(conIdx);
						if (concept) {
							cint64 opCode = concept->getOperatorCode();
							CRole* role = concept->getRole();
							// a reflexive property is represented by a self restriction on it, and OWL 2 DL allows
							// reflexive non-simple properties, SNOMED CT has two; a stated self restriction on such a
							// property is therefore not reported either, it says no more than the reflexivity
							bool reflexivity = opCode == CCSELF && role && (role->isReflexive() || role->getInverseRole() && role->getInverseRole()->isReflexive());
							if ((opCode == CCATMOST || opCode == CCATLEAST || opCode == CCSELF) && !reflexivity && nonSimpleRoleSet.contains(role)) {
								restrictionRoleNameSet.insert(getRoleName(role));
							}
						}
					}
					QSet<QString> axiomRoleNameSet;
					for (CRole* role : nonSimpleRoleSet) {
						if (role->isFunctional() || role->isInverseFunctional() || role->isIrreflexive() || role->isAsymmetric() || role->hasDisjointRoles()) {
							axiomRoleNameSet.insert(getRoleName(role));
						}
					}

					if (!restrictionRoleNameSet.isEmpty() || !axiomRoleNameSet.isEmpty()) {
						QStringList parts;
						if (!restrictionRoleNameSet.isEmpty()) {
							parts.append(QString("cardinality or self restrictions on %1").arg(getRoleNameListString(restrictionRoleNameSet)));
						}
						if (!axiomRoleNameSet.isEmpty()) {
							parts.append(QString("functional, inverse functional, irreflexive, asymmetric or disjoint property axioms on %1").arg(getRoleNameListString(axiomRoleNameSet)));
						}
						LOG(WARN, "::Konclude::Reasoner::Preprocess::NonSimpleRoleRestrictionCheck", logTr("The ontology is not in OWL 2 DL: it has %1, which are non-simple properties (transitive, super properties of a property chain, or super properties of those). Other reasoners reject such an ontology; Konclude reasons with it, but its results for these restrictions are not guaranteed and may differ from run to run.")
								.arg(parts.join(", and ")), this);
					}
				}
				return ontology;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CFlattenedObjectPropertyTargetsQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CFlattenedObjectPropertyTargetsQuery::CFlattenedObjectPropertyTargetsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* individual, CRole* role, const QString& objectPropertyTargetQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividual = individual;
				mRole = role;

				mIndividualName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				mRoleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());

				queryName = objectPropertyTargetQueryName;
				if (queryName.isEmpty()) {
					if (!mIndividualName.isEmpty() && !mRoleName.isEmpty()) {
						queryString = QString("Get Flattened Object Property Targets");
					} else {
						queryString = QString("Get Flattened Object Property Targets for Individual '%1'").arg(mIndividualName);
					}
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;

				mCalcConfig = configuration;
			}

			CFlattenedObjectPropertyTargetsQuery::~CFlattenedObjectPropertyTargetsQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CFlattenedObjectPropertyTargetsQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CFlattenedObjectPropertyTargetsQuery::constructResult(CRealization* realization) {
				return constructIndividualsResult(realization);
			}

			CIndividualsResult *CFlattenedObjectPropertyTargetsQuery::constructIndividualsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CRoleRealization* roleRealization = realization->getRoleRealization();
					if (roleRealization) {
						mResult = new CIndividualsResult();
						CIndividualsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs);
						roleRealization->visitTargetIndividuals(mIndividual,mRole,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CFlattenedObjectPropertyTargetsQuery::getQueryName() {
				return queryName;
			}

			QString CFlattenedObjectPropertyTargetsQuery::getQueryString() {
				return queryString;
			}

			bool CFlattenedObjectPropertyTargetsQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CFlattenedObjectPropertyTargetsQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CFlattenedObjectPropertyTargetsQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

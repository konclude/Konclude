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

#include "CFlattenedObjectPropertySourcesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CFlattenedObjectPropertySourcesQuery::CFlattenedObjectPropertySourcesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& individualReference, const QString& individualName, CRole* role, const QString& objectPropertyTargetQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualReference = individualReference;
				mRole = role;

				mIndividualName = individualName;
				mRoleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());

				queryName = objectPropertyTargetQueryName;
				if (queryName.isEmpty()) {
					if (!mIndividualName.isEmpty() && !mRoleName.isEmpty()) {
						queryString = QString("Get Flattened Object Property Sources");
					} else {
						queryString = QString("Get Flattened Object Property Sources for Individual '%1'").arg(mIndividualName);
					}
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;

				mCalcConfig = configuration;
			}

			CFlattenedObjectPropertySourcesQuery::~CFlattenedObjectPropertySourcesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CFlattenedObjectPropertySourcesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CFlattenedObjectPropertySourcesQuery::constructResult(CRealization* realization) {
				return constructIndividualsResult(realization);
			}

			CIndividualsResult *CFlattenedObjectPropertySourcesQuery::constructIndividualsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CRoleRealization* roleRealization = realization->getRoleRealization();
					if (roleRealization) {
						mResult = new CIndividualsResult();
						CIndividualsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs, mOntology->getIndividualNameResolver());
						roleRealization->visitSourceIndividuals(mIndividualReference,mRole,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CFlattenedObjectPropertySourcesQuery::getQueryName() {
				return queryName;
			}

			QString CFlattenedObjectPropertySourcesQuery::getQueryString() {
				return queryString;
			}

			bool CFlattenedObjectPropertySourcesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CFlattenedObjectPropertySourcesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CFlattenedObjectPropertySourcesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CFlattenedObjectPropertySourcesQuery::getDynamicRealizationRequirement() {
				return new COntologyProcessingRoleRealizationRequirement(mRole, true, mIndividualReference);
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

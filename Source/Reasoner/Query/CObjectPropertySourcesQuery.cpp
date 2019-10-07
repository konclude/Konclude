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

#include "CObjectPropertySourcesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CObjectPropertySourcesQuery::CObjectPropertySourcesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& individualReference, const QString& individualName, CRole* role, const QString& objectPropertyTargetQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualReference = individualReference;
				mRole = role;

				mIndividualName = individualName;
				mRoleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());

				queryName = objectPropertyTargetQueryName;
				if (queryName.isEmpty()) {
					if (!mIndividualName.isEmpty() && !mRoleName.isEmpty()) {
						queryString = QString("Get Object Property Sources");
					} else {
						queryString = QString("Get Object Property Sources for Individual '%1'").arg(mIndividualName);
					}
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);
				mAnonymousIndividuals = CConfigDataReader::readConfigBoolean(configuration, "Konclude.OWLlink.ResultAnonymousIndividualsIntegration", false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;

				mCalcConfig = configuration;
			}

			CObjectPropertySourcesQuery::~CObjectPropertySourcesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CObjectPropertySourcesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CObjectPropertySourcesQuery::constructResult(CRealization* realization) {
				return constructIndividualSynsetsResult(realization);
			}

			CIndividualSynsetsResult *CObjectPropertySourcesQuery::constructIndividualSynsetsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CRoleRealization* roleRealization = realization->getRoleRealization();
					if (roleRealization) {
						mResult = new CIndividualSynsetsResult();
						CIndividualSynsetsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs, mAnonymousIndividuals, mOntology->getIndividualNameResolver());
						roleRealization->visitSourceIndividuals(mIndividualReference,mRole,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CObjectPropertySourcesQuery::getQueryName() {
				return queryName;
			}

			QString CObjectPropertySourcesQuery::getQueryString() {
				return queryString;
			}

			bool CObjectPropertySourcesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CObjectPropertySourcesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CObjectPropertySourcesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CObjectPropertySourcesQuery::getDynamicRealizationRequirement() {
				return new COntologyProcessingRoleRealizationRequirement(mRole, true, mIndividualReference);
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

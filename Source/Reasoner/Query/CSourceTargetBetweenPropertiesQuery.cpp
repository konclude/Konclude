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

#include "CSourceTargetBetweenPropertiesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSourceTargetBetweenPropertiesQuery::CSourceTargetBetweenPropertiesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& sourceIndi, const QString& sourceIndiName, const CIndividualReference& targetIndi, const QString& targetIndiName, const QString& queryName)
					: CRealizationPremisingQuery(ontology,configuration) {

				mSourceIndiName = sourceIndiName;
				mTargetIndiName = targetIndiName;
				mSourceIndi = sourceIndi;
				mTargetIndi = targetIndi;
				
				mQueryName = queryName;
				if (mQueryName.isEmpty()) {
					if (!mSourceIndiName.isEmpty() && mTargetIndiName.isEmpty()) {
						mQueryString = QString("Get properties between %1 and %2").arg(mSourceIndiName).arg(mTargetIndiName);
					} else if (!mSourceIndiName.isEmpty()) {
						mQueryString = QString("Get properties for source individual %1").arg(mSourceIndiName);
					} else if (!mTargetIndiName.isEmpty()) {
						mQueryString = QString("Get properties for target individual %1").arg(mTargetIndiName);
					} else {
						mQueryString = QString("Unknown get properties query");
					}
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;
			}


			CSourceTargetBetweenPropertiesQuery::~CSourceTargetBetweenPropertiesQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}



			CQueryResult* CSourceTargetBetweenPropertiesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSourceTargetBetweenPropertiesQuery::constructResult(CRealization* realization) {
				if (realization && realization->getRoleRealization()) {
					return constructPropertySynsetsResult(realization->getRoleRealization());
				}
				return nullptr;
			}

			CPropertySynsetsResult *CSourceTargetBetweenPropertiesQuery::constructPropertySynsetsResult(CRoleRealization *roleRealization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (roleRealization && (mSourceIndi.isNonEmpty() || mTargetIndi.isNonEmpty())) {
					mResult = new CPropertySynsetsResult(false);
					CPropertySynsetsResultVisitorGenerator propSynsetResVisGen(mResult, mUseAbbreviatedIRIs);
					if (mSourceIndi.isNonEmpty() && mTargetIndi.isNonEmpty()) {
						roleRealization->visitBetweeenIndividualRoles(mSourceIndi,mTargetIndi,&propSynsetResVisGen);
					} else if (mSourceIndi.isNonEmpty()) {
						roleRealization->visitSourceIndividualRoles(mSourceIndi,&propSynsetResVisGen);
					} else if (mTargetIndi.isNonEmpty()) {
						roleRealization->visitTargetIndividualRoles(mTargetIndi,&propSynsetResVisGen);
					}
				}
				return mResult;
			}


			QString CSourceTargetBetweenPropertiesQuery::getQueryName() {
				return mQueryName;
			}

			QString CSourceTargetBetweenPropertiesQuery::getQueryString() {
				return mQueryString;
			}

			bool CSourceTargetBetweenPropertiesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSourceTargetBetweenPropertiesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Properties");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSourceTargetBetweenPropertiesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CSourceTargetBetweenPropertiesQuery::getDynamicRealizationRequirement() {
				if (mSourceIndi.isNonEmpty() && mTargetIndi.isNonEmpty()) {
					return new COntologyProcessingRoleRealizationRequirement(nullptr, false, mSourceIndi, mTargetIndi);
				} else if (mSourceIndi.isNonEmpty()) {
					return new COntologyProcessingRoleRealizationRequirement(nullptr, false, mSourceIndi, CIndividualReference());
				} else if (mTargetIndi.isNonEmpty()) {
					return new COntologyProcessingRoleRealizationRequirement(nullptr, false, CIndividualReference(), mTargetIndi);
				}
				return nullptr;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

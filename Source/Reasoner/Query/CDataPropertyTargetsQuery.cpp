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
 */#include "CDataPropertyTargetsQuery.h"


// Other includes
#include "Reasoner/Ontology/COntologyProcessingDataLiteralRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStatus.h"
#include "Reasoner/Realization/CDataLiteralRealization.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {


			class CDataLiteralSetResultVisitor : public Realization::CDataLiteralRealizationVisitor {
				public:
					CDataLiteralSetResultVisitor(CDataLiteralSetResult* result) {
						mResult = result;
					}
					virtual bool visitDataRole(CRole* dataRole, bool deterministic) {
						return true;
					}
					virtual bool visitDataLiteral(CRole* dataRole, CDataLiteral* dataLiteral, bool deterministic) {
						if (mResult) {
							mResult->addDataLiteral(dataLiteral);
						}
						return true;
					}
				protected:
					CDataLiteralSetResult* mResult;
			};


			CDataPropertyTargetsQuery::CDataPropertyTargetsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& individualReference, const QString& individualName, CRole* dataRole, const QString& dataPropertyTargetQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualReference = individualReference;
				mDataRole = dataRole;
				mIndividualName = individualName;
				mDataRoleName = QString();
				if (dataRole && dataRole->hasPropertyName()) {
					mDataRoleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
				}

				queryName = dataPropertyTargetQueryName;
				if (queryName.isEmpty()) {
					queryString = QString("Get Data Property Targets for Individual '%1'").arg(mIndividualName);
				} else {
					queryString = queryName;
				}
				mResult = nullptr;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;
				mRequiresDataRoleClassification = true;

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mCalcConfig = configuration;
			}

			CDataPropertyTargetsQuery::~CDataPropertyTargetsQuery() {
				if (mResult) {
					delete mResult;
				}
			}


			CQueryResult* CDataPropertyTargetsQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CDataPropertyTargetsQuery::constructResult(CRealization* realization) {
				return constructDataLiteralSetResult(realization);
			}


			CDataLiteralSetResult* CDataPropertyTargetsQuery::constructDataLiteralSetResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = nullptr;
				}
				if (realization) {
					Realization::CDataLiteralRealization* dataLiteralRealization = realization->getDataLiteralRealization();
					if (dataLiteralRealization) {
						mResult = new CDataLiteralSetResult();
						CDataLiteralSetResultVisitor resultGenerator(mResult);
						dataLiteralRealization->visitDataLiterals(mIndividualReference, mDataRole, &resultGenerator);
					}
				}
				return mResult;
			}


			QString CDataPropertyTargetsQuery::getQueryName() {
				return queryName;
			}

			QString CDataPropertyTargetsQuery::getQueryString() {
				return queryString;
			}

			bool CDataPropertyTargetsQuery::hasAnswer() {
				return mResult != nullptr;
			}

			QString CDataPropertyTargetsQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Literals");
				}
				return mResult->getQueryResultString();
			}

			bool CDataPropertyTargetsQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CDataPropertyTargetsQuery::getDynamicRealizationRequirement() {
				return new COntologyProcessingDataLiteralRealizationRequirement(mDataRole, mIndividualReference);
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
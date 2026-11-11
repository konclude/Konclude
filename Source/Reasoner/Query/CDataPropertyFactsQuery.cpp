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

#include "CDataPropertyFactsQuery.h"


// Other includes
#include "Reasoner/Ontology/COntologyProcessingDataLiteralRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStatus.h"
#include "Reasoner/Ontology/CIndividualNameResolver.h"
#include "Reasoner/Ontology/CIRIName.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"
#include "Reasoner/Ontology/CRBox.h"
#include "Reasoner/Realization/CDataLiteralRealization.h"
#include "Reasoner/Realization/CRoleRealization.h"
#include "Reasoner/Realization/CSameRealization.h"
#include "Reasoner/Query/CPropertySynsetsResult.h"
#include "Reasoner/Query/CIndividualSynsetsResult.h"
#include "Reasoner/Query/CIndividualsResult.h"

#include "Config/CConfigDataReader.h"

#include <functional>


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {


			class CDataRoleSetVisitor : public Realization::CDataLiteralRealizationVisitor {
				public:
					CDataRoleSetVisitor(std::function<bool(CRole*)> roleFilterFunction) {
						mRoleFilterFunction = roleFilterFunction;
					}
					virtual bool visitDataRole(CRole* dataRole, bool deterministic) {
						if (dataRole) {
							return mRoleFilterFunction(dataRole);
						}
						return true;
					}
					virtual bool visitDataLiteral(CRole* dataRole, CDataLiteral* dataLiteral, bool deterministic) {
						return true;
					}
				protected:
					std::function<bool(CRole*)> mRoleFilterFunction;
			};


			class CDataRoleNamesVisitor : public Realization::CRoleRealizationRoleVisitor {
				public:
					CDataRoleNamesVisitor(QStringList* nameList, bool abbreviatedIRIs) {
						mNameList = nameList;
						mAbbreviatedIRIs = abbreviatedIRIs;
					}
					virtual bool visitRole(CRole* role, Realization::CRoleRealization* roleRealization) {
						if (role) {
							QString roleName;
							if (mAbbreviatedIRIs) {
								roleName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(role->getPropertyNameLinker());
							}
							if (roleName.isEmpty()) {
								roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
							}
							if (!roleName.isEmpty() && !mNameList->contains(roleName)) {
								mNameList->append(roleName);
							}
						}
						return true;
					}
				protected:
					QStringList* mNameList;
					bool mAbbreviatedIRIs;
			};


			class CDataIndividualNamesVisitor : public Realization::CSameRealizationIndividualVisitor {
				public:
					CDataIndividualNamesVisitor(CIndividualNameResolver* indiNameResolver, bool abbreviatedIRIs, bool integrateAnonymousIndividuals, QStringList* nameList) {
						mIndiNameResolver = indiNameResolver;
						mAbbreviatedIRIs = abbreviatedIRIs;
						mIntegrateAnonymousIndividuals = integrateAnonymousIndividuals;
						mNameList = nameList;
					}
					virtual bool visitIndividual(const CIndividualReference& indiRef, Realization::CSameRealization* sameRealization) {
						if (mIndiNameResolver && (mIntegrateAnonymousIndividuals || !mIndiNameResolver->isAnonymous(indiRef))) {
							QString individualName = mIndiNameResolver->getIndividualName(indiRef, mAbbreviatedIRIs);
							if (!individualName.isEmpty() && !mNameList->contains(individualName)) {
								mNameList->append(individualName);
							}
						}
						return true;
					}
				protected:
					CIndividualNameResolver* mIndiNameResolver;
					bool mAbbreviatedIRIs;
					bool mIntegrateAnonymousIndividuals;
					QStringList* mNameList;
			};


			CDataPropertyFactsQuery::CDataPropertyFactsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, DATAPROPERTYFACTSQUERYKIND queryKind, const CIndividualReference& individualReference, const QString& individualName, CRole* dataRole, CDataLiteral* dataLiteral, bool negative, const QString& queryNameString)
					: CRealizationPremisingQuery(ontology,configuration) {
				mQueryKind = queryKind;

				mIndividualReference = individualReference;
				mIndividualName = individualName;
				mDataRole = dataRole;
				mDataRoleName = QString();
				if (dataRole && dataRole->hasPropertyName()) {
					mDataRoleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
				}
				mDataLiteral = dataLiteral;
				mNegative = negative;

				queryName = queryNameString;
				if (queryName.isEmpty()) {
					switch (mQueryKind) {
						case DPFQK_DATAPROPERTIESOFSOURCE: queryString = QString("Get Data Properties of Source '%1'").arg(mIndividualName); break;
						case DPFQK_DATAPROPERTIESBETWEEN: queryString = QString("Get Data Properties Between Individual '%1' and a Literal").arg(mIndividualName); break;
						case DPFQK_DATAPROPERTIESOFLITERAL: queryString = QString("Get Data Properties of a Literal"); break;
						case DPFQK_DATAPROPERTYSOURCES: queryString = QString("Get Data Property Sources for Data Property '%1'").arg(mDataRoleName); break;
						case DPFQK_FLATTENEDDATAPROPERTYSOURCES: queryString = QString("Get Flattened Data Property Sources for Data Property '%1'").arg(mDataRoleName); break;
					}
				} else {
					queryString = queryName;
				}

				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration, "Konclude.OWLlink.AbbreviatedIRIs", false);
				mAnonymousIndividuals = CConfigDataReader::readConfigBoolean(configuration, "Konclude.OWLlink.ResultAnonymousIndividualsIntegration", false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresRoleRealisation = true;
				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;
				mRequiresDataRoleClassification = true;

				mCalcConfig = configuration;
			}


			CDataPropertyFactsQuery::~CDataPropertyFactsQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CDataPropertyFactsQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult* CDataPropertyFactsQuery::constructResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = nullptr;
				}
				mAddedPropertySynsetKeySet.clear();
				mAddedIndividualSynsetKeySet.clear();
				if (realization && realization->getDataLiteralRealization()) {
					switch (mQueryKind) {
						case DPFQK_DATAPROPERTIESOFSOURCE:
						case DPFQK_DATAPROPERTIESBETWEEN:
						case DPFQK_DATAPROPERTIESOFLITERAL:
							mResult = constructPropertySynsetsResult(realization);
							break;
						case DPFQK_DATAPROPERTYSOURCES:
							mResult = constructIndividualSynsetsResult(realization, false);
							break;
						case DPFQK_FLATTENEDDATAPROPERTYSOURCES:
							mResult = constructIndividualSynsetsResult(realization, true);
							break;
					}
				}
				return mResult;
			}


			CQueryResult* CDataPropertyFactsQuery::constructPropertySynsetsResult(CRealization* realization) {
				Realization::CDataLiteralRealization* dataLiteralRealization = realization->getDataLiteralRealization();
				QSet<CRole*> dataRoleSet;

				if (mQueryKind == DPFQK_DATAPROPERTIESOFSOURCE) {
					if (mIndividualReference.isNonEmpty()) {
						CDataRoleSetVisitor roleVisitor([&](CRole* dataRole)->bool {
							dataRoleSet.insert(dataRole);
							return true;
						});
						dataLiteralRealization->visitDataRoles(mIndividualReference, &roleVisitor);
					}
				} else if (mQueryKind == DPFQK_DATAPROPERTIESBETWEEN) {
					if (mIndividualReference.isNonEmpty() && mDataLiteral) {
						CDataRoleSetVisitor roleVisitor([&](CRole* dataRole)->bool {
							if (isDataLiteralInstance(realization, dataRole, mIndividualReference)) {
								dataRoleSet.insert(dataRole);
							}
							return true;
						});
						dataLiteralRealization->visitDataRoles(mIndividualReference, &roleVisitor);
					}
				} else if (mQueryKind == DPFQK_DATAPROPERTIESOFLITERAL) {
					if (mDataLiteral) {
						cint64 individualCount = mOntology->getABox()->getIndividualCount();
						for (cint64 individualId = 0; individualId < individualCount; ++individualId) {
							CIndividual* individual = mOntology->getABox()->getIndividualVector()->getData(individualId);
							if (!individual) {
								continue;
							}
							CIndividualReference individualReference(individual, individualId);
							CDataRoleSetVisitor roleVisitor([&](CRole* dataRole)->bool {
								if (isDataLiteralInstance(realization, dataRole, individualReference)) {
									dataRoleSet.insert(dataRole);
								}
								return true;
							});
							dataLiteralRealization->visitDataRoles(individualReference, &roleVisitor);
						}
					}
				}

				CPropertySynsetsResult* result = new CPropertySynsetsResult(true);
				collectPropertySynsets(realization, dataRoleSet, result);

				// owl:topDataProperty relates every individual to every literal, so it is part of
				// every data property synset answer (OWLlink specification)
				CRole* topDataRole = mOntology->getRBox()->getTopDataRole();
				if (topDataRole) {
					CPropertySynsetResult* topSynset = getPropertySynset(realization, topDataRole);
					if (topSynset) {
						QStringList topNames = topSynset->getEquivalentPropertyNameList();
						topNames.sort();
						QString topKey = topNames.join("|");
						if (!mAddedPropertySynsetKeySet.contains(topKey)) {
							mAddedPropertySynsetKeySet.insert(topKey);
							result->addPropertySynset(topSynset);
						} else {
							delete topSynset;
						}
					}
				}
				return result;
			}


			bool CDataPropertyFactsQuery::collectPropertySynsets(CRealization* realization, const QSet<CRole*>& dataRoleSet, CPropertySynsetsResult* result) {
				for (CRole* dataRole : dataRoleSet) {
					CPropertySynsetResult* synset = getPropertySynset(realization, dataRole);
					if (!synset) {
						continue;
					}
					QStringList names = synset->getEquivalentPropertyNameList();
					names.sort();
					QString key = names.join("|");
					if (mAddedPropertySynsetKeySet.contains(key)) {
						delete synset;
						continue;
					}
					mAddedPropertySynsetKeySet.insert(key);
					result->addPropertySynset(synset);
				}
				return true;
			}


			CPropertySynsetResult* CDataPropertyFactsQuery::getPropertySynset(CRealization* realization, CRole* dataRole) {
				QStringList nameList;
				Realization::CRoleRealization* roleRealization = realization ? realization->getRoleRealization() : nullptr;
				if (roleRealization && dataRole) {
					CRoleInstantiatedItem* roleItem = roleRealization->getRoleInstantiatedItem(dataRole);
					if (roleItem) {
						CDataRoleNamesVisitor roleVisitor(&nameList, mUseAbbreviatedIRIs);
						roleRealization->visitRoles(roleItem, &roleVisitor);
					}
				}
				if (nameList.isEmpty()) {
					QString dataRoleName = getDataRoleName(dataRole);
					if (!dataRoleName.isEmpty()) {
						nameList.append(dataRoleName);
					}
				}
				if (nameList.isEmpty()) {
					return nullptr;
				}
				CPropertySynsetResult* synset = new CPropertySynsetResult(true);
				synset->addEquivalentPropertyNames(nameList);
				return synset;
			}


			CQueryResult* CDataPropertyFactsQuery::constructIndividualSynsetsResult(CRealization* realization, bool flattened) {
				Realization::CDataLiteralRealization* dataLiteralRealization = realization->getDataLiteralRealization();
				cint64 individualCount = mOntology->getABox()->getIndividualCount();
				CIndividualSynsetsResult* synsetsResult = nullptr;
				CIndividualsResult* individualsResult = nullptr;
				if (flattened) {
					individualsResult = new CIndividualsResult();
				} else {
					synsetsResult = new CIndividualSynsetsResult();
				}

				for (cint64 individualId = 0; individualId < individualCount; ++individualId) {
					CIndividual* individual = mOntology->getABox()->getIndividualVector()->getData(individualId);
					if (!individual) {
						continue;
					}
					CIndividualReference individualReference(individual, individualId);
					bool hasValue = isDataLiteralInstance(realization, mDataRole, individualReference);
					if (mNegative) {
						hasValue = !hasValue;
					}
					if (!hasValue) {
						continue;
					}
					if (!mAnonymousIndividuals && mOntology->getIndividualNameResolver()->isAnonymous(individualReference)) {
						continue;
					}
					if (flattened) {
						QString individualName = getIndividualName(individualReference);
						if (!individualName.isEmpty()) {
							individualsResult->addIndividual(individualName);
						}
					} else {
						CIndividualSynsetResult* synset = getIndividualSynset(realization, individualReference);
						if (synset) {
							QStringList names = synset->getEquivalentIndividualNameList();
							names.sort();
							QString key = names.join("|");
							if (!mAddedIndividualSynsetKeySet.contains(key)) {
								mAddedIndividualSynsetKeySet.insert(key);
								synsetsResult->addIndividualSynset(synset);
							} else {
								delete synset;
							}
						}
					}
				}

				if (flattened) {
					return individualsResult;
				}
				return synsetsResult;
			}


			CIndividualSynsetResult* CDataPropertyFactsQuery::getIndividualSynset(CRealization* realization, const CIndividualReference& indiRef) {
				QStringList nameList;
				Realization::CSameRealization* sameRealization = realization ? realization->getSameRealization() : nullptr;
				if (sameRealization) {
					CDataIndividualNamesVisitor individualVisitor(mOntology->getIndividualNameResolver(), mUseAbbreviatedIRIs, mAnonymousIndividuals, &nameList);
					sameRealization->visitSameIndividuals(indiRef, &individualVisitor);
				}
				if (nameList.isEmpty()) {
					QString individualName = getIndividualName(indiRef);
					if (!individualName.isEmpty()) {
						nameList.append(individualName);
					}
				}
				if (nameList.isEmpty()) {
					return nullptr;
				}
				return new CIndividualSynsetResult(nameList);
			}


			bool CDataPropertyFactsQuery::isDataLiteralInstance(CRealization* realization, CRole* dataRole, const CIndividualReference& indiRef) {
				if (!dataRole || !mDataLiteral || !realization) {
					return false;
				}
				if (isTopDataRole(dataRole)) {
					// the top data property relates every individual to every literal
					return true;
				}
				Realization::CDataLiteralRealization* dataLiteralRealization = realization->getDataLiteralRealization();
				if (!dataLiteralRealization) {
					return false;
				}
				return dataLiteralRealization->isDataLiteralInstance(dataRole, indiRef, mDataLiteral);
			}


			bool CDataPropertyFactsQuery::isTopDataRole(CRole* dataRole) {
				CRole* topDataRole = mOntology->getRBox()->getTopDataRole();
				return topDataRole && topDataRole == dataRole;
			}


			QString CDataPropertyFactsQuery::getDataRoleName(CRole* dataRole) {
				QString dataRoleName;
				if (dataRole && dataRole->hasPropertyName()) {
					if (mUseAbbreviatedIRIs) {
						dataRoleName = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(dataRole->getPropertyNameLinker());
					}
					if (dataRoleName.isEmpty()) {
						dataRoleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
					}
				}
				return dataRoleName;
			}


			QString CDataPropertyFactsQuery::getIndividualName(const CIndividualReference& indiRef) {
				CIndividualNameResolver* nameResolver = mOntology->getIndividualNameResolver();
				if (nameResolver) {
					return nameResolver->getIndividualName(indiRef, mUseAbbreviatedIRIs);
				}
				return QString();
			}


			QString CDataPropertyFactsQuery::getQueryName() {
				return queryName;
			}

			QString CDataPropertyFactsQuery::getQueryString() {
				return queryString;
			}

			bool CDataPropertyFactsQuery::hasAnswer() {
				return mResult != nullptr;
			}

			QString CDataPropertyFactsQuery::getAnswerString() {
				if (!mResult) {
					if (mQueryKind == DPFQK_DATAPROPERTYSOURCES || mQueryKind == DPFQK_FLATTENEDDATAPROPERTYSOURCES) {
						return QString("Unknown Individuals");
					}
					return QString("Unknown Data Properties");
				}
				return mResult->getQueryResultString();
			}

			bool CDataPropertyFactsQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CDataPropertyFactsQuery::getDynamicRealizationRequirement() {
				if (mQueryKind == DPFQK_DATAPROPERTIESOFSOURCE || mQueryKind == DPFQK_DATAPROPERTIESBETWEEN) {
					return new COntologyProcessingDataLiteralRealizationRequirement(mDataRole, mIndividualReference);
				}
				// the reverse direction asks about all named individuals, so their data values are realized
				return new COntologyProcessingDataLiteralRealizationRequirement(mDataRole, CIndividualReference());
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

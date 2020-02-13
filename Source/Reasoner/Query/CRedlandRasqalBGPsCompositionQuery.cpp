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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRedlandRasqalBGPsCompositionQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CRedlandRasqalBGPsCompositionQuery::CRedlandRasqalBGPsCompositionQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QSet<QString>& dependentKBNames, const QString& queryText, librdf_world* redlandWorld, librdf_query* redlandQuery, rasqal_query* rasqalQuery, CConfigurationBase* configuration, const QString& queryName) : CComplexAnsweringCompositionQuery(baseOntology, expressionOntology,configuration,queryName) {
				mDependentKBNames = dependentKBNames;
				mRedlandWorld = redlandWorld;
				mRedlandQuery = redlandQuery;
				mRasqalQuery = rasqalQuery;
				mQueryText = queryText;
			}

			CRedlandRasqalBGPsCompositionQuery::~CRedlandRasqalBGPsCompositionQuery() {
			}


			QString CRedlandRasqalBGPsCompositionQuery::getQueryText() {
				return mQueryText;
			}


			QSet<QString>* CRedlandRasqalBGPsCompositionQuery::getDependentKnowledgeBaseNames() {
				return &mDependentKBNames;
			}

			librdf_world* CRedlandRasqalBGPsCompositionQuery::getRedlandWorld() {
				return mRedlandWorld;
			}

			librdf_query* CRedlandRasqalBGPsCompositionQuery::getRedlandQuery() {
				return mRedlandQuery;
			}

			rasqal_query* CRedlandRasqalBGPsCompositionQuery::getRasqalQuery() {
				return mRasqalQuery;
			}

			QHash<QString, TDependentOntologyPair>* CRedlandRasqalBGPsCompositionQuery::getDependentKnowledgeBaseRevisionHash() {
				return &mDependentKnowledgeBaseRevisionHash;
			}

			CRedlandRasqalBGPsCompositionQuery* CRedlandRasqalBGPsCompositionQuery::setDependentKnowledgeBaseRevision(const QString& kbName, TDependentOntologyPair depOntPair) {
				mDependentKnowledgeBaseRevisionHash.insert(kbName, depOntPair);
				return this;
			}

			CRedlandRasqalBGPsCompositionQuery* CRedlandRasqalBGPsCompositionQuery::setDependentKnowledgeBaseRevision(const QString& kbName, CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology) {
				TDependentOntologyPair depOntPair(baseOntology, expressionOntology);
				return setDependentKnowledgeBaseRevision(kbName, depOntPair);
			}


			CConcreteOntology* CRedlandRasqalBGPsCompositionQuery::getDependentBaseKnowledgeBaseRevision(const QString& kbName) {
				TDependentOntologyPair depOntPair = mDependentKnowledgeBaseRevisionHash.value(kbName);
				return depOntPair.first;
			}

			CConcreteOntology* CRedlandRasqalBGPsCompositionQuery::getDependentExpressionKnowledgeBaseRevisionBase(const QString& kbName) {
				TDependentOntologyPair depOntPair = mDependentKnowledgeBaseRevisionHash.value(kbName);
				return depOntPair.second;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION

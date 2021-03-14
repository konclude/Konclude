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

#include "CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper(COntologyBuilder* ontologyBuilder) : CConcreteOntologyRedlandTriplesDataExpressionMapper(ontologyBuilder) {
				mConfExtractSimpleABoxAssertions = true;
			}

			CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::~CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper() {
			}




			CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper* CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::useEntitiesVariableExpressionMapping(const QHash<QString, CBuildExpression*>& freshEntityUseVariableExpressionHash, CTriplesData* tripleData) {

				CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);

				librdf_model* model = redlandTriplesData->getRedlandIndexedModel();
				librdf_statement *partial_statement = librdf_new_statement(redlandTriplesData->getRedlandWorld());
				librdf_world* world = redlandTriplesData->getRedlandWorld();


				for (QHash<QString, CBuildExpression*>::const_iterator it = freshEntityUseVariableExpressionHash.constBegin(), itEnd = freshEntityUseVariableExpressionHash.constEnd(); it != itEnd; ++it) {
					const QString& entityString = it.key();
					CBuildExpression* buildExp = it.value();
					if (dynamic_cast<CClassVariableExpression*>(buildExp)) {
						insertEntityUsingVariableExpression(buildExp, entityString, world, mClassNodeIdentifierDataHash, mClassNodeHandlingList);
					} else if (dynamic_cast<CObjectPropertyVariableExpression*>(buildExp)) {
						insertEntityUsingVariableExpression(buildExp, entityString, world, mObjectPropertyNodeIdentifierDataHash, mObjectPropertyNodeHandlingList);
					} else if (dynamic_cast<CDataPropertyVariableExpression*>(buildExp)) {
						insertEntityUsingVariableExpression(buildExp, entityString, world, mDataPropertyNodeIdentifierDataHash, mDataPropertyNodeHandlingList);
					} else if (dynamic_cast<CIndividualVariableExpression*>(buildExp)) {
						insertEntityUsingVariableExpression(buildExp, entityString, world, mNamedIndividualNodeIdentifierDataHash, mNamedIndividualNodeHandlingList);
					} else if (dynamic_cast<CDataLiteralVariableExpression*>(buildExp)) {
						QList<CRedlandNodeProcessingData*> nodeHandlingList;
						insertEntityUsingVariableExpression(buildExp, entityString, world, mDataLiteralNodeIdentifierDataHash, nodeHandlingList);
					}
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper* CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::insertEntityUsingVariableExpression(CBuildExpression* expression, const QString& entityUri, librdf_world* world, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList) {
				QByteArray uriData = entityUri.toUtf8();
				librdf_uri* uri = librdf_new_uri(world, (const unsigned char*)uriData.constData());
				librdf_node* classNode = librdf_new_node_from_uri(world, uri);

				CRedlandNodeProcessingData*& processingData = nodeIdentifierDataHash[CRedlandNodeHasher(classNode)];
				if (!processingData) {
					processingData = new CRedlandNodeProcessingData(classNode, expression);
					nodeHandlingList.append(processingData);
					processingData->mExpression = expression;
				}
				return this;
			}



			QList<CAxiomExpression*> CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::takeQueryingCollectedAxiomExpressions() {
				QList<CAxiomExpression*> tmpList = mQueryingCollectedAxiomExpList;
				mQueryingCollectedAxiomExpList.clear();
				return tmpList;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper::handleParsedOntologyAxiomExpression(CAxiomExpression* axiomExpression) {
				mQueryingCollectedAxiomExpList.append(axiomExpression);
				return this;
			}




		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

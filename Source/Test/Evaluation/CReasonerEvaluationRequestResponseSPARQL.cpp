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

#include "CReasonerEvaluationRequestResponseSPARQL.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			CReasonerEvaluationRequestResponseSPARQL::CReasonerEvaluationRequestResponseSPARQL(const CSPARQLKnowledgeBaseSplittingOperationData& operationData) : mOperationData(operationData) {
			}


			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::setOperationData(const CSPARQLKnowledgeBaseSplittingOperationData& operationData) {
				mOperationData = operationData;
				return this;
			}

			CSPARQLKnowledgeBaseSplittingOperationData* CReasonerEvaluationRequestResponseSPARQL::getRequestOperationData() {
				return &mOperationData;
			}


			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::setRequestNode(const QString& reqNode) {
				mRequestNode = reqNode;
				return this;
			}

			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::addResponseNode(const QDomElement& resNode) {
				mResponseNodes.append(resNode);
				return this;
			}

			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::setModifiedNode(const QString& resNode) {
				mModifiedNode = resNode;
				return this;
			}

			QString CReasonerEvaluationRequestResponseSPARQL::getRequestString() {
				return mRequestNode;
			}

			QList<QDomElement> CReasonerEvaluationRequestResponseSPARQL::getResponseNodes() {
				return mResponseNodes;
			}

			QString CReasonerEvaluationRequestResponseSPARQL::getModifiedNode() {
				return mModifiedNode;
			}

			QString CReasonerEvaluationRequestResponseSPARQL::getOperationName() {
				return mOperationName;
			}

			QString CReasonerEvaluationRequestResponseSPARQL::getOperationMode() {
				return mOperationMode;
			}

			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::setOperationName(const QString& operationName) {
				mOperationName = operationName;
				return this;
			}

			CReasonerEvaluationRequestResponseSPARQL* CReasonerEvaluationRequestResponseSPARQL::setOperationMode(const QString& operationMode) {
				mOperationMode = operationMode;
				return this;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

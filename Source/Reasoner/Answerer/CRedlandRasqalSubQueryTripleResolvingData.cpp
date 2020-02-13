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

#include "CRedlandRasqalSubQueryTripleResolvingData.h"
#include "CRedlandRasqalSubQueryProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalSubQueryTripleResolvingData::CRedlandRasqalSubQueryTripleResolvingData(CRedlandRasqalSubQueryProcessingData* subQueryData, cint64 triplePos, cint64 subjectPos, cint64 predicatePos, cint64 objectPos) {
				mSubQueryData = subQueryData;
				mTriplePos = triplePos;
				mPos[0] = subjectPos;
				mPos[1] = predicatePos;
				mPos[2] = objectPos;
				mResolved = false;
				mVariableBound[0] = false;
				mVariableBound[1] = false;
				mVariableBound[2] = false;
				mVariable[0] = nullptr;
				mVariable[1] = nullptr;
				mVariable[2] = nullptr;
				mBoundNode[0] = nullptr;
				mBoundNode[1] = nullptr;
				mBoundNode[2] = nullptr;
				mBoundLiteral[0] = nullptr;
				mBoundLiteral[1] = nullptr;
				mBoundLiteral[2] = nullptr;
				mTransferringStatement[0] = nullptr;
				mTransferringStatement[1] = nullptr;
				mTransferringStatement[2] = nullptr;
			}


			CRedlandRasqalSubQueryTripleResolvingData::~CRedlandRasqalSubQueryTripleResolvingData() {
			}

			CRedlandRasqalSubQueryProcessingData* CRedlandRasqalSubQueryTripleResolvingData::getSubQueryProcessingData() {
				return mSubQueryData;
			}


			bool CRedlandRasqalSubQueryTripleResolvingData::isResolved() {
				return mResolved;
			}


			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setResolved(bool resolved) {
				mResolved = resolved;
				return this;
			}

			cint64 CRedlandRasqalSubQueryTripleResolvingData::getTriplePosition() {
				return mTriplePos;
			}

			cint64 CRedlandRasqalSubQueryTripleResolvingData::getSubjectBindingsVectorResolvingPosition() {
				return mPos[0];
			}

			cint64 CRedlandRasqalSubQueryTripleResolvingData::getPredicateBindingsVectorResolvingPosition() {
				return mPos[1];
			}

			cint64 CRedlandRasqalSubQueryTripleResolvingData::getObjectBindingsVectorResolvingPosition() {
				return mPos[2];
			}


			bool CRedlandRasqalSubQueryTripleResolvingData::getSubjectVariableBound() {
				return mVariableBound[0];
			}

			bool CRedlandRasqalSubQueryTripleResolvingData::getPredicateVariableBound() {
				return mVariableBound[1];
			}

			bool CRedlandRasqalSubQueryTripleResolvingData::getObjectVariableBound() {
				return mVariableBound[2];
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setSubjectVariableBound(bool bound) {
				mVariableBound[0] = bound;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setPredicateVariableBound(bool bound) {
				mVariableBound[1] = bound;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setObjectVariableBound(bool bound) {
				mVariableBound[2] = bound;
				return this;
			}

			cint64 CRedlandRasqalSubQueryTripleResolvingData::getBindingsVectorResolvingPosition(cint64 pos) {
				return mPos[pos];
			}

			bool CRedlandRasqalSubQueryTripleResolvingData::getVariableBound(cint64 pos) {
				return mVariableBound[pos];
			}


			rasqal_variable* CRedlandRasqalSubQueryTripleResolvingData::getVariable(cint64 pos) {
				return mVariable[pos];
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setVariable(cint64 pos, rasqal_variable* var) {
				mVariable[pos] = var;
				return this;
			}



			rasqal_variable* CRedlandRasqalSubQueryTripleResolvingData::getSubjectVariable() {
				return mVariable[0];
			}

			rasqal_variable* CRedlandRasqalSubQueryTripleResolvingData::getPredicateVariable() {
				return mVariable[1];
			}

			rasqal_variable* CRedlandRasqalSubQueryTripleResolvingData::getObjectVariable() {
				return mVariable[2];
			}


			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setSubjectVariable(rasqal_variable* var) {
				mVariable[0] = var;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setPredicateVariable(rasqal_variable* var) {
				mVariable[1] = var;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setObjectVariable(rasqal_variable* var) {
				mVariable[2] = var;
				return this;
			}


			librdf_node* CRedlandRasqalSubQueryTripleResolvingData::getBoundNode(cint64 pos) {
				return mBoundNode[pos];
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setBoundNode(cint64 pos, librdf_node* node) {
				mBoundNode[pos] = node;
				return this;
			}


			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setSubjectBoundNode(librdf_node* node) {
				mBoundNode[0] = node;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setPredicateBoundNode(librdf_node* node) {
				mBoundNode[1] = node;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setObjectBoundNode(librdf_node* node) {
				mBoundNode[2] = node;
				return this;
			}



			rasqal_literal* CRedlandRasqalSubQueryTripleResolvingData::getBoundLiteral(cint64 pos) {
				return mBoundLiteral[pos];
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setBoundLiteral(cint64 pos, rasqal_literal* node) {
				mBoundLiteral[pos] = node;
				return this;
			}


			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setSubjectBoundLiteral(rasqal_literal* node) {
				mBoundLiteral[0] = node;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setPredicateBoundLiteral(rasqal_literal* node) {
				mBoundLiteral[1] = node;
				return this;
			}

			CRedlandRasqalSubQueryTripleResolvingData* CRedlandRasqalSubQueryTripleResolvingData::setObjectBoundLiteral(rasqal_literal* node) {
				mBoundLiteral[2] = node;
				return this;
			}


			librdf_statement* CRedlandRasqalSubQueryTripleResolvingData::getNextTransferringStatement() {
				librdf_world* world = mSubQueryData->getRedlandWorld();
				librdf_statement*& statement = mTransferringStatement[mNextTransferrringStatementPos];
				if (!statement) {
					statement = librdf_new_statement(world);
				}
				if (++mNextTransferrringStatementPos >= 3) {
					mNextTransferrringStatementPos = 0;
				}
				return statement;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION

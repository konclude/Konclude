/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CConceptOperator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {



			CConceptOperator** CConceptOperator::mConceptOperatorVector = nullptr;
			QMutex* CConceptOperator::mCreateLockMutex = new QMutex();


			CConceptOperator::CConceptOperator(cint64 opCode) {
				mOperatorCode = opCode;
				if (mOperatorCode == CCNONE) {
					mTypeFlag = CCF_NONE;
				} else if (mOperatorCode == CCTOP) {
					mTypeFlag = CCF_TOP;
				} else if (mOperatorCode == CCBOTTOM) {
					mTypeFlag = CCF_BOTTOM;
				} else if (mOperatorCode == CCNOT) {
					mTypeFlag = CCF_NOT;
				} else if (mOperatorCode == CCAND) {
					mTypeFlag = CCF_AND;
				} else if (mOperatorCode == CCOR) {
					mTypeFlag = CCF_OR;
				} else if (mOperatorCode == CCATMOST) {
					mTypeFlag = CCF_ATMOST;
				} else if (mOperatorCode == CCATLEAST) {
					mTypeFlag = CCF_ATLEAST;
				} else if (mOperatorCode == CCALL) {
					mTypeFlag = CCF_ALL;
				} else if (mOperatorCode == CCSOME) {
					mTypeFlag = CCF_SOME;
				} else if (mOperatorCode == CCEQ) {
					mTypeFlag = CCF_EQ;
				} else if (mOperatorCode == CCSUB) {
					mTypeFlag = CCF_SUB;
				} else if (mOperatorCode == CCNOMINAL) {
					mTypeFlag = CCF_NOMINAL;
				} else if (mOperatorCode == CCSELF) {
					mTypeFlag = CCF_SELF;
				} else if (mOperatorCode == CCAQCHOOCE) {
					mTypeFlag = CCF_AQCHOOCE;
				} else if (mOperatorCode == CCAQALL) {
					mTypeFlag = CCF_AQALL;
				} else if (mOperatorCode == CCAQSOME) {
					mTypeFlag = CCF_AQSOME;
				} else if (mOperatorCode == CCAQAND) {
					mTypeFlag = CCF_AQAND;
				} else if (mOperatorCode == CCVALUE) {
					mTypeFlag = CCF_VALUE;
				} else if (mOperatorCode == CCNOMVAR) {
					mTypeFlag = CCF_NOMVAR;
				} else if (mOperatorCode == CCNOMTEMPLREF) {
					mTypeFlag = CCF_NOMTEMPLREF;
				} else if (mOperatorCode == CCIMPL) {
					mTypeFlag = CCF_IMPL;
				} else if (mOperatorCode == CCIMPLTRIG) {
					mTypeFlag = CCF_IMPLTRIG;
				} else if (mOperatorCode == CCIMPLALL) {
					mTypeFlag = CCF_IMPLALL;
				} else if (mOperatorCode == CCIMPLAQALL) {
					mTypeFlag = CCF_IMPLAQALL;
				} else if (mOperatorCode == CCIMPLAQAND) {
					mTypeFlag = CCF_IMPLAQAND;
				} else if (mOperatorCode == CCBRANCHIMPL) {
					mTypeFlag = CCF_BRANCHIMPL;
				} else if (mOperatorCode == CCBRANCHTRIG) {
					mTypeFlag = CCF_BRANCHTRIG;
				} else if (mOperatorCode == CCBRANCHALL) {
					mTypeFlag = CCF_BRANCHALL;
				} else if (mOperatorCode == CCBRANCHAQALL) {
					mTypeFlag = CCF_BRANCHAQALL;
				} else if (mOperatorCode == CCBRANCHAQAND) {
					mTypeFlag = CCF_BRANCHAQAND;
				} else if (mOperatorCode == CCEQCAND) {
					mTypeFlag = CCF_EQCAND;
				} else if (mOperatorCode == CCPBINDTRIG) {
					mTypeFlag = CCF_PBINDTRIG;
				} else if (mOperatorCode == CCPBINDIMPL) {
					mTypeFlag = CCF_PBINDIMPL;
				} else if (mOperatorCode == CCPBINDGROUND) {
					mTypeFlag = CCF_PBINDGROUND;
				} else if (mOperatorCode == CCPBINDALL) {
					mTypeFlag = CCF_PBINDALL;
				} else if (mOperatorCode == CCPBINDAND) {
					mTypeFlag = CCF_PBINDAND;
				} else if (mOperatorCode == CCPBINDAQAND) {
					mTypeFlag = CCF_PBINDAQAND;
				} else if (mOperatorCode == CCPBINDAQALL) {
					mTypeFlag = CCF_PBINDAQALL;
				} else if (mOperatorCode == CCPBINDVARIABLE) {
					mTypeFlag = CCF_PBINDVARIABLE;
				} else if (mOperatorCode == CCPBINDCYCLE) {
					mTypeFlag = CCF_PBINDCYCLE;
				} else if (mOperatorCode == CCVARBINDTRIG) {
					mTypeFlag = CCF_VARBINDTRIG;
				} else if (mOperatorCode == CCVARBINDJOIN) {
					mTypeFlag = CCF_VARBINDJOIN;
				} else if (mOperatorCode == CCVARBINDGROUND) {
					mTypeFlag = CCF_VARBINDGROUND;
				} else if (mOperatorCode == CCVARBINDALL) {
					mTypeFlag = CCF_VARBINDALL;
				} else if (mOperatorCode == CCVARBINDAND) {
					mTypeFlag = CCF_VARBINDAND;
				} else if (mOperatorCode == CCVARBINDAQAND) {
					mTypeFlag = CCF_VARBINDAQAND;
				} else if (mOperatorCode == CCVARBINDAQALL) {
					mTypeFlag = CCF_VARBINDAQALL;
				} else if (mOperatorCode == CCVARBINDVARIABLE) {
					mTypeFlag = CCF_VARBINDVARIABLE;
				} else if (mOperatorCode == CCVARBINDIMPL) {
					mTypeFlag = CCF_VARBINDIMPL;
				} else if (mOperatorCode == CCVARPBACKTRIG) {
					mTypeFlag = CCF_VARPBACKTRIG;
				} else if (mOperatorCode == CCVARPBACKALL) {
					mTypeFlag = CCF_VARPBACKALL;
				} else if (mOperatorCode == CCVARPBACKAQAND) {
					mTypeFlag = CCF_VARPBACKAQAND;
				} else if (mOperatorCode == CCVARPBACKAQALL) {
					mTypeFlag = CCF_VARPBACKAQALL;
				} else if (mOperatorCode == CCBACKACTIVTRIG) {
					mTypeFlag = CCF_BACKACTIVTRIG;
				} else if (mOperatorCode == CCBACKACTIVIMPL) {
					mTypeFlag = CCF_BACKACTIVIMPL;
				}
			}

			cint64 CConceptOperator::getOperatorCode() {
				return mOperatorCode;
			}


			CConceptOperator* CConceptOperator::getConceptOperator(cint64 opCode) {
				CConceptOperator* conOp = nullptr;
				if (!mConceptOperatorVector) {
					mCreateLockMutex->lock();
					if (!mConceptOperatorVector) {
						generateConceptOperators();
					}
					mCreateLockMutex->unlock();
				}
				conOp = mConceptOperatorVector[opCode];
				return conOp;
			}


			bool CConceptOperator::hasPartialOperatorCodeFlag(cint64 operatorCodeFlag) {
				return (mTypeFlag & operatorCodeFlag) != 0;
			}

			bool CConceptOperator::hasAllOperatorCodeFlags(cint64 operatorCodeFlags) {
				return (mTypeFlag & operatorCodeFlags) != operatorCodeFlags;
			}



			void CConceptOperator::generateConceptOperators() {
				cint64 operatorCount = 200;
				mConceptOperatorVector = new CConceptOperator*[operatorCount];
				for (cint64 i = 0; i < operatorCount; ++i) {
					mConceptOperatorVector[i] = nullptr;
				}
				mConceptOperatorVector = &mConceptOperatorVector[operatorCount/2];

				mConceptOperatorVector[CCNONE] = new CConceptOperator(CCNONE);
				mConceptOperatorVector[CCTOP] = new CConceptOperator(CCTOP);
				mConceptOperatorVector[CCBOTTOM] = new CConceptOperator(CCBOTTOM);
				mConceptOperatorVector[CCNOT] = new CConceptOperator(CCNOT);
				mConceptOperatorVector[CCAND] = new CConceptOperator(CCAND);
				mConceptOperatorVector[CCOR] = new CConceptOperator(CCOR);
				mConceptOperatorVector[CCATMOST] = new CConceptOperator(CCATMOST);
				mConceptOperatorVector[CCATLEAST] = new CConceptOperator(CCATLEAST);
				mConceptOperatorVector[CCALL] = new CConceptOperator(CCALL);
				mConceptOperatorVector[CCSOME] = new CConceptOperator(CCSOME);
				mConceptOperatorVector[CCEQ] = new CConceptOperator(CCEQ);
				mConceptOperatorVector[CCSUB] = new CConceptOperator(CCSUB);
				mConceptOperatorVector[CCNOMINAL] = new CConceptOperator(CCNOMINAL);
				mConceptOperatorVector[CCSELF] = new CConceptOperator(CCSELF);
				mConceptOperatorVector[CCAQCHOOCE] = new CConceptOperator(CCAQCHOOCE);
				mConceptOperatorVector[CCAQALL] = new CConceptOperator(CCAQALL);
				mConceptOperatorVector[CCAQSOME] = new CConceptOperator(CCAQSOME);
				mConceptOperatorVector[CCAQAND] = new CConceptOperator(CCAQAND);
				mConceptOperatorVector[CCVALUE] = new CConceptOperator(CCVALUE);
				mConceptOperatorVector[CCNOMVAR] = new CConceptOperator(CCNOMVAR);
				mConceptOperatorVector[CCNOMTEMPLREF] = new CConceptOperator(CCNOMTEMPLREF);
				mConceptOperatorVector[CCIMPL] = new CConceptOperator(CCIMPL);
				mConceptOperatorVector[CCIMPLTRIG] = new CConceptOperator(CCIMPLTRIG);
				mConceptOperatorVector[CCIMPLALL] = new CConceptOperator(CCIMPLALL);
				mConceptOperatorVector[CCIMPLAQALL] = new CConceptOperator(CCIMPLAQALL);
				mConceptOperatorVector[CCIMPLAQAND] = new CConceptOperator(CCIMPLAQAND);
				mConceptOperatorVector[CCBRANCHIMPL] = new CConceptOperator(CCBRANCHIMPL);
				mConceptOperatorVector[CCBRANCHTRIG] = new CConceptOperator(CCBRANCHTRIG);
				mConceptOperatorVector[CCBRANCHALL] = new CConceptOperator(CCBRANCHALL);
				mConceptOperatorVector[CCBRANCHAQALL] = new CConceptOperator(CCBRANCHAQALL);
				mConceptOperatorVector[CCBRANCHAQAND] = new CConceptOperator(CCBRANCHAQAND);
				mConceptOperatorVector[CCEQCAND] = new CConceptOperator(CCEQCAND);
				mConceptOperatorVector[CCPBINDTRIG] = new CConceptOperator(CCPBINDTRIG);
				mConceptOperatorVector[CCPBINDIMPL] = new CConceptOperator(CCPBINDIMPL);
				mConceptOperatorVector[CCPBINDGROUND] = new CConceptOperator(CCPBINDGROUND);
				mConceptOperatorVector[CCPBINDALL] = new CConceptOperator(CCPBINDALL);
				mConceptOperatorVector[CCPBINDAND] = new CConceptOperator(CCPBINDAND);
				mConceptOperatorVector[CCPBINDAQAND] = new CConceptOperator(CCPBINDAQAND);
				mConceptOperatorVector[CCPBINDAQALL] = new CConceptOperator(CCPBINDAQALL);
				mConceptOperatorVector[CCPBINDVARIABLE] = new CConceptOperator(CCPBINDVARIABLE);
				mConceptOperatorVector[CCPBINDCYCLE] = new CConceptOperator(CCPBINDCYCLE);
				mConceptOperatorVector[CCVARBINDTRIG] = new CConceptOperator(CCVARBINDTRIG);
				mConceptOperatorVector[CCVARBINDJOIN] = new CConceptOperator(CCVARBINDJOIN);
				mConceptOperatorVector[CCVARBINDGROUND] = new CConceptOperator(CCVARBINDGROUND);
				mConceptOperatorVector[CCVARBINDALL] = new CConceptOperator(CCVARBINDALL);
				mConceptOperatorVector[CCVARBINDAQAND] = new CConceptOperator(CCVARBINDAQAND);
				mConceptOperatorVector[CCVARBINDAQALL] = new CConceptOperator(CCVARBINDAQALL);
				mConceptOperatorVector[CCVARBINDVARIABLE] = new CConceptOperator(CCVARBINDVARIABLE);
				mConceptOperatorVector[CCVARBINDIMPL] = new CConceptOperator(CCVARBINDIMPL);
				mConceptOperatorVector[CCVARPBACKTRIG] = new CConceptOperator(CCVARPBACKTRIG);
				mConceptOperatorVector[CCVARPBACKALL] = new CConceptOperator(CCVARPBACKALL);
				mConceptOperatorVector[CCVARPBACKAQAND] = new CConceptOperator(CCVARPBACKAQAND);
				mConceptOperatorVector[CCVARPBACKAQALL] = new CConceptOperator(CCVARPBACKAQALL);
				mConceptOperatorVector[CCBACKACTIVTRIG] = new CConceptOperator(CCBACKACTIVTRIG);
				mConceptOperatorVector[CCBACKACTIVIMPL] = new CConceptOperator(CCBACKACTIVIMPL);
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

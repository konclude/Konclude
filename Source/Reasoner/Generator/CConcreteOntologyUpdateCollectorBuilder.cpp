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

#include "CConcreteOntologyUpdateCollectorBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {

			CConcreteOntologyUpdateCollectorBuilder::CConcreteOntologyUpdateCollectorBuilder(CConcreteOntology* updateConcreteOntology) : CConcreteOntologyBuildDataUpdater(updateConcreteOntology) {
				LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcretOntologyUpdateBuilder",logTr("ConcretOntologyUpdateBuilder initialized."),this);
			}

			CConcreteOntologyUpdateCollectorBuilder::~CConcreteOntologyUpdateCollectorBuilder() {
			}





			bool CConcreteOntologyUpdateCollectorBuilder::initializeBuilding() {
				CConcreteOntologyBuildDataUpdater::initializeBuilding();

				mTellAxiomSet = mOntoBuild->getTellAxiomSet();
				mRetractAxiomSet = mOntoBuild->getRetractAxiomSet();

				mTellUpdatedAxiomSet = mOntoBuild->getUpdatedTellAxiomSet();
				mRetractUpdatedAxiomSet = mOntoBuild->getUpdatedRetractAxiomSet();

				mChangeAxiomList = mOntoBuild->getChangeAxiomList();
				mChangeUpdatedAxiomList = mOntoBuild->getUpdatedChangeAxiomList();

				return true;
			}



			bool CConcreteOntologyUpdateCollectorBuilder::completeBuilding() {

				mOntoBuild->setNextAxiomNumber(mNextAxiomNumber+mNextMaxAxiomNumberOffset);
				mOntoBuild->setNextEntityNumber(mNextEntityNumber);

				FOREACHIT (CAxiomExpression* axiomExp, *mRetractUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				}
				FOREACHIT (CAxiomExpression* axiomExp, *mTellUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				}


				return true;
			}



			bool CConcreteOntologyUpdateCollectorBuilder::tellOntologyAxiom(CAxiomExpression* axiom) {
				if (!mTellAxiomSet->contains(axiom)) {
					mRetractAxiomSet->remove(axiom);
					mTellAxiomSet->insert(axiom);
					if (mRetractUpdatedAxiomSet->contains(axiom)) {
						mRetractUpdatedAxiomSet->remove(axiom);
					} else {
						mTellUpdatedAxiomSet->insert(axiom);
					}
				}
				return true;
			}

			bool CConcreteOntologyUpdateCollectorBuilder::retractOntologyAxiom(CAxiomExpression* axiom) {
				if (mTellAxiomSet->contains(axiom)) {
					mTellAxiomSet->remove(axiom);
					mRetractAxiomSet->insert(axiom);
					if (mTellUpdatedAxiomSet->contains(axiom)) {
						mTellUpdatedAxiomSet->remove(axiom);
					} else {
						mRetractUpdatedAxiomSet->insert(axiom);
					}
				}
				return true;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
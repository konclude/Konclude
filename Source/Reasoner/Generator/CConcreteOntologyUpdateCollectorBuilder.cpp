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

				mLocTellUpdatedAxiomSet.clear();
				mLocRetractUpdatedAxiomSet.clear();
				mLocAddedImportOntologies.clear();

				return true;
			}



			bool CConcreteOntologyUpdateCollectorBuilder::completeBuilding() {

				mOntoBuild->setNextAxiomNumber(mNextAxiomNumber+mNextMaxAxiomNumberOffset);
				mOntoBuild->setNextEntityNumber(mNextEntityNumber);

				FOREACHIT (CAxiomExpression* axiomExp, mLocRetractUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,false) );
				}
				FOREACHIT (CAxiomExpression* axiomExp, mLocTellUpdatedAxiomSet) {
					mChangeUpdatedAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
					mChangeAxiomList->append( QPair<CAxiomExpression*,bool>(axiomExp,true) );
				}


				return true;
			}



			bool CConcreteOntologyUpdateCollectorBuilder::tellOntologyAxiom(CAxiomExpression* axiom) {
				if (!mTellAxiomSet->contains(axiom)) {
					mRetractAxiomSet->remove(axiom);
					mTellAxiomSet->insert(axiom);
					if (mLocRetractUpdatedAxiomSet.contains(axiom)) {
						mRetractUpdatedAxiomSet->remove(axiom);
						mLocRetractUpdatedAxiomSet.remove(axiom);
					} else {
						mTellUpdatedAxiomSet->insert(axiom);
						mLocTellUpdatedAxiomSet.insert(axiom);
					}
				}
				return true;
			}

			bool CConcreteOntologyUpdateCollectorBuilder::retractOntologyAxiom(CAxiomExpression* axiom) {
				if (mTellAxiomSet->contains(axiom)) {
					mTellAxiomSet->remove(axiom);
					mRetractAxiomSet->insert(axiom);
					if (mLocTellUpdatedAxiomSet.contains(axiom)) {
						mTellUpdatedAxiomSet->remove(axiom);
						mLocTellUpdatedAxiomSet.remove(axiom);
					} else {
						mRetractUpdatedAxiomSet->insert(axiom);
						mLocRetractUpdatedAxiomSet.insert(axiom);
					}
				}
				return true;
			}



			bool CConcreteOntologyUpdateCollectorBuilder::addOntologyImport(const QStringRef& name) {
				if (CConcreteOntologyBuildDataUpdater::addOntologyImport(name)) {
					mLocAddedImportOntologies.append(name.toString());
					return true;
				}
				return false;
			}

			bool CConcreteOntologyUpdateCollectorBuilder::addOntologyImport(const QString& name) {
				if (CConcreteOntologyBuildDataUpdater::addOntologyImport(name)) {
					mLocAddedImportOntologies.append(name);
					return true;
				}
				return false;
			}


			QList<QString> CConcreteOntologyUpdateCollectorBuilder::takeAddedImportOntologyList() {
				QList<QString> tmpList(mLocAddedImportOntologies);
				mLocAddedImportOntologies.clear();
				return tmpList;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
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

#include "COntologyRevisionMultiPersistencer.h"
#include "CNameBasedOntologyRevisionPersistencerFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionMultiPersistencer::COntologyRevisionMultiPersistencer(CConfiguration* config) : CLogIdentifier("::Konclude::Reasoner::Persistence::MultiPersistencer", this) {
					mConfig = config;
					QStringList persistencerList = CConfigDataReader::readConfigStringList(mConfig, "Konclude.Persistence.MultiPersistencer.PersisterList");
					
					for (QString persistencerName : persistencerList) {
						CNameBasedOntologyRevisionPersistencerFactory factory(persistencerName, mConfig);
						COntologyRevisionPersistencer* persistencer = factory.createOntologyRevisionPersistencer();
						mPersisterList.append(persistencer);
					}
				}

				COntologyRevisionMultiPersistencer::~COntologyRevisionMultiPersistencer() {
				}





				bool COntologyRevisionMultiPersistencer::persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev) {
					bool successfullyWritten = true;
					for (COntologyRevisionPersistencer* persister : mPersisterList) {
						successfullyWritten &= persister->persistOntologyRevision(ontoRev, prevOntoRev);
					}
					return successfullyWritten;
				}






				bool COntologyRevisionMultiPersistencer::hasOntologyPersisted(const QString& ontologyName) {
					for (COntologyRevisionPersistencer* persister : mPersisterList) {
						if (persister->hasOntologyPersisted(ontologyName)) {
							return true;
						}
					}
					return false;
				}



				bool COntologyRevisionMultiPersistencer::deletePersistedOntology(const QString& ontologyName) {
					bool successfullyDeleted = true;
					for (COntologyRevisionPersistencer* persister : mPersisterList) {
						successfullyDeleted &= persister->deletePersistedOntology(ontologyName);
					}
					return successfullyDeleted;
				}


				bool COntologyRevisionMultiPersistencer::hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {
					for (COntologyRevisionPersistencer* persister : mPersisterList) {
						if (persister->hasNewOntologyRevision(ontologyName, ontoRev)) {
							return true;
						}
					}
					return false;
				}



				bool COntologyRevisionMultiPersistencer::loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {
					bool successfullyLoaded = true;
					for (COntologyRevisionPersistencer* persister : mPersisterList) {
						if (persister->hasNewOntologyRevision(ontologyName, ontoRev)) {
							successfullyLoaded &= persister->deletePersistedOntology(ontologyName);
						}
					}
					return successfullyLoaded;
				}


			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

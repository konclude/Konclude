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

#include "COntologyRevisionDirectoryOWLRDFFilePersistencer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionDirectoryOWLRDFFilePersistencer::COntologyRevisionDirectoryOWLRDFFilePersistencer(CConfiguration* config) : COntologyRevisionDirectoryFilePersistencerBase(config) {
					QString confOWLRDFBaseDirectory = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWLRDFFilePersistencer.BaseDirectory","./Data/Ontologies/Persistence/");
					if (!confOWLRDFBaseDirectory.isEmpty()) {
						mConfBaseDirectory = confOWLRDFBaseDirectory;
					}
					mFileEndingWritingName = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWLRDFFilePersistencer.WritingFileEnding");
					mSerializingFormat = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWLRDFFilePersistencer.SerializationFormat");
				}

				COntologyRevisionDirectoryOWLRDFFilePersistencer::~COntologyRevisionDirectoryOWLRDFFilePersistencer() {
				}




				bool COntologyRevisionDirectoryOWLRDFFilePersistencer::writeOntologyToFile(QFile* file, COntologyRevision* ontoRev) {

					CConcreteOntology* ontology = ontoRev->getOntology();

					COntologyAxiomExpressionsRenderVisitor renderer;
					COntologyRedlandRDFRenderer rdfRenderer(nullptr);


					CONTOLOGYAXIOMSET<CAxiomExpression*>* currAxiomSet = ontology->getBuildData()->getTellAxiomSet();
					CQtList<CAxiomExpression*> axiomList;
					for (CAxiomExpression* axiomExp : *currAxiomSet) {
						axiomList.append(axiomExp);
					}

					renderer.visitAndRender(axiomList, ontology, &rdfRenderer);

					QList<CTriplesData*> triplesDataList;
					COntologyTriplesData* ontTriplesData = ontoRev->getOntology()->getOntologyTriplesData();
					if (ontTriplesData) {
						for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
							triplesDataList.append(tripleData);
						}
					}

					renderer.visitAndRender(&triplesDataList, ontology, &rdfRenderer);


					CRedlandStoredTriplesData* triplesData = rdfRenderer.getRenderedTripleData(nullptr);
					if (triplesData) {

						QByteArray serFormByteArray = mSerializingFormat.toUtf8();

						unsigned char* modelString = librdf_model_to_string(triplesData->getRedlandIndexedModel(), nullptr, serFormByteArray.constData(), nullptr, nullptr);
						file->write((const char*)modelString);

						delete triplesData;

						return true;
					}

					return false;
				}




			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

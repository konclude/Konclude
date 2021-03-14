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

#include "COntologyRevisionDirectoryFilePersistencerBase.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionDirectoryFilePersistencerBase::COntologyRevisionDirectoryFilePersistencerBase(CConfiguration* config) : CLogIdentifier("::Konclude::Reasoner::Persistence::DirectoryOWL2XMLFilePersistencer", this) {
					mConfig = config;
					mOWLNS = "http://www.w3.org/2002/07/owl#";
					mConfBaseDirectory = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryFilePersistencer.BaseDirectory","./Data/Ontologies/Persistence/");
					
					mFileEndingWritingName = ".owl";

					mFileEndingLoadingName = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.OntologyRevisionPersistencer.LoadingFileEnding", ".*");

					mKeepingVersionLimit = CConfigDataReader::readConfigInteger(mConfig, "Konclude.Persistence.OntologyRevisionPersistencer.KeepingVersionsLimit", -1);
				}

				COntologyRevisionDirectoryFilePersistencerBase::~COntologyRevisionDirectoryFilePersistencerBase() {
				}




				QString COntologyRevisionDirectoryFilePersistencerBase::getNextPersistingOntologyRevisionString(const QString& ontologyName, const QStringList& pathSepStrings, COntologyRevision* ontoRev) {
					QString lastVersionFileNameString = mOntologyNameLastFileHash.value(ontologyName);

					QString dirName = pathSepStrings.join('/');
					QString fileNameStart = pathSepStrings.join('_') + ".";
					QString fileNameEnd = mFileEndingWritingName;

					lastVersionFileNameString.remove(0,fileNameStart.length());
					lastVersionFileNameString.remove(fileNameEnd);
					lastVersionFileNameString.remove("R");
					cint64 lastVersion = lastVersionFileNameString.toLongLong();

					cint64 newVersion = lastVersion+1;

					QString newVersionString = QString("R%1").arg(newVersion);
					return newVersionString;
				}



				bool COntologyRevisionDirectoryFilePersistencerBase::persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev) {


					if (mOntologyNameFileHash.contains(prevOntoRev) && ontoRev->getOntology()->getBuildData()->getChangeAxiomList()->size() == prevOntoRev->getOntology()->getBuildData()->getChangeAxiomList()->size()) {
						COntologyTriplesData* triplesData = ontoRev->getOntology()->getOntologyTriplesData();
						COntologyTriplesData* prevTriplesData = prevOntoRev->getOntology()->getOntologyTriplesData();
						if (prevTriplesData && triplesData && triplesData->getLatestTriplesData(false) == prevTriplesData->getLatestTriplesData(false) || !prevTriplesData && !triplesData) {
							mOntologyNameFileHash.insert(ontoRev, mOntologyNameFileHash.value(prevOntoRev));
							return true;
						}
					}

					CConcreteOntology* ontology = ontoRev->getOntology();
					QString ontName = ontology->getOntologyName();
					cint64 ontoVers = ontoRev->getOntologyRevisionDepth();

					QStringList pathSepStrings = getOntologyFileNameParts(ontName);

					QString dirName = pathSepStrings.join('/');
					QString revName = getNextPersistingOntologyRevisionString(ontName, pathSepStrings, ontoRev);
					QString fileName = pathSepStrings.join('_') + "." + revName;
					QString owlFileName = fileName + mFileEndingWritingName;
					QString tmpFileName = fileName + "-tmp-XXXXXX";

					if (QFile::exists(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(owlFileName))) {
						return false;
					}


					bool successfullyWritten = false;
					QDir baseDir(mConfBaseDirectory);
					baseDir.mkpath(dirName);
					QTemporaryFile tmpFile(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(tmpFileName));
					tmpFile.setAutoRemove(false);
					if (tmpFile.open()) {
						LOG(INFO, getLogDomain(), logTr("Trying saving new revision %3 of ontology '%1' to file '%2'").arg(ontName).arg(owlFileName).arg(ontoVers), this);
						bool successfulWritten = writeOntologyToFile(&tmpFile, ontoRev);
						tmpFile.close();
						if (successfulWritten) {
							LOG(INFO, getLogDomain(), logTr("Saved new revision %3 of ontology '%1' to file '%2'").arg(ontName).arg(owlFileName).arg(ontoVers), this);
							successfullyWritten = true;
						} else {
							LOG(ERROR, getLogDomain(), logTr("Failed to save new revision %3 of ontology '%1' to file '%2'").arg(ontName).arg(owlFileName).arg(ontoVers), this);
						}
					}

					if (successfullyWritten) {
						successfullyWritten = tmpFile.rename(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(owlFileName));
						if (!successfullyWritten) {
							tmpFile.remove();
						} else {
							mOntologyNameFileHash.insert(ontoRev, owlFileName);
							mOntologyNameLastFileHash.insert(ontName, owlFileName);
						}
						deletePersistingLimitOntologyVersions(ontName);
					}


					return successfullyWritten;
				}






				bool COntologyRevisionDirectoryFilePersistencerBase::hasOntologyPersisted(const QString& ontologyName) {
					QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);
					QString dirName = pathSepStrings.join('/');
					QString fileName = pathSepStrings.join('_');
					QString owlFileNameFilter = fileName + "*" + mFileEndingLoadingName;


					QStringList nameFilter(owlFileNameFilter);
					QDir directory(mConfBaseDirectory + dirName);
					if (!directory.exists()) {
						return false;
					}
					QStringList ontoFiles = directory.entryList(nameFilter, QDir::Files);
					if (ontoFiles.isEmpty()) {
						return false;
					}
					return true;
				}



				bool COntologyRevisionDirectoryFilePersistencerBase::deletePersistedOntology(const QString& ontologyName) {
					QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);
					QString dirName = pathSepStrings.join('/');
					QString fileName = pathSepStrings.join('_');
					QString owlFileNameFilter = fileName + "*" + mFileEndingLoadingName;


					QStringList nameFilter(owlFileNameFilter);
					QDir directory(mConfBaseDirectory + dirName);
					if (directory.exists()) {
						return false;
					}
					QStringList ontoFiles = directory.entryList(nameFilter, QDir::Files);
					if (ontoFiles.isEmpty()) {
						return false;
					}
					for (QString ontoFile : ontoFiles) {
						QFile file(mConfBaseDirectory + dirName + "/" + ontoFile);
						file.remove();
					}
					
					return true;
				}





				cint64 isOntologyFileNameNewer(const QString& currentOntologyName, const QString& newOntologyName) {
					if (currentOntologyName.length() > newOntologyName.length()) {
						return -1;
					}
					if (currentOntologyName.length() < newOntologyName.length()) {
						return 1;
					}
					int comp = QString::compare(currentOntologyName, newOntologyName, Qt::CaseInsensitive);
					return comp;
				}


				bool COntologyRevisionDirectoryFilePersistencerBase::hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {

					QString lastVersionFileNameString = mOntologyNameFileHash.value(ontoRev);
					QString lastFileNameString = mOntologyNameLastFileHash.value(ontologyName);
					if (lastVersionFileNameString != lastFileNameString) {
						return true;
					}

					QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);


					QString dirName = pathSepStrings.join('/');
					QString fileName = pathSepStrings.join('_');
					QString owlFileNameFilter = fileName + "*" + mFileEndingLoadingName;


					QStringList nameFilter(owlFileNameFilter);
					QDir directory(mConfBaseDirectory+dirName);
					QStringList ontoFiles = directory.entryList(nameFilter,QDir::Files);


					QString newFileNameString = lastFileNameString;
					for (const QString& fileName : ontoFiles) {
						if (isOntologyFileNameNewer(lastFileNameString,fileName) > 0) {
							newFileNameString = fileName;
						}
					}
					if (newFileNameString != lastFileNameString) {
						mOntologyNameLastFileHash.insert(ontologyName,newFileNameString);
						return true;
					}
					return false;
				}






				QStringList COntologyRevisionDirectoryFilePersistencerBase::getPersistedOntologyVersionFilesSortedLatest(const QString& ontologyName) {
					QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);


					QString dirName = pathSepStrings.join('/');
					QString fileName = pathSepStrings.join('_');
					QString owlFileNameFilter = fileName + "*" + mFileEndingLoadingName;


					QStringList nameFilter(owlFileNameFilter);
					QDir directory(mConfBaseDirectory + dirName);
					QStringList ontoVersionFiles = directory.entryList(nameFilter, QDir::Files);

					qSort(ontoVersionFiles.begin(), ontoVersionFiles.end(), isOntologyFileNameNewer);
					return ontoVersionFiles;
				}


				bool COntologyRevisionDirectoryFilePersistencerBase::deletePersistingLimitOntologyVersions(const QString& ontologyName) {

					if (mKeepingVersionLimit > 0) {
						QStringList ontoVersionFiles = getPersistedOntologyVersionFilesSortedLatest(ontologyName);
						if (ontoVersionFiles.size() > mKeepingVersionLimit) {
							QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);

							QString dirName = pathSepStrings.join('/');
							QString fileName = pathSepStrings.join('_');

							QStringList::const_iterator ontoVersionFilesIterator = ontoVersionFiles.constBegin();
							for (cint64 versionNumber = 0; versionNumber < ontoVersionFiles.size(); ++versionNumber) {
								if (versionNumber >= mKeepingVersionLimit) {
									QString ontoFile = *ontoVersionFilesIterator;
									QFile file(mConfBaseDirectory + dirName + "/" + ontoFile);
									file.remove();
								}
								++ontoVersionFilesIterator;
							}
						}
					}
					return false;
				}



				bool COntologyRevisionDirectoryFilePersistencerBase::loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {


					if (hasNewOntologyRevision(ontologyName,ontoRev)) {

						QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);
						QString dirName = pathSepStrings.join('/');

						QString lastVersionFileNameString = mOntologyNameLastFileHash.value(ontologyName);

						CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ontoRev->getOntology());
						builder->initializeBuilding();

						bool parsingSucceeded = false;

						QString fileNmae = mConfBaseDirectory + dirName + "/" + lastVersionFileNameString;
						QFile ontoVersionFile(fileNmae);

						LOG(INFO, getLogDomain(), logTr("Trying loading last revision of ontology '%1' from file '%2'").arg(ontologyName).arg(fileNmae), this);
						if (ontoVersionFile.open(QIODevice::ReadOnly)) {
							ontoVersionFile.close();

							parsingSucceeded = loadOntologyFromFile(ontologyName, fileNmae, &ontoVersionFile, ontoRev, builder);
							if (parsingSucceeded) {
								builder->completeBuilding();
								mOntologyNameFileHash.insert(ontoRev, lastVersionFileNameString);
							} else {
								LOG(ERROR, getLogDomain(), logTr("Parsing of file '%1' failed.").arg(lastVersionFileNameString), this);
							}
						} else {
							LOG(ERROR, getLogDomain(), logTr("File '%1' cannot be opened.").arg(lastVersionFileNameString), this);
						}

						delete builder;


						return parsingSucceeded;
					} else {
						return false;
					}
				}


				QStringList COntologyRevisionDirectoryFilePersistencerBase::getOntologyFileNameParts(const QString& ontologyName) {
					QStringList pathSepStrings;
					QString currPartString;
					for (cint64 i = 0; i < ontologyName.length(); ++i) {
						QChar partChar = ontologyName.at(i);
						bool separator = false;
						if (partChar == ':' || partChar == '/' || partChar == '\\' || partChar == '?' || partChar == '*' || partChar == '%' || partChar == '$' || partChar == '<' || partChar == '>' || partChar == '|' || partChar == '\"') {
							separator = true;
						}
						if (separator) {
							if (!currPartString.isEmpty()) {
								pathSepStrings += currPartString;
							}
							currPartString = QString();
						} else {
							currPartString += partChar;
						}
					}
					if (!currPartString.isEmpty()) {
						pathSepStrings += currPartString;
					}
					return pathSepStrings;
				}



				bool COntologyRevisionDirectoryFilePersistencerBase::loadOntologyFromFile(const QString& ontologyString, const QString& fileNameString, QFile* file, COntologyRevision* ontoRev, CConcreteOntologyUpdateCollectorBuilder *builder) {
					COntologyMultiAutoParsingLoader loader;
					QList<QString> importOntologiesList;
					QStringList parserErrorList;
					CCommandRecordRouter commandRecordRouter(nullptr, this);
					bool loaded = loader.parseOntology(file, ontologyString, fileNameString, ontoRev->getOntology(), builder, ontoRev->getOntologyConfiguration(), importOntologiesList, parserErrorList, commandRecordRouter);
					return loaded;
				}


			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

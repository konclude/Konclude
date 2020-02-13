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

#include "CSPOntologyRevisionManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {


			CSPOntologyRevisionManager::CSPOntologyRevisionManager(COntologyRevisionPersistencer* persistencer) : CLogIdentifier("::Konclude::Reasoner::Revision::SPOntologyRevisionManager",this) {
				classificationMan = 0;
				nextOntologyID = 0;
				nextRevisionPathID = 0;
				nextUnamedOntologyNumber = 0;
				confProvider = 0;
				mBaseOnto = nullptr;

				mLastOntologyRevisionUpdateKBNameOrNewAnonymous = QString("http://Konclude.com/Ontologies/$LastCreatedKnowledgeBaseIfAvailableAndCreateNewAnonymousKnowledgeBaseOtherwise");
				mLastOntologyRevisionUpdateKBName = QString("http://Konclude.com/Ontologies/?LastRevisionUpdateKnowledgeBase");
				mLastOntologyCreatedKBName = QString("http://Konclude.com/Ontologies/?LastCreatedKnowledgeBase");
				mFirstOntologyRevisionUpdateKBName = QString("http://Konclude.com/Ontologies/$FirstRevisionUpdateKnowledgeBase");
				mFirstOntologyCreatedKBName = QString("http://Konclude.com/Ontologies/$FirstCreatedKnowledgeBase");

				mHasFirstCreatedOntology = false;
				mHasFirstOntologyRevisionUpdate = false;

				mOntoRevPersistencer = persistencer;
			}


			CSPOntologyRevisionManager::~CSPOntologyRevisionManager() {
				qDeleteAll(onRevContainer);
			}


			COntologyRevisionManager *CSPOntologyRevisionManager::initializeManager(CConfigurationProvider *configurationProvider) {
				confProvider = configurationProvider;
				classificationMan = CConfigManagerReader::readClassificationManagerConfig(confProvider);
				return this;
			}


			qint64 CSPOntologyRevisionManager::getOntologyCount() {
				return ontologyCount;
			}



			CConcreteOntology* CSPOntologyRevisionManager::getBasementOntology(CCommandRecordRouter& commandRecordRouter) {
				CConcreteOntology* usedBasementOntology = nullptr;

				if (mBaseOnto) {
					QString resolveBasementKBNameString = CConfigDataReader::readConfigString(confProvider,"Konclude.Ontology.Revision.BasementResolveKBName");
					if (!resolveBasementKBNameString.isEmpty()) {
						qint64 revPath = nameIDHash.value(resolveBasementKBNameString);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {
							usedBasementOntology = currOntologyRev->getOntology();
						}
					}
				}

				if (!usedBasementOntology) {
					if (!mBaseOnto) {
						CUnspecifiedMessageInformationRecord::makeRecord("Creating ontology basement.",&commandRecordRouter);
						mBaseOnto = new CConcreteOntology(confProvider->getCurrentConfiguration());
						CConcreteOntologyBasementBuilder* basementOntoBuilder = new CConcreteOntologyBasementBuilder(mBaseOnto);
						basementOntoBuilder->initializeBuilding();
						basementOntoBuilder->buildOntologyBasement();
						basementOntoBuilder->completeBuilding();
						delete basementOntoBuilder;
						COntologyConfigDependedPreProcesser* preProcessor = new COntologyConfigDependedPreProcesser(commandRecordRouter);
						preProcessor->preprocess(mBaseOnto,confProvider->getCurrentConfiguration());
						delete preProcessor;
						CUnspecifiedMessageInformationRecord::makeRecord("Ontology basement created.",&commandRecordRouter);
					}
					usedBasementOntology = mBaseOnto;
				}
				return usedBasementOntology;
			}



			COntologyRevision* CSPOntologyRevisionManager::createNewOntologyRevisionFromBasementOntology(const QString& ontologyName, CCommandRecordRouter& commandRecordRouter) {
				qint64 ontologyID = nextOntologyID++;
				qint64 revPath = nextRevisionPathID++;

				COntologyConfigurationExtension *ontConfig = new COntologyConfigurationExtension(confProvider->getCurrentConfiguration(), 0);
				CConcreteOntology* baseOnto = getBasementOntology(commandRecordRouter);
				CConcreteOntology *ont = new CConcreteOntology(baseOnto, ontConfig);
				ont->setOntologyID(ontologyID);
				ont->setOntologyName(ontologyName);



				COntologyRevision *onRev = new COntologyRevision(ont, 0, ontConfig);
				onRev->setOntologyRevisionPath(revPath);
				onRev->setOntologyRevisionDepth(0);

				onRevContainer.insert(onRev);

				revisionHash.insert(revPath, onRev);
				nameIDHash.insert(ontologyName, revPath);

				if (!mHasFirstCreatedOntology) {
					nameIDHash.insert(mFirstOntologyCreatedKBName, revPath);
					mHasFirstCreatedOntology = true;
				}
				nameIDHash.insert(mLastOntologyRevisionUpdateKBName, revPath);
				nameIDHash.insert(mLastOntologyRevisionUpdateKBNameOrNewAnonymous, revPath);
				nameIDHash.insert(mLastOntologyCreatedKBName, revPath);


				if (mOntoRevPersistencer && mOntoRevPersistencer->hasNewOntologyRevision(ontologyName, onRev)) {
					LOGINFO(getLogDomain(), QString("Trying to load persisted version of KnowledgeBase '%1'.").arg(ontologyName), this);
					if (!mOntoRevPersistencer->loadLatestOntologyRevision(ontologyName, onRev)) {
						LOGWARN(getLogDomain(), QString("Failed to load persisted version of KnowledgeBase '%1'.").arg(ontologyName), this);
					}
					else {
						LOGINFO(getLogDomain(), QString("Successfully loaded newly persisted version of KnowledgeBase '%1'.").arg(ontologyName), this);
					}
				}

				ontologyCount++;
				return onRev;
			}



			CCommandDelegater *CSPOntologyRevisionManager::delegateCommand(CCommand *command) {
				if (dynamic_cast<CCreateKnowledgeBaseCommand *>(command)) {
					CCreateKnowledgeBaseCommand *cKBC = dynamic_cast<CCreateKnowledgeBaseCommand *>(command);

					CCommandRecordRouter commandRecordRouter(command,this);

					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = cKBC->getKnowledgeBaseName();
					kbName = createNewOntology(kbName, cKBC->getPrefixList(), commandRecordRouter);
					cKBC->setKnowledgeBaseName(kbName);


					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CCreateKnowledgeBaseRevisionUpdateCommand *>(command)) {
					CCreateKnowledgeBaseRevisionUpdateCommand *cKBRevUpC = (CCreateKnowledgeBaseRevisionUpdateCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = cKBRevUpC->getKnowledgeBaseName();
					bool forceCreation = cKBRevUpC->requireCreateIfNotExist();
					bool reportError = cKBRevUpC->reportErrorCreateIfNotExist();

					COntologyRevision* nextOntologyRev = createNewOntologyRevision(kbName, forceCreation, reportError, commandRecordRouter);
					cKBRevUpC->setOntologyRevision(nextOntologyRev);

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CReleaseKnowledgeBaseCommand *>(command)) {
					CReleaseKnowledgeBaseCommand *rKBRevUpC = (CReleaseKnowledgeBaseCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = rKBRevUpC->getKnowledgeBaseName();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' does not exist.").arg(kbName),&commandRecordRouter);
					} else {
						nameIDHash.remove(kbName);
					}
					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CGetKnowledgeBaseConfigCommand *>(command)) {
					CGetKnowledgeBaseConfigCommand *getConfigComm = (CGetKnowledgeBaseConfigCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = getConfigComm->getKnowlegeBaseString();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' does not exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {
							COntologyConfigurationExtension *config = currOntologyRev->getOntologyConfiguration();
							CConfigData *configData = config->getRelatedConfigChange(getConfigComm->getConfigNameString());
							getConfigComm->setConfigData(configData);
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(kbName),&commandRecordRouter);
						}
					}
					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CGetKnowledgeBaseSettingsCommand *>(command)) {
					CGetKnowledgeBaseSettingsCommand *getConfigComm = (CGetKnowledgeBaseSettingsCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = getConfigComm->getKnowlegeBaseString();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {
							COntologyConfigurationExtension *config = currOntologyRev->getOntologyConfiguration();
							getConfigComm->setConfiguration(config);
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(kbName),&commandRecordRouter);
						}
					}
					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CSetKnowledgeBaseConfigCommand *>(command)) {
					CSetKnowledgeBaseConfigCommand *setConfigComm = (CSetKnowledgeBaseConfigCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = setConfigComm->getKnowlegeBaseString();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {
							COntologyConfigurationExtension *config = currOntologyRev->getOntologyConfiguration();
							if (!config->isConfigLevelLocked(setConfigComm->getConfigNameString())) {
								CCreateKnowledgeBaseRevisionUpdateCommand *createUpdateRevComm = new CCreateKnowledgeBaseRevisionUpdateCommand(setConfigComm->getKnowlegeBaseString(),setConfigComm);
								delegateCommand(createUpdateRevComm);
								COntologyRevision *ontoRev = createUpdateRevComm->getOntologyRevision();
								if (ontoRev) {
									COntologyConfigurationExtension *config = ontoRev->getOntologyConfiguration();
									CConfigData *configData = config->createAndSetConfig(setConfigComm->getConfigNameString());
									if (configData) {
										if (configData->supportsStringConvertion()) {
											configData->readFromString(setConfigComm->getConfigValueString());
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Value for configuration property '%1' could not read from string.").arg(setConfigComm->getConfigNameString()),&commandRecordRouter);
										}
									} else {
										CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' not available.").arg(setConfigComm->getConfigNameString()),&commandRecordRouter);
									}
								} else {
									CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(setConfigComm->getKnowlegeBaseString()),&commandRecordRouter);
								}
								CInstallKnowledgeBaseRevisionUpdateCommand *installRevUpdateComm = new CInstallKnowledgeBaseRevisionUpdateCommand(setConfigComm->getKnowlegeBaseString(),createUpdateRevComm,setConfigComm);
								delegateCommand(installRevUpdateComm);								
							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' is locked.").arg(setConfigComm->getConfigNameString()),&commandRecordRouter);
							}
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(kbName),&commandRecordRouter);
						}
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CInstallKnowledgeBaseRevisionUpdateCommand *>(command)) {
					CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = (CInstallKnowledgeBaseRevisionUpdateCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = inKBRevUpC->getKnowledgeBaseName();
					COntologyRevision *nextOntRev = inKBRevUpC->getOntologyRevision();

					if (!nextOntRev) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("New KnowledgeBase '%1' revision not available.").arg(kbName),&commandRecordRouter);
					} else {

						qint64 nextRevPath = nextOntRev->getOntologyRevisionPath();
						qint64 nextRevDepth = nextOntRev->getOntologyRevisionDepth();

						if (!nameIDHash.contains(kbName)) {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
						} else {
							qint64 revPath = nameIDHash.value(kbName);
							COntologyRevision *ontologyRev = revisionHash.value(revPath,0);
							if (ontologyRev) {
								kbName = ontologyRev->getOntology()->getOntologyName();
								qint64 revDepth = ontologyRev->getOntologyRevisionDepth();
								if (nextRevDepth > revDepth+1) {

									COntologyRevision* prevOntRev = nextOntRev->getPreviousOntologyRevision();
									CConcreteOntology* prevOnt = prevOntRev->getOntology();

									CConcreteOntology* currOnt = ontologyRev->getOntology();
									CConcreteOntology* nextOnt = nextOntRev->getOntology();

									//CConcreteOntologyMergingBuilder mergingBuilder(currOnt,nextOnt,prevOnt);
									//mergingBuilder.initializeBuilding();
									//mergingBuilder.mergingBuilding();
									//mergingBuilder.completeBuilding();

									CConcreteOntologyMergingRebuildingBuilder mergingRebuildingBuilder(nextOnt);
									mergingRebuildingBuilder.initializeBuilding();
									mergingRebuildingBuilder.mergeOntology(currOnt, prevOnt);
									mergingRebuildingBuilder.completeBuilding();

									// install new knowledge base revision
									revisionHash.insert(revPath,nextOntRev);
									onRevContainer.insert(nextOntRev);
									CUnspecifiedMessageInformationRecord::makeRecord(QString("Merging KnowledgeBase '%1' because of concurrent modification.").arg(kbName),&commandRecordRouter);
								} else {
									// install new knowledge base revision
									revisionHash.insert(revPath,nextOntRev);
									onRevContainer.insert(nextOntRev);
								}
								if (!mHasFirstOntologyRevisionUpdate) {
									mHasFirstOntologyRevisionUpdate = true;
									nameIDHash.insert(mFirstOntologyCreatedKBName, revPath);
								}
								nameIDHash.insert(mLastOntologyRevisionUpdateKBName, revPath);
								nameIDHash.insert(mLastOntologyRevisionUpdateKBNameOrNewAnonymous, revPath);

								if (mOntoRevPersistencer) {
									int remainingPersistingTryCount = 3;
									while (!mOntoRevPersistencer->persistOntologyRevision(nextOntRev, ontologyRev) && remainingPersistingTryCount-- > 0) {
										LOGINFO(getLogDomain(),QString("Persisting of KnowledgeBase '%1' failed, probably due concurrently persisting another version.").arg(kbName),this);
										if (mOntoRevPersistencer->hasNewOntologyRevision(kbName,ontologyRev)) {
											if (!mOntoRevPersistencer->loadLatestOntologyRevision(kbName,nextOntRev)) {
												LOGERROR(getLogDomain(),QString("Loading of newly persisted KnowledgeBase '%1' failed.").arg(kbName),this);
												remainingPersistingTryCount = 0;
											} else {
												LOGINFO(getLogDomain(),QString("Successfully loaded newly persisted version of KnowledgeBase '%1'.").arg(kbName),this);
											}
										} else {
											LOGWARN(getLogDomain(),QString("No new version of KnowledgeBase '%1' has been persisted, repeating persisting attempt.").arg(kbName),this);
										}
									}
								}


							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(kbName),&commandRecordRouter);
							}
						}

					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CGetCurrentKnowledgeBaseRevisionCommand *>(command)) {
					CGetCurrentKnowledgeBaseRevisionCommand *gCurrKBRevUpC = (CGetCurrentKnowledgeBaseRevisionCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = gCurrKBRevUpC->getKnowledgeBaseName();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);


						if (mOntoRevPersistencer && mOntoRevPersistencer->hasNewOntologyRevision(kbName,currOntologyRev)) {

							qint64 ontologyID = nextOntologyID++;

							CConcreteOntology *currOnt = currOntologyRev->getOntology();
							COntologyConfigurationExtension *currOntConfig = currOntologyRev->getOntologyConfiguration();
							COntologyConfigurationExtension *nextOntConfig = new COntologyConfigurationExtension(currOntConfig,currOntConfig->getAdditionalConfigurationList(),currOntConfig->getConfigurationID()+1);

							CConcreteOntology *nextOnto = new CConcreteOntology(currOnt,nextOntConfig);
							nextOnto->setOntologyName(currOnt->getOntologyName());
							nextOnto->setOntologyID(ontologyID);

							COntologyRevision *nextOntologyRev = new COntologyRevision(nextOnto,currOntologyRev,nextOntConfig);
							nextOntologyRev->setOntologyRevisionDepth(currOntologyRev->getOntologyRevisionDepth()+1);


							LOGINFO(getLogDomain(),QString("Trying to load newly persisted version of KnowledgeBase '%1'.").arg(kbName),this);
							if (!mOntoRevPersistencer->loadLatestOntologyRevision(kbName,nextOntologyRev)) {
								LOGERROR(getLogDomain(),QString("Failed to load newly persisted version of KnowledgeBase '%1'.").arg(kbName),this);
								delete nextOntologyRev;
							} else {
								LOGINFO(getLogDomain(),QString("Successfully loaded newly persisted version of KnowledgeBase '%1'.").arg(kbName),this);
								revisionHash.insert(revPath,nextOntologyRev);
								onRevContainer.insert(nextOntologyRev);
								currOntologyRev = nextOntologyRev;
							}
						}


						if (currOntologyRev) {
							gCurrKBRevUpC->setOntologyRevision(currOntologyRev);
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
						}
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				}

				return this;
			}



			COntologyRevision* CSPOntologyRevisionManager::createNewOntologyRevision(const QString& ontologyName, bool forceCreation, bool reportError, CCommandRecordRouter& commandRecordRouter) {
				QString kbName = ontologyName;

				bool kbExist = nameIDHash.contains(kbName);

				bool createdKBForLastOntologyRevisionUpdateKBNameOrNewAnonymous = false;
				if (!kbExist && mLastOntologyRevisionUpdateKBNameOrNewAnonymous == kbName) {
					kbName = createNewOntology("", nullptr, commandRecordRouter);
					createdKBForLastOntologyRevisionUpdateKBNameOrNewAnonymous = true;
					kbExist = nameIDHash.contains(kbName);
				}

				if (!kbExist && !forceCreation) {
					if (reportError) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' does not exist.").arg(kbName), &commandRecordRouter);
					}
				} else {

					if (!kbExist) {
						createNewOntologyRevisionFromBasementOntology(kbName, commandRecordRouter);
					}

					qint64 revPath = nameIDHash.value(kbName);
					COntologyRevision *currOntologyRev = revisionHash.value(revPath, nullptr);
					if (currOntologyRev) {

						qint64 ontologyID = nextOntologyID++;

						CConcreteOntology *currOnt = currOntologyRev->getOntology();
						COntologyConfigurationExtension *currOntConfig = currOntologyRev->getOntologyConfiguration();
						COntologyConfigurationExtension *nextOntConfig = new COntologyConfigurationExtension(currOntConfig, currOntConfig->getAdditionalConfigurationList(), currOntConfig->getConfigurationID() + 1);

						CConcreteOntology *ont = new CConcreteOntology(currOnt, nextOntConfig);
						kbName = currOnt->getOntologyName();
						ont->setOntologyName(currOnt->getOntologyName());
						ont->setOntologyID(ontologyID);

						COntologyRevision *nextOntologyRev = new COntologyRevision(ont, currOntologyRev, nextOntConfig);
						nextOntologyRev->setOntologyRevisionDepth(currOntologyRev->getOntologyRevisionDepth() + 1);


						if (mOntoRevPersistencer && mOntoRevPersistencer->hasNewOntologyRevision(kbName, currOntologyRev)) {
							LOGINFO(getLogDomain(), QString("Trying to load newly persisted version of KnowledgeBase '%1'.").arg(kbName), this);
							if (!mOntoRevPersistencer->loadLatestOntologyRevision(kbName, nextOntologyRev)) {
								LOGERROR(getLogDomain(), QString("Failed to load newly persisted version of KnowledgeBase '%1'.").arg(kbName), this);
							}
							else {
								LOGINFO(getLogDomain(), QString("Successfully loaded newly persisted version of KnowledgeBase '%1'.").arg(kbName), this);
							}
						}

						if (createdKBForLastOntologyRevisionUpdateKBNameOrNewAnonymous) {
							nameIDHash.insert(mLastOntologyRevisionUpdateKBNameOrNewAnonymous, revPath);
						}


						return nextOntologyRev;
					} else {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' could not be resolved.").arg(kbName), &commandRecordRouter);
					}
				}

				return nullptr;
			}

			
			QString CSPOntologyRevisionManager::createNewOntology(const QString& ontologyName, QList<CNamePrefix*>* prefixList, CCommandRecordRouter& commandRecordRouter) {
				QString kbName = ontologyName;
				if (kbName.isEmpty()) {
					QString unKBName = QString("http://Konclude.com/Ontologies/UnnamedOntologies/%1/%2/").arg(nextUnamedOntologyNumber++).arg(QDateTime::currentMSecsSinceEpoch());
					while (nameIDHash.contains(unKBName)) {
						unKBName = QString("http://Konclude.com/Ontologies/UnnamedOntologies/%1/%2/").arg(nextUnamedOntologyNumber++).arg(QDateTime::currentMSecsSinceEpoch());
					}
					kbName = unKBName;
				}
				if (nameIDHash.contains(kbName)) {
					// ontology already exists
					CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' already exists.").arg(kbName), &commandRecordRouter);
				} else {

					COntologyRevision* ontRev = createNewOntologyRevisionFromBasementOntology(kbName, commandRecordRouter);

					if (prefixList) {
						COntologyStringMapping* ontoStrings = ontRev->getOntology()->getStringMapping();
						CMAPPINGHASH<QString, CNamePrefix*>* abbreviatedNamePrefixMapHash = ontoStrings->getAbbreviatedNamePrefixHash(true);
						CMAPPINGHASH<QString, CNamePrefix*>* namePrefixMapHash = ontoStrings->getNamePrefixHash(true);
						foreach(CNamePrefix* prefix, *prefixList) {
							CNamePrefix* newPrefix = new CNamePrefix(prefix->getAbbreviatedPrefixString(), prefix->getPrefixString());
							abbreviatedNamePrefixMapHash->insert(newPrefix->getAbbreviatedPrefixString(), newPrefix);
							namePrefixMapHash->insert(newPrefix->getPrefixString(), newPrefix);
						}
					}
				}

				return kbName;
			}


		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude


/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CSPOntologyRevisionManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {


			CSPOntologyRevisionManager::CSPOntologyRevisionManager() : CLogIdentifier("::Konclude::Reasoner::Revision::SPOntologyRevisionManager",this) {
				classificationMan = 0;
				nextOntologyID = 0;
				nextRevisionPathID = 0;
				nextUnamedOntologyNumber = 0;
				confProvider = 0;
				mBaseOnto = nullptr;
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


			CCommandDelegater *CSPOntologyRevisionManager::delegateCommand(CCommand *command) {
				if (dynamic_cast<CCreateKnowledgeBaseCommand *>(command)) {
					CCreateKnowledgeBaseCommand *cKBC = dynamic_cast<CCreateKnowledgeBaseCommand *>(command);

					CCommandRecordRouter commandRecordRouter(command,this);

					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = cKBC->getKnowledgeBaseName();

					if (kbName.isEmpty()) {
						QString unKBName = QString("http://Konclude.com/Ontologies/UnnamedOntologies/%1/%2/").arg(nextUnamedOntologyNumber++).arg(QDateTime::currentMSecsSinceEpoch());
						while (nameIDHash.contains(unKBName)) {
							unKBName = QString("http://Konclude.com/Ontologies/UnnamedOntologies/%1/%2/").arg(nextUnamedOntologyNumber++).arg(QDateTime::currentMSecsSinceEpoch());
						}
						kbName = unKBName;
					}
					cKBC->setKnowledgeBaseName(kbName);
					if (nameIDHash.contains(kbName)) {
						// ontology already exists
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' already exists.").arg(kbName),&commandRecordRouter);
					} else {

						qint64 ontologyID = nextOntologyID++;
						qint64 revPath = nextRevisionPathID++;

						COntologyConfigurationExtension *ontConfig = new COntologyConfigurationExtension(confProvider->getCurrentConfiguration(),0);
						CConcreteOntology* baseOnto = getBasementOntology(commandRecordRouter);
						CConcreteOntology *ont = new CConcreteOntology(baseOnto,ontConfig);
						ont->setOntologyID(ontologyID);
						ont->setOntologyName(kbName);

						if (cKBC->hasPrefixes()) {
							COntologyStringMapping* ontoStrings = ont->getStringMapping();
							CMAPPINGHASH<QString,CNamePrefix*>* abbreviatedNamePrefixMapHash = ontoStrings->getAbbreviatedNamePrefixHash(true);
							CMAPPINGHASH<QString,CNamePrefix*>* namePrefixMapHash = ontoStrings->getNamePrefixHash(true);
							QList<CNamePrefix*>* prefixList = cKBC->getPrefixList();
							foreach (CNamePrefix* prefix, *prefixList) {
								CNamePrefix* newPrefix = new CNamePrefix(prefix->getAbbreviatedPrefixString(),prefix->getPrefixString());
								abbreviatedNamePrefixMapHash->insert(newPrefix->getAbbreviatedPrefixString(),newPrefix);
								namePrefixMapHash->insert(newPrefix->getPrefixString(),newPrefix);
							}
						}


						COntologyRevision *onRev = new COntologyRevision(ont,0,ontConfig);
						onRev->setOntologyRevisionPath(revPath);

						onRevContainer.insert(onRev);

						revisionHash.insert(revPath,onRev);
						nameIDHash.insert(kbName,revPath);

						ontologyCount++;
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CCreateKnowledgeBaseRevisionUpdateCommand *>(command)) {
					CCreateKnowledgeBaseRevisionUpdateCommand *cKBRevUpC = (CCreateKnowledgeBaseRevisionUpdateCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = cKBRevUpC->getKnowledgeBaseName();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {

							qint64 ontologyID = nextOntologyID++;

							CConcreteOntology *currOnt = currOntologyRev->getOntology();
							COntologyConfigurationExtension *currOntConfig = currOntologyRev->getOntologyConfiguration();
							COntologyConfigurationExtension *nextOntConfig = new COntologyConfigurationExtension(currOntConfig,currOntConfig->getAdditionalConfigurationList(),currOntConfig->getConfigurationID()+1);

							CConcreteOntology *ont = new CConcreteOntology(currOnt,nextOntConfig);
							ont->setOntologyName(currOnt->getOntologyName());
							ont->setOntologyID(ontologyID);

							COntologyRevision *nextOntologyRev = new COntologyRevision(ont,currOntologyRev,nextOntConfig);


							cKBRevUpC->setOntologyRevision(nextOntologyRev);
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
						}
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

				} else if (dynamic_cast<CReleaseKnowledgeBaseCommand *>(command)) {
					CReleaseKnowledgeBaseCommand *rKBRevUpC = (CReleaseKnowledgeBaseCommand *)command;
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					QString kbName = rKBRevUpC->getKnowledgeBaseName();
					if (!nameIDHash.contains(kbName)) {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
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
						CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' doesn't exist.").arg(kbName),&commandRecordRouter);
					} else {
						qint64 revPath = nameIDHash.value(kbName);
						COntologyRevision *currOntologyRev = revisionHash.value(revPath,0);
						if (currOntologyRev) {
							COntologyConfigurationExtension *config = currOntologyRev->getOntologyConfiguration();
							CConfigData *configData = config->getRelatedConfigChange(getConfigComm->getConfigNameString());
							getConfigComm->setConfigData(configData);
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
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
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
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
									CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(setConfigComm->getKnowlegeBaseString()),&commandRecordRouter);
								}
								CInstallKnowledgeBaseRevisionUpdateCommand *installRevUpdateComm = new CInstallKnowledgeBaseRevisionUpdateCommand(setConfigComm->getKnowlegeBaseString(),createUpdateRevComm,setConfigComm);
								delegateCommand(installRevUpdateComm);								
							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' is locked.").arg(setConfigComm->getConfigNameString()),&commandRecordRouter);
							}
						} else {
							CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
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
								qint64 revDepth = ontologyRev->getOntologyRevisionDepth();
								if (nextRevDepth > revDepth+1) {

									COntologyRevision* prevOntRev = nextOntRev->getPreviousOntologyRevision();
									CConcreteOntology* prevOnt = prevOntRev->getOntology();

									CConcreteOntology* currOnt = ontologyRev->getOntology();
									CConcreteOntology* nextOnt = nextOntRev->getOntology();

									CConcreteOntologyMergingBuilder mergingBuilder(currOnt,nextOnt,prevOnt);
									mergingBuilder.initializeBuilding();
									mergingBuilder.mergingBuilding();
									mergingBuilder.completeBuilding();

									// install new knowledge base revision
									revisionHash.insert(revPath,nextOntRev);
									onRevContainer.insert(nextOntRev);
									CUnspecifiedMessageInformationRecord::makeRecord(QString("Merging KnowledgeBase '%1' because of concurrent modification.").arg(kbName),&commandRecordRouter);
								} else {
									// install new knowledge base revision
									revisionHash.insert(revPath,nextOntRev);
									onRevContainer.insert(nextOntRev);
								}
							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("KnowledgeBase '%1' couldn't be resolved.").arg(kbName),&commandRecordRouter);
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



		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude


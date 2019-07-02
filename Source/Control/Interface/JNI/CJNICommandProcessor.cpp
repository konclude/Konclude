/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CJNICommandProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNICommandProcessor::CJNICommandProcessor(CConfiguration* loaderConfig) : COWLlinkProcessor(false) {
					mLoaderConfig = loaderConfig;
					reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(mLoaderConfig);
					mProcessingCommand = nullptr;
					startThread();
				}


				CJNICommandProcessor::~CJNICommandProcessor() {
				}

				void CJNICommandProcessor::threadStarted() {
					COWLlinkProcessor::threadStarted();

				}

				void CJNICommandProcessor::threadStopped() {
					COWLlinkProcessor::threadStopped();
				}



				CJNICommandProcessor* CJNICommandProcessor::processCommand(CCommand* command, bool blockUntilProcessed) {
					CBlockingCallbackData* blockingCallback = nullptr;
					if (blockUntilProcessed) {
						blockingCallback = new CBlockingCallbackData();
						command->addProcessedCallback(blockingCallback);
					}
					LOG(INFO,"::Konclude::JNI::CommandProcessor",QString("Starting processing command '%1'.").arg(command->getBriefCommandDescription()),0);
					delegateCommand(command);
					LOG(INFO,"::Konclude::JNI::CommandProcessor",QString("Finished processing command '%1'.").arg(command->getBriefCommandDescription()),0);
					if (blockingCallback) {
						blockingCallback->waitForCallback();
						delete blockingCallback;
					}
					return this;
				}




				COntologyRevision* CJNICommandProcessor::getNewlyCreatedAnonymousOntologyRevision() {
					CCreateKnowledgeBaseCommand* ckbc = new CCreateKnowledgeBaseCommand("");
					processCommand(ckbc);
					QString kbString = ckbc->getKnowledgeBaseName();
					delete ckbc;
					CCreateKnowledgeBaseRevisionUpdateCommand* ckbruc = new CCreateKnowledgeBaseRevisionUpdateCommand(kbString);
					processCommand(ckbruc);
					COntologyRevision* ontRev = ckbruc->getOntologyRevision();
					delete ckbruc;
					return ontRev;
				}

				COntologyRevision* CJNICommandProcessor::getNewOntologyRevision(COntologyRevision* prevOntRev) {
					CCreateKnowledgeBaseRevisionUpdateCommand* ckbruc = new CCreateKnowledgeBaseRevisionUpdateCommand(prevOntRev->getOntology()->getOntologyName());
					processCommand(ckbruc);
					COntologyRevision* ontRev = ckbruc->getOntologyRevision();
					delete ckbruc;
					return ontRev;

				}


				bool CJNICommandProcessor::prepareOntologyQuerying(COntologyRevision* ontRev) {
					CPreprocessKnowledgeBaseRequirementsForQueryCommand* pkbfqc = new CPreprocessKnowledgeBaseRequirementsForQueryCommand(ontRev->getOntology());
					processCommand(pkbfqc);
					delete pkbfqc;
					return true;
				}


				COntologyRevision* CJNICommandProcessor::installNewOntologyRevision(COntologyRevision* ontRev) {
					CInstallKnowledgeBaseRevisionUpdateCommand* ikbruc = new CInstallKnowledgeBaseRevisionUpdateCommand(ontRev->getOntology()->getOntologyName(),ontRev);
					processCommand(ikbruc);
					delete ikbruc;
					CGetCurrentKnowledgeBaseRevisionCommand* gckbrc = new CGetCurrentKnowledgeBaseRevisionCommand(ontRev->getOntology()->getOntologyName());
					processCommand(gckbrc);
					ontRev = gckbrc->getOntologyRevision();
					return ontRev;
				}



				CJNICommandProcessor* CJNICommandProcessor::calculateOntologyQuery(CQuery* query) {
					CCalculateQueryCommand* cqc = new CCalculateQueryCommand(query);
					processCommand(cqc);
					delete cqc;
					return this;
				}



				COWLlinkProcessor* CJNICommandProcessor::initializeOWLlinkContent() {
					return this;
				}



				COWLlinkProcessor* CJNICommandProcessor::concludeOWLlinkContent() {
					return this;
				}


				CConfiguration* CJNICommandProcessor::getConfiguration() {
					if (!reasonerCommander) {
						return mLoaderConfig;
					} else {
						CGetDescriptionCommand *getDesComm = new CGetDescriptionCommand();
						reasonerCommander->delegateCommand(getDesComm);
						CCommandExecutedBlocker commExeBlocker;
						commExeBlocker.waitExecutedCommand(getDesComm);
						CConfiguration *config = getDesComm->getConfiguration();
						return config;
					}
				}


			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

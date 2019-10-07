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

#include "CJNIInstanceManager.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIInstanceManager::CJNIInstanceManager() {
					mCommandProcessor = nullptr;
					mHandler = nullptr;
					mJNILoaderFactory = nullptr;
					mJNICommandLoader = nullptr;
					mOntRevData = nullptr;
					mCurrentExpBuilder = nullptr;
				}

				CJNIInstanceManager::~CJNIInstanceManager() {
					delete mCommandProcessor;
					delete mHandler;
					delete mJNILoaderFactory;
					delete mJNICommandLoader;
					delete mCurrentExpBuilder;
				}

				CJNICommandProcessor* CJNIInstanceManager::getJNICommandProcessor() {
					return mCommandProcessor;
				}

				CJNIInstanceManager* CJNIInstanceManager::setJNICommandProcessor(CJNICommandProcessor* processor) {
					mCommandProcessor = processor;
					return this;
				}

				CJNIHandler* CJNIInstanceManager::getJNIHandler() {
					return mHandler;
				}

				CJNIInstanceManager* CJNIInstanceManager::setJNIHandler(CJNIHandler* handler) {
					mHandler = handler;
					return this;
				}

				CLoaderFactory* CJNIInstanceManager::getJNILoaderFactory() {
					return mJNILoaderFactory;
				}

				CJNIInstanceManager* CJNIInstanceManager::setJNILoaderFactory(CLoaderFactory* loaderFactory) {
					mJNILoaderFactory = loaderFactory;
					return this;
				}


				CJNICommandLoader* CJNIInstanceManager::getJNICommandLoader() {
					return mJNICommandLoader;
				}

				CJNIInstanceManager* CJNIInstanceManager::setJNICommandLoader(CJNICommandLoader* commandLoader) {
					mJNICommandLoader = commandLoader;
					return this;
				}

				CJNIOntologyRevisionData* CJNIInstanceManager::getOntologyRevisionData() {
					return mOntRevData;
				}

				CJNIInstanceManager* CJNIInstanceManager::setOntologyRevisionData(CJNIOntologyRevisionData* ontRev) {
					mOntRevData = ontRev;
					return this;
				}



				CJNIAxiomExpressionVisitingLoader* CJNIInstanceManager::getCurrentAxiomExpressionBuilder() {
					return mCurrentExpBuilder;
				}

				CJNIInstanceManager* CJNIInstanceManager::setCurrentAxiomExpressionBuilder(CJNIAxiomExpressionVisitingLoader* builder) {
					mCurrentExpBuilder = builder;
					return this;
				}



			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

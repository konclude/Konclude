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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIINSTANCEMANAGER_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIINSTANCEMANAGER_H

// Libraries includes


// Namespace includes
#include "JNISettings.h"
#include "CJNIOntologyRevisionData.h"

// Other includes
#include "Control/Loader/CLoaderFactory.h"
#include "Control/Loader/CJNICommandLoader.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Control {

		using namespace Loader;

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIInstanceManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIInstanceManager {

					// public methods
					public:
						//! Constructor
						CJNIInstanceManager();

						//! Destructor
						virtual ~CJNIInstanceManager();


						CJNICommandProcessor* getJNICommandProcessor();
						CJNIInstanceManager* setJNICommandProcessor(CJNICommandProcessor* processor);


						CJNIHandler* getJNIHandler();
						CJNIInstanceManager* setJNIHandler(CJNIHandler* handler);

						CLoaderFactory* getJNILoaderFactory();
						CJNIInstanceManager* setJNILoaderFactory(CLoaderFactory* loaderFactory);
						
						CJNICommandLoader* getJNICommandLoader();
						CJNIInstanceManager* setJNICommandLoader(CJNICommandLoader* commandLoader);


						CJNIOntologyRevisionData* getOntologyRevisionData();
						CJNIInstanceManager* setOntologyRevisionData(CJNIOntologyRevisionData* ontRev);


						CJNIAxiomExpressionVisitingLoader* getCurrentAxiomExpressionBuilder();
						CJNIInstanceManager* setCurrentAxiomExpressionBuilder(CJNIAxiomExpressionVisitingLoader* builder);


					// protected methods
					protected:

					// protected variables
					protected:

						CJNICommandProcessor* mCommandProcessor;
						CJNIHandler* mHandler;

						CJNICommandLoader* mJNICommandLoader;
						CLoaderFactory* mJNILoaderFactory;

						CJNIOntologyRevisionData* mOntRevData;
						CJNIAxiomExpressionVisitingLoader* mCurrentExpBuilder;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIINSTANCEMANAGER_H

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

#ifndef KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTCALLBACKDATA_H
#define KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTCALLBACKDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COntologyRealizingDynamicRequirmentProcessingStatistics.h"

// Other includes

#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfigurationBase.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		namespace Realizer {


			/*! 
			 *
			 *		\class		COntologyRealizingDynamicRequirmentCallbackData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRealizingDynamicRequirmentCallbackData {
				// public methods
				public:
					//! Constructor
					COntologyRealizingDynamicRequirmentCallbackData(CCallbackData* callback = nullptr);

					COntologyRealizingDynamicRequirmentCallbackData* incProcessingRequirmentCount(cint64 incCount = 1);
					COntologyRealizingDynamicRequirmentCallbackData* decProcessingRequirmentCount(cint64 decCount = 1);
					
					COntologyRealizingDynamicRequirmentCallbackData* setProcessingFinishedCallback(CCallbackData* callback);
					CCallbackData* getProcessingFinishedCallback();

					cint64 getCurrentProcessingRequirmentCount();
					bool hasCurrentProcessingRequirmentCount();

					COntologyRealizingDynamicRequirmentProcessingStatistics* getStatistics();


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyRealizingDynamicRequirmentProcessingStatistics mStatistics;
					cint64 mProcessingRequirmentCount;
					CCallbackData* mCallback;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CONTOLOGYREALIZINGDYNAMICREQUIRMENTCALLBACKDATA_H

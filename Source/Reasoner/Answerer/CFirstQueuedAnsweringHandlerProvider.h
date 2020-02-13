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

#ifndef KONCLUDE_REASONER_ANSWERER_CFIRSTQUEUEDANSWERINGHANDLERPROVIDER_H
#define KONCLUDE_REASONER_ANSWERER_CFIRSTQUEUEDANSWERINGHANDLERPROVIDER_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandlerProvider.h"
#include "CAnsweringHandlerFactory.h"
#include "CAnsweringStatisticsCollectionStrings.h"


// Other includes
#include "Reasoner/Query/CComplexAnsweringQuery.h"
#include "Reasoner/Query/CComplexAnsweringCompositionQuery.h"




// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {



			/*! 
			 *
			 *		\class		CFirstQueuedAnsweringHandlerProvider
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CFirstQueuedAnsweringHandlerProvider : public CAnsweringHandlerProvider {
				// public methods
				public:
					//! Constructor
					CFirstQueuedAnsweringHandlerProvider(CAnsweringHandlerFactory* answeringHandlerFactory);

					//! Destructor
					virtual ~CFirstQueuedAnsweringHandlerProvider();

					virtual CAnsweringHandler* getAnsweringHandler(CComplexAnsweringQuery* complexAnsweringQuery);
					virtual CAnsweringHandler* getAnsweringHandler(CConcreteOntology* ontology, bool composedQuery);


					virtual CAnsweringHandlerProvider* releaseAnsweringHandler(CAnsweringHandler* answeringHandler);

					virtual CAnsweringHandlerProvider* addAnsweringStatistics(CConcreteOntology* ontology, CAnsweringStatistics* stats);

				// protected methods
				protected:

				// protected variables
				protected:

					class CAnsweringHandlerData {
						public:
							QList<CAnsweringHandler*> mQueuedHandlerList;
							QSet<CAnsweringHandler*> mProcessingHandlerSet;
							bool mStatisticsCollection = false;
							CAnsweringStatisticsCollectionStrings* mCollectedStats = nullptr;
					};

					QHash<CConcreteOntology*, CAnsweringHandlerData*> mOntoloyHandlerDataHash;
					QHash<CAnsweringHandler*, CAnsweringHandlerData*> mHandlerHandlerDataHash;
					CAnsweringHandlerFactory* mAnsweringHandlerFactory;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CFIRSTQUEUEDANSWERINGHANDLERPROVIDER_H

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

#ifndef KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCE_H
#define KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCE_H

// Libraries includes


// Namespace includes
#include "ConsistenceSettings.h"
#include "CConsistenceData.h"
#include "CConsistenceStatistics.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistence {

			/*! 
			 *
			 *		\class		CConsistence
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConsistence {
				// public methods
				public:
					//! Constructor
					CConsistence();

					//! Destructor
					virtual ~CConsistence();


					bool isOntologyConsistent();
					CConsistence* setOntologyConsistent(bool consistency);

					CConsistenceData* getConsistenceModelData();
					bool hasConsistenceModelData();
					CConsistence* setConsistenceModelData(CConsistenceData* consData);


					bool areIndividualsRepresentativelyStored();
					bool areAllIndividualsRepresentativelyStored();

					CConsistence* setIndividualsRepresentativelyStored(bool representativelyStored);
					CConsistence* setAllIndividualsRepresentativelyStored(bool representativelyStored);

					bool hasConsistenceStatistics();

					CConsistenceStatistics* getConsistenceStatistics();

					CConsistence* setConsistenceStatistics(CConsistenceStatistics* statColl);


				// protected methods
				protected:

				// protected variables
				protected:
					bool mConsistent;
					CConsistenceData* mConsData;
					bool mRepresentativelyStored;
					bool mAllRepresentativelyStored;

					CConsistenceStatistics* mStatsColl;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCE_H

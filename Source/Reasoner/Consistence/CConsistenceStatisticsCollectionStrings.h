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

#ifndef KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCESTATISTICSCOLLECTIONSTRINGS_H
#define KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCESTATISTICSCOLLECTIONSTRINGS_H

// Libraries includes


// Namespace includes
#include "ConsistenceSettings.h"
#include "CConsistenceStatistics.h"
#include "CPrecomputationStatistics.h"
#include "CConsistenceCalculationStatisticsCollection.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Consistence {

			/*! 
			 *
			 *		\class		CConsistenceStatisticsCollectionStrings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConsistenceStatisticsCollectionStrings : public CConsistenceStatistics, public CPrecomputationStatistics {
				// public methods
				public:
					//! Constructor
					CConsistenceStatisticsCollectionStrings();

					//! Destructor
					virtual ~CConsistenceStatisticsCollectionStrings();

					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);
					CConsistenceCalculationStatisticsCollection* getCalculationStatisticsCollectorLinker();
					CConsistenceCalculationStatisticsCollection* createCalculationStatisticsCollection();

					virtual QList<QString> getStatisticsNameStringList();
					virtual cint64 getStatisticIntegerValue(const QString& statName);

				// protected methods
				protected:

				// protected variables
				protected:
					CConsistenceCalculationStatisticsCollection* mCalcStatCollLinker;
					bool mStatStringsCollected;
					QHash<QString,cint64> mStatNameStringsValueHash;
					QSet<QString> mStatNameStrings;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistence

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTENCE_CCONSISTENCESTATISTICSCOLLECTIONSTRINGS_H

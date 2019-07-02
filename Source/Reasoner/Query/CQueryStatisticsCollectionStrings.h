/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CQUERYSTATISTICSCOLLECTIONSTRINGS_H
#define KONCLUDE_REASONER_QUERY_CQUERYSTATISTICSCOLLECTIONSTRINGS_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CQueryStatistics.h"
#include "CQueryCalculationStatisticsCollection.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {

			/*! 
			 *
			 *		\class		CQueryStatisticsCollectionStrings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CQueryStatisticsCollectionStrings : public CQueryStatistics {
				// public methods
				public:
					//! Constructor
					CQueryStatisticsCollectionStrings();

					//! Destructor
					virtual ~CQueryStatisticsCollectionStrings();

					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);
					CQueryCalculationStatisticsCollection* getCalculationStatisticsCollectorLinker();
					CQueryCalculationStatisticsCollection* createCalculationStatisticsCollection();

					virtual QList<QString> getStatisticsNameStringList();
					virtual cint64 getStatisticIntegerValue(const QString& statName);

				// protected methods
				protected:

				// protected variables
				protected:
					CQueryCalculationStatisticsCollection* mCalcStatCollLinker;
					bool mStatStringsCollected;
					QHash<QString,cint64> mStatNameStringsValueHash;
					QSet<QString> mStatNameStrings;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CQUERYSTATISTICSCOLLECTIONSTRINGS_H

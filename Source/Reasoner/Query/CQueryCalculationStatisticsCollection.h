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

#ifndef KONCLUDE_REASONER_QUERY_CQUERYCALCULATIONSTATISTICSCOLLECTION_H
#define KONCLUDE_REASONER_QUERY_CQUERYCALCULATIONSTATISTICSCOLLECTION_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Query {

			/*! 
			 *
			 *		\class		CQueryCalculationStatisticsCollection
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CQueryCalculationStatisticsCollection : public CLinkerBase<CQueryCalculationStatisticsCollection*,CQueryCalculationStatisticsCollection>, public CCalculationStatisticsCollector {
				// public methods
				public:
					//! Constructor
					CQueryCalculationStatisticsCollection();

					//! Destructor
					virtual ~CQueryCalculationStatisticsCollection();


					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);

					CQueryCalculationStatisticsCollection* appendStatisticsStringNames(QSet<QString>* statNamesSet);
					cint64 getStatisticIntegerValue(const QString& statName);

				// protected methods
				protected:

				// protected variables
				protected:
					QHash<QString,cint64> mStatNameStringsValueHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CQUERYCALCULATIONSTATISTICSCOLLECTION_H

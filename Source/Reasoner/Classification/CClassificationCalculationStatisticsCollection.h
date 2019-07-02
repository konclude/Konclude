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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONCALCULATIONSTATISTICSCOLLECTION_H
#define KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONCALCULATIONSTATISTICSCOLLECTION_H

// Libraries includes


// Namespace includes
#include "ClassificationSettings.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Classification {

			/*! 
			 *
			 *		\class		CClassificationCalculationStatisticsCollection
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationCalculationStatisticsCollection : public CLinkerBase<CClassificationCalculationStatisticsCollection*,CClassificationCalculationStatisticsCollection>, public CCalculationStatisticsCollector {
				// public methods
				public:
					//! Constructor
					CClassificationCalculationStatisticsCollection();

					//! Destructor
					virtual ~CClassificationCalculationStatisticsCollection();


					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);

					CClassificationCalculationStatisticsCollection* appendStatisticsStringNames(QSet<QString>* statNamesSet);
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

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONCALCULATIONSTATISTICSCOLLECTION_H

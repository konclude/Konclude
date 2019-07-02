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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICSCOLLECTIONSTRINGS_H
#define KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICSCOLLECTIONSTRINGS_H

// Libraries includes


// Namespace includes
#include "ClassificationSettings.h"
#include "CClassificationStatistics.h"
#include "CClassificationCalculationStatisticsCollection.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Classification {

			/*! 
			 *
			 *		\class		CClassificationStatisticsCollectionStrings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationStatisticsCollectionStrings : public CClassificationStatistics {
				// public methods
				public:
					//! Constructor
					CClassificationStatisticsCollectionStrings();

					//! Destructor
					virtual ~CClassificationStatisticsCollectionStrings();

					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);
					CClassificationCalculationStatisticsCollection* getCalculationStatisticsCollectorLinker();
					CClassificationCalculationStatisticsCollection* createCalculationStatisticsCollection();

					virtual QList<QString> getStatisticsNameStringList();
					virtual cint64 getStatisticIntegerValue(const QString& statName);

				// protected methods
				protected:

				// protected variables
				protected:
					CClassificationCalculationStatisticsCollection* mCalcStatCollLinker;
					bool mStatStringsCollected;
					QHash<QString,cint64> mStatNameStringsValueHash;
					QSet<QString> mStatNameStrings;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICSCOLLECTIONSTRINGS_H

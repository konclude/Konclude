/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICS_H
#define KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICS_H

// Libraries includes
#include <QList>
#include <QString>

// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Classification {

			/*! 
			 *
			 *		\class		CClassificationStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationStatistics {
				// public methods
				public:
					//! Constructor
					CClassificationStatistics();

					//! Destructor
					virtual ~CClassificationStatistics();

					virtual QList<QString> getStatisticsNameStringList() = 0;
					virtual cint64 getStatisticIntegerValue(const QString& statName) = 0;
					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFICATION_CCLASSIFICATIONSTATISTICS_H

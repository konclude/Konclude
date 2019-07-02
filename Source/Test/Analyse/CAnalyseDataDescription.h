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

#ifndef KONCLUDE_TEST_ANALYSE_CANALYSEDATADESCRIPTION_H
#define KONCLUDE_TEST_ANALYSE_CANALYSEDATADESCRIPTION_H

// Libraries includes
#include <QString>
#include <QVector>

// Namespace includes
#include "AnalyseSettings.h"
#include "CAnalyseDescription.h"
#include "CAnalyseValueDescription.h"
#include "CAnalyseUpdateDescription.h"
#include "CAnalyseData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CAnalyseDataDescription
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnalyseDataDescription : public CAnalyseDescription {
				// public methods
				public:
					//! Constructor
					CAnalyseDataDescription();

					//! Destructor
					virtual ~CAnalyseDataDescription();

					virtual QString getDataName();
					virtual CAnalyseDataDescription *setDataName(const QString &name);

					virtual CAnalyseValueDescription *getValueDescription(qint64 index);
					virtual CAnalyseValueDescription *getValueDescription(const QString &valueName);
					virtual CAnalyseUpdateDescription *installValueDescription(CAnalyseValueDescription *valueDescription, CAnalyseValueUpdateReader *updateReader = 0);

					virtual CAnalyseUpdateDescription *getUpdateDescription(qint64 index);

					virtual CAnalyseData *createData();
					virtual qint64 getAnalyseValueCount();

				// protected methods
				protected:

				// protected variables
				protected:
					QString dataName;
					QVector<CAnalyseUpdateDescription *> valueUpdateDesVec;
					QHash<QString,qint64> desNameIndexHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CANALYSEDATADESCRIPTION_H

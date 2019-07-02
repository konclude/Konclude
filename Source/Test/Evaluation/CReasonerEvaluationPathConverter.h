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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPATHCONVERTER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPATHCONVERTER_H

// Libraries includes
#include <QDir>
#include <QMap>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Test {

		namespace Evaluation {

			class CVersionNumbersItem {
				public:
					bool operator<(const CVersionNumbersItem& item) const {
						// sort version with highest numbers first in the map
						QList<cint64> tmpList1(mNumberList);
						QList<cint64> tmpList2(item.mNumberList);
						while (!tmpList1.isEmpty() && !tmpList2.isEmpty()) {
							cint64 number1 = tmpList1.takeFirst();
							cint64 number2 = tmpList2.takeFirst();
							if (number1 > number2) {
								return true;
							} else if (number2 > number1) {
								return false;
							}
						}
						if (tmpList2.isEmpty()) {
							return true;
						} else {
							return false;
						}
					}

					CVersionNumbersItem* addNumber(cint64 number) {
						mNumberList.append(number);
						return this;
					}
					QList<cint64> mNumberList;
			};

			/*! 
			 *
			 *		\class		CReasonerEvaluationPathConverter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationPathConverter {
				// public variables
				public:


				// public methods
				public:
					//! Constructor
					CReasonerEvaluationPathConverter();

					static QString getCorrectedPathString(const QString& pathString);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPATHCONVERTER_H

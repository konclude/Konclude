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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			template<class T>
			QString defaultConvertToStringFunction(const T& value) {
				return QString("%1").arg(value);
			}


			/*! 
			 *
			 *		\class		CReasonerEvaluationTableMultiFormatOutputWriter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CReasonerEvaluationTableMultiFormatOutputWriter {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationTableMultiFormatOutputWriter();

					//! Destructor
					virtual ~CReasonerEvaluationTableMultiFormatOutputWriter();


					QStringList writeCVSTable(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);
					QStringList writeGNUPlotDataTable(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);


					QStringList writeCVSTablesSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);
					QStringList writeGNUPlotDataTablesSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);

					QStringList writeCVSTablesSortedSplitted(cint64 splitSize, const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);
					QStringList writeGNUPlotDataTablesSortedSplitted(cint64 splitSize, const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);

					QStringList writeCVSTablesSeparateSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);
					QStringList writeGNUPlotDataTablesSeparateSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&) = &defaultConvertToStringFunction);

					CReasonerEvaluationTableMultiFormatOutputWriter<T>* buildSortedIndex();

					QString getSplitIndexString(cint64 index, cint64 maxIndex);

					CReasonerEvaluationTableMultiFormatOutputWriter<T>* addColumnTitles(const QStringList& titleList);
					CReasonerEvaluationTableMultiFormatOutputWriter<T>* addNewTableRow(const QString& rowTitle, const QList<T>& rowValues);
					CReasonerEvaluationTableMultiFormatOutputWriter<T>* addNewTableRow(const QString& rowTitle);
					CReasonerEvaluationTableMultiFormatOutputWriter<T>* addValuesToLastTableRow(const QList<T>& rowValues);


				// protected methods
				protected:
					QString getGNUPlotCompatibleTitle(const QString& string);

				// protected variables
				protected:
					class CReasonerEvaluationTableRow {
						public:
							QString mTitle;
							QList<T> mRowValueList;
					};

					QStringList mTitles;
					QList<CReasonerEvaluationTableRow*> mRowList;
					CReasonerEvaluationTableRow* mLastRow;

					QMap<QString,CReasonerEvaluationTableRow*> mTitleColumnSortedMap;
					QList< QMap<T,CReasonerEvaluationTableRow*>* > mColumnSortMapList;
					bool mSortedIndexBuilded;

				// private methods
				private:

				// private variables
				private:

			};


			class CTestReasonerEvaluationTableMultiFormatOutputWriter : public CReasonerEvaluationTableMultiFormatOutputWriter<cint64> {
			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#include "CReasonerEvaluationTableMultiFormatOutputWriter.cpp"

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_H

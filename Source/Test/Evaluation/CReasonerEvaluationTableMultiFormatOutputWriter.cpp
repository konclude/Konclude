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
#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_CPP
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_CPP

#include "CReasonerEvaluationTableMultiFormatOutputWriter.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>::CReasonerEvaluationTableMultiFormatOutputWriter() {
				mLastRow = nullptr;
				mSortedIndexBuilded = false;
			}


			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>::~CReasonerEvaluationTableMultiFormatOutputWriter() {
			}


			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeCVSTable(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				QString outputFileString(baseFileName+"-table.csv");
				QFile outputFile(outputFileString);
				if (outputFile.open(QIODevice::WriteOnly)) {
					outputFileList<<outputFileString;
					QString titleString = QString("%1\r\n").arg(mTitles.join("\t"));
					outputFile.write(titleString.toLocal8Bit());
					foreach (CReasonerEvaluationTableRow* row, mRowList) {
						QString rowString = row->mTitle+"\t";
						foreach (T value, row->mRowValueList) {
							QString valueString = (*convertStringFunction)(value);
							rowString += QString("%1\t").arg(valueString);
						}
						rowString += "\r\n";
						outputFile.write(rowString.toLocal8Bit());
					}
				}
				return outputFileList;
			}


			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeGNUPlotDataTable(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				QString outputFileString(baseFileName+"-table.dat");
				QFile outputFile(outputFileString);
				if (outputFile.open(QIODevice::WriteOnly)) {
					outputFileList<<outputFileString;
					QStringList mIndexAddedTitleList(mTitles);
					mIndexAddedTitleList.insert(1,"Index");
					QString titleString = QString("\"%1\"\r\n").arg(mIndexAddedTitleList.join("\"\t\""));
					outputFile.write(titleString.toLocal8Bit());

					cint64 rowIndex = 0;
					foreach (CReasonerEvaluationTableRow* row, mRowList) {
						QString rowString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowIndex++);
						foreach (T value, row->mRowValueList) {
							QString valueString = (*convertStringFunction)(value);
							rowString += QString("%1\t").arg(valueString);
						}
						rowString += "\r\n";
						outputFile.write(rowString.toLocal8Bit());
					}
				}
				return outputFileList;
			}

			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeGNUPlotDataTablesSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				cint64 titleIndex = 0;
				cint64 rowCount = mRowList.count();
				foreach (QString title, mTitles) {
					QString outputFileASCString(baseFileName+QString("-sorted-ascending-by-%1-table.dat").arg(title));
					QString outputFileDESString(baseFileName+QString("-sorted-descending-by-%1-table.dat").arg(title));

					QString desFileContentString;

					QFile outputFileASC(outputFileASCString);
					if (outputFileASC.open(QIODevice::WriteOnly)) {

						QStringList mIndexAddedTitleList(mTitles);
						mIndexAddedTitleList.insert(1,"Index");
						QString titleString = QString("\"%1\"\r\n").arg(mIndexAddedTitleList.join("\"\t\""));
						outputFileASC.write(titleString.toLocal8Bit());

						cint64 rowIndex = 0;
						if (titleIndex == 0) {
							for (typename QMap<QString,CReasonerEvaluationTableRow*>::const_iterator it = mTitleColumnSortedMap.constBegin(), itEnd = mTitleColumnSortedMap.constEnd(); it != itEnd; ++it) {
								CReasonerEvaluationTableRow* row = it.value();
								QString rowASCString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowIndex);
								QString rowDESString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowCount-rowIndex-1);
								foreach (T value, row->mRowValueList) {
									QString valueString = (*convertStringFunction)(value);
									rowASCString += QString("%1\t").arg(valueString);
									rowDESString += QString("%1\t").arg(valueString);
								}
								rowASCString += "\r\n";
								rowDESString += "\r\n";
								outputFileASC.write(rowASCString.toLocal8Bit());
								desFileContentString.prepend(rowDESString);
								++rowIndex;
							}
						} else {
							QMap<T,CReasonerEvaluationTableRow*>* sortedMap = mColumnSortMapList.at(titleIndex-1);
							for (typename QMap<T,CReasonerEvaluationTableRow*>::const_iterator it = sortedMap->constBegin(), itEnd = sortedMap->constEnd(); it != itEnd; ++it) {
								CReasonerEvaluationTableRow* row = it.value();
								QString rowASCString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowIndex);
								QString rowDESString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowCount-rowIndex-1);
								foreach (T value, row->mRowValueList) {
									QString valueString = (*convertStringFunction)(value);
									rowASCString += QString("%1\t").arg(valueString);
									rowDESString += QString("%1\t").arg(valueString);
								}
								rowASCString += "\r\n";
								rowDESString += "\r\n";
								outputFileASC.write(rowASCString.toLocal8Bit());
								desFileContentString.prepend(rowDESString);
								++rowIndex;
							}
						}
						outputFileList<<outputFileASCString;

						desFileContentString.prepend(titleString);
						QFile outputFileDES(outputFileDESString);
						if (outputFileDES.open(QIODevice::WriteOnly)) {
							outputFileDES.write(desFileContentString.toLocal8Bit());
							outputFileDES.close();
							outputFileList<<outputFileDESString;
						}

					}
					++titleIndex;
				}
				return outputFileList;
			}



			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeGNUPlotDataTablesSeparateSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				QString outputFileASCString(baseFileName+QString("-separate-sorted-ascending-table.dat"));
				QString outputFileDESString(baseFileName+QString("-separate-sorted-descending-table.dat"));

				QString desFileContentString;
				cint64 rowCount = mRowList.count();

				QFile outputFileASC(outputFileASCString);
				if (outputFileASC.open(QIODevice::WriteOnly)) {

					QStringList mIndexAddedTitleList(mTitles);
					mIndexAddedTitleList.insert(1,"Index");

					QString titleString = QString("\"%1\"\r\n").arg(mIndexAddedTitleList.join("\"\t\""));
					outputFileASC.write(titleString.toLocal8Bit());


					typename QMap<QString,CReasonerEvaluationTableRow*>::iterator itTitles = mTitleColumnSortedMap.begin(), itTitlesEnd = mTitleColumnSortedMap.end();
					QList< typename QMap<T,CReasonerEvaluationTableRow*>::iterator > itList;

					for (typename QList< QMap<T,CReasonerEvaluationTableRow*>* >::const_iterator it = mColumnSortMapList.constBegin(), itEnd = mColumnSortMapList.constEnd(); it != itEnd; ++it) {
						QMap<T,CReasonerEvaluationTableRow*>* sortMap = *it;
						itList.append(sortMap->begin());
					}

					cint64 rowIndex = 0;
					while (itTitles != itTitlesEnd) {
						QString rowASCTitleString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(itTitles.key())).arg(rowIndex);
						QString rowDESTitleString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(itTitles.key())).arg(rowCount-rowIndex-1);

						cint64 colIndex = 0;
						for (typename QList< typename QMap<T,CReasonerEvaluationTableRow*>::iterator >::iterator it = itList.begin(), itEnd = itList.end(); it != itEnd; ++it) {
							typename QMap<T,CReasonerEvaluationTableRow*>::iterator& itCol = *it;

							CReasonerEvaluationTableRow* row = itCol.value();
							T value = row->mRowValueList.at(colIndex);
							QString valueString = (*convertStringFunction)(value);
							rowASCTitleString += QString("%1\t").arg(valueString);
							rowDESTitleString += QString("%1\t").arg(valueString);

							++itCol;
							++colIndex;
						}
						rowASCTitleString += "\r\n";
						rowDESTitleString += "\r\n";
						outputFileASC.write(rowASCTitleString.toLocal8Bit());
						desFileContentString.prepend(rowDESTitleString);

						++itTitles;
						++rowIndex;
					}

					desFileContentString.prepend(titleString);
					QFile outputFileDES(outputFileDESString);
					if (outputFileDES.open(QIODevice::WriteOnly)) {
						outputFileDES.write(desFileContentString.toLocal8Bit());
						outputFileDES.close();
						outputFileList<<outputFileDESString;
					}



					outputFileList<<outputFileASCString;
				}
				return outputFileList;
			}




			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeGNUPlotDataTablesSortedSplitted(cint64 splitSize, const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				cint64 rowCount = mRowList.count();
				for (cint64 rowStartIndex = 0; rowStartIndex < rowCount; rowStartIndex += splitSize) {
					cint64 rowEndIndex = rowStartIndex+splitSize;
					rowEndIndex = qMin(rowEndIndex,rowCount);
					cint64 titleIndex = 0;
					foreach (QString title, mTitles) {
						QString outputFileASCString(baseFileName+QString("-sorted-ascending-by-%1-split-from-%2-to-%3-table.dat").arg(title).arg(getSplitIndexString(rowStartIndex+1,rowCount)).arg(getSplitIndexString(rowEndIndex,rowCount)));
						QString outputFileDESString(baseFileName+QString("-sorted-descending-by-%1-split-from-%2-to-%3-table.dat").arg(title).arg(getSplitIndexString(rowCount-rowEndIndex+1,rowCount)).arg(getSplitIndexString(rowCount-rowStartIndex,rowCount)));

						QString desFileContentString;

						QFile outputFileASC(outputFileASCString);
						if (outputFileASC.open(QIODevice::WriteOnly)) {

							QStringList mIndexAddedTitleList(mTitles);
							mIndexAddedTitleList.insert(1,"Index");
							QString titleString = QString("\"%1\"\r\n").arg(mIndexAddedTitleList.join("\"\t\""));
							outputFileASC.write(titleString.toLocal8Bit());

							cint64 rowIndex = 0;
							if (titleIndex == 0) {
								cint64 rowIndex = 0;
								for (typename QMap<QString,CReasonerEvaluationTableRow*>::const_iterator it = mTitleColumnSortedMap.constBegin(), itEnd = mTitleColumnSortedMap.constEnd(); it != itEnd; ++it) {
									if (rowIndex >= rowStartIndex && rowIndex < rowEndIndex) {
										CReasonerEvaluationTableRow* row = it.value();
										QString rowASCString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowIndex-rowStartIndex);
										QString rowDESString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowEndIndex-rowIndex-1);
										foreach (T value, row->mRowValueList) {
											QString valueString = (*convertStringFunction)(value);
											rowASCString += QString("%1\t").arg(valueString);
											rowDESString += QString("%1\t").arg(valueString);
										}
										rowASCString += "\r\n";
										rowDESString += "\r\n";
										outputFileASC.write(rowASCString.toLocal8Bit());
										desFileContentString.prepend(rowDESString);
									}
									++rowIndex;
								}
							} else {
								cint64 rowIndex = 0;
								QMap<T,CReasonerEvaluationTableRow*>* sortedMap = mColumnSortMapList.at(titleIndex-1);
								for (typename QMap<T,CReasonerEvaluationTableRow*>::const_iterator it = sortedMap->constBegin(), itEnd = sortedMap->constEnd(); it != itEnd; ++it) {
									if (rowIndex >= rowStartIndex && rowIndex < rowEndIndex) {
										CReasonerEvaluationTableRow* row = it.value();
										QString rowASCString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowIndex-rowStartIndex);
										QString rowDESString = QString("\"%1\"\t%2\t").arg(getGNUPlotCompatibleTitle(row->mTitle)).arg(rowEndIndex-rowIndex-1);
										foreach (T value, row->mRowValueList) {
											QString valueString = (*convertStringFunction)(value);
											rowASCString += QString("%1\t").arg(valueString);
											rowDESString += QString("%1\t").arg(valueString);
										}
										rowASCString += "\r\n";
										rowDESString += "\r\n";
										outputFileASC.write(rowASCString.toLocal8Bit());
										desFileContentString.prepend(rowDESString);
									}
									++rowIndex;
								}
							}
							outputFileList<<outputFileASCString;

							desFileContentString.prepend(titleString);
							QFile outputFileDES(outputFileDESString);
							if (outputFileDES.open(QIODevice::WriteOnly)) {
								outputFileDES.write(desFileContentString.toLocal8Bit());
								outputFileDES.close();
								outputFileList<<outputFileDESString;
							}

						}
						++titleIndex;
					}
				}
				return outputFileList;
			}


			template<class T>
			QString CReasonerEvaluationTableMultiFormatOutputWriter<T>::getGNUPlotCompatibleTitle(const QString& string) {
				QString comString = string;
				comString.replace("-request.xml","");
				comString.replace("_","-");
				if (comString.length() >= 40) {
					comString = comString.mid(0,37);
					comString += "...";
				}
				return comString;
			}



			template<class T>
			QString CReasonerEvaluationTableMultiFormatOutputWriter<T>::getSplitIndexString(cint64 index, cint64 maxIndex) {
				QString maxIndString = QString::number(maxIndex);
				QString indexString = QString::number(index);
				while (indexString.size() < maxIndString.size()) {
					indexString.insert(0,"0");
				}
				return indexString;
			}


			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeCVSTablesSortedSplitted(cint64 splitSize, const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				cint64 rowCount = mRowList.count();
				for (cint64 rowStartIndex = 0; rowStartIndex < rowCount; rowStartIndex += splitSize) {
					cint64 rowEndIndex = rowStartIndex+splitSize;
					rowEndIndex = qMin(rowEndIndex,rowCount);


					cint64 titleIndex = 0;
					foreach (QString title, mTitles) {
						QString outputFileASCString(baseFileName+QString("-sorted-ascending-by-%1-split-from-%2-to-%3-table.csv").arg(title).arg(getSplitIndexString(rowStartIndex+1,rowCount)).arg(getSplitIndexString(rowEndIndex,rowCount)));
						QString outputFileDESString(baseFileName+QString("-sorted-descending-by-%1-split-from-%2-to-%3-table.csv").arg(title).arg(getSplitIndexString(rowCount-rowEndIndex+1,rowCount)).arg(getSplitIndexString(rowCount-rowStartIndex,rowCount)));

						QString desFileContentString;

						QFile outputFileASC(outputFileASCString);
						if (outputFileASC.open(QIODevice::WriteOnly)) {

							QString titleString = QString("%1\r\n").arg(mTitles.join("\t"));
							outputFileASC.write(titleString.toLocal8Bit());
							if (titleIndex == 0) {
								cint64 rowIndex = 0;
								for (typename QMap<QString,CReasonerEvaluationTableRow*>::const_iterator it = mTitleColumnSortedMap.constBegin(), itEnd = mTitleColumnSortedMap.constEnd(); it != itEnd; ++it) {
									if (rowIndex >= rowStartIndex && rowIndex < rowEndIndex) {
										CReasonerEvaluationTableRow* row = it.value();
										QString rowString = row->mTitle+"\t";
										foreach (T value, row->mRowValueList) {
											QString valueString = (*convertStringFunction)(value);
											rowString += QString("%1\t").arg(valueString);
										}
										rowString += "\r\n";
										outputFileASC.write(rowString.toLocal8Bit());

										desFileContentString.prepend(rowString);
									}
									++rowIndex;
								}
							} else {
								QMap<T,CReasonerEvaluationTableRow*>* sortedMap = mColumnSortMapList.at(titleIndex-1);
								cint64 rowIndex = 0;
								for (typename QMap<T,CReasonerEvaluationTableRow*>::const_iterator it = sortedMap->constBegin(), itEnd = sortedMap->constEnd(); it != itEnd; ++it) {
									if (rowIndex >= rowStartIndex && rowIndex < rowEndIndex) {
										CReasonerEvaluationTableRow* row = it.value();
										QString rowString = row->mTitle+"\t";
										foreach (T value, row->mRowValueList) {
											QString valueString = (*convertStringFunction)(value);
											rowString += QString("%1\t").arg(valueString);
										}
										rowString += "\r\n";
										outputFileASC.write(rowString.toLocal8Bit());

										desFileContentString.prepend(rowString);
									}
									++rowIndex;
								}
							}
							outputFileList<<outputFileASCString;

							desFileContentString.prepend(titleString);
							QFile outputFileDES(outputFileDESString);
							if (outputFileDES.open(QIODevice::WriteOnly)) {
								outputFileDES.write(desFileContentString.toLocal8Bit());
								outputFileDES.close();
								outputFileList<<outputFileDESString;
							}

						}
						++titleIndex;
					}
				}
				return outputFileList;
			}

			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeCVSTablesSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				cint64 titleIndex = 0;
				foreach (QString title, mTitles) {
					QString outputFileASCString(baseFileName+QString("-sorted-ascending-by-%1-table.csv").arg(title));
					QString outputFileDESString(baseFileName+QString("-sorted-descending-by-%1-table.csv").arg(title));

					QString desFileContentString;

					QFile outputFileASC(outputFileASCString);
					if (outputFileASC.open(QIODevice::WriteOnly)) {

						QString titleString = QString("%1\r\n").arg(mTitles.join("\t"));
						outputFileASC.write(titleString.toLocal8Bit());
						if (titleIndex == 0) {
							for (typename QMap<QString,CReasonerEvaluationTableRow*>::const_iterator it = mTitleColumnSortedMap.constBegin(), itEnd = mTitleColumnSortedMap.constEnd(); it != itEnd; ++it) {
								CReasonerEvaluationTableRow* row = it.value();
								QString rowString = row->mTitle+"\t";
								foreach (T value, row->mRowValueList) {
									QString valueString = (*convertStringFunction)(value);
									rowString += QString("%1\t").arg(valueString);
								}
								rowString += "\r\n";
								outputFileASC.write(rowString.toLocal8Bit());
								desFileContentString.prepend(rowString);
							}
						} else {
							QMap<T,CReasonerEvaluationTableRow*>* sortedMap = mColumnSortMapList.at(titleIndex-1);
							for (typename QMap<T,CReasonerEvaluationTableRow*>::const_iterator it = sortedMap->constBegin(), itEnd = sortedMap->constEnd(); it != itEnd; ++it) {
								CReasonerEvaluationTableRow* row = it.value();
								QString rowString = row->mTitle+"\t";
								foreach (T value, row->mRowValueList) {
									QString valueString = (*convertStringFunction)(value);
									rowString += QString("%1\t").arg(valueString);
								}
								rowString += "\r\n";
								outputFileASC.write(rowString.toLocal8Bit());
								desFileContentString.prepend(rowString);
							}
						}
						outputFileList<<outputFileASCString;

						desFileContentString.prepend(titleString);
						QFile outputFileDES(outputFileDESString);
						if (outputFileDES.open(QIODevice::WriteOnly)) {
							outputFileDES.write(desFileContentString.toLocal8Bit());
							outputFileDES.close();
							outputFileList<<outputFileDESString;
						}

					}
					++titleIndex;
				}
				return outputFileList;
			}

			template<class T>
			QStringList CReasonerEvaluationTableMultiFormatOutputWriter<T>::writeCVSTablesSeparateSorted(const QString& baseFileName, QString (*convertStringFunction)(const T&)) {
				QStringList outputFileList;
				QString outputFileString(baseFileName+QString("-separate-sorted-table.csv"));
				QFile outputFile(outputFileString);
				if (outputFile.open(QIODevice::WriteOnly)) {

					outputFileList.append(outputFileString);

					QString titleString = QString("%1\r\n").arg(mTitles.join("\t"));
					outputFile.write(titleString.toLocal8Bit());


					typename QMap<QString,CReasonerEvaluationTableRow*>::iterator itTitles = mTitleColumnSortedMap.begin(), itTitlesEnd = mTitleColumnSortedMap.end();
					QList< typename QMap<T,CReasonerEvaluationTableRow*>::iterator > itList;

					for (typename QList< QMap<T,CReasonerEvaluationTableRow*>* >::const_iterator it = mColumnSortMapList.constBegin(), itEnd = mColumnSortMapList.constEnd(); it != itEnd; ++it) {
						QMap<T,CReasonerEvaluationTableRow*>* sortMap = *it;
						itList.append(sortMap->begin());
					}

					while (itTitles != itTitlesEnd) {
						QString rowTitleString = QString("%1\t").arg(itTitles.key());

						cint64 colIndex = 0;
						for (typename QList< typename QMap<T,CReasonerEvaluationTableRow*>::iterator >::iterator it = itList.begin(), itEnd = itList.end(); it != itEnd; ++it) {
							typename QMap<T,CReasonerEvaluationTableRow*>::iterator& itCol = *it;

							CReasonerEvaluationTableRow* row = itCol.value();
							T value = row->mRowValueList.at(colIndex);
							QString valueString = (*convertStringFunction)(value);
							rowTitleString += QString("%1\t").arg(valueString);

							++itCol;
							++colIndex;
						}
						rowTitleString += "\r\n";
						outputFile.write(rowTitleString.toLocal8Bit());

						++itTitles;
					}

				}
				return outputFileList;
			}


			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>* CReasonerEvaluationTableMultiFormatOutputWriter<T>::addColumnTitles(const QStringList& titleList) {
				mTitles += titleList;
				mSortedIndexBuilded = false;
				return this;
			}

			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>* CReasonerEvaluationTableMultiFormatOutputWriter<T>::addNewTableRow(const QString& rowTitle, const QList<T>& rowValues) {
				mLastRow = new CReasonerEvaluationTableRow();
				mLastRow->mTitle = rowTitle;
				mLastRow->mRowValueList += rowValues;
				mRowList.append(mLastRow);
				mSortedIndexBuilded = false;
				return this;
			}

			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>* CReasonerEvaluationTableMultiFormatOutputWriter<T>::addValuesToLastTableRow(const QList<T>& rowValues) {
				mLastRow->mRowValueList += rowValues;
				mSortedIndexBuilded = false;
				return this;
			}

			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>* CReasonerEvaluationTableMultiFormatOutputWriter<T>::addNewTableRow(const QString& rowTitle) {
				mLastRow = new CReasonerEvaluationTableRow();
				mLastRow->mTitle = rowTitle;
				mRowList.append(mLastRow);
				mSortedIndexBuilded = false;
				return this;
			}

			template<class T>
			CReasonerEvaluationTableMultiFormatOutputWriter<T>* CReasonerEvaluationTableMultiFormatOutputWriter<T>::buildSortedIndex() {
				if (!mSortedIndexBuilded) {
					if (!mColumnSortMapList.isEmpty()) {
						qDeleteAll(mColumnSortMapList);
						mColumnSortMapList.clear();
						mTitleColumnSortedMap.clear();
					}
					mSortedIndexBuilded = true;

					foreach (CReasonerEvaluationTableRow* row, mRowList) {
						mTitleColumnSortedMap.insertMulti(row->mTitle,row);
						cint64 columnIndex = 0;
						foreach (T value, row->mRowValueList) {
							if (mColumnSortMapList.count() <= columnIndex) {
								mColumnSortMapList.append(new QMap<T,CReasonerEvaluationTableRow*>());
							}
							QMap<T,CReasonerEvaluationTableRow*>* columnSortMap = mColumnSortMapList.at(columnIndex);
							columnSortMap->insertMulti(value,row);
							++columnIndex;
						}
					}
				}
				return this;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTABLEMULTIFORMATOUTPUTWRITER_CPP

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

#include "CReasonerEvaluationPathConverter.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationPathConverter::CReasonerEvaluationPathConverter() {
			}

			QString CReasonerEvaluationPathConverter::getCorrectedPathString(const QString& pathString) {
				QString updatedPathString = pathString;


				cint64 nextPos = 0;
				while (updatedPathString.indexOf("$$",nextPos) != -1) {
					nextPos = updatedPathString.indexOf("$$",nextPos);

					if (updatedPathString.indexOf("$$_LATEST+1_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_LATEST+1_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_LATEST+1_$$").length();

						QString latestBeginString = updatedPathString.mid(0,latestStartPos);
						cint64 lastSlashPos = latestBeginString.lastIndexOf("/");
						QString firstPathPart = latestBeginString.mid(0,lastSlashPos);
						QDir firstPathDir(firstPathPart);
						QStringList entryList = firstPathDir.entryList(QDir::Files | QDir::Dirs);
						cint64 maxNumber = 0;
						cint64 maxNumberStringLength = 3;
						foreach (QString entry, entryList) {
							QString numberString;
							for (QString::const_iterator it = entry.constBegin(), itEnd = entry.constEnd(); it != itEnd; ++it) {
								QChar charater = *it;
								if (charater.isDigit()) {
									numberString += charater;
								}
							}
							bool convertSucc;
							cint64 number = numberString.toLongLong(&convertSucc);
							if (convertSucc) {
								maxNumber = qMax(maxNumber,number);
								maxNumberStringLength = qMax((cint64)numberString.length(),maxNumberStringLength);
							}
						}
						QString newNumberString = QString::number(maxNumber+1);
						while (newNumberString.length() < maxNumberStringLength) {
							newNumberString.prepend("0");
						}

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,newNumberString);

					} else if (updatedPathString.indexOf("$$_LATEST_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_LATEST_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_LATEST_$$").length();

						QString latestBeginString = updatedPathString.mid(0,latestStartPos);
						cint64 lastSlashPos = latestBeginString.lastIndexOf("/");
						QString firstPathPart = latestBeginString.mid(0,lastSlashPos);
						QDir firstPathDir(firstPathPart);
						QStringList entryList = firstPathDir.entryList(QDir::Files | QDir::Dirs);
						cint64 maxNumber = 0;
						cint64 maxNumberStringLength = 0;
						foreach (QString entry, entryList) {
							QString numberString;
							for (QString::const_iterator it = entry.constBegin(), itEnd = entry.constEnd(); it != itEnd; ++it) {
								QChar charater = *it;
								if (charater.isDigit()) {
									numberString += charater;
								}
							}
							bool convertSucc;
							cint64 number = numberString.toLongLong(&convertSucc);
							if (convertSucc) {
								maxNumber = qMax(maxNumber,number);
								maxNumberStringLength = qMax((cint64)numberString.length(),maxNumberStringLength);
							}
						}
						QString newNumberString = QString::number(maxNumber);
						while (newNumberString.length() < maxNumberStringLength) {
							newNumberString.prepend("0");
						}

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,newNumberString);


					} else if (updatedPathString.indexOf("$$_DATETIME_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_DATETIME_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_DATETIME_$$").length();

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,QDateTime::currentDateTime().toString(Qt::ISODate).replace(":","-").replace(".","-"));

					} else if (updatedPathString.indexOf("$$_DATE_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_DATE_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_DATE_$$").length();

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,QDateTime::currentDateTime().date().toString(Qt::ISODate).replace(":","-").replace(".","-"));

					} else if (updatedPathString.indexOf("$$_TIME_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_TIME_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_TIME_$$").length();

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,QDateTime::currentDateTime().time().toString(Qt::ISODate).replace(":","-").replace(".","-"));

					} else if (updatedPathString.indexOf("$$_LATEST_STRING_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_LATEST_STRING_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_LATEST_STRING_$$").length();

						QString latestBeginString = updatedPathString.mid(0,latestStartPos);
						cint64 lastSlashPos = latestBeginString.lastIndexOf("/");
						QString firstPathPart = latestBeginString.mid(0,lastSlashPos);
						QDir firstPathDir(firstPathPart);
						QStringList entryList = firstPathDir.entryList(QDir::Files | QDir::Dirs);
						cint64 maxNumber = 0;
						QString maxNumberString;
						foreach (QString entry, entryList) {
							QString numberString;
							for (QString::const_iterator it = entry.constBegin(), itEnd = entry.constEnd(); it != itEnd; ++it) {
								QChar charater = *it;
								if (charater.isDigit()) {
									numberString += charater;
								}
							}
							bool convertSucc;
							cint64 number = numberString.toLongLong(&convertSucc);
							if (convertSucc) {
								if (maxNumberString.isEmpty() || number > maxNumber) {
									maxNumberString = entry;
									maxNumber = number;
								}
							}
						}

						updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,maxNumberString);


					} else if (updatedPathString.indexOf("$$_LATEST_VERSION_$$") == nextPos) {

						cint64 latestStartPos = updatedPathString.indexOf("$$_LATEST_VERSION_$$");
						cint64 latestEndPos = latestStartPos+QString("$$_LATEST_VERSION_$$").length();

						QString latestBeginString = updatedPathString.mid(0,latestStartPos);
						cint64 lastSlashPos = latestBeginString.lastIndexOf("/");
						QString firstPathPart = latestBeginString.mid(0,lastSlashPos);
						QString beginMatchPart = latestBeginString.mid(lastSlashPos+1,latestStartPos-lastSlashPos-1);
						QString endMatchPart = latestBeginString.mid(latestEndPos,updatedPathString.indexOf("/",latestEndPos)-latestEndPos);
						QDir firstPathDir(firstPathPart);
						QStringList entryList = firstPathDir.entryList(QDir::Files | QDir::Dirs);
						cint64 maxNumber = 0;

						QMap<CVersionNumbersItem,QString> versionNumberItemMap;


						foreach (QString entry, entryList) {
							bool hasFirstNumber = false;
							bool stopUseNummers = false;
							if (beginMatchPart.isEmpty() || entry.startsWith(beginMatchPart)) {
								QString endMatch;

								CVersionNumbersItem versionNumberItem;
								QString numberString;
								for (QString::const_iterator it = entry.constBegin(), itEnd = entry.constEnd(); it != itEnd; ++it) {
									QChar charater = *it;
									bool nonNumber = false;
									if (charater.isDigit()) {
										if (!stopUseNummers) {
											numberString += charater;
											hasFirstNumber = true;
										}
									} else if (charater.isLetter()) {
										nonNumber = true;
										if (hasFirstNumber) {
											stopUseNummers = true;
										}
									} else {
										nonNumber = true;
									}
									if (nonNumber) {
										if (!numberString.isEmpty()) {
											bool convertSucc;
											cint64 number = numberString.toLongLong(&convertSucc);
											if (convertSucc) {
												versionNumberItem.addNumber(number);
											}
											numberString.clear();
										}
									}
									if (stopUseNummers) {
										endMatch += charater;
									}
								}
								if (!numberString.isEmpty()) {
									bool convertSucc;
									cint64 number = numberString.toLongLong(&convertSucc);
									if (convertSucc) {
										versionNumberItem.addNumber(number);
									}
									numberString.clear();
								}
								if (endMatchPart.isEmpty() || endMatch == endMatchPart) {
									versionNumberItemMap.insert(versionNumberItem,entry);
								}
							}
						}

						if (!versionNumberItemMap.isEmpty()) {
							QMap<CVersionNumbersItem,QString>::const_iterator it = versionNumberItemMap.constBegin();
							QString maxNumberString(it.value());
							updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,maxNumberString);
						}




					} else if (updatedPathString.indexOf("$$_LATEST_VERSION-") == nextPos) {

						cint64 latestSubstractNumberBegin = nextPos+QString("$$_LATEST_VERSION-").length();
						cint64 latestSubstractNumberEnd = updatedPathString.indexOf("_$$",latestSubstractNumberBegin);
						QString latestSubstractNumberString = updatedPathString.mid(latestSubstractNumberBegin,latestSubstractNumberBegin-latestSubstractNumberEnd);

						QString latestMinusXString = latestSubstractNumberString;
						if (updatedPathString.indexOf(latestMinusXString) == nextPos) {

							cint64 substractNumber = latestSubstractNumberString.toLongLong();

							cint64 latestStartPos = updatedPathString.indexOf(latestMinusXString);
							cint64 latestEndPos = latestStartPos+QString(latestMinusXString).length();

							QString latestBeginString = updatedPathString.mid(0,latestStartPos);
							cint64 lastSlashPos = latestBeginString.lastIndexOf("/");
							QString firstPathPart = latestBeginString.mid(0,lastSlashPos);
							QString beginMatchPart = latestBeginString.mid(lastSlashPos+1,latestStartPos-lastSlashPos-1);
							QString endMatchPart = latestBeginString.mid(latestEndPos,updatedPathString.indexOf("/",latestEndPos)-latestEndPos);
							QDir firstPathDir(firstPathPart);
							QStringList entryList = firstPathDir.entryList(QDir::Files | QDir::Dirs);
							cint64 maxNumber = 0;

							QMap<CVersionNumbersItem,QString> versionNumberItemMap;


							foreach (QString entry, entryList) {
								bool hasFirstNumber = false;
								bool stopUseNummers = false;
								if (beginMatchPart.isEmpty() || entry.startsWith(beginMatchPart)) {
									QString endMatch;

									CVersionNumbersItem versionNumberItem;
									QString numberString;
									for (QString::const_iterator it = entry.constBegin(), itEnd = entry.constEnd(); it != itEnd; ++it) {
										QChar charater = *it;
										bool nonNumber = false;
										if (charater.isDigit()) {
											if (!stopUseNummers) {
												numberString += charater;
												hasFirstNumber = true;
											}
										} else if (charater.isLetter()) {
											nonNumber = true;
											if (hasFirstNumber) {
												stopUseNummers = true;
											}
										} else {
											nonNumber = true;
										}
										if (nonNumber) {
											if (!numberString.isEmpty()) {
												bool convertSucc;
												cint64 number = numberString.toLongLong(&convertSucc);
												if (convertSucc) {
													versionNumberItem.addNumber(number);
												}
												numberString.clear();
											}
										}
										if (stopUseNummers) {
											endMatch += charater;
										}
									}
									if (!numberString.isEmpty()) {
										bool convertSucc;
										cint64 number = numberString.toLongLong(&convertSucc);
										if (convertSucc) {
											versionNumberItem.addNumber(number);
										}
										numberString.clear();
									}
									if (endMatchPart.isEmpty() || endMatch == endMatchPart) {
										versionNumberItemMap.insert(versionNumberItem,entry);
									}
								}
							}

							if (versionNumberItemMap.count() > substractNumber) {
								QMap<CVersionNumbersItem,QString>::const_iterator it = versionNumberItemMap.constBegin();
								while (substractNumber > 0) {
									--substractNumber;
									++it;
								}
								QString maxNumberString(it.value());
								updatedPathString.replace(latestStartPos,latestEndPos-latestStartPos,maxNumberString);
							}
						}
					}

					

					++nextPos;

				}

				return updatedPathString;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

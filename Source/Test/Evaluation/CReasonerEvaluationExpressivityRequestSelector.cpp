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

#include "CReasonerEvaluationExpressivityRequestSelector.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			CReasonerEvaluationExpressivityRequestSelector::CReasonerEvaluationExpressivityRequestSelector(const QList<COntologyExpressivity>& equalBoundList, const QString& baseExpressivityDir) {
				mEqualBoundList = equalBoundList;
				mBaseExpressivityDir = baseExpressivityDir;
			}

			CReasonerEvaluationExpressivityRequestSelector::CReasonerEvaluationExpressivityRequestSelector(const QList<COntologyExpressivity>& lowerNotIncludedBoundList, const QList<COntologyExpressivity>& upperIncludedBoundList, const QString& baseExpressivityDir) {
				mLowerNotIncludedBoundList = lowerNotIncludedBoundList;
				mUpperIncludedBoundList = upperIncludedBoundList;
				mBaseExpressivityDir = baseExpressivityDir;
			}


			CReasonerEvaluationExpressivityRequestSelector::CReasonerEvaluationExpressivityRequestSelector() {
			}


			bool CReasonerEvaluationExpressivityRequestSelector::isGroupSelected(const QString& groupString) {
				return true;
			}

			bool CReasonerEvaluationExpressivityRequestSelector::isRequestSelected(const QString& groupString, const QString& requestString) {
				QString expressivityString;
				bool foundExpressivity = false;
				QString infoFileString = mBaseExpressivityDir+groupString+requestString+".txt";
				QFile infoFile(infoFileString);
				if (infoFile.open(QIODevice::ReadOnly)) {
					while (!foundExpressivity && !infoFile.atEnd()) {
						QString line = infoFile.readLine();
						QStringList keyValueLists = line.split("\t");
						if (keyValueLists.count() >= 2) {
							if (keyValueLists.first().trimmed() == "Expressivness:") {
								foundExpressivity = true;
								expressivityString = keyValueLists.last().trimmed();
							}
						}
					}
					infoFile.close();
				}
				if (!foundExpressivity) {
					return false;
				} else {
					COntologyExpressivity expressivity(expressivityString);
					foreach (COntologyExpressivity expBound, mEqualBoundList) {
						if (expressivity.isExpressivityEqual(&expBound)) {
							return true;
						}
					}
					if (!mEqualBoundList.isEmpty()) {
						return false;
					}
					foreach (COntologyExpressivity expBound, mExcludedBoundList) {
						if (expressivity.isExpressivityEqual(&expBound)) {
							return false;
						}
					}
					foreach (COntologyExpressivity expBound, mLowerNotIncludedBoundList) {
						if (expBound.isExpressivityEntailed(&expressivity)) {
							return false;
						}
					}
					foreach (COntologyExpressivity expBound, mLowerIncludedBoundList) {
						if (expBound.isExpressivityEntailed(&expressivity) && !expressivity.isExpressivityEqual(&expBound)) {
							return false;
						}
					}
					bool entailedByOneUpperBounds = false;
					foreach (COntologyExpressivity expBound, mUpperNotIncludedBoundList) {
						if (expBound.isExpressivityEntailed(&expressivity) || !expressivity.isExpressivityEqual(&expBound)) {
							entailedByOneUpperBounds |= true;
						}
					}
					foreach (COntologyExpressivity expBound, mUpperIncludedBoundList) {
						if (expBound.isExpressivityEntailed(&expressivity)) {
							entailedByOneUpperBounds |= true;
						}
					}
					if (!entailedByOneUpperBounds) {
						return false;
					}
				}
				return true;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

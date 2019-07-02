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

#include "CReasonerEvaluationDirectoryStructureSelectorFactory.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationDirectoryStructureSelectorFactory::CReasonerEvaluationDirectoryStructureSelectorFactory(const QString& groupDirString, const QString& dirString, const QString& baseEvalName, const QString& baseOutputDir) {
				mDirectoryString = dirString;
				mBaseEvalName = baseEvalName;
				mBaseOutputDir = baseOutputDir;
				mGroupDirString = groupDirString;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationDirectoryStructureSelectorFactory::getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector) {
				if (!mDirectoryString.endsWith("/")) {
					mDirectoryString += QString("/");
				}
				QList<CReasonerEvaluationStringMatchGroupSelector*> selList;
				CReasonerEvaluationGroupRequestSelector* selectors = getSelectors(nullptr,mDirectoryString,selList);
				if (selectors) {
					selectors = selectors->append(prevSelector);
				} else {
					selectors = prevSelector;
				}
				return selectors;
			}


			CReasonerEvaluationGroupRequestSelector* CReasonerEvaluationDirectoryStructureSelectorFactory::getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector, const QString& dirString, const QList<CReasonerEvaluationStringMatchGroupSelector*>& subDirAddList) {
				CReasonerEvaluationGroupRequestSelector* selectors = prevSelector;

				QDir directory(dirString);
				QStringList subDirStringList = directory.entryList(QDir::Dirs);
				QStringList fileStringList = directory.entryList(QDir::Files);

				QString dirGroupString = dirString;
				dirGroupString.replace(mGroupDirString,"");

				QList<CReasonerEvaluationStringMatchGroupSelector*> tmpSubDirAddList(subDirAddList);

				if (subDirStringList.count() > 3 || fileStringList.count() > 0) {
					CReasonerEvaluationStringMatchGroupSelector* tmpSelector = new CReasonerEvaluationStringMatchGroupSelector();
					QString selectorName(mBaseEvalName+" :: "+dirGroupString);
					//if (selectorName.endsWith("/")) {
					//	selectorName = selectorName.remove(selectorName.count()-1,1);
					//}
					//selectorName = selectorName.replace("/"," :: ");
					tmpSelector->setNameString(selectorName);
					tmpSelector->setOutputString(mBaseOutputDir+dirGroupString);
					selectors = tmpSelector->append(selectors);
					tmpSubDirAddList.append(tmpSelector);
				}


				foreach (CReasonerEvaluationStringMatchGroupSelector* selector, tmpSubDirAddList) {
					selector->addGroupString(dirGroupString);
				}
				foreach (QString subDir, subDirStringList) {
					if (subDir != ".." && subDir != ".") {
						selectors = getSelectors(selectors,dirString+subDir+"/",tmpSubDirAddList);
					}
				}
				return selectors;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

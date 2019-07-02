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

#include "CKoncludeInfo.h"
#include "revision-git.h"


namespace Konclude {


	CKoncludeInfo::CKoncludeInfo() {
	}


	QString CKoncludeInfo::getKoncludeName() {
		return KONCLUDE_NAME;
	}

	QString CKoncludeInfo::getKoncludeNameExtension() {
		return KONCLUDE_NAME_EXTENSION;
	}

	QString CKoncludeInfo::getKoncludeDescription() {
		return KONCLUDE_DESCRIPTION;
	}

	QString CKoncludeInfo::getKoncludeVersionString() {
		return QString(KONCLUDE_VERSION_GIT_TAG_NAME_STRING)+QString("-%1 - ").arg(KONCLUDE_VERSION_GIT_REVISION_NUMBER)+QString(KONCLUDE_VERSION_GIT_REVISION_HASH_STRING);
	}

	QString CKoncludeInfo::getKoncludeCompilationDateString() {
		return QString(__DATE__);
	}

	QString CKoncludeInfo::getKoncludeBitPlatformString() {
		return QString("%1-bit").arg(sizeof(void*)*8);
	}



	int CKoncludeInfo::getKoncludeMajorVersionNumber() {
		int majorVersionNumber = KONCLUDE_VERSION_MAJOR;
		QStringList splittedList = QString(KONCLUDE_VERSION_GIT_TAG_NAME_STRING).split(".");
		if (splittedList.count() >= 3) {
			QString majorVersionString = splittedList.first();
			bool successfulConverted = false;
			int majorVersionNumberConv = majorVersionString.toInt(&successfulConverted);
			if (successfulConverted) {
				majorVersionNumber = majorVersionNumberConv;
			}
		}
		return majorVersionNumber;
	}

	int CKoncludeInfo::getKoncludeMinorVersionNumber() {
		int minorVersionNumber = KONCLUDE_VERSION_MINOR;
		QStringList splittedList = QString(KONCLUDE_VERSION_GIT_TAG_NAME_STRING).split(".");
		if (splittedList.count() >= 3) {
			QString minorVersionString = splittedList.at(1);
			bool successfulConverted = false;
			int minorVersionNumberConv = minorVersionString.toInt(&successfulConverted);
			if (successfulConverted) {
				minorVersionNumber = minorVersionNumberConv;
			}
		}
		return minorVersionNumber;
	}

	int CKoncludeInfo::getKoncludeBuildVersionNumber() {
		int buildVersionNumber = KONCLUDE_VERSION_BUILD;
		QStringList splittedList = QString(KONCLUDE_VERSION_GIT_TAG_NAME_STRING).split(".");
		if (splittedList.count() >= 3) {
			QString buildVersionString = splittedList.at(2);
			bool successfulConverted = false;
			int buildVersionNumberConv = buildVersionString.toInt(&successfulConverted);
			if (successfulConverted) {
				buildVersionNumber = buildVersionNumberConv;
			}
		}
		return buildVersionNumber;
	}

	int CKoncludeInfo::getKoncludeRevisionVersionNumber() {
		int revisionVersionNumber = KONCLUDE_VERSION_GIT_REVISION_NUMBER;
		return revisionVersionNumber;
	}


}; // end namespace Konclude

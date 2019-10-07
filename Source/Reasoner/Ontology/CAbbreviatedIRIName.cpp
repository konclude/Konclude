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

#include "CAbbreviatedIRIName.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CAbbreviatedIRIName::CAbbreviatedIRIName(CNamePrefix *namePrefix, const QString &nameString) : CIRIName(nameString) {
				prefix = namePrefix;
			}


			CAbbreviatedIRIName *CAbbreviatedIRIName::init(CNamePrefix *namePrefix, const QString &iriNameString) {
				CIRIName::init(iriNameString);
				prefix = namePrefix;
				return this;
			}


			CAbbreviatedIRIName::~CAbbreviatedIRIName() {
			}

			QString CAbbreviatedIRIName::getIRIName() {
				return prefix->getPrefixString()+iriString;
			}


			QString CAbbreviatedIRIName::getAbbreviatedIRIName() {
				return iriString;
			}

			QString CAbbreviatedIRIName::getAbbreviatedPrefixWithAbbreviatedIRIName(const QString &joiningString) {
				return prefix->getAbbreviatedPrefixString()+joiningString+iriString;
			}

			CNamePrefix *CAbbreviatedIRIName::getNamePrefix() {
				return prefix;
			}


			QStringList CAbbreviatedIRIName::getAbbreviatedIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference) {
				QStringList stringList;
				qint64 firstNameVersion = -1;
				while (nameLinker) {
					CName *name = nameLinker->getData();
					qint64 nameVerID = name->getNameVersionID();
					if (firstNameVersion <= 0) {
						firstNameVersion = nameVerID;
					}
					if (firstNameVersion - nameVerID > nameVersionDifference && nameVersionDifference != -1) {
						break;
					}
					CAbbreviatedIRIName *iriName = dynamic_cast<CAbbreviatedIRIName *>(name);
					if (iriName) {
						stringList.append(iriName->getAbbreviatedIRIName());
					}
					nameLinker = nameLinker->getNext();
				}
				return stringList;
			}


			QStringList CAbbreviatedIRIName::getAbbreviatedPrefixWithAbbreviatedIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference, const QString &joiningString) {
				QStringList stringList;
				qint64 firstNameVersion = -1;
				while (nameLinker) {
					CName *name = nameLinker->getData();
					qint64 nameVerID = name->getNameVersionID();
					if (firstNameVersion <= 0) {
						firstNameVersion = nameVerID;
					}
					if (firstNameVersion - nameVerID > nameVersionDifference && nameVersionDifference != -1) {
						break;
					}
					CAbbreviatedIRIName *iriName = dynamic_cast<CAbbreviatedIRIName *>(name);
					if (iriName) {
						stringList.append(iriName->getAbbreviatedPrefixWithAbbreviatedIRIName(joiningString));
					}
					nameLinker = nameLinker->getNext();
				}
				return stringList;
			}


			QString CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(CLinker<CName *> *nameLinker, qint64 nameVersionDifference, const QString &joiningString) {
				QString stringName;
				qint64 firstNameVersion = -1;
				while (nameLinker) {
					CName *name = nameLinker->getData();
					qint64 nameVerID = name->getNameVersionID();
					if (firstNameVersion <= 0) {
						firstNameVersion = nameVerID;
					}
					if (firstNameVersion - nameVerID > nameVersionDifference && nameVersionDifference != -1) {
						break;
					}
					CAbbreviatedIRIName *iriName = dynamic_cast<CAbbreviatedIRIName *>(name);
					if (iriName) {
						stringName = iriName->getAbbreviatedPrefixWithAbbreviatedIRIName(joiningString);
						break;
					}
					nameLinker = nameLinker->getNext();
				}
				return stringName;
			}


			bool CAbbreviatedIRIName::hasAbbreviatedIRIName(CLinker<CName *> *nameLinker, CNamePrefix *namePrefix, const QString &checkName, qint64 nameVersionDifference) {
				qint64 firstNameVersion = -1;
				while (nameLinker) {
					CName *name = nameLinker->getData();
					qint64 nameVerID = name->getNameVersionID();
					if (firstNameVersion <= 0) {
						firstNameVersion = nameVerID;
					}
					if (firstNameVersion - nameVerID > nameVersionDifference && nameVersionDifference != -1) {
						break;
					}
					CAbbreviatedIRIName *iriName = dynamic_cast<CAbbreviatedIRIName *>(name);
					if (iriName) {
						if (iriName->getNamePrefix() == namePrefix && checkName == iriName->getAbbreviatedIRIName()) {
							return true;
						}
					}
					nameLinker = nameLinker->getNext();
				}
				return false;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

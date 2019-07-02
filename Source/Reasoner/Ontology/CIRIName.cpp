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

#include "CIRIName.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CIRIName::CIRIName(const QString &nameString) {
				iriString = nameString;
			}


			CIRIName::~CIRIName() {
			}


			CIRIName *CIRIName::init(const QString &iriNameString) {
				iriString = iriNameString;
				return this;
			}


			QString CIRIName::getIRIName() {
				return iriString;
			}

			CIRIName *CIRIName::setIRIName(const QString &iriNameString) {
				iriString = iriNameString;
				return this;
			}

			QStringList CIRIName::getIRINames(CLinker<CName *> *nameLinker, qint64 nameVersionDifference) {
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
					CIRIName *iriName = dynamic_cast<CIRIName *>(name);
					if (iriName) {
						stringList.append(iriName->getIRIName());
					}
					nameLinker = nameLinker->getNext();
				}
				return stringList;
			}


			QString CIRIName::getRecentIRIName(CLinker<CName *> *nameLinker, qint64 nameVersionDifference) {
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
					CIRIName *iriName = dynamic_cast<CIRIName *>(name);
					if (iriName) {
						stringName = iriName->getIRIName();
						break;
					}
					nameLinker = nameLinker->getNext();
				}
				return stringName;
			}

			bool CIRIName::hasIRIName(CLinker<CName *> *nameLinker, const QString &checkName, qint64 nameVersionDifference) {
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
					CIRIName *iriName = dynamic_cast<CIRIName *>(name);
					if (iriName) {
						if (checkName == iriName->getIRIName()) {
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

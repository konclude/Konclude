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

#include "CConformanceTask.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			QList<CConformanceTask::TYPE> CConformanceTask::allTypes() {
				QList<TYPE> types;
				types << TYPE::CONSISTENCY << TYPE::SATISFIABILITY << TYPE::CLASSIFICATION
					  << TYPE::OWLLINK << TYPE::SPARQL;
				return types;
			}


			QString CConformanceTask::typeToString(TYPE type) {
				switch (type) {
					case TYPE::CONSISTENCY: return QString("consistency");
					case TYPE::SATISFIABILITY: return QString("satisfiability");
					case TYPE::CLASSIFICATION: return QString("classification");
					case TYPE::OWLLINK: return QString("owllink");
					case TYPE::SPARQL: return QString("sparql");
				}
				return QString();
			}


			bool CConformanceTask::typeFromString(const QString& text, TYPE& type) {
				QString normalizedText = text.trimmed().toLower();
				for (TYPE availableType : allTypes()) {
					if (QString::compare(normalizedText, typeToString(availableType), Qt::CaseInsensitive) == 0) {
						type = availableType;
						return true;
					}
				}
				return false;
			}


			QString CConformanceTask::typeToTestClassName(TYPE type) {
				switch (type) {
					case TYPE::CONSISTENCY: return QString("kt:ConsistencyTest");
					case TYPE::SATISFIABILITY: return QString("kt:SatisfiabilityTest");
					case TYPE::CLASSIFICATION: return QString("kt:ClassificationTest");
					case TYPE::OWLLINK: return QString();
					case TYPE::SPARQL: return QString("kt:SparqlTest");
				}
				return QString();
			}


			bool CConformanceTask::typeFromTestClassName(const QString& className, TYPE& type) {
				QString classNamePart = localName(className);
				for (TYPE availableType : allTypes()) {
					QString markerName = typeToTestClassName(availableType);
					if (!markerName.isEmpty() && QString::compare(classNamePart, localName(markerName), Qt::CaseInsensitive) == 0) {
						type = availableType;
						return true;
					}
				}
				return false;
			}


			QString CConformanceTask::verdictToString(TYPE type, VERDICT verdict) {
				if (verdict == VERDICT::UNSET) {
					return QString();
				}
				bool positive = (verdict == VERDICT::POSITIVE);
				switch (type) {
					case TYPE::CONSISTENCY: return positive ? QString("consistent") : QString("inconsistent");
					case TYPE::SATISFIABILITY: return positive ? QString("satisfiable") : QString("unsatisfiable");
					case TYPE::CLASSIFICATION: return positive ? QString("match") : QString("mismatch");
					case TYPE::OWLLINK: return positive ? QString("true") : QString("false");
					case TYPE::SPARQL: return positive ? QString("true") : QString("false");
				}
				return QString();
			}


			QString CConformanceTask::localName(const QString& iri) {
				QString localName = iri;
				int hashIndex = localName.lastIndexOf('#');
				if (hashIndex >= 0) {
					localName = localName.mid(hashIndex + 1);
				}
				int colonIndex = localName.lastIndexOf(':');
				if (colonIndex >= 0) {
					localName = localName.mid(colonIndex + 1);
				}
				return localName;
			}


			bool CConformanceTask::verdictFromString(TYPE type, const QString& text, VERDICT& verdict) {
				QString trimmedText = text.trimmed();
				if (!trimmedText.isEmpty()) {
					if (QString::compare(trimmedText, verdictToString(type, VERDICT::POSITIVE), Qt::CaseInsensitive) == 0) {
						verdict = VERDICT::POSITIVE;
						return true;
					}
					if (QString::compare(trimmedText, verdictToString(type, VERDICT::NEGATIVE), Qt::CaseInsensitive) == 0) {
						verdict = VERDICT::NEGATIVE;
						return true;
					}
				}
				return false;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

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

#include "COntologyRevision.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {


			COntologyRevision::COntologyRevision(CConcreteOntology *ontology, COntologyRevision *previousOntologyRevision, COntologyConfigurationExtension *ontRevConfig) {
				revisionDepth = 0;
				rootRev = 0;
				revisionPath = 0;
				prevRev = previousOntologyRevision;
				onto = ontology;
				config = ontRevConfig;
				if (prevRev) {
					revisionDepth = prevRev->getOntologyRevisionDepth() + 1;
					rootRev = prevRev->getRootOntologyRevision();
					revisionPath = prevRev->getOntologyRevisionPath();
				}
			}


			COntologyRevision::~COntologyRevision() {
				delete config;
				delete onto;
			}

			COntologyRevision *COntologyRevision::getPreviousOntologyRevision() {
				return prevRev;
			}

			COntologyRevision *COntologyRevision::getRootOntologyRevision() {
				return rootRev;
			}



			CConcreteOntology *COntologyRevision::getOntology() {
				return onto;
			}

			COntologyRevision *COntologyRevision::setOntology(CConcreteOntology *ontology) {
				onto = ontology;
				return this;
			}

			qint64 COntologyRevision::getOntologyRevisionDepth() {
				return revisionDepth;
			}

			COntologyRevision *COntologyRevision::setOntologyRevisionDepth(qint64 depth) {
				revisionDepth = depth;
				return this;
			}

			qint64 COntologyRevision::getOntologyRevisionPath() {
				return revisionPath;
			}

			COntologyRevision *COntologyRevision::setOntologyRevisionPath(qint64 ontologyRevisionPath) {
				revisionPath = ontologyRevisionPath;
				return this;
			}

			COntologyConfigurationExtension *COntologyRevision::getOntologyConfiguration() {
				return config;
			}

			COntologyRevision *COntologyRevision::setOntologyConfiguration(COntologyConfigurationExtension *ontRevConfig) {
				config = ontRevConfig;
				return this;
			}

	

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

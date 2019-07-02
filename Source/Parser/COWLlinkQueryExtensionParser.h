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

#ifndef KONCLUDE_PARSER_COWLlinkQueryExtensionParser_H
#define KONCLUDE_PARSER_COWLlinkQueryExtensionParser_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "COWLlinkQtXMLResultParser.h"

// Other includes
#include "Reasoner/Classifier/CClassificationCalculationSupport.h"
#include "Reasoner/Classifier/CPartialPruningTaxonomy.h"
#include "Reasoner/Classifier/CTaxonomySatisfiableSubsumptionCalculationSponsor.h"

#include "Reasoner/Ontology/COntology.h"
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner;
	using namespace Classifier;
	using namespace Ontology;

	namespace Parser {




		/*! 
		 *
		 *		\class		COWLlinkQueryExtensionParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLlinkQueryExtensionParser {
			// public methods
			public:
				//! Constructor
				COWLlinkQueryExtensionParser();

				//! Destructor
				virtual ~COWLlinkQueryExtensionParser();

				virtual CTaxonomyPremisingQuerySupport *parseTaxonomyQuerySupport(CConcreteOntology *ontology, QDomElement *queryNode);

			// protected methods
			protected:
				CClassificationCalculationSupport *parseTaxonomyCalculationSupport(CConcreteOntology *ontology, QDomElement *node);

			// private methods
			private:

			// private variables
			private:


		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLlinkQueryExtensionParser_H

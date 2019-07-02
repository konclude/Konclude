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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIEDCALLBACKDATACONTEXT_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIEDCALLBACKDATACONTEXT_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"

// Other includes
#include "Concurrent/Callback/CCallbackDataContext.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassifiedCallbackDataContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassifiedCallbackDataContext : public CCallbackDataContext {
				// public methods
				public:
					//! Constructor
					CClassifiedCallbackDataContext(CTaxonomy *taxanomy, CConcreteOntology *ontology);

					//! Destructor
					virtual ~CClassifiedCallbackDataContext();

					virtual CConcreteOntology *getOntology();
					virtual CTaxonomy *getTaxonomy();

				// protected methods
				protected:

				// protected variables
				protected:
					CTaxonomy *tax;
					CConcreteOntology *onto;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFIEDCALLBACKDATACONTEXT_H

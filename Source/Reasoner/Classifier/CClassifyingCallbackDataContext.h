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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFYINGCALLBACKDATACONTEXT_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFYINGCALLBACKDATACONTEXT_H

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
			 *		\class		CClassifyingCallbackDataContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassifyingCallbackDataContext : public CCallbackDataContext {
				// public methods
				public:
					//! Constructor
					CClassifyingCallbackDataContext(bool classifying, CTaxonomy *taxanomy, CConcreteOntology *ontology);

					//! Destructor
					virtual ~CClassifyingCallbackDataContext();

					virtual CConcreteOntology *getOntology();
					virtual CTaxonomy *getTaxonomy();
					virtual bool isClassifying();

				// protected methods
				protected:

				// protected variables
				protected:
					CTaxonomy *tax;
					CConcreteOntology *onto;
					bool ontClassifying;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFYINGCALLBACKDATACONTEXT_H

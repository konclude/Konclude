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

#ifndef KONCLUDE_REASONER_REALIZER_CREALIZINGCALLBACKDATACONTEXT_H
#define KONCLUDE_REASONER_REALIZER_CREALIZINGCALLBACKDATACONTEXT_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"

// Other includes
#include "Concurrent/Callback/CCallbackDataContext.h"

#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		CRealizingCallbackDataContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizingCallbackDataContext : public CCallbackDataContext {
				// public methods
				public:
					//! Constructor
					CRealizingCallbackDataContext(bool realized, CConcreteOntology* ontology);

					virtual CConcreteOntology *getOntology();
					virtual bool isRealized();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					bool mRealizered;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CREALIZINGCALLBACKDATACONTEXT_H

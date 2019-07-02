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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CCACHINGTAGS_H
#define KONCLUDE_REASONER_ONTOLOGY_CCACHINGTAGS_H

// Libraries includes

// Namespace includes
#include "OntologySettings.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CCachingTags
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCachingTags {
				// public methods
				public:
					//! Constructor
					CCachingTags();

					cint64 getLastCachingTag();
					cint64 getMaxCachedTag();
					cint64 getMinCachedTag();

					CCachingTags* setLastCachingTag(cint64 tag);
					CCachingTags* setMaxCachedTag(cint64 tag);
					CCachingTags* setMinCachedTag(cint64 tag);

					bool setMaxCachedTagCandidate(cint64 tag);
					bool setMinCachedTagCandidate(cint64 tag);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mLastCachingTag;
					cint64 mMaxCachedTag;
					cint64 mMinCachedTag;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CCACHINGTAGS_H

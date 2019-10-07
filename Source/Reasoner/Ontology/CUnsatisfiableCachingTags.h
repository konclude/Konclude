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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CUNSATISFIABLECACHINGTAGS_H
#define KONCLUDE_REASONER_ONTOLOGY_CUNSATISFIABLECACHINGTAGS_H

// Libraries includes

// Namespace includes
#include "OntologySettings.h"
#include "CCachingTags.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CUnsatisfiableCachingTags
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CUnsatisfiableCachingTags : public CCachingTags {
				// public methods
				public:
					//! Constructor
					CUnsatisfiableCachingTags();


					cint64 getMinUnsatisfiableCachedSize();

					CCachingTags* setMinUnsatisfiableCachedSize(cint64 size);

					bool setMinUnsatisfiableCachedSizeCandidate(cint64 size);

					bool updateCachingTags(cint64 cachedTagCandidate, cint64 cachingNumberTag, cint64 sizeCandidate);

					bool candidateTags(cint64& minMaxCachedTag, cint64& maxMinCachedTag, cint64& minUnsatCachedSize, cint64 requiredLastCachingTag);


					bool hasCandidateTags(cint64 minMaxCachedTag, cint64 maxMinCachedTag, cint64 requiredLastCachingTag);
					bool candidateMinUnsatisfiableSize(cint64& minUnsatCachedSize, cint64 cachedTag);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mMinUnsatCachedSize;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CUNSATISFIABLECACHINGTAGS_H

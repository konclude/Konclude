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

#ifndef KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_H
#define KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CATaxonomy.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CEATaxonomy
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CEATaxonomy : public CATaxonomy<T,N,E> {
				// public methods
				public:
					//! Constructor
					CEATaxonomy();

					bool isElementNotSubsumption(const E& notSubsumerElement, const E& notSubsumedElement);
					bool isElementDisjoint(const E& disjointElement1, const E& disjointElement2);
					bool isElementNotDisjoint(const E& disjointElement1, const E& disjointElement2);

					virtual bool isNodeNotSubsumption(N* notSubsumerNode, N* notSubsumedNode) = 0;
					virtual bool isNodeDisjoint(N* disjointNode1, N* disjointNode2) = 0;
					virtual bool isNodeNotDisjoint(N* disjointNode1, N* disjointNode2) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CEATaxonomy.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_H

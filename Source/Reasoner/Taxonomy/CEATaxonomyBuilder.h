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

#ifndef KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_H
#define KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_H

// Libraries includes


// Namespace includes
#include "TaxonomySettings.h"
#include "CATaxonomyBuilder.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			/*! 
			 *
			 *		\class		CEATaxonomyBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class N, class E>
			class CEATaxonomyBuilder : public CATaxonomyBuilder<T,N,E> {
				// public methods
				public:
					//! Constructor
					CEATaxonomyBuilder();


					T* setElementNotSubsumption(const E& notSubsumerElement, const E& notSubsumedElement);
					T* setElementDisjoints(const E& disjointElement1, const E& disjointElement2);

					virtual T* setNodeNotSubsumption(N* notSubsumerNode, N* notSubsumedNode) = 0;
					virtual T* setNodeDisjoints(N* disjointNode1, N* disjointNode2) = 0;


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

#include "CEATaxonomyBuilder.cpp"

#endif // KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_H

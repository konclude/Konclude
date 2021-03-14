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

#ifndef KONCLUDE_REASONER_TRIPLES_CTRIPLESDATAITERATOR_H
#define KONCLUDE_REASONER_TRIPLES_CTRIPLESDATAITERATOR_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Triples {




			/*! 
			 *
			 *		\class		CTriplesDataIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTriplesDataIterator {
				// public methods
				public:
					//! Constructor
					CTriplesDataIterator();



					enum TRIPLE_NODE_TYPE {
						TRIPLE_URI_NODE_TYPE,
						TRIPLE_BLANK_NODE_TYPE,
						TRIPLE_LITERAL_NODE_TYPE
					};

					virtual QString getNodeString(cint64 pos) = 0;
					virtual TRIPLE_NODE_TYPE getNodeType(cint64 pos) = 0;

					virtual bool isURINodeType(cint64 pos);
					virtual bool isBlankNodeType(cint64 pos);
					virtual bool isLiteralNodeType(cint64 pos);


					virtual bool hasValue() = 0;
					virtual CTriplesDataIterator* moveNext() = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CTRIPLESDATAITERATOR_H

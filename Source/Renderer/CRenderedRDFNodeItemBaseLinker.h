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

#ifndef KONCLUDE_RENDERER_CRENDEREDRDFNODEITEMBASELINKER_H
#define KONCLUDE_RENDERER_CRENDEREDRDFNODEITEMBASELINKER_H

// Libraries includes

// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		CRenderedRDFNodeItemBaseLinker
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CRenderedRDFNodeItemBaseLinker : public CRenderedItemLinker {
			// public methods
			public:
				//! Constructor
				CRenderedRDFNodeItemBaseLinker();
				virtual ~CRenderedRDFNodeItemBaseLinker();

				enum NODE_ITEM_TYPE {
					NODE_ITEM_NONE_TYPE,
					NODE_ITEM_CLASS_TYPE,
					NODE_ITEM_OBJECT_PROPERTY_TYPE,
					NODE_ITEM_DATA_PROPERTY_TYPE,
					NODE_ITEM_INDIVIDUAL_PROPERTY_TYPE,
					NODE_ITEM_DATATYPE_TYPE,
					NODE_ITEM_LITERAL_TYPE,
					NODE_ITEM_PROPERTY_CHAIN_TYPE,
				};


				CRenderedRDFNodeItemBaseLinker* getNextItemLinker();

				NODE_ITEM_TYPE getNodeItemType();
				CRenderedRDFNodeItemBaseLinker* setNodeItemType(NODE_ITEM_TYPE type);

			// protected methods
			protected:

			// protected variables
			protected:
				NODE_ITEM_TYPE mNodeItemType = NODE_ITEM_NONE_TYPE;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CRENDEREDRDFNODEITEMBASELINKER_H

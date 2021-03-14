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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_RENDERER_CONTOLOGYREDLANDRDFRENDERER_H
#define KONCLUDE_RENDERER_CONTOLOGYREDLANDRDFRENDERER_H

// Libraries includes
#include <QUuid>


// Namespace includes
#include "RendererSettings.h"
#include "COntologyRDFRendererBase.h"
#include "CRenderedRedlandRDFNodeItemBaseLinker.h"
#include "CRenderedRedlandRDFTripleItemBaseLinker.h"


// Other includes
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Triples/CRedlandStoredTriplesData.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Reasoner::Ontology;
	using namespace Reasoner::Triples;

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyRedlandRDFRenderer
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyRedlandRDFRenderer : public COntologyRDFRendererBase {
			// public methods
			public:
				//! Constructor
				COntologyRedlandRDFRenderer(librdf_world* world);

				//! Destructor
				virtual ~COntologyRedlandRDFRenderer();




				virtual CRenderedRDFTripleItemBaseLinker* createRenderedTripleItemLinker();
				virtual COntologyRedlandRDFRenderer* releaseRenderedTripleItemLinkers(CRenderedItemLinker* linkers);

				virtual CRenderedRDFNodeItemBaseLinker* createRenderedNodeItemLinker();
				virtual COntologyRedlandRDFRenderer* releaseRenderedNodeItemLinkers(CRenderedItemLinker* linkers);

				CRedlandStoredTriplesData* getRenderedTripleData(CRedlandStoredTriplesData* tripleData = nullptr);



			// protected methods
			protected:


				virtual CRenderedRDFNodeItemBaseLinker* createRenderedIRIResourceNodeItem(const QString& iri);
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedBlankNodeItem(const QString& identifier);
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedBlankNodeItem(cint64 id);
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedLiteralNodeItem(const QString& literal, const QString& datatype);
				virtual CRenderedRDFTripleItemBaseLinker* createTriple(CRenderedRDFNodeItemBaseLinker* subjectNodeItem, CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedRDFNodeItemBaseLinker* objectNodeItem);

				COntologyRedlandRDFRenderer* addRenderedTripleItemLinkerToTripleData(CRenderedRDFTripleItemBaseLinker* renderedItemLinker, CXLinker<librdf_statement*>*& firstLinker, CXLinker<librdf_statement*>*& lastLinker);

			// protected variables
			protected:
				QList<CRenderedRedlandRDFNodeItemBaseLinker*> mRenderedNodeItemContainer;
				CRenderedRedlandRDFNodeItemBaseLinker* mTmpRenderedNodeItemLinker;
				QList<CRenderedRedlandRDFTripleItemBaseLinker*> mRenderedTripleItemContainer;
				CRenderedRedlandRDFTripleItemBaseLinker* mTmpRenderedTripleItemLinker;

				librdf_world* mWorld;

				QString mBlankNodeIdentifierPrefix;
				QString mDefaultDatatypeString;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYREDLANDRDFRENDERER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION

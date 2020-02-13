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

#ifndef KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONCONVERTIONINDEXER_H
#define KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONCONVERTIONINDEXER_H


// Libraries includes
#include <QHash>
#include <QCache>


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandNodeHasher.h"
#include "CRedlandStoredTriplesData.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCache.h"
#include "CRedlandStoredTriplesSimpleABoxAssertionsAccessor.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CIndividualProcessData.h"

#include "Reasoner/Preprocess/CDataLiteralNormalizerPreProcess.h"
#include "Reasoner/Preprocess/CPreProcessContextBase.h"
#include "Reasoner/Preprocess/CPreProcessContext.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Preprocess;

		namespace Triples {


			/*!
			 *
			 *		\class		CRedlandStoredTriplesIndividualAssertionConvertionIndexer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualAssertionConvertionIndexer {
				// public methods
				public:
					CRedlandStoredTriplesIndividualAssertionConvertionIndexer();
					virtual ~CRedlandStoredTriplesIndividualAssertionConvertionIndexer();


					bool indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData);





				// protected functions
				protected:

					class CIndividualNodeData {
					public:
						CIndividualNodeData(librdf_node* individualNode, CIndividual* individual) {
							mIndividual = individual;
							mIndividualNode = individualNode;
						}

						librdf_node* mIndividualNode;
						CIndividual* mIndividual;
					};

					CIndividualNodeData* getIndividualData(librdf_node* subjectNode, CIndividualNodeData*& lastIndiData, librdf_node*& lastSubjectNode, CConcreteOntology* ontology);
					CIndividualNodeData* getIndividualData(librdf_node* nodeNode, CConcreteOntology* ontology);

					CDatatype* getDatatypeFromDatatypeUri(librdf_uri* uri, CConcreteOntology* ontology, CRedlandStoredTriplesData* ontologyTripleData);
					CDataLiteralValue* createDataLiteralValue(CDataLiteral* dataLiteral, CConcreteOntology* ontology);

					void identifyIndividuals(librdf_statement* statement, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject, CIndividualNodeData* &lastIndiData, librdf_node* lastSubjectNode, CConcreteOntology* updateConcreteOntology, cint64 &conceptAssertionCount, CRedlandStoredTriplesData* redlandTriplesData, cint64 &dataPropertyAssertionCount, cint64 &objectPropertyAssertionCount);

				// protected variables
				protected:

					
					class CRoleNodeData {
					public:
						CRoleNodeData(librdf_node* roleNode, CRole* role, bool dataRole) {
							mRole = role;
							mDataRole = dataRole;
							mRoleNode = roleNode;
						}

						librdf_node* mRoleNode;
						CRole* mRole;
						bool mDataRole;
					};


					class CConceptNodeData {
					public:
						CConceptNodeData(librdf_node* conceptNode, CConcept* concept) {
							mConcept = concept;
							mConceptNode = conceptNode;
						}

						librdf_node* mConceptNode;
						CConcept* mConcept;
					};


					QHash<CRedlandNodeHasher, CRoleNodeData*> mRoleNodeDataHash;
					QHash<CRedlandNodeHasher, CConceptNodeData*> mConceptNodeDataHash;
					QHash<CRedlandNodeHasher, CIndividualNodeData*> mIndividualNodeDataHash;

					cint64 mNextIndiId;
					CMemoryAllocationManager* mMemMan;

					QHash<CRedlandUriHasher, CDatatype*> mDatatypeIriDatatypeHash;
					bool mDatatypeHashInitialized = false;
					QString mAnonymousOntologyIdentifier;

					CDatatype* mDefaultDatatype;
					CDataLiteralNormalizerPreProcess mDataLiteralNormalizer;
					CPreProcessContext* mPreprocessContext;

			};





		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONCONVERTIONINDEXER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION

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

#ifndef KONCLUDE_REASONER_GENERATOR_CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer_H
#define KONCLUDE_REASONER_GENERATOR_CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer_H


// Libraries includes
#include <QHash>
#include <QCache>
#include <QtConcurrent>


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesIndividualAssertionConvertionIndexer.h"
#include "CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData.h"
#include "CRedlandStoredTriplesIndexingBaseBatchLinkerVector.h"
#include "CRedlandStoredTriplesIndexingBaseBatchLinker.h"
#include "CRedlandStoredTriplesIndexingBaseResolvingLinker.h"
#include "CRedlandStoredTriplesIndividualResolvingHashMemoryManaged.h"


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
			 *		\class		CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer : public CRedlandStoredTriplesIndividualAssertionConvertionIndexer {
				// public methods
				public:
					CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer();
					virtual ~CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer();



					virtual bool indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData);


				// protected functions
				protected:
					void procssIndividualIndexing(CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& mappedIndi2HashKeyVec, CRedlandStoredTriplesData* redlandTriplesData, CConcreteOntology* updateConcreteOntology);

					virtual void indexTriples(CRedlandStoredTriplesData* redlandTriplesData, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject, CConcreteOntology* updateConcreteOntology);

					void indexHashExistingIndividuals(CConcreteOntology* updateConcreteOntology);

					void handleStatementMapping(librdf_statement* statement, CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> &keyMappedVec, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject);

					void indexTripleData(QList<CTriplesData*>* triplesDataList, CConcreteOntology* updateConcreteOntology);

					cint64 getCStringHashValue(const char* p);
					cint64 getNodeHashValue(librdf_node* redlandNode);
					CIndividual* getIndividual(librdf_node* redlandNode, CRedlandStoredTriplesIndividualResolvingHashMemoryManaged* indiResolveHash, CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>* keyMappedVec);

					void clearMemoryPools(CMemoryPool* memPools);

				// protected variables
				protected:
					bool mExistingEntitiesInitialized = false;
					bool mExistingIndividualsHashed = false;
					cint64 mIndiResolvingKeyCount = 211;
					QVector<CRedlandStoredTriplesIndividualResolvingHashMemoryManaged*> mIndiResolvingHashVec;

					function<void(CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& vec, const CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& reduceVec)> mIndiKeyReduceFunc;
					function<void(CRedlandStoredTriplesData*& redlandTriplesData)> mTripleDeleteMapFunc;
					function<void(CMemoryPool* memPools)> mClearMemoryPoolsFunc;


			};





		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer_H

#endif // !KONCLUDE_REDLAND_INTEGRATION

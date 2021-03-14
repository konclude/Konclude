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

#include "COntologyRedlandRDFRenderer.h"


namespace Konclude {

	namespace Renderer {


		COntologyRedlandRDFRenderer::COntologyRedlandRDFRenderer(librdf_world* world) {
			mBlankNodeIdentifierPrefix = QString("kbni_%1_").arg(QUuid::createUuid().toString().replace("{", "").replace("}",""));;
			//mBlankNodeIdentifierPrefix = QString("konclude_redland_rdf_rendering_blank_node_identifier_");

			mTmpRenderedNodeItemLinker = nullptr;
			mTmpRenderedTripleItemLinker = nullptr;

			mDefaultDatatypeString = PREFIX_XML_STRING_DATATYPE;


			mWorld = world;
			if (!mWorld) {
				mWorld = librdf_new_world();
				librdf_world_open(mWorld);
			}

			initRenderedNodeItems();
		}

		COntologyRedlandRDFRenderer::~COntologyRedlandRDFRenderer() {
		}





		CRedlandStoredTriplesData* COntologyRedlandRDFRenderer::getRenderedTripleData(CRedlandStoredTriplesData* tripleData) {

			if (!tripleData) {
				tripleData = new CRedlandStoredTriplesData();
				tripleData->initTriplesData(CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_QUERYING, nullptr);
			}

			bool inMemory = false;
			librdf_storage* indexedStorage = tripleData->getRedlandIndexedStorage();
			if (!indexedStorage && mWorld) {
				indexedStorage = librdf_new_storage(mWorld, "memory", NULL, "contexts='yes'");
				tripleData->setRedlandIndexedStorageData(indexedStorage);
				inMemory = true;
			}

			librdf_model* indexedModel = tripleData->getRedlandIndexedModel();
			if (!indexedModel && indexedStorage) {
				indexedModel = librdf_new_model(mWorld, indexedStorage, NULL);
				tripleData->setRedlandIndexedModelData(indexedModel);
			}


			CXLinker<librdf_statement*>* firstLinker = nullptr;
			CXLinker<librdf_statement*>* lastLinker = nullptr;

			if (indexedModel) {
				addRenderedTripleItemLinkerToTripleData(mListRenderedItemLinker, firstLinker, lastLinker);
				addRenderedTripleItemLinkerToTripleData(mFacetRenderedItemLinker, firstLinker, lastLinker);
				addRenderedTripleItemLinkerToTripleData(mObjRenderedItemLinker, firstLinker, lastLinker);
				addRenderedTripleItemLinkerToTripleData(mAxiomRenderedItemLinker, firstLinker, lastLinker);
				QByteArray bytes = QString("http://test.de").toUtf8();
				raptor_uri* uri = (raptor_uri*)librdf_new_uri(mWorld, (const unsigned char*)bytes.constData());
				librdf_node* conNode = librdf_new_node_from_uri(mWorld, uri);
				if (firstLinker) {
					tripleData->setRedlandStatementLinker(firstLinker);

					for (CXLinker<librdf_statement*>* linkerIt = firstLinker; linkerIt; linkerIt = linkerIt->getNext()) {
						librdf_statement* statement = linkerIt->getData();
						librdf_statement* statementCopy = librdf_new_statement_from_statement(statement);
						if (inMemory) {
							librdf_storage_context_add_statement(indexedStorage, conNode, statementCopy);
						} else {
							librdf_model_add_statement(indexedModel, statementCopy);
						}
					}
				}
			} else {
				delete tripleData;
				tripleData = nullptr;
			}

			return tripleData;

		}



		COntologyRedlandRDFRenderer* COntologyRedlandRDFRenderer::addRenderedTripleItemLinkerToTripleData(CRenderedRDFTripleItemBaseLinker* renderedItemLinker, CXLinker<librdf_statement*>*& firstLinker, CXLinker<librdf_statement*>*& lastLinker) {
			for (CRenderedRDFTripleItemBaseLinker* renderedItemLinkerIt = renderedItemLinker; renderedItemLinkerIt; renderedItemLinkerIt = renderedItemLinkerIt->getNextItemLinker()) {
				CRenderedRedlandRDFTripleItemBaseLinker* redlandRenderedItemLinker = (CRenderedRedlandRDFTripleItemBaseLinker*)renderedItemLinkerIt;
				CXLinker<librdf_statement*>* nextLinker = new CXLinker<librdf_statement *>();
				nextLinker->initLinker(redlandRenderedItemLinker->getRedlandTripleStatement());
				if (lastLinker) {
					lastLinker->setNext(nextLinker);
					lastLinker = nextLinker;
				} else {
					firstLinker = lastLinker = nextLinker;
				}
			}
			return this;
		}



		CRenderedRDFTripleItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedTripleItemLinker() {
			CRenderedRedlandRDFTripleItemBaseLinker* linker = mTmpRenderedTripleItemLinker;
			if (linker) {
				mTmpRenderedTripleItemLinker = (CRenderedRedlandRDFTripleItemBaseLinker*)mTmpRenderedTripleItemLinker->getNextItemLinker();
				linker->clearNext();
			} else {
				linker = new CRenderedRedlandRDFTripleItemBaseLinker();
				mRenderedTripleItemContainer.append(linker);
			}
			return linker;
		}

		COntologyRedlandRDFRenderer* COntologyRedlandRDFRenderer::releaseRenderedTripleItemLinkers(CRenderedItemLinker* linkers) {
			if (linkers) {
				for (CRenderedRedlandRDFTripleItemBaseLinker* linkerIt = (CRenderedRedlandRDFTripleItemBaseLinker*)linkers; linkerIt; linkerIt = (CRenderedRedlandRDFTripleItemBaseLinker*)linkerIt->getNextItemLinker()) {
					linkerIt->clearRedlandTripleStatement();
				}
				mTmpRenderedTripleItemLinker = (CRenderedRedlandRDFTripleItemBaseLinker*)linkers->append(mTmpRenderedTripleItemLinker);
			}
			return this;
		}

		CRenderedRDFNodeItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedNodeItemLinker() {
			CRenderedRedlandRDFNodeItemBaseLinker* linker = mTmpRenderedNodeItemLinker;
			if (linker) {
				mTmpRenderedNodeItemLinker = (CRenderedRedlandRDFNodeItemBaseLinker*)mTmpRenderedNodeItemLinker->getNextItemLinker();
				linker->clearNext();
			} else {
				linker = new CRenderedRedlandRDFNodeItemBaseLinker();
				mRenderedNodeItemContainer.append(linker);
			}
			return linker;
		}

		COntologyRedlandRDFRenderer* COntologyRedlandRDFRenderer::releaseRenderedNodeItemLinkers(CRenderedItemLinker* linkers) {
			if (linkers) {
				for (CRenderedRedlandRDFNodeItemBaseLinker* linkerIt = (CRenderedRedlandRDFNodeItemBaseLinker*)linkers; linkerIt; linkerIt = (CRenderedRedlandRDFNodeItemBaseLinker*)linkerIt->getNextItemLinker()) {
					linkerIt->clearRedlandNode();
				}
				mTmpRenderedNodeItemLinker = (CRenderedRedlandRDFNodeItemBaseLinker*)linkers->append(mTmpRenderedNodeItemLinker);
			}
			return this;
		}





		CRenderedRDFNodeItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedIRIResourceNodeItem(const QString& iri) {
			CRenderedRedlandRDFNodeItemBaseLinker* nodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)createRenderedNodeItemLinker();
			QByteArray bytes = iri.toUtf8();
			raptor_uri* uri = (raptor_uri*)librdf_new_uri(mWorld, (const unsigned char*)bytes.constData());
			librdf_node* node = librdf_new_node_from_uri(mWorld, uri);
			nodeItem->setRedlandNode(node);
			return nodeItem;
		}


		CRenderedRDFNodeItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedBlankNodeItem(const QString& identifier) {
			CRenderedRedlandRDFNodeItemBaseLinker* nodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)createRenderedNodeItemLinker();
			QByteArray bytes = identifier.toUtf8();
			librdf_node* node = librdf_new_node_from_blank_identifier(mWorld, (const unsigned char*)bytes.constData());
			nodeItem->setRedlandNode(node);
			return nodeItem;
		}

		CRenderedRDFNodeItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedBlankNodeItem(cint64 id) {
			CRenderedRedlandRDFNodeItemBaseLinker* nodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)createRenderedNodeItemLinker();
			QString identifier = QString("%1%2").arg(mBlankNodeIdentifierPrefix).arg(id);
			QByteArray bytes = identifier.toUtf8();
			librdf_node* node = librdf_new_node_from_blank_identifier(mWorld, (const unsigned char*)bytes.constData());
			nodeItem->setRedlandNode(node);
			return nodeItem;
		}

		CRenderedRDFNodeItemBaseLinker* COntologyRedlandRDFRenderer::createRenderedLiteralNodeItem(const QString& literal, const QString& datatype) {
			CRenderedRedlandRDFNodeItemBaseLinker* nodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)createRenderedNodeItemLinker();
			QByteArray datatypeBytes = datatype.toUtf8();
			librdf_uri* datatypeUri = nullptr;
			if (datatype != mDefaultDatatypeString) {
				librdf_new_uri(mWorld, (const unsigned char*)datatypeBytes.constData());
			}
			QByteArray literalBytes = literal.toUtf8();
			librdf_node* node = librdf_new_node_from_typed_literal(mWorld, (const unsigned char*)literalBytes.constData(), NULL, datatypeUri);
			nodeItem->setRedlandNode(node);
			return nodeItem;
		};


		CRenderedRDFTripleItemBaseLinker* COntologyRedlandRDFRenderer::createTriple(CRenderedRDFNodeItemBaseLinker* subjectNodeItem, CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedRDFNodeItemBaseLinker* objectNodeItem) {
			CRenderedRedlandRDFTripleItemBaseLinker* tripleItem = (CRenderedRedlandRDFTripleItemBaseLinker*)createRenderedTripleItemLinker();
			tripleItem->setRDFNodeItem(0, subjectNodeItem);
			tripleItem->setRDFNodeItem(1, predicateNodeItem);
			tripleItem->setRDFNodeItem(2, objectNodeItem);
			CRenderedRedlandRDFNodeItemBaseLinker* redlandSubjectNodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)subjectNodeItem;
			CRenderedRedlandRDFNodeItemBaseLinker* redlandPredicateNodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)predicateNodeItem;
			CRenderedRedlandRDFNodeItemBaseLinker* redlandObjectNodeItem = (CRenderedRedlandRDFNodeItemBaseLinker*)objectNodeItem;
			librdf_statement* statement = librdf_new_statement_from_nodes(mWorld, librdf_new_node_from_node(redlandSubjectNodeItem->getRedlandNode()), librdf_new_node_from_node(redlandPredicateNodeItem->getRedlandNode()), librdf_new_node_from_node(redlandObjectNodeItem->getRedlandNode()));
			tripleItem->setRedlandTripleStatement(statement);
			return tripleItem;
		}


	}; // end namespace Renderer

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

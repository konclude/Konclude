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

#include "CRDFRedlandExternalTriplesDSNConnectingParser.h"


namespace Konclude {

	namespace Parser {



		CRDFRedlandExternalTriplesDSNConnectingParser::CRDFRedlandExternalTriplesDSNConnectingParser(COntologyBuilder* ontologyBuilder, CConfiguration* configuration) : CLogIdentifier("::Konclude::Parser::RDFRedlandRemoteDataLinkingParser", this) {
			mOntologyBuilder = ontologyBuilder;
			mConfiguration = configuration;

		}


		CRDFRedlandExternalTriplesDSNConnectingParser::~CRDFRedlandExternalTriplesDSNConnectingParser() {
		}



		bool CRDFRedlandExternalTriplesDSNConnectingParser::connectExternalTriples(const QString& triplesDBName, const QString& dsnConfig) {

			bool localTriplesDataFlag = false;
			CRedlandStoredTriplesData* prevTripleData = (CRedlandStoredTriplesData*)mOntologyBuilder->getLatestTriplesData(false, &localTriplesDataFlag);
			CRedlandStoredTriplesData* tripleData = nullptr;


			if (!tripleData) {
				tripleData = new CRedlandStoredTriplesData();
				tripleData->initTriplesData(CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_NEW, prevTripleData);
			}

			librdf_world* world = tripleData->getRedlandWorld();
			if (!world) {
				world = librdf_new_world();
				librdf_world_open(world);
				tripleData->setRedlandWorldData(world);
			}

			QByteArray triplesDBNameArray = triplesDBName.toUtf8();
			QByteArray dsnConfigArray = dsnConfig.toUtf8();

			librdf_storage* indexedStorage = tripleData->getRedlandIndexedStorage();
			if (!indexedStorage && world) {
				//indexedStorage = librdf_new_storage(world, "virtuoso", "http://LUBM-test", "dsn='VirtuosoTest',user='dba',password='dba'");
				indexedStorage = librdf_new_storage(world, "virtuoso", triplesDBNameArray.constData(), dsnConfigArray.constData());
				tripleData->setRedlandIndexedStorageData(indexedStorage);
			}

			librdf_model* indexedModel = tripleData->getRedlandIndexedModel();
			if (!indexedModel && indexedStorage) {
				indexedModel = librdf_new_model(world, indexedStorage, NULL);
				tripleData->setRedlandIndexedModelData(indexedModel);
			}

			//QString filename = QString("./Tests/lubm-univ-bench-data-1.ttl");
			//QString filenameUri = QString("file://./Tests/lubm-univ-bench-data-1.ttl");
			//QByteArray filenameUriByteArray(filenameUri.toUtf8());
			//const char* filenameBaseUri = filenameUriByteArray.constData();
			//librdf_uri *baseUri = librdf_new_uri(tripleData->getRedlandWorld(), (const unsigned char*)filenameBaseUri);


			//QString redlandParsingFormat = QString("turtle");
			//QByteArray mRedlandParsingFormatByteArray = redlandParsingFormat.toUtf8();
			//const char* parser_name = mRedlandParsingFormatByteArray.constData();
			//librdf_parser* parser = librdf_new_parser(tripleData->getRedlandWorld(), parser_name, NULL, NULL);
			//if (!parser) {
			//	mParsingError = true;
			//}

			//QByteArray filenameByteArray(filename.toUtf8());
			//FILE* file = fopen(filenameByteArray.constData(), "r");
			//// parser closes file
			//if (parser && librdf_parser_parse_file_handle_into_model(parser, file, 1, baseUri, tripleData->getRedlandIndexedModel())) {
			//	mParsingError = true;
			//}
			//if (parser) {
			//	librdf_free_parser(parser);
			//}


			/*else {
				model = librdf_new_model_from_model(prevModel);
				tripleData->setRedlandModelData(model);
			}*/


			if (!indexedModel) {
				mParsingError = true;
				delete tripleData;
				tripleData = nullptr;
			}

			if (mParsingError) {
				delete tripleData;
				tripleData = nullptr;
			} else {
				mOntologyBuilder->addTriplesData(tripleData);
			}

			return !mParsingError;
		}







		QString CRDFRedlandExternalTriplesDSNConnectingParser::getErrorString() {
			return mErrorString;
		}

		bool CRDFRedlandExternalTriplesDSNConnectingParser::hasError() {
			return mParsingError;
		}



	}; // end namespace Parser

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

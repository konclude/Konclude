/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRDFRedlandRaptorParser.h"


namespace Konclude {

	namespace Parser {



		CRDFRedlandRaptorParser::CRDFRedlandRaptorParser(COntologyBuilder* ontologyBuilder, CTRIPLES_DATA_UPDATE_TYPE updateType, QString redlandParsingFormat, CConfiguration* configuration) {
			mOntologyBuilder = ontologyBuilder;
			mUpdateType = updateType;
			mRedlandParsingFormat = redlandParsingFormat;
			mConfiguration = configuration;

			mConfLoadTriplesIntoStore = true;
			if (mConfiguration) {
				mConfLoadTriplesIntoStore = CConfigDataReader::readConfigBoolean(mConfiguration, "Konclude.Parser.RedlandRaptor.LoadTriplesIntoStore", true);
			}
		}


		CRDFRedlandRaptorParser::~CRDFRedlandRaptorParser() {
		}



		CRedlandStoredTriplesData* CRDFRedlandRaptorParser::getUpdatingTripleData() {

			bool localTriplesDataFlag = false;
			CRedlandStoredTriplesData* prevTripleData = (CRedlandStoredTriplesData*)mOntologyBuilder->getLatestTriplesData(false, &localTriplesDataFlag);
			CRedlandStoredTriplesData* tripleData = nullptr;
			if (localTriplesDataFlag && prevTripleData->getUpdateType() == mUpdateType) {
				return prevTripleData;
			}


			if (!tripleData) {
				tripleData = new CRedlandStoredTriplesData();
				tripleData->initTriplesData(mUpdateType, prevTripleData);
			}

			librdf_world* world = tripleData->getRedlandWorld();
			if (!world) {
				world = librdf_new_world();
				librdf_world_open(world);
				tripleData->setRedlandWorldData(world);
			}

			librdf_storage* indexedStorage = tripleData->getRedlandIndexedStorage();
			if (!indexedStorage && world) {
				indexedStorage = librdf_new_storage(world, "hashes", NULL, "hash-type='memory',index-predicates='yes'");
				tripleData->setRedlandIndexedStorageData(indexedStorage);
			}

			librdf_model* indexedModel = tripleData->getRedlandIndexedModel();
			if (!indexedModel && indexedStorage) {
				indexedModel = librdf_new_model(world, indexedStorage, NULL);
				tripleData->setRedlandIndexedModelData(indexedModel);
			}
			/*else {
				model = librdf_new_model_from_model(prevModel);
				tripleData->setRedlandModelData(model);
			}*/


			if (!indexedModel) {
				delete tripleData;
				tripleData = nullptr;
			}

			return tripleData;

		}





		bool CRDFRedlandRaptorParser::parseTriplesFile(const QString& filename) {


			mParsingError = false;
			CRedlandStoredTriplesData* tripleData = getUpdatingTripleData();

			QString filenameUri = QString("file://" + filename);
			QByteArray filenameUriByteArray(filenameUri.toUtf8());
			const char* filenameBaseUri = filenameUriByteArray.constData();
			librdf_uri *baseUri = librdf_new_uri(tripleData->getRedlandWorld(), (const unsigned char*)filenameBaseUri);


			QByteArray mRedlandParsingFormatByteArray = mRedlandParsingFormat.toUtf8();
			const char* parser_name = mRedlandParsingFormatByteArray.constData();
			librdf_parser* parser = librdf_new_parser(tripleData->getRedlandWorld(), parser_name, NULL, NULL);
			if (!parser) {
				mParsingError = true;
			}

			QByteArray filenameByteArray(filename.toUtf8());
			FILE* file = fopen(filenameByteArray.constData(), "r");
			// parser closes file
			if (parser && librdf_parser_parse_file_handle_into_model(parser, file, 1, baseUri, tripleData->getRedlandIndexedModel())) {
				mParsingError = true;
			}
			if (parser) {
				librdf_free_parser(parser);
			}

			if (mParsingError) {
				delete tripleData;
				tripleData = nullptr;
			} else {
				mOntologyBuilder->addTriplesData(tripleData);
			}

			return !mParsingError;
		}



		int raptor_iostream_read_bytes_from_qiodevice(void *context, void *ptr, size_t size, size_t nmemb) {
			QIODevice* ioDevice = (QIODevice*)context;
			qint64 readElements = 0;
			if (size == 1) {
				readElements = ioDevice->read((char*)ptr, nmemb);
			} else {
				for (size_t i = 0; i < nmemb && !ioDevice->atEnd(); ++i) {
					qint64 read = ioDevice->read((char*)ptr, size);
					if (read >= size) {
						ptr = (void*)((char*)ptr + size);
						readElements++;
					} else {
						break;
					}
				}
			}
			return readElements;
		}


		int raptor_iostream_read_eof_func_from_qiodevice(void *context) {
			QIODevice* ioDevice = (QIODevice*)context;
			if (ioDevice->atEnd()) {
				return 1;
			}
			return 0;
		}
		

		bool CRDFRedlandRaptorParser::parseTriples(QIODevice* ioDevice, const QString& baseUriString) {
			raptor_iostream_handler qioDeviceIOStreamHandler;
			qioDeviceIOStreamHandler.version = 2;
			qioDeviceIOStreamHandler.read_bytes = &raptor_iostream_read_bytes_from_qiodevice;
			qioDeviceIOStreamHandler.read_eof = &raptor_iostream_read_eof_func_from_qiodevice;
			qioDeviceIOStreamHandler.init = nullptr;
			qioDeviceIOStreamHandler.finish = nullptr;
			qioDeviceIOStreamHandler.write_byte = nullptr;
			qioDeviceIOStreamHandler.write_bytes = nullptr;
			qioDeviceIOStreamHandler.write_end = nullptr;

			CRedlandStoredTriplesData* tripleData = getUpdatingTripleData();
			raptor_world* raptor_world_ptr = librdf_world_get_raptor(tripleData->getRedlandWorld());
			raptor_iostream* raptor_stream = raptor_new_iostream_from_handler(raptor_world_ptr, ioDevice, &qioDeviceIOStreamHandler);


			QByteArray baseUriStringByteArray(baseUriString.toUtf8());
			librdf_uri *baseUri = librdf_new_uri(tripleData->getRedlandWorld(), (const unsigned char*)baseUriStringByteArray.constData());


			QByteArray mRedlandParsingFormatByteArray = mRedlandParsingFormat.toUtf8();
			const char* parser_name = mRedlandParsingFormatByteArray.constData();
			librdf_parser* parser = librdf_new_parser(tripleData->getRedlandWorld(), parser_name, NULL, NULL);
			if (!parser) {
				mParsingError = true;
			}


			//if (parser && raptor_stream && librdf_parser_parse_iostream_into_model(parser, raptor_stream, baseUri, tripleData->getRedlandStreamedModel())) {
			//	mParsingError = true;
			//}

			librdf_stream* tripleStream = nullptr;
			if (parser && raptor_stream) {
				tripleStream = librdf_parser_parse_iostream_as_stream(parser, raptor_stream, baseUri);
				if (!tripleStream) {
					mParsingError = true;
				}

				CXLinker<librdf_statement*>* statementLinker = nullptr;
				CXLinker<librdf_statement*>* lastStatementLinker = nullptr;
				while (!librdf_stream_end(tripleStream)) {
					librdf_statement* statement = librdf_stream_get_object(tripleStream);

					if (!librdf_model_contains_statement(tripleData->getRedlandIndexedModel(), statement)) {
						librdf_statement* statementCopy = librdf_new_statement_from_statement(statement);
						CXLinker<librdf_statement*>* newStatementLinker = new CXLinker<librdf_statement*>();
						newStatementLinker->initLinker(statementCopy, nullptr);
						if (statementLinker) {
							lastStatementLinker->setNext(newStatementLinker);
							lastStatementLinker = newStatementLinker;
						} else {
							statementLinker = newStatementLinker;
							lastStatementLinker = newStatementLinker;
						}
						if (mConfLoadTriplesIntoStore) {
							librdf_model_add_statement(tripleData->getRedlandIndexedModel(), statement);
						}
					}
					librdf_stream_next(tripleStream);
				}
				tripleData->setRedlandStatementLinker(statementLinker);
			}


			if (raptor_stream) {
				raptor_free_iostream(raptor_stream);
			}

			if (parser) {
				librdf_free_parser(parser);
			}

			if (mParsingError) {
				delete tripleData;
				tripleData = nullptr;
			} else {
				mOntologyBuilder->addTriplesData(tripleData);
			}

			return !mParsingError;
		}



	}; // end namespace Parser

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
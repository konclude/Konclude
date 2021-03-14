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

#include "CRDFRedlandRaptorSimpleConcurrentParser.h"


namespace Konclude {

	namespace Parser {



		CRDFRedlandRaptorSimpleConcurrentParser::CRDFRedlandRaptorSimpleConcurrentParser(COntologyBuilder* ontologyBuilder, CTRIPLES_DATA_UPDATE_TYPE updateType, QString redlandParsingFormat, CConfiguration* configuration) : CRDFRedlandRaptorParser(ontologyBuilder, updateType, redlandParsingFormat, configuration) {
			mConcurrentOffsetSplitSize = CConfigDataReader::readConfigInteger(mConfiguration, "Konclude.Parser.RedlandRaptor.ConcurrentBytesBatchingSize", 50000000);
		}


		CRDFRedlandRaptorSimpleConcurrentParser::~CRDFRedlandRaptorSimpleConcurrentParser() {
		}


		class TParseOffsetData {
		public:
			TParseOffsetData(cint64 startOffset = 0, cint64 endOffset = 0) {
				mStartOffset = startOffset;
				mEndOffset = endOffset;
			}
			TParseOffsetData(const TParseOffsetData& data) {
				mStartOffset = data.mStartOffset;
				mEndOffset = data.mEndOffset;
				mIndex = data.mIndex;
			}
			cint64 mIndex = 0;
			cint64 mStartOffset = 0;
			cint64 mLastPos = 0;
			cint64 mEndOffset = 0;
			QIODevice* mDevice;
			bool mStartCharFound = false;
			bool mEndCharFound = false;

			QByteArray* mBasePrefixDataArray = nullptr;
			cint64 mBasePrefixDataArrayReadPos = 0;
			bool mBasePrefixDataArrayRead = false;

		};

		int raptor_iostream_read_bytes_from_qiodevice_with_offset(void *context, void *ptr, size_t size, size_t nmemb) {
			TParseOffsetData* parseOffsetData = (TParseOffsetData*)context;

			char* begCharPtr = (char*)ptr;
			qint64 readElements = 0;
			if (!parseOffsetData->mBasePrefixDataArrayRead && parseOffsetData->mBasePrefixDataArray) {
				const char* data = parseOffsetData->mBasePrefixDataArray->constData();
				int length = parseOffsetData->mBasePrefixDataArray->length();
				while (readElements < nmemb && parseOffsetData->mBasePrefixDataArrayReadPos < length) {
					char* charPointer = (char*)ptr;
					int nextElByteCount = 0;
					while ((size_t)nextElByteCount < size && parseOffsetData->mBasePrefixDataArrayReadPos < length) {
						charPointer[nextElByteCount++] = data[parseOffsetData->mBasePrefixDataArrayReadPos++];
					}
					if ((size_t)nextElByteCount >= size) {
						readElements++;
						nextElByteCount = 0;
						ptr = (void*)((char*)ptr + size);
					}
				}
				if (parseOffsetData->mBasePrefixDataArrayReadPos >= length) {
					parseOffsetData->mBasePrefixDataArrayRead = true;
				}
			}

			QIODevice* ioDevice = parseOffsetData->mDevice;
			parseOffsetData->mLastPos = ioDevice->pos();
			cint64 maxSize = parseOffsetData->mEndOffset - parseOffsetData->mLastPos;
			if (maxSize > 0 && readElements < nmemb) {
				if (size == 1) {
					size_t nmembCount = qMin((size_t)(nmemb - readElements), (size_t)maxSize);
					readElements += ioDevice->read((char*)ptr, nmembCount);
					ptr = (void*)((char*)ptr + nmembCount);
				} else {
					size_t nmembCount = qMin((size_t)(nmemb - readElements), (size_t)maxSize/size);
					for (size_t i = 0; i < nmembCount && !ioDevice->atEnd(); ++i) {
						qint64 read = ioDevice->read((char*)ptr, size);
						if (read >= size) {
							ptr = (void*)((char*)ptr + size);
							readElements++;
						} else {
							break;
						}
					}
				}
			}
			bool hasSpace = false;
			bool hasPoint = false;
			while (readElements < nmemb && !parseOffsetData->mEndCharFound && !ioDevice->atEnd()) {

				char dataChar[2];
				int nextElByteCount = 0;
				while (!ioDevice->atEnd() && (size_t)nextElByteCount < size) {
					cint64 read = ioDevice->read(dataChar, 1);
					if (read == 1) {
						if (dataChar[0] == ' ') {
							hasSpace = true;
						} else if (dataChar[0] == '.' && hasSpace && !hasPoint) {
							hasPoint = true;
						} else if (dataChar[0] == '\r' && hasPoint) {
						} else if (dataChar[0] == '\n' && hasPoint) {
							parseOffsetData->mEndCharFound = true;
						} else {
							hasSpace = false;
						}
						char* charPointer = (char*)ptr;
						charPointer[nextElByteCount++] = dataChar[0];
					}
				}
				if ((size_t)nextElByteCount >= size) {
					readElements++;
					nextElByteCount = 0;
					ptr = (void*)((char*)ptr + size);
				}


			}
			if (readElements > nmemb) {
				cout << "elements buffer length overwritten" << endl;
			}
			if ((char*)ptr > begCharPtr + nmemb) {
				cout << "pointer buffer length overwritten" << endl;
			}
			return readElements;
		}


		int raptor_iostream_read_eof_func_from_qiodevice_with_offset(void *context) {
			TParseOffsetData* parseOffsetData = (TParseOffsetData*)context;
			QIODevice* ioDevice = parseOffsetData->mDevice;
			if (ioDevice->atEnd() || parseOffsetData->mEndCharFound) {
				return 1;
			}
			return 0;
		}


		bool CRDFRedlandRaptorSimpleConcurrentParser::parseTriples(QIODevice* ioDevice, const QString& baseUriString) {

			QString fileString = baseUriString;
			if (fileString.startsWith("file:")) {
				fileString = fileString.replace("file:", "");
			}
			QFile* file = new QFile(fileString);
			while (!file->open(QIODevice::ReadOnly) && fileString.startsWith("/")) {
				fileString = fileString.mid(1);
				delete file;
				file = new QFile(fileString);
			}
			delete file;
			file = new QFile(fileString);
			if (!file->open(QIODevice::ReadOnly) || mConfLoadTriplesIntoStore || (mRedlandParsingFormat != "turtle" && mRedlandParsingFormat != "ntriples")) {
				delete file;
				return CRDFRedlandRaptorParser::parseTriples(ioDevice, baseUriString);
			}
			cint64 bytesAvailable = ioDevice->bytesAvailable();
			delete file;




			class TParsedTripleData {
			public:
				CRedlandStoredTriplesData* mTriplesData = nullptr;
			};


			QVector<TParseOffsetData> parseOffsetDataVec((bytesAvailable / mConcurrentOffsetSplitSize)+1);

			cint64 idx = 0;
			for (cint64 i = 0; i < bytesAvailable; i += mConcurrentOffsetSplitSize) {
				TParseOffsetData data = TParseOffsetData(i, i + mConcurrentOffsetSplitSize);
				data.mIndex = idx;
				parseOffsetDataVec[idx++] = data;
			}
			LOG(INFO, getLogDomain(), logTr("Starting concurrent triple parsing for file '%1' with %2 partitions.").arg(fileString).arg(parseOffsetDataVec.size()), this);


			QString* raptorParsingErrorString = nullptr;
			bool warningNoStore = false;

			QByteArray basePrefixDataArray;

			while (!ioDevice->atEnd()) {
				QByteArray data = ioDevice->readLine().trimmed();
				QString dataString = data;
				dataString = dataString.toUpper();
				bool isBasePrefixData = false;
				if (dataString.startsWith("PREFIX") || dataString.startsWith("@PREFIX")) {
					isBasePrefixData = true;
				}
				if (dataString.startsWith("BASE") || dataString.startsWith("@BASE")) {
					isBasePrefixData = true;
				}
				if (isBasePrefixData) {
					basePrefixDataArray.append(data);
					basePrefixDataArray.append(QString("\r\n"));
				} else if (!dataString.startsWith("#")) {
					break;
				}
			}

			function<CRedlandStoredTriplesData*(const TParseOffsetData& parseData)> parseTriplesMapFunc = [&](const TParseOffsetData& parseData) ->CRedlandStoredTriplesData* {
				QFile file(fileString);
				if (parseData.mStartOffset != parseData.mEndOffset && file.open(QIODevice::ReadOnly)) {

					TParseOffsetData tmpParseData(parseData);
					tmpParseData.mDevice = &file;

					file.seek(parseData.mStartOffset);

					cint64 startSkipCount = 0;
					if (!tmpParseData.mStartCharFound && parseData.mStartOffset > 0) {
						tmpParseData.mBasePrefixDataArray = &basePrefixDataArray;
						char dataChar[2];
						bool hasSpace = false;
						bool hasPoint = false;
						while (!tmpParseData.mStartCharFound && !tmpParseData.mDevice->atEnd()) {
							cint64 read = tmpParseData.mDevice->read(dataChar, 1);
							if (read == 1) {
								if (dataChar[0] == ' ') {
									hasSpace = true;
								} else if (dataChar[0] == '.' && hasSpace && !hasPoint) {
									hasPoint = true;
								} else if (dataChar[0] == '\r' && hasPoint) {
								} else if (dataChar[0] == '\n' && hasPoint) {
									tmpParseData.mStartCharFound = true;
								} else {
									hasSpace = false;
								}
								startSkipCount++;
							}
						}
					} else {
						tmpParseData.mBasePrefixDataArrayRead = true;
						tmpParseData.mStartCharFound = true;
					}

					raptor_iostream_handler qioDeviceIOStreamHandler;
					qioDeviceIOStreamHandler.version = 2;
					qioDeviceIOStreamHandler.read_bytes = &raptor_iostream_read_bytes_from_qiodevice_with_offset;
					qioDeviceIOStreamHandler.read_eof = &raptor_iostream_read_eof_func_from_qiodevice_with_offset;
					qioDeviceIOStreamHandler.init = nullptr;
					qioDeviceIOStreamHandler.finish = nullptr;
					qioDeviceIOStreamHandler.write_byte = nullptr;
					qioDeviceIOStreamHandler.write_bytes = nullptr;
					qioDeviceIOStreamHandler.write_end = nullptr;

					CRedlandStoredTriplesData* tripleData = getUpdatingTripleData();
					raptor_world* raptor_world_ptr = librdf_world_get_raptor(tripleData->getRedlandWorld());


					raptor_world_set_log_handler(raptor_world_ptr, &raptorParsingErrorString, &raptor_log_handler_error_notifier);

					raptor_iostream* raptor_stream = raptor_new_iostream_from_handler(raptor_world_ptr, &tmpParseData, &qioDeviceIOStreamHandler);


					QByteArray baseUriStringByteArray(baseUriString.toUtf8());
					librdf_uri *baseUri = librdf_new_uri(tripleData->getRedlandWorld(), (const unsigned char*)baseUriStringByteArray.constData());


					QByteArray mRedlandParsingFormatByteArray = mRedlandParsingFormat.toUtf8();
					const char* parser_name = mRedlandParsingFormatByteArray.constData();
					librdf_parser* parser = librdf_new_parser(tripleData->getRedlandWorld(), parser_name, NULL, NULL);
					if (!parser) {
						mErrorString = QString("Failed to create parser");
						mParsingError = true;
					}


					//if (parser && raptor_stream && librdf_parser_parse_iostream_into_model(parser, raptor_stream, baseUri, tripleData->getRedlandStreamedModel())) {
					//	mParsingError = true;
					//}

					librdf_stream* tripleStream = nullptr;
					if (parser && raptor_stream) {
						tripleStream = librdf_parser_parse_iostream_as_stream(parser, raptor_stream, baseUri);
						if (!tripleStream) {
							mErrorString = QString("Failed to load data stream");
							mParsingError = true;
						}

						librdf_node* critPredicate1 = librdf_new_node_from_uri_string(tripleData->getRedlandWorld(), (const unsigned char*)PREFIX_RDFS_SUBCLASS_OF);
						librdf_node* critPredicate2 = librdf_new_node_from_uri_string(tripleData->getRedlandWorld(), (const unsigned char*)PREFIX_OWL_COMPLEMENT_OF);

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
								if (!warningNoStore && (librdf_node_equals(critPredicate1, librdf_statement_get_predicate(statementCopy)) || librdf_node_equals(critPredicate2, librdf_statement_get_predicate(statementCopy)))) {
									warningNoStore = true;
									LOG(WARN, getLogDomain(), logTr("(Nontrivial) OWL axioms and expressions cannot be extracted from triples since they are not loaded into the triple store."), this);
								}
							}
							librdf_stream_next(tripleStream);
						}
						librdf_free_node(critPredicate1);
						librdf_free_node(critPredicate2);
						tripleData->setRedlandStatementLinker(statementLinker);
					}

					if (raptor_stream) {
						raptor_free_iostream(raptor_stream);
					}

					if (parser) {
						librdf_free_parser(parser);
					}

					return tripleData;


				}
				return nullptr;

			};


			QVector<CRedlandStoredTriplesData*> triplesDataVec = QtConcurrent::blockingMapped(parseOffsetDataVec, parseTriplesMapFunc);



			if (raptorParsingErrorString) {
				mParsingError = true;
				mErrorString = *raptorParsingErrorString;
				delete raptorParsingErrorString;
				raptorParsingErrorString = nullptr;
			}


			if (mParsingError) {
				qDeleteAll(triplesDataVec);
			} else {
				for (CRedlandStoredTriplesData* tripleData : triplesDataVec) {
					mOntologyBuilder->addTriplesData(tripleData);
				}
			}

			return !mParsingError;
		}



	}; // end namespace Parser

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

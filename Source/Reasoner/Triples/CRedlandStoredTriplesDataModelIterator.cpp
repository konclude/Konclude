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

#include "CRedlandStoredTriplesDataModelIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesDataModelIterator::CRedlandStoredTriplesDataModelIterator(librdf_stream* stream) {
				mStream = stream;
				mDefaultDatatypeString = PREFIX_XML_STRING_DATATYPE;
			}

			CRedlandStoredTriplesDataModelIterator::~CRedlandStoredTriplesDataModelIterator() {
				librdf_free_stream(mStream);
			}


			QString CRedlandStoredTriplesDataModelIterator::getNodeString(cint64 pos) {
				librdf_statement* statement = librdf_stream_get_object(mStream);

				librdf_node* node = nullptr;
				if (pos == 0) {
					node = librdf_statement_get_subject(statement);
				} else if (pos == 1) {
					node = librdf_statement_get_predicate(statement);
				} else if (pos == 2) {
					node = librdf_statement_get_object(statement);
				}

				TRIPLE_NODE_TYPE type = getNodeType(node);
				if (type == CTriplesDataIterator::TRIPLE_URI_NODE_TYPE) {
					librdf_uri* nodeUri = librdf_node_get_uri(node);
					return QString::fromUtf8((const char*)librdf_uri_as_string(nodeUri));

				} else if (type == CTriplesDataIterator::TRIPLE_BLANK_NODE_TYPE) {
					const char* nodeBlankIdentifier = (const char*)librdf_node_get_blank_identifier(node);
					return QString::fromUtf8(nodeBlankIdentifier);

				} else {
					size_t litValLen = 0;
					const char* literalValue = (const char*)librdf_node_get_literal_value_as_counted_string(node, &litValLen);
					librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(node);
					QString datatypeUriString = mDefaultDatatypeString;
					if (datatypeUri) {
						datatypeUriString = QString::fromUtf8((const char*)librdf_uri_as_string(datatypeUri));
					}
					QString literalValueString = QString::fromUtf8(literalValue, litValLen);
					QString dataLiteralString = QString("\"%1\"^^%2").arg(literalValueString).arg(datatypeUriString);
					return dataLiteralString;
				}


			}

			CTriplesDataIterator::TRIPLE_NODE_TYPE CRedlandStoredTriplesDataModelIterator::getNodeType(cint64 pos) {
				librdf_statement* statement = librdf_stream_get_object(mStream);
				librdf_node* node = nullptr;
				if (pos == 0) {
					node = librdf_statement_get_subject(statement);
				} else if (pos == 1) {
					node = librdf_statement_get_predicate(statement);
				} else if (pos == 2) {
					node = librdf_statement_get_object(statement);
				}
				return getNodeType(node);
			}



			bool CRedlandStoredTriplesDataModelIterator::hasValue() {
				return !librdf_stream_end(mStream);
			}

			CTriplesDataIterator* CRedlandStoredTriplesDataModelIterator::moveNext() {
				librdf_stream_next(mStream);
				return this;
			}


			CRedlandStoredTriplesDataModelIterator::TRIPLE_NODE_TYPE CRedlandStoredTriplesDataModelIterator::getNodeType(librdf_node* node) {
				if (librdf_node_is_resource(node)) {
					return CTriplesDataIterator::TRIPLE_URI_NODE_TYPE;
				} else if (librdf_node_is_blank(node)) {
					return CTriplesDataIterator::TRIPLE_BLANK_NODE_TYPE;
				} else {
					return CTriplesDataIterator::TRIPLE_LITERAL_NODE_TYPE;
				}
			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION

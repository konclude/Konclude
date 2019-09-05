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

#ifndef KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS
#define KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS

// Libraries includes
#include <QtGlobal>
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/Container/CQtHash.h"
#include "Utilities/Container/CQtList.h"
#include "Utilities/Container/CQtSet.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Memory/CObjectAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\file		OntologySettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */



			// forward declarations

			class COntology;
			class CConcept;
			class CVariable;
			class CRole;
			class CRoleChain;
			class CBoxContext;
			class CIndividual;
			class CTerminology;
			class CConcreteOntology;
			class CTBox;
			class CABox;
			class CNamePrefix;
			class CConceptVector;
			class CProcessReference;

			class COntologyProcessingSteps;
			class COntologyProcessingStepVector;
			class COntologyProcessingStepData;
			class COntologyProcessingStep;
			class COntologyProcessingRequirement;
			class COntologyProcessingStatus;

			class COntologyTriplesAssertionsAccessor;

			typedef QPair<CConcept*, bool> TConceptNegPair;

			// Concept Constructor Codes
			static const qint64 CCNONE = 0;
			static const qint64 CCATOM = 0;

			static const qint64 CCTOP = 1;
			static const qint64 CCBOTTOM = -1;

			static const qint64 CCNOT = -2;

			static const qint64 CCAND = 3;
			static const qint64 CCOR = -3;


			static const qint64 CCATMOST = 4;
			static const qint64 CCATLEAST = -4;

			static const qint64 CCALL = 5;
			static const qint64 CCSOME = -5;

			static const qint64 CCEQ = 6;
			static const qint64 CCSUB = 7;

			static const qint64 CCNOMINAL = 8;

			static const qint64 CCSELF = 9;

			static const qint64 CCAQCHOOCE = 10;
			static const qint64 CCAQALL = 11;
			static const qint64 CCAQSOME = -11;
			static const qint64 CCAQAND = 12;

			static const qint64 CCVALUE = 13;

			static const qint64 CCNOMVAR = 14;
			static const qint64 CCNOMTEMPLREF = 15;

			static const qint64 CCIMPL = 16;
			static const qint64 CCIMPLTRIG = 17;
			static const qint64 CCIMPLALL = 18;
			static const qint64 CCIMPLAQALL = 19;
			static const qint64 CCIMPLAQAND = 20;

			static const qint64 CCBRANCHIMPL = 21;
			static const qint64 CCBRANCHTRIG = 22;
			static const qint64 CCBRANCHALL = 23;
			static const qint64 CCBRANCHAQALL = 24;
			static const qint64 CCBRANCHAQAND = 25;

			static const qint64 CCEQCAND = 26;

			static const qint64 CCPBINDTRIG = 27;
			static const qint64 CCPBINDIMPL = 28;
			static const qint64 CCPBINDGROUND = 29;
			static const qint64 CCPBINDALL = 30;
			static const qint64 CCPBINDAND = 31;
			static const qint64 CCPBINDAQAND = 32;
			static const qint64 CCPBINDAQALL = 33;
			static const qint64 CCPBINDVARIABLE = 34;
			static const qint64 CCPBINDCYCLE = 35;

			static const qint64 CCVARBINDTRIG = 36;
			static const qint64 CCVARBINDJOIN = 37;
			static const qint64 CCVARBINDGROUND = 38;
			static const qint64 CCVARBINDALL = 39;
			static const qint64 CCVARBINDAND = 40;
			static const qint64 CCVARBINDAQAND = 41;
			static const qint64 CCVARBINDAQALL = 42;
			static const qint64 CCVARBINDVARIABLE = 43;
			static const qint64 CCVARBINDIMPL = 44;

			static const qint64 CCVARPBACKTRIG = 45;
			static const qint64 CCVARPBACKALL = 46;
			static const qint64 CCVARPBACKAQAND = 47;
			static const qint64 CCVARPBACKAQALL = 48;

			static const qint64 CCBACKACTIVTRIG = 49;
			static const qint64 CCBACKACTIVIMPL = 50;


			static const qint64 CCDATATYPE = 51;
			static const qint64 CCDATALITERAL = 52;
			static const qint64 CCDATARESTRICTION = 53;


			static const qint64 CCMARKER = 54;

			static const qint64 CCNOMINALIMPLI = 55;
			static const qint64 CCDATATYPEIMPLI = 56;
			static const qint64 CCDATALITERALIMPLI = 57;
			static const qint64 CCDATARESTRICTIONIMPLI = 58;

			static const qint64 CCVARBINDPREPARE = 59;
			static const qint64 CCVARBINDFINALZE = 60;



			// Prefixes

			#define PREFIX_OWL										"http://www.w3.org/2002/07/owl#"
			#define PREFIX_RDF										"http://www.w3.org/1999/02/22-rdf-syntax-ns#"
			#define PREFIX_XML										"http://www.w3.org/XML/1998/namespace"
			#define PREFIX_XSD										"http://www.w3.org/2001/XMLSchema#"
			#define PREFIX_RDFS										"http://www.w3.org/2000/01/rdf-schema#"

			#define PREFIX_OWL_THING								"http://www.w3.org/2002/07/owl#Thing"
			#define PREFIX_OWL_NOTHING								"http://www.w3.org/2002/07/owl#Nothing"

			#define PREFIX_OWL_TOPOBJECTPROPERTY					"http://www.w3.org/2002/07/owl#topObjectProperty"
			#define PREFIX_OWL_BOTTOMOBJECTPROPERTY					"http://www.w3.org/2002/07/owl#bottomObjectProperty"

			#define PREFIX_OWL_TOPDATAPROPERTY						"http://www.w3.org/2002/07/owl#topDataProperty"
			#define PREFIX_OWL_BOTTOMDATAPROPERTY					"http://www.w3.org/2002/07/owl#bottomDataProperty"



			#define PREFIX_OWL_TOP_DATATYPE							"http://www.w3.org/2002/07/owl#topDatatype"
			#define PREFIX_OWL_BOTTOM_DATATYPE						"http://www.w3.org/2002/07/owl#bottomDatatype"
			#define PREFIX_OWL_REAL_DATATYPE						"http://www.w3.org/2002/07/owl#real"
			#define PREFIX_OWL_RATIONAL_DATATYPE					"http://www.w3.org/2002/07/owl#rational"
			#define PREFIX_XML_INTEGER_DATATYPE						"http://www.w3.org/2001/XMLSchema#integer"
			#define PREFIX_XML_DECIMAL_DATATYPE						"http://www.w3.org/2001/XMLSchema#decimal"

			#define PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE			"http://www.w3.org/2001/XMLSchema#nonNegativeInteger"
			#define PREFIX_XML_POSITIVEINTEGER_DATATYPE				"http://www.w3.org/2001/XMLSchema#positiveInteger"
			#define PREFIX_XML_NONPOSITIVEINTEGER_DATATYPE			"http://www.w3.org/2001/XMLSchema#nonPositiveInteger"
			#define PREFIX_XML_NEGATIVEINTEGER_DATATYPE				"http://www.w3.org/2001/XMLSchema#negativeInteger"
			#define PREFIX_XML_LONG_DATATYPE						"http://www.w3.org/2001/XMLSchema#long"
			#define PREFIX_XML_INT_DATATYPE							"http://www.w3.org/2001/XMLSchema#int"
			#define PREFIX_XML_SHORT_DATATYPE						"http://www.w3.org/2001/XMLSchema#short"
			#define PREFIX_XML_BYTE_DATATYPE						"http://www.w3.org/2001/XMLSchema#byte"
			#define PREFIX_XML_UNSIGNEDLONG_DATATYPE				"http://www.w3.org/2001/XMLSchema#unsignedLong"
			#define PREFIX_XML_UNSIGNEDINT_DATATYPE					"http://www.w3.org/2001/XMLSchema#unsignedInt"
			#define PREFIX_XML_UNSIGNEDSHORT_DATATYPE				"http://www.w3.org/2001/XMLSchema#unsignedShort"
			#define PREFIX_XML_UNSIGNEDBYTE_DATATYPE				"http://www.w3.org/2001/XMLSchema#unsignedByte"


			#define PREFIX_XML_PLAINLITERAL_DATATYPE				"http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral"
			#define PREFIX_XML_STRING_DATATYPE						"http://www.w3.org/2001/XMLSchema#string"
			#define PREFIX_XML_NORMALIZEDSTRING_DATATYPE			"http://www.w3.org/2001/XMLSchema#normalizedString"
			#define PREFIX_XML_TOKEN_DATATYPE						"http://www.w3.org/2001/XMLSchema#token"
			#define PREFIX_XML_NAME_DATATYPE						"http://www.w3.org/2001/XMLSchema#Name"
			#define PREFIX_XML_NCNAME_DATATYPE						"http://www.w3.org/2001/XMLSchema#NCName"
			#define PREFIX_XML_NMTOKEN_DATATYPE						"http://www.w3.org/2001/XMLSchema#NMTOKEN"
			#define PREFIX_XML_LANGUAGE_DATATYPE					"http://www.w3.org/2001/XMLSchema#language"


			#define PREFIX_XML_BOOLEAN_DATATYPE						"http://www.w3.org/2001/XMLSchema#boolean"


			#define PREFIX_XML_DOUBLE_DATATYPE						"http://www.w3.org/2001/XMLSchema#double"
			#define PREFIX_XML_FLOAT_DATATYPE						"http://www.w3.org/2001/XMLSchema#float"


			#define PREFIX_XML_IRI_DATATYPE							"http://www.w3.org/2001/XMLSchema#anyURI"


			#define PREFIX_XML_BINARYHEX_DATATYPE					"http://www.w3.org/2001/XMLSchema#hexBinary"
			#define PREFIX_XML_BINARYBASE64_DATATYPE				"http://www.w3.org/2001/XMLSchema#base64Binary"


			#define PREFIX_XML_XML_DATATYPE							"http://www.w3.org/1999/02/22-rdf-syntax-ns#XMLLiteral"


			#define PREFIX_XML_DATETIME_DATATYPE					"http://www.w3.org/2001/XMLSchema#dateTime"
			#define PREFIX_XML_DATETIMESTAMP_DATATYPE				"http://www.w3.org/2001/XMLSchema#dateTimeStamp"



			#define PREFIX_MIN_INCLUSIVE_FACET						"http://www.w3.org/2001/XMLSchema#minInclusive"
			#define PREFIX_MIN_EXCLUSIVE_FACET						"http://www.w3.org/2001/XMLSchema#minExclusive"
			#define PREFIX_MAX_INCLUSIVE_FACET						"http://www.w3.org/2001/XMLSchema#maxInclusive"
			#define PREFIX_MAX_EXCLUSIVE_FACET						"http://www.w3.org/2001/XMLSchema#maxExclusive"


			#define PREFIX_MAX_LENGTH_FACET							"http://www.w3.org/2001/XMLSchema#maxLength"
			#define PREFIX_MIN_LENGTH_FACET							"http://www.w3.org/2001/XMLSchema#minLength"
			#define PREFIX_LENGTH_FACET								"http://www.w3.org/2001/XMLSchema#length"
			#define PREFIX_PATTERN_FACET							"http://www.w3.org/2001/XMLSchema#pattern"
			#define PREFIX_LANGUAGE_RANGE_FACET						"http://www.w3.org/2001/XMLSchema#langRange"






			// Datatype Facet Codes

			static const qint64 CDFC_RATIONAL					= 1;
			static const qint64 CDFC_DECIMAL					= 2;
			static const qint64 CDFC_INTEGER					= 3;

			static const qint64 CDFC_STRING						= 4;
			static const qint64 CDFC_NORMALIZED_STRING			= 5;
			static const qint64 CDFC_TOKEN						= 6;
			static const qint64 CDFC_NAME						= 7;
			static const qint64 CDFC_NCNAME						= 8;
			static const qint64 CDFC_NMTOKEN					= 9;
			static const qint64 CDFC_LANGUAGE					= 10;
			static const qint64 CDFC_DATETIMESTAMP				= 11;

			static const qint64 CDFC_MIN_EXCLUSIVE				= 20;
			static const qint64 CDFC_MIN_INCLUSIVE				= 21;
			static const qint64 CDFC_MAX_INCLUSIVE				= 22;
			static const qint64 CDFC_MAX_EXCLUSIVE				= 23;

			static const qint64 CDFC_LENGTH						= 24;
			static const qint64 CDFC_MIN_LENGTH_INCLUSIVE		= 25;
			static const qint64 CDFC_MAX_LENGTH_INCLUSIVE		= 26;

			static const qint64 CDFC_PATTERN					= 27;
			static const qint64 CDFC_LANGUAGE_RANGE				= 28;





			// OWL expressions
#define PREFIX_OWL_CLASS "http://www.w3.org/2002/07/owl#Class"
#define PREFIX_OWL_OBJECT_PROPERTY "http://www.w3.org/2002/07/owl#ObjectProperty"
#define PREFIX_OWL_DATATYPE_PROPERTY "http://www.w3.org/2002/07/owl#DatatypeProperty"
#define PREFIX_RDFS_DATATYPE "http://www.w3.org/2000/01/rdf-schema#Datatype"
#define PREFIX_RDF_TYPE "http://www.w3.org/1999/02/22-rdf-syntax-ns#type"
#define PREFIX_OWL_RESTRICTION "http://www.w3.org/2002/07/owl#Restriction"
#define PREFIX_OWL_NAMED_INDIVIDUAL "http://www.w3.org/2002/07/owl#NamedIndividual"

#define PREFIX_OWL_INTERSECTION_OF "http://www.w3.org/2002/07/owl#intersectionOf"
#define PREFIX_OWL_UNION_OF "http://www.w3.org/2002/07/owl#unionOf"
#define PREFIX_OWL_COMPLEMENT_OF "http://www.w3.org/2002/07/owl#complementOf"
#define PREFIX_OWL_ONE_OF "http://www.w3.org/2002/07/owl#oneOf"
#define PREFIX_OWL_DATATYPE_COMPLEMENT_OF "http://www.w3.org/2002/07/owl#datatypeComplementOf"
#define PREFIX_OWL_ON_DATATYPE "http://www.w3.org/2002/07/owl#onDatatype"
#define PREFIX_OWL_WITH_RESTRICTION "http://www.w3.org/2002/07/owl#withRestrictions"
#define PREFIX_OWL_ON_PROPERTY "http://www.w3.org/2002/07/owl#onProperty"
#define PREFIX_OWL_ON_CLASS "http://www.w3.org/2002/07/owl#onClass"

#define PREFIX_OWL_SOME_VALUES_FROM "http://www.w3.org/2002/07/owl#someValuesFrom"
#define PREFIX_OWL_ALL_VALUES_FROM "http://www.w3.org/2002/07/owl#allValuesFrom"
#define PREFIX_OWL_HAS_VALUE "http://www.w3.org/2002/07/owl#hasValue"
#define PREFIX_OWL_HAS_SELF "http://www.w3.org/2002/07/owl#hasSelf"

#define PREFIX_OWL_MIN_CARDINALITY "http://www.w3.org/2002/07/owl#minCardinality"
#define PREFIX_OWL_MAX_CARDINALITY "http://www.w3.org/2002/07/owl#maxCardinality"
#define PREFIX_OWL_CARDINALITY "http://www.w3.org/2002/07/owl#cardinality"

#define PREFIX_OWL_MIN_QUALIFIED_CARDINALITY "http://www.w3.org/2002/07/owl#minQualifiedCardinality"
#define PREFIX_OWL_MAX_QUALIFIED_CARDINALITY "http://www.w3.org/2002/07/owl#maxQualifiedCardinality"
#define PREFIX_OWL_QUALIFIED_CARDINALITY "http://www.w3.org/2002/07/owl#qualifiedCardinality"
#define PREFIX_OWL_ON_DATA_RANGE "http://www.w3.org/2002/07/owl#onDataRange"




#define PREFIX_OWL_ALL_DISJOINT_CLASSES "http://www.w3.org/2002/07/owl#AllDisjointClasses"
#define PREFIX_OWL_ALL_DISJOINT_PROPERTIES "http://www.w3.org/2002/07/owl#AllDisjointProperties"
#define PREFIX_OWL_ALL_DIFFERENT "http://www.w3.org/2002/07/owl#AllDifferent"
#define PREFIX_OWL_ALL_NEGATIVE_PROPERTY_ASSERTION "http://www.w3.org/2002/07/owl#NegativePropertyAssertion"

#define PREFIX_OWL_MEMBERS "http://www.w3.org/2002/07/owl#members"
#define PREFIX_OWL_DISTINCT_MEMBERS "http://www.w3.org/2002/07/owl#distinctMembers"
			
#define PREFIX_OWL_INVERSE_OF "http://www.w3.org/2002/07/owl#inverseOf"


#define PREFIX_OWL_SOURCE_INDIVIDUAL "http://www.w3.org/2002/07/owl#sourceIndividual"
#define PREFIX_OWL_ASSERTION_PROPERTY "http://www.w3.org/2002/07/owl#assertionProperty"
#define PREFIX_OWL_TARGET_INDIVIDUAL "http://www.w3.org/2002/07/owl#targetIndividual"
#define PREFIX_OWL_TARGET_VALUE "http://www.w3.org/2002/07/owl#targetValue"

#define PREFIX_OWL_SAME_AS "http://www.w3.org/2002/07/owl#sameAs"
#define PREFIX_OWL_DIFFERENT_FROM "http://www.w3.org/2002/07/owl#differentFrom"


#define PREFIX_RDFS_SUBCLASS_OF "http://www.w3.org/2000/01/rdf-schema#subClassOf"
#define PREFIX_OWL_EQUIVALENT_CLASS "http://www.w3.org/2002/07/owl#equivalentClass"
#define PREFIX_OWL_DISJOINT_WITH "http://www.w3.org/2002/07/owl#disjointWith"
#define PREFIX_OWL_DISJOINT_UNION_OF "http://www.w3.org/2002/07/owl#disjointUnionOf"

#define PREFIX_RDFS_SUBPROPERTY_OF "http://www.w3.org/2000/01/rdf-schema#subPropertyOf"
#define PREFIX_OWL_PROPERTY_CHAIN_AXIOM "http://www.w3.org/2002/07/owl#propertyChainAxiom"
#define PREFIX_OWL_EQUIVALENT_PROPERTY "http://www.w3.org/2002/07/owl#equivalentProperty"
#define PREFIX_OWL_PROPERTY_DISJOINT_WITH "http://www.w3.org/2002/07/owl#propertyDisjointWith"
#define PREFIX_RDFS_DOMAIN "http://www.w3.org/2000/01/rdf-schema#domain"
#define PREFIX_RDFS_RANGE "http://www.w3.org/2000/01/rdf-schema#range"



#define PREFIX_OWL_FUNCTIONAL_PROPERTY "http://www.w3.org/2002/07/owl#FunctionalProperty"
#define PREFIX_OWL_INVERSE_FUNCTIONAL_PROPERTY "http://www.w3.org/2002/07/owl#InverseFunctionalProperty"
#define PREFIX_OWL_REFLEXIVE_PROPERTY "http://www.w3.org/2002/07/owl#ReflexiveProperty"
#define PREFIX_OWL_IRREFLEXIVE_PROPERTY "http://www.w3.org/2002/07/owl#IrreflexiveProperty"
#define PREFIX_OWL_SYMMETRIC_PROPERTY "http://www.w3.org/2002/07/owl#SymmetricProperty"
#define PREFIX_OWL_ASYMMETRIC_PROPERTY "http://www.w3.org/2002/07/owl#AsymmetricProperty"
#define PREFIX_OWL_TRANSITIVE_PROPERTY "http://www.w3.org/2002/07/owl#TransitiveProperty"



#define PREFIX_RDF_FIRST "http://www.w3.org/1999/02/22-rdf-syntax-ns#first"
#define PREFIX_RDF_REST "http://www.w3.org/1999/02/22-rdf-syntax-ns#rest"
#define PREFIX_RDF_NIL "http://www.w3.org/1999/02/22-rdf-syntax-ns#nil"


#define CMAPPINGHASH CQtHash
#define CMAPPINGLIST CQtList

#define CBUILDHASH CQtHash
#define CBUILDSET CQtSet
#define CBUILDLIST CQtList

#define CONTOLOGYAXIOMSET CQtSet
#define CONTOLOGYAXIOMLIST CQtList

#define CBOXSET CQtSet
#define CBOXHASH CQtHash
#define CBOXLIST CQtList


#define CBOXMAP CQtManagedRestrictedModificationMap







const cint64 DEFAULTVECTOR2POTSIZE = 9;


const cint64 CCONCEPTVECTOR2POTSIZE						= DEFAULTVECTOR2POTSIZE;
const cint64 CROLEVECTOR2POTSIZE						= DEFAULTVECTOR2POTSIZE;
const cint64 CINDIVIDUALVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CCONCEPTDEPENDENCEVECTOR2POTSIZE			= DEFAULTVECTOR2POTSIZE;
const cint64 CROLEDEPENDENCEVECTOR2POTSIZE				= DEFAULTVECTOR2POTSIZE;
const cint64 CINDIVIDUALDEPENDENCEVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;
const cint64 CPSEUDOMODELVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CROLECHAINVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CCONCEPTDATAVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;
const cint64 CBRANCHINGTRIGGERVECTOR2POTSIZE			= DEFAULTVECTOR2POTSIZE;
const cint64 CIMPLICATIONREPLACEMENTVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;
const cint64 CNOMINALSCHEMATEMPLATEVECTOR2POTSIZE		= DEFAULTVECTOR2POTSIZE;
const cint64 CDATATYPEVECTOR2POTSIZE					= DEFAULTVECTOR2POTSIZE;

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_ONTOLOGY_ONTOLOGYSETTINGS
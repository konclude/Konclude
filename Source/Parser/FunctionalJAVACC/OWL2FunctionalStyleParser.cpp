/* OWL2FunctionalStyleParser.cc */
#include "OWL2FunctionalStyleParser.h"
namespace Konclude {
namespace Parser {
namespace FunctionalJAVACC {
  unsigned int jj_la1_0[] = {
0x0,0x0,0x200,0x0,0x0,0x800,0x40000,0x0,0x0,0x100,0x3f000,0x80000,0x800000,0x800000,0x0,0x0,0xff000000,0x0,0xff000000,0xff000000,0xff000000,0x800000,0x800000,0x800000,0x40000,0x0,0x0,0xff000000,0x0,0x180000,0x80000,0x0,0x0,0x80000,0x80000,0x0,0x0,0x0,0x0,0x0,0x0,};
  unsigned int jj_la1_1[] = {
0x0,0x0,0x0,0x0,0x0,0x0,0xffffe000,0x0,0x0,0x0,0x0,0x0,0x78,0x78,0x0,0x0,0x1f87,0x0,0x1f87,0x1f87,0x1f87,0x78,0x78,0x78,0xffffe000,0x0,0x1e000,0x1f87,0x3ffe0000,0x0,0x0,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,};
  unsigned int jj_la1_2[] = {
0x40100000,0x2000,0x0,0x40100000,0x40100000,0x0,0x3dfff,0x40500000,0x80000,0x0,0x0,0x40100000,0x40100000,0x40100000,0x0,0x40100000,0x40100000,0x40500000,0x40100000,0x40100000,0x40100000,0x40100000,0x40100000,0x40100000,0x3dfff,0x2000,0x0,0x40100000,0x0,0x40100000,0x40100000,0xf,0x40100000,0x40100000,0x40100000,0x1fc0,0x40500000,0x2000,0x40500000,0x3c000,0x40500000,};
  unsigned int jj_la1_3[] = {
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x0,};

  /** Constructor with user supplied TokenManager. */



QString OWL2FunctionalStyleParser::languageTag() {Token* t;if (!hasError) {
    
    t = jj_consume_token(LANGTAG);
    }
    
return JAVACC_TO_QSTRING(t->image);
assert(false);
  }


QString OWL2FunctionalStyleParser::nodeId() {Token* t;if (!hasError) {
    
    jj_consume_token(BLANK_NODE_PREFIX);
    }
    if (!hasError) {
    
    t = jj_consume_token(PN_LOCAL);
    }
    
return JAVACC_TO_QSTRING(t->image);
assert(false);
  }


QString OWL2FunctionalStyleParser::fullIri() {Token* t;if (!hasError) {
    
    t = jj_consume_token(FULL_IRI);
    }
    
return JAVACC_TO_QSTRING(t->image);
assert(false);
  }


QString OWL2FunctionalStyleParser::iri() {QString x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case FULL_IRI:{if (!hasError) {
      
      x = fullIri();
      }
      
      break;
      }
    case PNAME_NS:{if (!hasError) {
      
      x = abbreviatedIri();
      }
      
      break;
      }
    default:
      jj_la1[0] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


QString OWL2FunctionalStyleParser::abbreviatedIri() {Token* t;
        QString abbName;if (!hasError) {
    
    abbName = prefixName();
    }
    if (!hasError) {
    
    t = jj_consume_token(PN_LOCAL);
    }
    
return getBuilder()->resolveAbbreviationName(abbName+JAVACC_TO_QSTRING(t->image));
assert(false);
  }


QString OWL2FunctionalStyleParser::prefixName() {Token* t = nullptr;if (!hasError) {
    
    t = jj_consume_token(PNAME_NS);
    }
    
return JAVACC_TO_QSTRING(t->image);
assert(false);
  }


void OWL2FunctionalStyleParser::ontologyAnnotations() {if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case ANNOTATION:{
        ;
        break;
        }
      default:
        jj_la1[1] = jj_gen;
        goto end_label_1;
      }if (!hasError) {
      
      annotation();
      }
      
    }
    end_label_1: ;
    }
    
  }


void OWL2FunctionalStyleParser::ontologyDocument() {if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PREFIX:{
        ;
        break;
        }
      default:
        jj_la1[2] = jj_gen;
        goto end_label_2;
      }if (!hasError) {
      
      prefixDeclaration();
      }
      
    }
    end_label_2: ;
    }
    if (!hasError) {
    
    ontology();
    }
    
  }


void OWL2FunctionalStyleParser::prefixDeclaration() {QString abbName;
        QString fullName;if (!hasError) {
    
    jj_consume_token(PREFIX);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    abbName = prefixName();
    }
    if (!hasError) {
    
    jj_consume_token(EQUALS);
    }
    if (!hasError) {
    
    fullName = fullIri();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
	if (abbName.endsWith(":")) {
		abbName = abbName.remove(abbName.length()-1,1);
	}
getBuilder()->addNameAbbreviation(fullName,abbName);
  }


void OWL2FunctionalStyleParser::ontology() {if (!hasError) {
    
    jj_consume_token(ONTOLOGY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      ontologyIri();
      }
      if (!hasError) {
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PNAME_NS:
      case FULL_IRI:{if (!hasError) {
        
        versionIri();
        }
        
        break;
        }
      default:
        jj_la1[3] = jj_gen;
        ;
      }
      }
      
      break;
      }
    default:
      jj_la1[4] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    directlyImportsDocuments();
    }
    if (!hasError) {
    
    ontologyAnnotations();
    }
    if (!hasError) {
    
    axioms();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
  }


QString OWL2FunctionalStyleParser::ontologyIri() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return x;
assert(false);
  }


QString OWL2FunctionalStyleParser::versionIri() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return x;
assert(false);
  }


void OWL2FunctionalStyleParser::directlyImportsDocuments() {QString x;if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case IMPORT:{
        ;
        break;
        }
      default:
        jj_la1[5] = jj_gen;
        goto end_label_3;
      }if (!hasError) {
      
      jj_consume_token(IMPORT);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = iri();
      }
      if (!hasError) {
      
getBuilder()->addOntologyImport(x);
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
    }
    end_label_3: ;
    }
    
  }


void OWL2FunctionalStyleParser::axioms() {CBuildExpressionPtr x;if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case DECLARATION:
      case SUB_CLASS_OF:
      case EQUIVALENT_CLASSES:
      case DISJOINT_CLASSES:
      case DISJOINT_UNION:
      case SUB_OBJECT_PROPERTY_OF:
      case EQUIVALENT_OBJECT_PROPERTIES:
      case DISJOINT_OBJECT_PROPERTIES:
      case OBJECT_PROPERTY_DOMAIN:
      case OBJECT_PROPERTY_RANGE:
      case INVERSE_OBJECT_PROPERTIES:
      case FUNCTIONAL_OBJECT_PROPERTY:
      case INVERSE_FUNCTIONAL_OBJECT_PROPERTY:
      case REFLEXIVE_OBJECT_PROPERTY:
      case IRREFLEXIVE_OBJECT_PROPERTY:
      case SYMMETRIC_OBJECT_PROPERTY:
      case ASYMMETRIC_OBJECT_PROPERTY:
      case TRANSITIVE_OBJECT_PROPERTY:
      case SUB_DATA_PROPERTY_OF:
      case EQUIVALENT_DATA_PROPERTIES:
      case DISJOINT_DATA_PROPERTIES:
      case DATA_PROPERTY_DOMAIN:
      case DATA_PROPERTY_RANGE:
      case FUNCTIONAL_DATA_PROPERTY:
      case DATATYPE_DEFINITION:
      case HAS_KEY:
      case SAME_INDIVIDUAL:
      case DIFFERENT_INDIVIDUALS:
      case CLASS_ASSERTION:
      case OBJECT_PROPERTY_ASSERTION:
      case NEGATIVE_OBJECT_PROPERTY_ASSERTION:
      case DATA_PROPERTY_ASSERTION:
      case NEGATIVE_DATA_PROPERTY_ASSERTION:
      case ANNOTATION_ASSERTION:
      case SUB_ANNOTATION_PROPERTY_OF:
      case ANNOTATION_PROPERTY_DOMAIN:
      case ANNOTATION_PROPERTY_RANGE:{
        ;
        break;
        }
      default:
        jj_la1[6] = jj_gen;
        goto end_label_4;
      }if (!hasError) {
      
      x = axiom();
      }
      if (!hasError) {
      
getBuilder()->tellOntologyAxiom(x);
      }
      
    }
    end_label_4: ;
    }
    
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::owlClass() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return getBuilder()->getClass(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::datatype() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return getBuilder()->getDatatype(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectProperty() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return getBuilder()->getObjectProberty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataProperty() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return getBuilder()->getDataProberty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationProperty() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::individual() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      x = namedIndividual();
      }
      
      break;
      }
    case BLANK_NODE_PREFIX:{if (!hasError) {
      
      x = anonymousIndividual();
      }
      
      break;
      }
    default:
      jj_la1[7] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::namedIndividual() {QString x;if (!hasError) {
    
    x = iri();
    }
    
return getBuilder()->getNamedIndividual(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::anonymousIndividual() {QString x;if (!hasError) {
    
    x = nodeId();
    }
    
return getBuilder()->getAnonymousIndividual(getBuilder()->getOntologyName(),x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::literal(bool annotation) {QString x,l;
        CBuildExpressionPtr d = nullptr;if (!hasError) {
    
    x = quotedQString();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case REFERENCE:{if (!hasError) {
      
      jj_consume_token(REFERENCE);
      }
      if (!hasError) {
      
      d = datatype();
      }
      
      break;
      }
    default:
      jj_la1[9] = jj_gen;if (!hasError) {
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case LANGTAG:{if (!hasError) {
        
        l = languageTag();
        }
        
        break;
        }
      default:
        jj_la1[8] = jj_gen;
        ;
      }
      }
      
    }
    }
    
if (annotation) {
                        return nullptr;
                }
                if (d == nullptr) {
                        d = getBuilder()->getDatatype("http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral");
                        if (l.isEmpty()) {
                                x += "@";
                        } else {
                                x += l;
                        }
                }
                CBuildExpressionPtr y = getBuilder()->getDataLexicalValue(x);
                return getBuilder()->getDataLiteral((CDataLexicalValueExpressionPtr)y,(CDatatypeExpressionPtr)d);
assert(false);
  }


QString OWL2FunctionalStyleParser::quotedQString() {Token* t;if (!hasError) {
    
    t = jj_consume_token(QUOTED_QString);
    }
    
return JAVACC_TO_QSTRING(t->image);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::declaration() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(DECLARATION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = entity();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDeclaration(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::entity() {CBuildExpressionPtr x = nullptr;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case CLASS:{if (!hasError) {
      
      jj_consume_token(CLASS);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = owlClass();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    case DATATYPE:{if (!hasError) {
      
      jj_consume_token(DATATYPE);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = datatype();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    case OBJECT_PROPERTY:{if (!hasError) {
      
      jj_consume_token(OBJECT_PROPERTY);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = objectProperty();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    case DATA_PROPERTY:{if (!hasError) {
      
      jj_consume_token(DATA_PROPERTY);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = dataProperty();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    case ANNOTATION_PROPERTY:{if (!hasError) {
      
      jj_consume_token(ANNOTATION_PROPERTY);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = annotationProperty();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    case NAMED_INDIVIDUAL:{if (!hasError) {
      
      jj_consume_token(NAMED_INDIVIDUAL);
      }
      if (!hasError) {
      
      jj_consume_token(OPEN_BRACKET);
      }
      if (!hasError) {
      
      x = namedIndividual();
      }
      if (!hasError) {
      
      jj_consume_token(CLOSE_BRACKET);
      }
      
      break;
      }
    default:
      jj_la1[10] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectPropertyExpression() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      x = objectProperty();
      }
      
      break;
      }
    case OBJECT_INVERSE_OF:{if (!hasError) {
      
      x = inverseObjectProperty();
      }
      
      break;
      }
    default:
      jj_la1[11] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::inverseObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(OBJECT_INVERSE_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = objectProperty();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getInverseObjectPropertyOf(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataPropertyExpression() {CBuildExpressionPtr x;if (!hasError) {
    
    x = dataProperty();
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataRange() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      x = datatype();
      }
      
      break;
      }
    case DATA_INTERSECTION_OF:{if (!hasError) {
      
      x = dataIntersectionOf();
      }
      
      break;
      }
    case DATA_UNION_OF:{if (!hasError) {
      
      x = dataUnionOf();
      }
      
      break;
      }
    case DATA_COMPLEMENT_OF:{if (!hasError) {
      
      x = dataComplementOf();
      }
      
      break;
      }
    case DATA_ONE_OF:{if (!hasError) {
      
      x = dataOneOf();
      }
      
      break;
      }
    case DATATYPE_RESTRICTION:{if (!hasError) {
      
      x = datatypeRestriction();
      }
      
      break;
      }
    default:
      jj_la1[12] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::dataRangeList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = dataRange();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case DATATYPE_RESTRICTION:
      case DATA_INTERSECTION_OF:
      case DATA_UNION_OF:
      case DATA_COMPLEMENT_OF:
      case DATA_ONE_OF:
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[13] = jj_gen;
        goto end_label_5;
      }
    }
    end_label_5: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataIntersectionOf() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DATA_INTERSECTION_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    v = dataRangeList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataIntersectionOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataUnionOf() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DATA_UNION_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    v = dataRangeList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataUnionOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataComplementOf() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(DATA_COMPLEMENT_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = dataRange();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataComplementOf(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataOneOf() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DATA_ONE_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = literal(false);
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case QUOTED_QString:{
        ;
        break;
        }
      default:
        jj_la1[14] = jj_gen;
        goto end_label_6;
      }
    }
    end_label_6: ;
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataOneOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::datatypeRestriction() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DATATYPE_RESTRICTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = datatype();
    }
    if (!hasError) {
    
    v = facetRestrictionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
v.append(x);
                return getBuilder()->getDatatypeRestriction(v);
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::facetRestrictionList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = facetRestriction();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[15] = jj_gen;
        goto end_label_7;
      }
    }
    end_label_7: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::facetRestriction() {QString f;
        CBuildExpressionPtr l;if (!hasError) {
    
    f = iri();
    }
    if (!hasError) {
    
    l = literal(false);
    }
    
CBuildExpressionPtr x = getBuilder()->getDataFacet(f);
                return getBuilder()->getDataFacetRestriction(l,x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::classExpression() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      x = owlClass();
      }
      
      break;
      }
    case OBJECT_INTERSECTION_OF:{if (!hasError) {
      
      x = objectIntersectionOf();
      }
      
      break;
      }
    case OBJECT_UNION_OF:{if (!hasError) {
      
      x = objectUnionOf();
      }
      
      break;
      }
    case OBJECT_COMPLEMENT_OF:{if (!hasError) {
      
      x = objectComplementOf();
      }
      
      break;
      }
    case OBJECT_ONE_OF:{if (!hasError) {
      
      x = objectOneOf();
      }
      
      break;
      }
    case OBJECT_SOME_VALUES_FROM:{if (!hasError) {
      
      x = objectSomeValuesFrom();
      }
      
      break;
      }
    case OBJECT_ALL_VALUES_FROM:{if (!hasError) {
      
      x = objectAllValuesFrom();
      }
      
      break;
      }
    case OBJECT_HAS_VALUE:{if (!hasError) {
      
      x = objectHasValue();
      }
      
      break;
      }
    case OBJECT_HAS_SELF:{if (!hasError) {
      
      x = objectHasSelf();
      }
      
      break;
      }
    case OBJECT_MIN_CARDINALITY:{if (!hasError) {
      
      x = objectMinCardinality();
      }
      
      break;
      }
    case OBJECT_MAX_CARDINALITY:{if (!hasError) {
      
      x = objectMaxCardinality();
      }
      
      break;
      }
    case OBJECT_EXACT_CARDINALITY:{if (!hasError) {
      
      x = objectExactCardinality();
      }
      
      break;
      }
    case DATA_SOME_VALUES_FROM:{if (!hasError) {
      
      x = dataSomeValuesFrom();
      }
      
      break;
      }
    case DATA_ALL_VALUES_FROM:{if (!hasError) {
      
      x = dataAllValuesFrom();
      }
      
      break;
      }
    case DATA_HAS_VALUE:{if (!hasError) {
      
      x = dataHasValue();
      }
      
      break;
      }
    case DATA_MIN_CARDINALITY:{if (!hasError) {
      
      x = dataMinCardinality();
      }
      
      break;
      }
    case DATA_MAX_CARDINALITY:{if (!hasError) {
      
      x = dataMaxCardinality();
      }
      
      break;
      }
    case DATA_EXACT_CARDINALITY:{if (!hasError) {
      
      x = dataExactCardinality();
      }
      
      break;
      }
    default:
      jj_la1[16] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectIntersectionOf() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(OBJECT_INTERSECTION_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    v = classExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectIntersectionOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectUnionOf() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(OBJECT_UNION_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    v = classExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectUnionOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectComplementOf() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(OBJECT_COMPLEMENT_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectComplementOf(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectOneOf() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(OBJECT_ONE_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = individual();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PNAME_NS:
      case BLANK_NODE_PREFIX:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[17] = jj_gen;
        goto end_label_8;
      }
    }
    end_label_8: ;
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectOneOf(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectSomeValuesFrom() {CBuildExpressionPtr x;
        CBuildExpressionPtr y;if (!hasError) {
    
    jj_consume_token(OBJECT_SOME_VALUES_FROM);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectSomeValuesFrom(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectAllValuesFrom() {CBuildExpressionPtr x;
        CBuildExpressionPtr y;if (!hasError) {
    
    jj_consume_token(OBJECT_ALL_VALUES_FROM);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectAllValuesFrom(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectHasValue() {CBuildExpressionPtr x;
        CBuildExpressionPtr y;if (!hasError) {
    
    jj_consume_token(OBJECT_HAS_VALUE);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectHasValue(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectHasSelf() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(OBJECT_HAS_SELF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectHasSelf(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectMinCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(OBJECT_MIN_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case OBJECT_INTERSECTION_OF:
    case OBJECT_UNION_OF:
    case OBJECT_COMPLEMENT_OF:
    case OBJECT_ONE_OF:
    case OBJECT_SOME_VALUES_FROM:
    case OBJECT_ALL_VALUES_FROM:
    case OBJECT_HAS_VALUE:
    case OBJECT_HAS_SELF:
    case OBJECT_MIN_CARDINALITY:
    case OBJECT_MAX_CARDINALITY:
    case OBJECT_EXACT_CARDINALITY:
    case DATA_SOME_VALUES_FROM:
    case DATA_ALL_VALUES_FROM:
    case DATA_HAS_VALUE:
    case DATA_MIN_CARDINALITY:
    case DATA_MAX_CARDINALITY:
    case DATA_EXACT_CARDINALITY:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = classExpression();
      }
      
      break;
      }
    default:
      jj_la1[18] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopClass();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getObjectMinCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectMaxCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(OBJECT_MAX_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case OBJECT_INTERSECTION_OF:
    case OBJECT_UNION_OF:
    case OBJECT_COMPLEMENT_OF:
    case OBJECT_ONE_OF:
    case OBJECT_SOME_VALUES_FROM:
    case OBJECT_ALL_VALUES_FROM:
    case OBJECT_HAS_VALUE:
    case OBJECT_HAS_SELF:
    case OBJECT_MIN_CARDINALITY:
    case OBJECT_MAX_CARDINALITY:
    case OBJECT_EXACT_CARDINALITY:
    case DATA_SOME_VALUES_FROM:
    case DATA_ALL_VALUES_FROM:
    case DATA_HAS_VALUE:
    case DATA_MIN_CARDINALITY:
    case DATA_MAX_CARDINALITY:
    case DATA_EXACT_CARDINALITY:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = classExpression();
      }
      
      break;
      }
    default:
      jj_la1[19] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopClass();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getObjectMaxCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectExactCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(OBJECT_EXACT_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case OBJECT_INTERSECTION_OF:
    case OBJECT_UNION_OF:
    case OBJECT_COMPLEMENT_OF:
    case OBJECT_ONE_OF:
    case OBJECT_SOME_VALUES_FROM:
    case OBJECT_ALL_VALUES_FROM:
    case OBJECT_HAS_VALUE:
    case OBJECT_HAS_SELF:
    case OBJECT_MIN_CARDINALITY:
    case OBJECT_MAX_CARDINALITY:
    case OBJECT_EXACT_CARDINALITY:
    case DATA_SOME_VALUES_FROM:
    case DATA_ALL_VALUES_FROM:
    case DATA_HAS_VALUE:
    case DATA_MIN_CARDINALITY:
    case DATA_MAX_CARDINALITY:
    case DATA_EXACT_CARDINALITY:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = classExpression();
      }
      
      break;
      }
    default:
      jj_la1[20] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopClass();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getObjectExactlyCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataSomeValuesFrom() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(DATA_SOME_VALUES_FROM);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = dataProperty();
    }
    if (!hasError) {
    
    y = dataRange();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopDataRange();
                }
                return getBuilder()->getDataSomeValuesFrom(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataAllValuesFrom() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(DATA_ALL_VALUES_FROM);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = dataProperty();
    }
    if (!hasError) {
    
    y = dataRange();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataAllValuesFrom(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataHasValue() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(DATA_HAS_VALUE);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = literal(false);
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataHasValue(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataMinCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(DATA_MIN_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case DATATYPE_RESTRICTION:
    case DATA_INTERSECTION_OF:
    case DATA_UNION_OF:
    case DATA_COMPLEMENT_OF:
    case DATA_ONE_OF:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = dataRange();
      }
      
      break;
      }
    default:
      jj_la1[21] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopDataRange();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getDataMinCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataMaxCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(DATA_MAX_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case DATATYPE_RESTRICTION:
    case DATA_INTERSECTION_OF:
    case DATA_UNION_OF:
    case DATA_COMPLEMENT_OF:
    case DATA_ONE_OF:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = dataRange();
      }
      
      break;
      }
    default:
      jj_la1[22] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopDataRange();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getDataMaxCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataExactCardinality() {Token* n;
        CBuildExpressionPtr x;
        CBuildExpressionPtr y = nullptr;if (!hasError) {
    
    jj_consume_token(DATA_EXACT_CARDINALITY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    n = jj_consume_token(NON_NEGATIVE_INTEGER);
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case DATATYPE_RESTRICTION:
    case DATA_INTERSECTION_OF:
    case DATA_UNION_OF:
    case DATA_COMPLEMENT_OF:
    case DATA_ONE_OF:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      y = dataRange();
      }
      
      break;
      }
    default:
      jj_la1[23] = jj_gen;
      ;
    }
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
if (y == nullptr) {
                        y = getBuilder()->getTopDataRange();
                }
                cint64 cardinality = JAVACC_TO_QSTRING(n->image).toLongLong();
                return getBuilder()->getDataExactCardinality(x,y,cardinality);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::axiom() {CBuildExpressionPtr x = nullptr;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case DECLARATION:{if (!hasError) {
      
      x = declaration();
      }
      
      break;
      }
    case SUB_CLASS_OF:
    case EQUIVALENT_CLASSES:
    case DISJOINT_CLASSES:
    case DISJOINT_UNION:{if (!hasError) {
      
      x = classAxiom();
      }
      
      break;
      }
    case SUB_OBJECT_PROPERTY_OF:
    case EQUIVALENT_OBJECT_PROPERTIES:
    case DISJOINT_OBJECT_PROPERTIES:
    case OBJECT_PROPERTY_DOMAIN:
    case OBJECT_PROPERTY_RANGE:
    case INVERSE_OBJECT_PROPERTIES:
    case FUNCTIONAL_OBJECT_PROPERTY:
    case INVERSE_FUNCTIONAL_OBJECT_PROPERTY:
    case REFLEXIVE_OBJECT_PROPERTY:
    case IRREFLEXIVE_OBJECT_PROPERTY:
    case SYMMETRIC_OBJECT_PROPERTY:
    case ASYMMETRIC_OBJECT_PROPERTY:
    case TRANSITIVE_OBJECT_PROPERTY:{if (!hasError) {
      
      x = objectPropertyAxiom();
      }
      
      break;
      }
    case SUB_DATA_PROPERTY_OF:
    case EQUIVALENT_DATA_PROPERTIES:
    case DISJOINT_DATA_PROPERTIES:
    case DATA_PROPERTY_DOMAIN:
    case DATA_PROPERTY_RANGE:
    case FUNCTIONAL_DATA_PROPERTY:{if (!hasError) {
      
      x = dataPropertyAxiom();
      }
      
      break;
      }
    case DATATYPE_DEFINITION:{if (!hasError) {
      
      x = datatypeDefinition();
      }
      
      break;
      }
    case HAS_KEY:{if (!hasError) {
      
      x = hasKey();
      }
      
      break;
      }
    case SAME_INDIVIDUAL:
    case DIFFERENT_INDIVIDUALS:
    case CLASS_ASSERTION:
    case OBJECT_PROPERTY_ASSERTION:
    case NEGATIVE_OBJECT_PROPERTY_ASSERTION:
    case DATA_PROPERTY_ASSERTION:
    case NEGATIVE_DATA_PROPERTY_ASSERTION:{if (!hasError) {
      
      x = assertion();
      }
      
      break;
      }
    case ANNOTATION_ASSERTION:
    case SUB_ANNOTATION_PROPERTY_OF:
    case ANNOTATION_PROPERTY_DOMAIN:
    case ANNOTATION_PROPERTY_RANGE:{if (!hasError) {
      
      x = annotationAxiom();
      }
      
      break;
      }
    default:
      jj_la1[24] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


void OWL2FunctionalStyleParser::axiomAnnotations() {if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case ANNOTATION:{
        ;
        break;
        }
      default:
        jj_la1[25] = jj_gen;
        goto end_label_9;
      }if (!hasError) {
      
      annotation();
      }
      
    }
    end_label_9: ;
    }
    
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::classAxiom() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case SUB_CLASS_OF:{if (!hasError) {
      
      x = subClassOf();
      }
      
      break;
      }
    case EQUIVALENT_CLASSES:{if (!hasError) {
      
      x = equivalentClasses();
      }
      
      break;
      }
    case DISJOINT_CLASSES:{if (!hasError) {
      
      x = disjointClasses();
      }
      
      break;
      }
    case DISJOINT_UNION:{if (!hasError) {
      
      x = disjointUnion();
      }
      
      break;
      }
    default:
      jj_la1[26] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::subClassOf() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(SUB_CLASS_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = classExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getSubClassOf(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::equivalentClasses() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(EQUIVALENT_CLASSES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = classExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getEquivalentClasses(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::disjointClasses() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DISJOINT_CLASSES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = classExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDisjointClasses(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::disjointUnion() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DISJOINT_UNION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = owlClass();
    }
    if (!hasError) {
    
    v = classExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
v.prepend(x);
                return getBuilder()->getDisjointUnion(v);
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::classExpressionList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = classExpression();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case OBJECT_INTERSECTION_OF:
      case OBJECT_UNION_OF:
      case OBJECT_COMPLEMENT_OF:
      case OBJECT_ONE_OF:
      case OBJECT_SOME_VALUES_FROM:
      case OBJECT_ALL_VALUES_FROM:
      case OBJECT_HAS_VALUE:
      case OBJECT_HAS_SELF:
      case OBJECT_MIN_CARDINALITY:
      case OBJECT_MAX_CARDINALITY:
      case OBJECT_EXACT_CARDINALITY:
      case DATA_SOME_VALUES_FROM:
      case DATA_ALL_VALUES_FROM:
      case DATA_HAS_VALUE:
      case DATA_MIN_CARDINALITY:
      case DATA_MAX_CARDINALITY:
      case DATA_EXACT_CARDINALITY:
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[27] = jj_gen;
        goto end_label_10;
      }
    }
    end_label_10: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectPropertyAxiom() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case SUB_OBJECT_PROPERTY_OF:{if (!hasError) {
      
      x = subObjectPropertyOf();
      }
      
      break;
      }
    case EQUIVALENT_OBJECT_PROPERTIES:{if (!hasError) {
      
      x = equivalentObjectProperties();
      }
      
      break;
      }
    case DISJOINT_OBJECT_PROPERTIES:{if (!hasError) {
      
      x = disjointObjectProperties();
      }
      
      break;
      }
    case INVERSE_OBJECT_PROPERTIES:{if (!hasError) {
      
      x = inverseObjectProperties();
      }
      
      break;
      }
    case OBJECT_PROPERTY_DOMAIN:{if (!hasError) {
      
      x = objectPropertyDomain();
      }
      
      break;
      }
    case OBJECT_PROPERTY_RANGE:{if (!hasError) {
      
      x = objectPropertyRange();
      }
      
      break;
      }
    case FUNCTIONAL_OBJECT_PROPERTY:{if (!hasError) {
      
      x = functionalObjectProperty();
      }
      
      break;
      }
    case INVERSE_FUNCTIONAL_OBJECT_PROPERTY:{if (!hasError) {
      
      x = inverseFunctionalObjectProperty();
      }
      
      break;
      }
    case REFLEXIVE_OBJECT_PROPERTY:{if (!hasError) {
      
      x = reflexiveObjectProperty();
      }
      
      break;
      }
    case IRREFLEXIVE_OBJECT_PROPERTY:{if (!hasError) {
      
      x = irreflexiveObjectProperty();
      }
      
      break;
      }
    case SYMMETRIC_OBJECT_PROPERTY:{if (!hasError) {
      
      x = symmetricObjectProperty();
      }
      
      break;
      }
    case ASYMMETRIC_OBJECT_PROPERTY:{if (!hasError) {
      
      x = asymmetricObjectProperty();
      }
      
      break;
      }
    case TRANSITIVE_OBJECT_PROPERTY:{if (!hasError) {
      
      x = transitiveObjectProperty();
      }
      
      break;
      }
    default:
      jj_la1[28] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::subObjectPropertyOf() {CBuildExpressionPtr x = nullptr;
        CBuildExpressionPtr z;if (!hasError) {
    
    jj_consume_token(SUB_OBJECT_PROPERTY_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case OBJECT_INVERSE_OF:
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      x = objectPropertyExpression();
      }
      
      break;
      }
    case OBJECT_PROPERTY_CHAIN:{if (!hasError) {
      
      x = propertyExpressionChain();
      }
      
      break;
      }
    default:
      jj_la1[29] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    if (!hasError) {
    
    z = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getSubObjectPropertyOf(x,z);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::propertyExpressionChain() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(OBJECT_PROPERTY_CHAIN);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    v = objectPropertyExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectPropertyChain(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::equivalentObjectProperties() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(EQUIVALENT_OBJECT_PROPERTIES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = objectPropertyExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getEquivalentObjectProperties(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::disjointObjectProperties() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DISJOINT_OBJECT_PROPERTIES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = objectPropertyExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDisjointObjectProperties(v);
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::objectPropertyExpressionList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = objectPropertyExpression();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case OBJECT_INVERSE_OF:
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[30] = jj_gen;
        goto end_label_11;
      }
    }
    end_label_11: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::inverseObjectProperties() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(INVERSE_OBJECT_PROPERTIES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getInverseObjectProperties(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectPropertyDomain() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(OBJECT_PROPERTY_DOMAIN);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectPropertyDomainExpression(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectPropertyRange() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(OBJECT_PROPERTY_RANGE);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectPropertyRangeExpression(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::functionalObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(FUNCTIONAL_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getFunctionalObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::inverseFunctionalObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(INVERSE_FUNCTIONAL_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getInverseFunctionalObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::reflexiveObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(REFLEXIVE_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getReflexiveObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::irreflexiveObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(IRREFLEXIVE_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getIrreflexiveObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::symmetricObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(SYMMETRIC_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getSymmetricObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::asymmetricObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(ASYMMETRIC_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getAsymmetricObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::transitiveObjectProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(TRANSITIVE_OBJECT_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getTransetiveObjectProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataPropertyAxiom() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case SUB_DATA_PROPERTY_OF:{if (!hasError) {
      
      x = subDataPropertyOf();
      }
      
      break;
      }
    case EQUIVALENT_DATA_PROPERTIES:{if (!hasError) {
      
      x = equivalentDataProperties();
      }
      
      break;
      }
    case DISJOINT_DATA_PROPERTIES:{if (!hasError) {
      
      x = disjointDataProperties();
      }
      
      break;
      }
    case DATA_PROPERTY_DOMAIN:{if (!hasError) {
      
      x = dataPropertyDomain();
      }
      
      break;
      }
    case DATA_PROPERTY_RANGE:{if (!hasError) {
      
      x = dataPropertyRange();
      }
      
      break;
      }
    case FUNCTIONAL_DATA_PROPERTY:{if (!hasError) {
      
      x = functionalDataProperty();
      }
      
      break;
      }
    default:
      jj_la1[31] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::subDataPropertyOf() {CBuildExpressionPtr x,y;if (!hasError) {
    
    jj_consume_token(SUB_DATA_PROPERTY_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = dataPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getSubDataPropertyOf(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::equivalentDataProperties() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(EQUIVALENT_DATA_PROPERTIES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = dataPropertyExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getEquivalentDataProperties(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::disjointDataProperties() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DISJOINT_DATA_PROPERTIES);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = dataPropertyExpressionList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDisjointDataProperties(v);
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::dataPropertyExpressionList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = dataPropertyExpression();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[32] = jj_gen;
        goto end_label_12;
      }
    }
    end_label_12: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataPropertyDomain() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(DATA_PROPERTY_DOMAIN);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataPropertyDomainExpression(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataPropertyRange() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(DATA_PROPERTY_RANGE);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = dataRange();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataPropertyRangeExpression(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::functionalDataProperty() {CBuildExpressionPtr x;if (!hasError) {
    
    jj_consume_token(FUNCTIONAL_DATA_PROPERTY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getFunctionalDataProperty(x);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::datatypeDefinition() {if (!hasError) {
    
    jj_consume_token(DATATYPE_DEFINITION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    datatype();
    }
    if (!hasError) {
    
    dataRange();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::hasKey() {if (!hasError) {
    
    jj_consume_token(HAS_KEY);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    classExpression();
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case OBJECT_INVERSE_OF:
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[33] = jj_gen;
        goto end_label_13;
      }if (!hasError) {
      
      objectPropertyExpression();
      }
      
    }
    end_label_13: ;
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case OBJECT_INVERSE_OF:
      case PNAME_NS:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[34] = jj_gen;
        goto end_label_14;
      }if (!hasError) {
      
      objectPropertyExpression();
      }
      
    }
    end_label_14: ;
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::assertion() {CBuildExpressionPtr x;if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case SAME_INDIVIDUAL:{if (!hasError) {
      
      x = sameIndividual();
      }
      
      break;
      }
    case DIFFERENT_INDIVIDUALS:{if (!hasError) {
      
      x = differentIndividuals();
      }
      
      break;
      }
    case CLASS_ASSERTION:{if (!hasError) {
      
      x = classAssertion();
      }
      
      break;
      }
    case OBJECT_PROPERTY_ASSERTION:{if (!hasError) {
      
      x = objectPropertyAssertion();
      }
      
      break;
      }
    case NEGATIVE_OBJECT_PROPERTY_ASSERTION:{if (!hasError) {
      
      x = negativeObjectPropertyAssertion();
      }
      
      break;
      }
    case DATA_PROPERTY_ASSERTION:{if (!hasError) {
      
      x = dataPropertyAssertion();
      }
      
      break;
      }
    case NEGATIVE_DATA_PROPERTY_ASSERTION:{if (!hasError) {
      
      x = negativeDataPropertyAssertion();
      }
      
      break;
      }
    default:
      jj_la1[35] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return x;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::sameIndividual() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(SAME_INDIVIDUAL);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = individualList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getSameIndividual(v);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::differentIndividuals() {CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    jj_consume_token(DIFFERENT_INDIVIDUALS);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    v = individualList();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDifferentIndividuals(v);
assert(false);
  }


CEXPRESSIONLIST               <CBuildExpressionPtr> OWL2FunctionalStyleParser::individualList() {CBuildExpressionPtr x;
        CEXPRESSIONLIST<CBuildExpressionPtr> v;if (!hasError) {
    
    while (!hasError) {if (!hasError) {
      
      x = individual();
      }
      if (!hasError) {
      
v.append(x);
      }
      
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case PNAME_NS:
      case BLANK_NODE_PREFIX:
      case FULL_IRI:{
        ;
        break;
        }
      default:
        jj_la1[36] = jj_gen;
        goto end_label_15;
      }
    }
    end_label_15: ;
    }
    
return v;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::classAssertion() {CBuildExpressionPtr x, y;if (!hasError) {
    
    jj_consume_token(CLASS_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = classExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getClassAssertion(x,y);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::objectPropertyAssertion() {CBuildExpressionPtr x, y, z;if (!hasError) {
    
    jj_consume_token(OBJECT_PROPERTY_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    z = individual();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getObjectPropertyAssertion(x,y,z);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::negativeObjectPropertyAssertion() {CBuildExpressionPtr x, y, z;if (!hasError) {
    
    jj_consume_token(NEGATIVE_OBJECT_PROPERTY_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = objectPropertyExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    z = individual();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getNegativeObjectPropertyAssertion(x,y,z);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::dataPropertyAssertion() {CBuildExpressionPtr x, y, z;if (!hasError) {
    
    jj_consume_token(DATA_PROPERTY_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    z = literal(false);
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getDataPropertyAssertion(x,y,z);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::negativeDataPropertyAssertion() {CBuildExpressionPtr x, y, z;if (!hasError) {
    
    jj_consume_token(NEGATIVE_DATA_PROPERTY_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    x = dataPropertyExpression();
    }
    if (!hasError) {
    
    y = individual();
    }
    if (!hasError) {
    
    z = literal(false);
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return getBuilder()->getNegativeDataPropertyAssertion(x,y,z);
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotation() {if (!hasError) {
    
    jj_consume_token(ANNOTATION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    annotationAnnotations();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    annotationValue();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


void OWL2FunctionalStyleParser::annotationAnnotations() {if (!hasError) {
    
    while (!hasError) {
      switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
      case ANNOTATION:{
        ;
        break;
        }
      default:
        jj_la1[37] = jj_gen;
        goto end_label_16;
      }if (!hasError) {
      
      annotation();
      }
      
    }
    end_label_16: ;
    }
    
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationValue() {if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case BLANK_NODE_PREFIX:{if (!hasError) {
      
      anonymousIndividual();
      }
      
      break;
      }
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      iri();
      }
      
      break;
      }
    case QUOTED_QString:{if (!hasError) {
      
      literal(true);
      }
      
      break;
      }
    default:
      jj_la1[38] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationAxiom() {if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case ANNOTATION_ASSERTION:{if (!hasError) {
      
      annotationAssertion();
      }
      
      break;
      }
    case SUB_ANNOTATION_PROPERTY_OF:{if (!hasError) {
      
      subAnnotationPropertyOf();
      }
      
      break;
      }
    case ANNOTATION_PROPERTY_DOMAIN:{if (!hasError) {
      
      annotationPropertyDomain();
      }
      
      break;
      }
    case ANNOTATION_PROPERTY_RANGE:{if (!hasError) {
      
      annotationPropertyRange();
      }
      
      break;
      }
    default:
      jj_la1[39] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationAssertion() {if (!hasError) {
    
    jj_consume_token(ANNOTATION_ASSERTION);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    annotationSubject();
    }
    if (!hasError) {
    
    annotationValue();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationSubject() {if (!hasError) {
    
    switch ((jj_ntk==-1)?jj_ntk_f():jj_ntk) {
    case PNAME_NS:
    case FULL_IRI:{if (!hasError) {
      
      iri();
      }
      
      break;
      }
    case BLANK_NODE_PREFIX:{if (!hasError) {
      
      anonymousIndividual();
      }
      
      break;
      }
    default:
      jj_la1[40] = jj_gen;
      jj_consume_token(-1);
      errorHandler->handleParseError(token, getToken(1), __FUNCTION__, this), hasError = true;
    }
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::subAnnotationPropertyOf() {if (!hasError) {
    
    jj_consume_token(SUB_ANNOTATION_PROPERTY_OF);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationPropertyDomain() {if (!hasError) {
    
    jj_consume_token(ANNOTATION_PROPERTY_DOMAIN);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    iri();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


CBuildExpressionPtr OWL2FunctionalStyleParser::annotationPropertyRange() {if (!hasError) {
    
    jj_consume_token(ANNOTATION_PROPERTY_RANGE);
    }
    if (!hasError) {
    
    jj_consume_token(OPEN_BRACKET);
    }
    if (!hasError) {
    
    axiomAnnotations();
    }
    if (!hasError) {
    
    annotationProperty();
    }
    if (!hasError) {
    
    iri();
    }
    if (!hasError) {
    
    jj_consume_token(CLOSE_BRACKET);
    }
    
return nullptr;
assert(false);
  }


 OWL2FunctionalStyleParser::OWL2FunctionalStyleParser(TokenManager *tm) : CJAVACCParser()
{
    head = NULL;
    ReInit(tm);
}
   OWL2FunctionalStyleParser::~OWL2FunctionalStyleParser()
{
  if (token_source) delete token_source;
  if (head) {
    Token *next, *t = head;
    while (t) {
      next = t->next;
      delete t;
      t = next;
    }
  }
  if (errorHandlerCreated) {
    delete errorHandler;
  }
}

void OWL2FunctionalStyleParser::ReInit(TokenManager *tm){
    if (head) delete head;
    errorHandler = new ErrorHandler();
    errorHandlerCreated = true;
    hasError = false;
    token_source = tm;
    head = token = new Token();
    token->kind = 0;
    token->next = NULL;
    jj_lookingAhead = false;
    jj_rescan = false;
    jj_done = false;
    jj_scanpos = jj_lastpos = NULL;
    jj_gc = 0;
    jj_kind = -1;
    trace_indent = 0;
    trace_enabled = false;
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 41; i++) jj_la1[i] = -1;
  }


Token * OWL2FunctionalStyleParser::jj_consume_token(int kind)  {
    Token *oldToken;
    if ((oldToken = token)->next != NULL) token = token->next;
    else token = token->next = token_source->getNextToken();
    jj_ntk = -1;
    if (token->kind == kind) {
      jj_gen++;
      return token;
    }
    token = oldToken;
    jj_kind = kind;
    JAVACC_STRING_TYPE image = kind >= 0 ? tokenImage[kind] : tokenImage[0];
    errorHandler->handleUnexpectedToken(kind, image.substr(1, image.size() - 2), getToken(1), this), hasError = true;
    return token;
  }


/** Get the next Token. */

Token * OWL2FunctionalStyleParser::getNextToken(){
    if (token->next != NULL) token = token->next;
    else token = token->next = token_source->getNextToken();
    jj_ntk = -1;
    jj_gen++;
    return token;
  }

/** Get the specific Token. */

Token * OWL2FunctionalStyleParser::getToken(int index){
    Token *t = token;
    for (int i = 0; i < index; i++) {
      if (t->next != NULL) t = t->next;
      else t = t->next = token_source->getNextToken();
    }
    return t;
  }


int OWL2FunctionalStyleParser::jj_ntk_f(){
    if ((jj_nt=token->next) == NULL)
      return (jj_ntk = (token->next=token_source->getNextToken())->kind);
    else
      return (jj_ntk = jj_nt->kind);
  }


 void  OWL2FunctionalStyleParser::parseError()   {
      fprintf(stderr, "Parse error at: %d:%d, after token: %s encountered: %s\n", token->beginLine, token->beginColumn, addUnicodeEscapes(token->image).c_str(), addUnicodeEscapes(getToken(1)->image).c_str());
   }


 void OWL2FunctionalStyleParser::enable_tracing()  {
  }

  /** Disable tracing. */

 void OWL2FunctionalStyleParser::disable_tracing()  {
  }


}
}
}

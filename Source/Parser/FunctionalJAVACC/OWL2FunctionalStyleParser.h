#ifndef OWL2FUNCTIONALSTYLEPARSER_H
#define OWL2FUNCTIONALSTYLEPARSER_H
#include "JavaCC.h"
#include "CharStream.h"
#include "Token.h"
#include "TokenManager.h"
#include "OWL2FunctionalStyleParserTokenManager.h"
#include "CJAVACCParser.h"

#include "OWL2FunctionalStyleParserConstants.h"
#include "ErrorHandler.h"
namespace Konclude {
namespace Parser {
namespace FunctionalJAVACC {
  struct JJCalls {
    int gen;
    Token *first;
    int arg;
    JJCalls *next;
    ~JJCalls() { if (next) delete next; }
    JJCalls() { next = NULL; arg = 0; gen = -1; first = NULL; }
  };

class OWL2FunctionalStyleParser : public CJAVACCParser {
   public:

QString languageTag();

QString nodeId();

QString fullIri();

QString iri();

QString abbreviatedIri();

QString prefixName();

void ontologyAnnotations();

void ontologyDocument();

void prefixDeclaration();

void ontology();

QString ontologyIri();

QString versionIri();

void directlyImportsDocuments();

void axioms();

CBuildExpressionPtr owlClass();

CBuildExpressionPtr datatype();

CBuildExpressionPtr objectProperty();

CBuildExpressionPtr dataProperty();

CBuildExpressionPtr annotationProperty();

CBuildExpressionPtr individual();

CBuildExpressionPtr namedIndividual();

CBuildExpressionPtr anonymousIndividual();

CBuildExpressionPtr literal(bool annotation);

QString quotedQString();

CBuildExpressionPtr declaration();

CBuildExpressionPtr entity();

CBuildExpressionPtr objectPropertyExpression();

CBuildExpressionPtr inverseObjectProperty();

CBuildExpressionPtr dataPropertyExpression();

CBuildExpressionPtr dataRange();

CEXPRESSIONLIST               <CBuildExpressionPtr> dataRangeList();

CBuildExpressionPtr dataIntersectionOf();

CBuildExpressionPtr dataUnionOf();

CBuildExpressionPtr dataComplementOf();

CBuildExpressionPtr dataOneOf();

CBuildExpressionPtr datatypeRestriction();

CEXPRESSIONLIST               <CBuildExpressionPtr> facetRestrictionList();

CBuildExpressionPtr facetRestriction();

CBuildExpressionPtr classExpression();

CBuildExpressionPtr objectIntersectionOf();

CBuildExpressionPtr objectUnionOf();

CBuildExpressionPtr objectComplementOf();

CBuildExpressionPtr objectOneOf();

CBuildExpressionPtr objectSomeValuesFrom();

CBuildExpressionPtr objectAllValuesFrom();

CBuildExpressionPtr objectHasValue();

CBuildExpressionPtr objectHasSelf();

CBuildExpressionPtr objectMinCardinality();

CBuildExpressionPtr objectMaxCardinality();

CBuildExpressionPtr objectExactCardinality();

CBuildExpressionPtr dataSomeValuesFrom();

CBuildExpressionPtr dataAllValuesFrom();

CBuildExpressionPtr dataHasValue();

CBuildExpressionPtr dataMinCardinality();

CBuildExpressionPtr dataMaxCardinality();

CBuildExpressionPtr dataExactCardinality();

CBuildExpressionPtr axiom();

void axiomAnnotations();

CBuildExpressionPtr classAxiom();

CBuildExpressionPtr subClassOf();

CBuildExpressionPtr equivalentClasses();

CBuildExpressionPtr disjointClasses();

CBuildExpressionPtr disjointUnion();

CEXPRESSIONLIST               <CBuildExpressionPtr> classExpressionList();

CBuildExpressionPtr objectPropertyAxiom();

CBuildExpressionPtr subObjectPropertyOf();

CBuildExpressionPtr propertyExpressionChain();

CBuildExpressionPtr equivalentObjectProperties();

CBuildExpressionPtr disjointObjectProperties();

CEXPRESSIONLIST               <CBuildExpressionPtr> objectPropertyExpressionList();

CBuildExpressionPtr inverseObjectProperties();

CBuildExpressionPtr objectPropertyDomain();

CBuildExpressionPtr objectPropertyRange();

CBuildExpressionPtr functionalObjectProperty();

CBuildExpressionPtr inverseFunctionalObjectProperty();

CBuildExpressionPtr reflexiveObjectProperty();

CBuildExpressionPtr irreflexiveObjectProperty();

CBuildExpressionPtr symmetricObjectProperty();

CBuildExpressionPtr asymmetricObjectProperty();

CBuildExpressionPtr transitiveObjectProperty();

CBuildExpressionPtr dataPropertyAxiom();

CBuildExpressionPtr subDataPropertyOf();

CBuildExpressionPtr equivalentDataProperties();

CBuildExpressionPtr disjointDataProperties();

CEXPRESSIONLIST               <CBuildExpressionPtr> dataPropertyExpressionList();

CBuildExpressionPtr dataPropertyDomain();

CBuildExpressionPtr dataPropertyRange();

CBuildExpressionPtr functionalDataProperty();

CBuildExpressionPtr datatypeDefinition();

CBuildExpressionPtr hasKey();

CBuildExpressionPtr assertion();

CBuildExpressionPtr sameIndividual();

CBuildExpressionPtr differentIndividuals();

CEXPRESSIONLIST               <CBuildExpressionPtr> individualList();

CBuildExpressionPtr classAssertion();

CBuildExpressionPtr objectPropertyAssertion();

CBuildExpressionPtr negativeObjectPropertyAssertion();

CBuildExpressionPtr dataPropertyAssertion();

CBuildExpressionPtr negativeDataPropertyAssertion();

CBuildExpressionPtr annotation();

void annotationAnnotations();

CBuildExpressionPtr annotationValue();

CBuildExpressionPtr annotationAxiom();

CBuildExpressionPtr annotationAssertion();

CBuildExpressionPtr annotationSubject();

CBuildExpressionPtr subAnnotationPropertyOf();

CBuildExpressionPtr annotationPropertyDomain();

CBuildExpressionPtr annotationPropertyRange();
  public: TokenManager *token_source;
  public: CharStream *jj_input_stream;
  /** Current token. */
  public: Token *token;
  /** Next token. */
  public: Token *jj_nt;
  private: int jj_ntk;
  private: JJCalls jj_2_rtns[1];
  private: bool jj_rescan;
  private: int jj_gc;
  private: Token *jj_scanpos, *jj_lastpos;
  private: int jj_la;
  /** Whether we are looking ahead. */
  private: bool jj_lookingAhead;
  private: bool jj_semLA;
  private: int jj_gen;
  private: int jj_la1[42];
  private: ErrorHandler *errorHandler;
  private: bool errorHandlerCreated;
  protected: bool hasError;
  public: void setErrorHandler(ErrorHandler *eh) {
    if (errorHandlerCreated) delete errorHandler;
    errorHandler = eh;
    errorHandlerCreated = false;
  }
 Token *head; 
 public: 

 OWL2FunctionalStyleParser(TokenManager *tm);
   public: virtual ~OWL2FunctionalStyleParser();

void ReInit(TokenManager *tm);

Token * jj_consume_token(int kind);

Token * getNextToken();

Token * getToken(int index);

int jj_ntk_f();
 private: int jj_kind;
  int **jj_expentries;
  int *jj_expentry;

  /** Generate ParseException. */

protected: virtual void  parseError();
  private: int trace_indent;
  private: bool trace_enabled;
  /** Enable tracing. */

public: void enable_tracing();

public: void disable_tracing();
public:
                inline COntologyBuilder* getBuilder() {
                        return mBuilder;
                }

        protected:
                COntologyBuilder* mBuilder;private: bool jj_done;

};
}
}
}
#endif

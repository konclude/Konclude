grammar OWL2FunctionalGrammer;

options {
  language = Cpp;  
  k = 2;
}

tokens {
/* Ontology Keywords */
  PREFIX = 'Prefix';
  ONTOLOGY = 'Ontology';
  IMPORT = 'Import';
  
/* Entities and Literals */
  CLASS = 'Class';
  DATATYPE = 'Datatype';
  OBJECT_PROPERTY = 'ObjectProperty';
  DATA_PROPERTY = 'DataProperty';
  ANNOTATION_PROPERTY = 'Annotation_Property';
  NAMED_INDIVIDUAL = 'NamedIndividual';
  
/* Declaration */
  DECLARATION = 'Declaration';
  
/* Property Expressions */
  OBJECT_INVERSE_OF = 'ObjectInverseOf';
  OBJECT_PROPERTY_CHAIN = 'ObjectPropertyChain';
    
/* Data Ranges */
  DATATYPE_RESTRICTION = 'DatatypeRestriction';
    
/* Class Expressions */  
  OBJECT_INTERSECTION_OF = 'ObjectIntersectionOf';  
  OBJECT_UNION_OF = 'ObjectUnionOf';
  OBJECT_COMPLEMENT_OF = 'ObjectComplementOf';
  OBJECT_ONE_OF = 'ObjectOneOf';
  OBJECT_SOME_VALUES_FROM = 'ObjectSomeValuesFrom';
  OBJECT_ALL_VALUES_FROM = 'ObjectAllValuesFrom';
  OBJECT_HAS_VALUE = 'ObjectHasValue';
  OBJECT_HAS_SELF = 'ObjectHasSelf';
  OBJECT_MIN_CARDINALITY = 'ObjectMinCardinality'; 
  OBJECT_MAX_CARDINALITY = 'ObjectMaxCardinality';
  OBJECT_EXACT_CARDINALITY = 'ObjectExactCardinality';
  DATA_INTERSECTION_OF = 'DataIntersectionOf';    
  DATA_UNION_OF = 'DataUnionOf';         
  DATA_COMPLEMENT_OF = 'DataComplementOf';    
  DATA_ONE_OF = 'DataOneOf';
  DATA_SOME_VALUES_FROM = 'DataSomeValuesFrom';  
  DATA_ALL_VALUES_FROM = 'DataAllValuesFrom';   
  DATA_HAS_VALUE = 'DataHasValue';                 
  DATA_MIN_CARDINALITY = 'DataMinCardinality';  
  DATA_MAX_CARDINALITY = 'DataMaxCardinality';  
  DATA_EXACT_CARDINALITY = 'DataExactCardinality';
   
/* Class Expressions Axioms */
  SUB_CLASS_OF = 'SubClassOf';
  EQUIVALENT_CLASSES = 'EquivalentClasses';
  DISJOINT_CLASSES = 'DisjointClasses';    
  DISJOINT_UNION = 'DisjointUnion';
    
/* Object Property Axioms */ 
  SUB_OBJECT_PROPERTY_OF = 'SubObjectPropertyOf';
  EQUIVALENT_OBJECT_PROPERTIES = 'EquivalentObjectProperties';
  DISJOINT_OBJECT_PROPERTIES = 'DisjointObjectProperties';
  OBJECT_PROPERTY_DOMAIN = 'ObjectPropertyDomain';
  OBJECT_PROPERTY_RANGE = 'ObjectPropertyRange';
  INVERSE_OBJECT_PROPERTIES = 'InverseObjectProperties'; 
  FUNCTIONAL_OBJECT_PROPERTY = 'FunctionalObjectProperty';
  INVERSE_FUNCTIONAL_OBJECT_PROPERTY = 'InverseFunctionalObjectProperty';
  REFLEXIVE_OBJECT_PROPERTY = 'ReflexiveObjectProperty';
  IRREFLEXIVE_OBJECT_PROPERTY = 'IrreflexiveObjectProperty';
  SYMMETRIC_OBJECT_PROPERTY = 'SymmetricObjectProperty';
  ASYMMETRIC_OBJECT_PROPERTY = 'AsymmetricObjectProperty';
  TRANSITIVE_OBJECT_PROPERTY = 'TransitiveObjectProperty';
   
/* Data Property Axioms */          
  SUB_DATA_PROPERTY_OF = 'SubDataPropertyOf';          
  EQUIVALENT_DATA_PROPERTIES = 'EquivalentDataProperties';   
  DISJOINT_DATA_PROPERTIES = 'DisjointDataProperties';     
  DATA_PROPERTY_DOMAIN = 'DataPropertyDomain';         
  DATA_PROPERTY_RANGE = 'DataPropertyRange';      
  FUNCTIONAL_DATA_PROPERTY = 'FunctionalDataProperty';     
  DATATYPE_DEFINITION = 'DatatypeDefinition';
  
/* Keys */        
  HAS_KEY = 'HasKey';
    
/* Assertions */
  SAME_INDIVIDUAL = 'SameIndividual';
  DIFFERENT_INDIVIDUALS = 'DifferentIndividuals';
  CLASS_ASSERTION = 'ClassAssertion';
  OBJECT_PROPERTY_ASSERTION = 'ObjectPropertyAssertion';
  NEGATIVE_OBJECT_PROPERTY_ASSERTION = 'NegativeObjectPropertyAssertion';
  DATA_PROPERTY_ASSERTION = 'DataPropertyAssertion';
  NEGATIVE_DATA_PROPERTY_ASSERTION = 'NegativeDataPropertyAssertion';
   
/* Annotations */
  ANNOTATION = 'Annotation';
  ANNOTATION_ASSERTION = 'AnnotationAssertion';
  SUB_ANNOTATION_PROPERTY_OF = 'SubAnnotationPropertyOf';
  ANNOTATION_PROPERTY_DOMAIN = 'AnnotationPropertyDomain';         
  ANNOTATION_PROPERTY_RANGE = 'AnnotationPropertyRange';
     
}


@header {
}

@members {
}


/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

WHITESPACE 
    : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+   { skip(); } 
    ;


NON_NEGATIVE_INTEGER 
    : ('1'..'9') DIGIT*
    ;

fragment
DIGIT 
    : '0'..'9'
    ;    

REFERENCE
    : '^^'
    ;
    
OPEN_BRACE
    : '('
    ;

CLOSE_BRACE
    : ')'
    ;    
 
EQUALS
    : '='
    ; 

/* a finite sequence of characters in which " (U+22) and \ (U+5C) 
*  occur only in pairs of the form \" (U+5C, U+22) and \\ (U+5C, U+5C), 
*  enclosed in a pair of " (U+22) characters
*/
QUOTED_STRING 
    :  '\"' ( ~('\\'|'\"') 
            |  ('\\' ('\"'|'\''|'\\')) 
            )* 
       '\"'
    ;

IRI_REF 
    : '<' ( 
           ~( '<' 
            | '>' 
            | '\"' 
            | '{' 
            | '}' 
            | '|' 
            | '^' 
            | '`' 
            | '\\'
            | '\u0000'..'\u0020'
            ) 
          )* '>'
     ;  /* see IRI_REF of [SPARQL] */

PNAME_NS 
    : PN_PREFIX? ':'
    ;


PNAME_LN 
    : PNAME_NS PN_LOCAL  
    ;

BLANK_NODE_LABEL
    : '_:' PN_LOCAL
    ;

LANGTAG
    : '@' ('a'..'z'|'A'..'Z')+ ('-' ('a'..'z'|'A'..'Z'|DIGIT)+)*    
    ;

           
fragment    
PN_CHARS_U 
    : PN_CHARS_BASE | '_'
    ;

fragment    
PN_CHARS 
    : PN_CHARS_U 
    | '-' 
    | DIGIT 
    | '\u00B7' 
    | '\u0300'..'\u036F' 
    | '\u203F'..'\u2040'
    ;    
    
fragment
PN_PREFIX 
    : PN_CHARS_BASE ( ( PN_CHARS | '.' )* PN_CHARS )?
    ;
    
fragment
PN_LOCAL 
    : ( PN_CHARS_U | DIGIT ) ( ( PN_CHARS | '.' )* PN_CHARS)?
    ;    

fragment
PN_CHARS_BASE
    : 'a'..'z' 
    | 'A'..'Z' 
    | '\u00C0'..'\u00D6' 
    | '\u00D8'..'\u00F6' 
    | '\u00F8'..'\u02FF' 
    | '\u0370'..'\u037D' 
    | '\u037F'..'\u1FFF' 
    | '\u200C'..'\u200D' 
    | '\u2070'..'\u218F' 
    | '\u2C00'..'\u2FEF' 
    | '\u3001'..'\uD7FF' 
    | '\uF900'..'\uFDCF' 
    | '\uFDF0'..'\uFFFD' 
//    | '\u10000'..'\uEFFFF'   // not supported by java
    ;    
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	










/* 2 Preliminary Definitions */

/* 2.3 Integers, Characters, Strings, Language Tags, and Node IDs */
/* @ (U+40) followed a nonempty sequence of characters matching the 
* langtag production from [BCP 47] 
*/
languageTag 
    : LANGTAG
    ;
    /* See LANGTAG in [SPARQL] */ 
/* a finite sequence of characters matching the BLANK_NODE_LABEL 
* production of [SPARQL]
*/
nodeId 
    :  BLANK_NODE_LABEL
    ;

/* 2.3 IRIs */
/* an iri as defined in [RFC3987], enclosed in a pair of < (U+3C) and > 
* (U+3E) characters
*/
fullIri
    : IRI_REF
    ; 
    /* See IRI_REF in [SPARQL] */
iri
    : fullIri
    | abbreviatedIri
    ;

/* a finite sequence of characters matching the PNAME_LN production of 
* [SPARQL] 
*/
abbreviatedIri
    : PNAME_LN
    ;

/* a finite sequence of characters matching the as PNAME_NS production of 
* [SPARQL] 
*/
prefixName 
    : PNAME_NS
    ;
	

	
	
	
/* 3 Ontologies */

/* 3.5 Ontology Annotations */
ontologyAnnotations 
    : annotation*
    ;

/* 3.7 Functional-Style Syntax */    
ontologyDocument
    : prefixDeclaration* ontology
    ;

prefixDeclaration 
    : PREFIX OPEN_BRACE prefixName EQUALS fullIri CLOSE_BRACE
    ;

ontology
    : ONTOLOGY OPEN_BRACE ( ontologyIri versionIri? )?
       directlyImportsDocuments
       ontologyAnnotations
       axioms
      CLOSE_BRACE
    ;

ontologyIri 
    : iri
    ;

versionIri 
    : iri
    ;

directlyImportsDocuments 
    : ( IMPORT OPEN_BRACE iri CLOSE_BRACE )*
    ;

axioms
	: (axiom)*  
    ;



/* 5 Entities and Literals */

/* 5.1 Classes */
class
    : iri
    ;

/* 5.2 Datatypes */    
datatype 
    : iri
    ;

/* 5.3 Object Properties */       
objectProperty
    : iri
    ;

/* 5.4 Data Properties */    
dataProperty 
    : iri
    ;

/* 5.5 Annotation Properties */    
annotationProperty 
    : iri
    ;

/* 5.6 Individuals */
individual 
    : namedIndividual 
    | anonymousIndividual
    ;

/* 5.6.1 Named Individuals */
namedIndividual 
    : iri
    ;

/* 5.6.2 Anonymous Individuals */    
anonymousIndividual 
    : nodeId
    ;

/* 5.7 Literals */
literal
    : QUOTED_STRING      
        ( REFERENCE datatype
        | languageTag?
        )
    ;

/* 5.8 Entity Declarations and Typing */    
declaration
    : DECLARATION OPEN_BRACE axiomAnnotations entity CLOSE_BRACE
    ;

entity
    : CLASS OPEN_BRACE class CLOSE_BRACE 
    | DATATYPE OPEN_BRACE datatype CLOSE_BRACE
    | OBJECT_PROPERTY OPEN_BRACE objectProperty CLOSE_BRACE 
    | DATA_PROPERTY OPEN_BRACE dataProperty CLOSE_BRACE
    | ANNOTATION_PROPERTY OPEN_BRACE annotationProperty CLOSE_BRACE
    | NAMED_INDIVIDUAL OPEN_BRACE namedIndividual CLOSE_BRACE
    ;

/* 6 Property Expressions */    
objectPropertyExpression
    : objectProperty
    | inverseObjectProperty
    ;

/* 6.1.1 Inverse Object Properties */    
inverseObjectProperty 
    : OBJECT_INVERSE_OF OPEN_BRACE objectProperty CLOSE_BRACE
    ;

/* 6.2 Data Property Expressions */    
dataPropertyExpression 
    : dataProperty
    ;

/* 7 Data Ranges */    
dataRange 
    : datatype 
    | dataIntersectionOf 
    | dataUnionOf 
    | dataComplementOf
    | dataOneOf 
    | dataRangeRestriction
    ;

/* 7.1 Intersection of Data Ranges */    
dataIntersectionOf 
    : DATA_INTERSECTION_OF OPEN_BRACE dataRange dataRange+ CLOSE_BRACE
    ;

/* 7.2 Union of Data Ranges */
dataUnionOf
    : DATA_UNION_OF OPEN_BRACE dataRange dataRange+ CLOSE_BRACE
    ;

/* 7.3 Complement of Data Ranges */    
dataComplementOf 
    : DATA_COMPLEMENT_OF OPEN_BRACE dataRange CLOSE_BRACE
    ;

/* 7.4 Enumeration of Literals */
dataOneOf 
    : DATA_ONE_OF OPEN_BRACE literal+ CLOSE_BRACE
    ;

/* 7.5 Datatype Restrictions */
dataRangeRestriction 
    : DATATYPE_RESTRICTION OPEN_BRACE 
        datatype 
        ( dataRangeFacetRestriction )+ 
      CLOSE_BRACE
    ;

dataRangeFacetRestriction 
    : iri literal
    ;


/* 8 Class Expressions */
classExpression
    : class
    | objectIntersectionOf
    | objectUnionOf 
    | objectComplementOf 
    | objectOneOf 
    | objectSomeValuesFrom
    | objectAllValuesFrom 
    | objectHasValue 
    | objectHasSelf 
    | objectMinCardinality 
    | objectMaxCardinality 
    | objectExactCardinality 
    | dataSomeValuesFrom 
    | dataAllValuesFrom 
    | dataHasValue 
    | dataMinCardinality 
    | dataMaxCardinality 
    | dataExactCardinality
    ;


/* 8.1 Propositional Connectives and Enumeration of Individuals */

/* 8.1.1 Intersection of Class Expressions */
objectIntersectionOf
    : OBJECT_INTERSECTION_OF OPEN_BRACE
    	    classExpression
    	  ( classExpression)+ 
    	CLOSE_BRACE	
    ;

/* 8.1.2 Union of Class Expressions */
objectUnionOf 
    : OBJECT_UNION_OF OPEN_BRACE classExpression classExpression+ CLOSE_BRACE
    ;

/* 8.1.3 Complement of Class Expressions */
objectComplementOf 
    : OBJECT_COMPLEMENT_OF OPEN_BRACE classExpression CLOSE_BRACE
    ;

/* 8.1.4 Enumeration of Individuals */    
objectOneOf 
    : OBJECT_ONE_OF OPEN_BRACE individual+ CLOSE_BRACE
    ;


/* 8.2 Object Property Restrictions */

/* 8.2.1 Existential Quantification */    
objectSomeValuesFrom
    : OBJECT_SOME_VALUES_FROM OPEN_BRACE 
         objectPropertyExpression 
         classExpression 
      CLOSE_BRACE
    ;

/* 8.2.2 Universal Quantification */
objectAllValuesFrom 
    : OBJECT_ALL_VALUES_FROM OPEN_BRACE 
        objectPropertyExpression 
        classExpression 
      CLOSE_BRACE
    ;

/* 8.2.3 Individual Value Restriction */
objectHasValue 
    : OBJECT_HAS_VALUE OPEN_BRACE 
        objectPropertyExpression 
        individual 
      CLOSE_BRACE
    ;

/* 8.2.4 Self-Restriction */
objectHasSelf 
    : OBJECT_HAS_SELF OPEN_BRACE 
        objectPropertyExpression 
      CLOSE_BRACE
    ;


/* 8.3 Object Property Cardinality Restrictions */

/* 8.3.1 Minimum Cardinality */
objectMinCardinality 
    : OBJECT_MIN_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        objectPropertyExpression 
        classExpression? 
      CLOSE_BRACE
    ;

/* 8.3.2 Maximum Cardinality */
objectMaxCardinality 
    : OBJECT_MAX_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        objectPropertyExpression 
        classExpression? 
      CLOSE_BRACE
    ;

/* 8.3.3 Exact Cardinality */    
objectExactCardinality 
    : OBJECT_EXACT_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        objectPropertyExpression 
        classExpression? 
      CLOSE_BRACE
    ;


/* 8.4 Data Property Restrictions */

/* 8.4.1 Existential Quantification */
dataSomeValuesFrom 
    : DATA_SOME_VALUES_FROM OPEN_BRACE 
        dataPropertyExpression+ 
        dataRange 
      CLOSE_BRACE
    ;

/* 8.4.2 Universal Quantification */
dataAllValuesFrom 
    : DATA_ALL_VALUES_FROM OPEN_BRACE 
        dataPropertyExpression+ 
        dataRange 
      CLOSE_BRACE
    ;

/* 8.4.3 Literal Value Restriction */
dataHasValue 
    : DATA_HAS_VALUE OPEN_BRACE 
        dataPropertyExpression 
        literal 
      CLOSE_BRACE
    ;


/* 8.5 Data Property Cardinality Restrictions */

/* 8.5.1 Minimum Cardinality */
dataMinCardinality 
    : DATA_MIN_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        dataPropertyExpression 
        dataRange? 
      CLOSE_BRACE
    ;

/* 8.5.2 Maximum Cardinality */
dataMaxCardinality 
    : DATA_MAX_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        dataPropertyExpression 
        dataRange? 
      CLOSE_BRACE
    ;

/* 8.5.3 Exact Cardinality */
dataExactCardinality 
    : DATA_EXACT_CARDINALITY OPEN_BRACE 
        NON_NEGATIVE_INTEGER 
        dataPropertyExpression 
        dataRange? 
      CLOSE_BRACE
    ;

/* 9 Axioms */
axiom
	  : declaration 
    | classAxiom
    | objectPropertyAxiom
    | dataPropertyAxiom 
    | datatypeDefinition 
    | hasKey 
    | assertion 
    | annotationAxiom
    ;

axiomAnnotations 
    : annotation*
    ;
 
/* 9.1 Class Expression Axioms */    
classAxiom
    : subClassOf
    | equivalentClasses
    | disjointClasses 
    | disjointUnion
    ;

/* 9.1.1 Subclass Axioms */
subClassOf
    : SUB_CLASS_OF OPEN_BRACE 
        axiomAnnotations 
        subClassExpression 
        superClassExpression 
      CLOSE_BRACE
    ;

subClassExpression
    : classExpression
    ;

superClassExpression
    : classExpression
    ;

/* 9.1.2 Equivalent Classes */
equivalentClasses
    : EQUIVALENT_CLASSES OPEN_BRACE 
        axiomAnnotations    
          classExpression
        ( classExpression )+ 
      CLOSE_BRACE  
    ;

/* 9.1.3 Disjoint Classes */
disjointClasses
    : DISJOINT_CLASSES OPEN_BRACE 
        axiomAnnotations 
        classExpression 
        classExpression+ 
      CLOSE_BRACE
    ;

/* 9.1.4 Disjoint Union of Class Expressions */
disjointUnion 
    : DISJOINT_UNION OPEN_BRACE 
        axiomAnnotations 
        class 
        disjointClassExpressions 
      CLOSE_BRACE
    ;

disjointClassExpressions 
    : classExpression classExpression+
    ;

/* 9.2 Object Property Axioms */    
objectPropertyAxiom
    : subObjectPropertyOf
    | equivalentObjectProperties 
    | disjointObjectProperties 
    | inverseObjectProperties 
    | objectPropertyDomain 
    | objectPropertyRange 
    | functionalObjectProperty 
    | inverseFunctionalObjectProperty 
    | reflexiveObjectProperty 
    | irreflexiveObjectProperty 
    | symmetricObjectProperty 
    | asymmetricObjectProperty 
    | transitiveObjectProperty
    ;

/* 9.2.1 Object Subproperties */
subObjectPropertyOf
    : SUB_OBJECT_PROPERTY_OF OPEN_BRACE
        axiomAnnotations 
        ( objectPropertyExpression
        | propertyExpressionChain
        ) objectPropertyExpression 
      CLOSE_BRACE 
    ;

propertyExpressionChain
    : OBJECT_PROPERTY_CHAIN OPEN_BRACE
          objectPropertyExpression
        ( objectPropertyExpression )+
      CLOSE_BRACE
    ;

/* 9.2.2 Equivalent Object Properties */    
equivalentObjectProperties 
    : EQUIVALENT_OBJECT_PROPERTIES OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        objectPropertyExpression+ 
      CLOSE_BRACE
    ;

/* 9.2.3 Disjoint Object Properties */
disjointObjectProperties 
    : DISJOINT_OBJECT_PROPERTIES OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        objectPropertyExpression+ 
      CLOSE_BRACE
    ;

/* 9.2.4 Inverse Object Properties */
inverseObjectProperties 
    : INVERSE_OBJECT_PROPERTIES OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.5 Object Property Domain */
objectPropertyDomain 
    : OBJECT_PROPERTY_DOMAIN OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        classExpression 
       CLOSE_BRACE
    ;

/* 9.2.6 Object Property Range */        
objectPropertyRange 
    : OBJECT_PROPERTY_RANGE OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        classExpression 
      CLOSE_BRACE
    ;

/* 9.2.7 Functional Object Properties */
functionalObjectProperty 
    : FUNCTIONAL_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.8 Inverse-Functional Object Properties */    
inverseFunctionalObjectProperty 
    : INVERSE_FUNCTIONAL_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.9 Reflexive Object Properties */
reflexiveObjectProperty 
    : REFLEXIVE_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.10 Irreflexive Object Properties */
irreflexiveObjectProperty 
    : IRREFLEXIVE_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.11 Symmetric Object Properties */
symmetricObjectProperty 
    : SYMMETRIC_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.2.12 Asymmetric Object Properties */
asymmetricObjectProperty 
    : ASYMMETRIC_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
       CLOSE_BRACE
    ;

/* 9.2.13 Transitive Object Properties */
transitiveObjectProperty
    : TRANSITIVE_OBJECT_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression
      CLOSE_BRACE
    ;

/* 9.3 Data Property Axioms */    
dataPropertyAxiom 
    : subDataPropertyOf 
    | equivalentDataProperties 
    | disjointDataProperties 
    | dataPropertyDomain 
    | dataPropertyRange 
    | functionalDataProperty
    ;

/* 9.3.1 Data Subproperties */
subDataPropertyOf 
    : SUB_DATA_PROPERTY_OF OPEN_BRACE 
        axiomAnnotations 
        subDataPropertyExpression 
        superDataPropertyExpression 
      CLOSE_BRACE
    ;
  
subDataPropertyExpression 
    : dataPropertyExpression
    ;
superDataPropertyExpression 
    : dataPropertyExpression
    ;

/* 9.3.2 Equivalent Data Properties */
equivalentDataProperties 
    : EQUIVALENT_DATA_PROPERTIES OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        dataPropertyExpression+ 
      CLOSE_BRACE
    ;

/* 9.3.3 Disjoint Data Properties */
disjointDataProperties 
    : DISJOINT_DATA_PROPERTIES OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        dataPropertyExpression+ 
      CLOSE_BRACE
    ;

/* 9.3.4 Data Property Domain */
dataPropertyDomain 
    : DATA_PROPERTY_DOMAIN OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        classExpression 
      CLOSE_BRACE
    ;

/* 9.3.5 Data Property Range */
dataPropertyRange 
    : DATA_PROPERTY_RANGE OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        dataRange 
      CLOSE_BRACE
    ;

/* 9.3.6 Functional Data Properties */
functionalDataProperty 
    : FUNCTIONAL_DATA_PROPERTY OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
      CLOSE_BRACE
    ;

/* 9.4 Datatype Definitions */
datatypeDefinition 
    : DATATYPE_DEFINITION OPEN_BRACE 
        axiomAnnotations 
        datatype 
        dataRange 
      CLOSE_BRACE
    ;

/* 9.5 Keys */
hasKey 
    : HAS_KEY OPEN_BRACE 
        axiomAnnotations 
        classExpression 
        OPEN_BRACE objectPropertyExpression* CLOSE_BRACE 
        OPEN_BRACE dataPropertyExpression* CLOSE_BRACE 
      CLOSE_BRACE
    ;

/* 9.6 Assertions */    
assertion 
    : sameIndividual 
    | differentIndividuals 
    | classAssertion 
    | objectPropertyAssertion 
    | negativeObjectPropertyAssertion 
    | dataPropertyAssertion 
    | negativeDataPropertyAssertion
    ;

sourceIndividual 
    : individual
    ;
targetIndividual 
    : individual
    ;
targetValue 
    : literal
    ;

/* 9.6.1 Individual Equality */
sameIndividual 
    : SAME_INDIVIDUAL OPEN_BRACE 
        axiomAnnotations 
        individual 
        individual+ 
      CLOSE_BRACE
    ;

/* 9.6.2 Individual Inequality */
differentIndividuals 
    : DIFFERENT_INDIVIDUALS OPEN_BRACE 
        axiomAnnotations 
        individual 
        individual+ 
      CLOSE_BRACE
    ;

/* 9.6.3 Class Assertions */
classAssertion 
    : CLASS_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        classExpression 
        individual 
      CLOSE_BRACE
    ;

/* 9.6.4 Positive Object Property Assertions */
objectPropertyAssertion 
    : OBJECT_PROPERTY_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        sourceIndividual 
        targetIndividual 
      CLOSE_BRACE
    ;

/* 9.6.5 Negative Object Property Assertions */
negativeObjectPropertyAssertion 
    : NEGATIVE_OBJECT_PROPERTY_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        objectPropertyExpression 
        sourceIndividual 
        targetIndividual 
      CLOSE_BRACE
    ;

/* 9.6.6 Positive Data Property Assertions */
dataPropertyAssertion 
    : DATA_PROPERTY_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        sourceIndividual 
        targetValue 
      CLOSE_BRACE
    ;

/* 9.6.7 Negative Data Property Assertions */
negativeDataPropertyAssertion 
    : NEGATIVE_DATA_PROPERTY_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        dataPropertyExpression 
        sourceIndividual 
        targetValue 
      CLOSE_BRACE
    ;


/* 10 Annotations */

/* 10.1 Annotations of Ontologies, Axioms, and other Annotations */
annotation 
    : ANNOTATION OPEN_BRACE 
        annotationAnnotations 
        annotationProperty 
        annotationValue 
      CLOSE_BRACE
    ;

annotationAnnotations  
    : annotation*
    ;

annotationValue 
    : anonymousIndividual | iri | literal
    ;

/* 10.2 Annotation Axioms */    
annotationAxiom 
    : annotationAssertion 
    | subAnnotationPropertyOf 
    | annotationPropertyDomain 
    | annotationPropertyRange
    ;

/* 10.2.1 Annotation Assertion */
annotationAssertion 
    : ANNOTATION_ASSERTION OPEN_BRACE 
        axiomAnnotations 
        annotationProperty 
        annotationSubject 
        annotationValue 
      CLOSE_BRACE
    ;

annotationSubject 
    : iri | anonymousIndividual
    ;

/* 10.2.2 Annotation Subproperties */
subAnnotationPropertyOf 
    : SUB_ANNOTATION_PROPERTY_OF OPEN_BRACE 
        axiomAnnotations 
        subAnnotationProperty 
        superAnnotationProperty 
      CLOSE_BRACE
    ;

subAnnotationProperty 
    : annotationProperty
    ;

superAnnotationProperty 
    : annotationProperty
    ;

/* 10.2.3 Annotation Property Domain */
annotationPropertyDomain 
    : ANNOTATION_PROPERTY_DOMAIN OPEN_BRACE 
        axiomAnnotations 
        annotationProperty 
        iri 
      CLOSE_BRACE
    ;

/* 10.2.4 Annotation Property Range */
annotationPropertyRange 
    : ANNOTATION_PROPERTY_RANGE OPEN_BRACE 
        axiomAnnotations 
        annotationProperty 
        iri 
      CLOSE_BRACE
    ;
	

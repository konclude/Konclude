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

#include "CConceptTextFormater.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConceptTextFormater::CConceptTextFormater() {
			}


			CConceptTextFormater::~CConceptTextFormater() {
			}




			QString CConceptTextFormater::getConceptString(CConcept *concept, bool negated, bool useTag, qint64 defSkipCount) {

				CConcept *con = concept;
				QString className;

				if (concept->hasClassName()) {
					defSkipCount--;
				}
				
				if (negated) {
					className += QChar(0x00AC);
				}
				if (defSkipCount <= 0 || useTag || concept->getDefinitionOperatorTag() == CCNONE) {
					className += QString::number(con->getConceptTag());
					QString iriClassNameString = CIRIName::getRecentIRIName(con->getClassNameLinker());
					if (!iriClassNameString.isEmpty()) {
						className += "~";
						if (iriClassNameString == "http://www.w3.org/2002/07/owl#Thing") {
							className += QChar(0x22A4);
						} else {
							className += iriClassNameString;
						}
					} 
				}


				if (!concept->hasClassName() || defSkipCount > 0) {
					if (!className.isEmpty()) {
						className += " := ";
					}

					QString conText;
					qint64 opCode = con->getProcessingOperatorTag();
					CConceptOperator* conOperator = con->getConceptOperator();
					if (negated) {
						conText += QChar(0x00AC);
					}
					conText += " (";
					if (opCode == CCATOM) {
						conText += QString::number(con->getConceptTag())+QString("~ATOM");
					}
					if (opCode == CCNOMINAL) {
						conText += QString::number(con->getConceptTag())+QString("~{%1~%2}").arg(con->getNominalIndividual()->getIndividualID()).arg(CIRIName::getRecentIRIName(con->getNominalIndividual()->getIndividualNameLinker()));
					}
					if (opCode == CCVALUE) {
						conText += QString(" VALUE ");
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						conText += QString::number(con->getConceptTag())+QString("~{%1~%2}").arg(con->getNominalIndividual()->getIndividualID()).arg(CIRIName::getRecentIRIName(con->getNominalIndividual()->getIndividualNameLinker()));
					}
					if (opCode == CCEQCAND) {
						conText += QString::number(con->getConceptTag())+QString("~EQCONCAND");
					}
					if (opCode == CCDATATYPE) {
						conText += QString::number(con->getConceptTag())+QString("~DATATYPE");
						CDatatype* datatype = con->getDatatype();
						if (datatype) {
							conText += QString("-%1").arg(datatype->getDatatypeIRI());
						}
					}
					if (opCode == CCDATARESTRICTION) {
						conText += QString::number(con->getConceptTag())+QString("~DATARESTRICTION");
						cint64 restrictionCode = con->getParameter();
						QString restrictionString;
						if (restrictionCode == CDFC_RATIONAL) {
							restrictionString = "rationals-only";
						} else if (restrictionCode == CDFC_DECIMAL) {
							restrictionString = "decimals-only";
						} else if (restrictionCode == CDFC_INTEGER) {
							restrictionString = "integers-only";
						} else if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
							restrictionString = "min-exclusive";
						} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
							restrictionString = "min-inclusive";
						} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
							restrictionString = "max-inclusive";
						} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
							restrictionString = "max-exclusive";
						}
						conText += QString("-%1").arg(restrictionString);
						CDataLiteral* dataLiteral = con->getDataLiteral();
						if (dataLiteral) {
							conText += QString(":-{%1}").arg(dataLiteral->getLexicalDataLiteralValueString());
						}
					}
					if (opCode == CCDATALITERAL) {
						conText += QString::number(con->getConceptTag())+QString("~DATALITERAL");
						CDataLiteral* dataLit = con->getDataLiteral();
						if (dataLit) {
							conText += QString("-{%1}").arg(dataLit->getLexicalDataLiteralValueString());
						}
					}
					if (opCode == CCIMPLTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~IMPLTRIG");
					}
					if (opCode == CCBRANCHTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~BRANCHTRIGGER");
					}
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AND_AQAND_TYPE)) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						while (opLinker) {
							if (opInserted) {
								conText += QString(" AND ");
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();						
						}
					}

					if (opCode == CCPBINDTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~PBINDTRIG");
					}
					if (opCode == CCVARBINDTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~VARBINDTRIG");
					}
					if (opCode == CCVARPBACKTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~VARPBACKTRIG");
					}
					if (opCode == CCBACKACTIVTRIG) {
						conText += QString::number(con->getConceptTag())+QString("~BACKACTIVTRIG");
					}
					if (opCode == CCAQCHOOCE) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						while (opLinker) {
							if (!opLinker->isNegated()) {
								conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							}
							opLinker = opLinker->getNext();						
						}
					}
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE)) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();

						QString impConText = getConceptString(opLinker->getData(),false,false,defSkipCount);

						opLinker = opLinker->getNext();
						while (opLinker) {
							if (opInserted) {
								conText += QString(", ");
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();						
						}
						conText += QString(" ---->>> ")+impConText;
					}
					if (opCode == CCPBINDCYCLE) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();

						QString impConText = getConceptString(opLinker->getData(),false,false,defSkipCount);

						opLinker = opLinker->getNext();
						while (opLinker) {
							if (opInserted) {
								conText += QString(", ");
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();						
						}
						conText += QString(" @---->>> ")+impConText;
					}
					if (opCode == CCVARBINDJOIN) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();

						QString impConText = getConceptString(opLinker->getData(),false,false,defSkipCount);

						opLinker = opLinker->getNext();
						while (opLinker) {
							if (opInserted) {
								conText += QString(", ");
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();						
						}
						conText += QString(" |><|---->>> ")+impConText;
					}
					if (opCode == CCOR) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						while (opLinker) {
							if (opInserted) {
								conText += QString(" OR ");
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}	
					if (opCode == CCSOME || opCode == CCAQSOME) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" SOME ");
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}
					if (opCode == CCPBINDVARIABLE) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" PBINDVARIABLE ");
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}
					if (opCode == CCVARBINDVARIABLE) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" VARBINDVARIABLE ");
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						conText += QString(" All ");
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						conText += QString::number(con->getOperandList()->getData()->getConceptTag());
					}

					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE)) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" ALL ");
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}
					if (opCode == CCATLEAST) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" ATLEAST ");
						conText += QString(" [%1]").arg(con->getParameter());
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}
					if (opCode == CCPBINDGROUND || opCode == CCVARBINDGROUND) {
						conText += QString(" GROUND ");
						conText += QString(" [%1]").arg(con->getParameter());
					}
					if (opCode == CCATMOST) {
						bool opInserted = false;
						CSortedNegLinker<CConcept *> *opLinker = con->getOperandList();
						conText += QString(" ATMOST ");
						conText += QString(" [%1]").arg(con->getParameter());
						if (con->getRole()->hasPropertyName()) {
							conText += QString(" %1. ").arg(CIRIName::getRecentIRIName(con->getRole()->getPropertyNameLinker()));
						} else {
							conText += QString(" %1. ").arg(con->getRole()->getRoleTag());
						}
						while (opLinker) {
							if (opInserted) {
								conText += ", ";
							}
							if (opLinker->isNegated()) {
								conText += QChar(0x00AC);
							}
							opInserted = true;
							//conText += QString::number(opLinker->getData()->getConceptTag());
							conText += getConceptString(opLinker->getData(),false,false,defSkipCount);
							opLinker = opLinker->getNext();
						}
					}	

					conText += ")  ";
					className += conText;
				}
				return className;
			}







		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

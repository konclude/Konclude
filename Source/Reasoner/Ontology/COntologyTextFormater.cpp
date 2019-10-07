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

#include "COntologyTextFormater.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyTextFormater::COntologyTextFormater() {
			}


			COntologyTextFormater::~COntologyTextFormater() {
			}



			QString COntologyTextFormater::getConceptString(CConcept *concept, CConcreteOntology* ontology) {
				QString conceptNameString;
				if (concept->hasClassName()) {
					conceptNameString = QString(",%1:").arg(CIRIName::getRecentIRIName(concept->getClassNameLinker()));
				}
				QString operandListString;
				CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
				while (opLinkerIt) {
					if (!operandListString.isEmpty()) {
						operandListString += QString(" | ");
					}
					if (opLinkerIt->getData()) {
						operandListString += QString("%1%2").arg((opLinkerIt->isNegated()) ? "-" : "+").arg(opLinkerIt->getData()->getConceptTag());
					} else {
						operandListString += QString("missing operand");
					}
					opLinkerIt = opLinkerIt->getNext();
				}

				QString addOperandListString;
				if (!operandListString.isEmpty()) {
					addOperandListString = QString(" (| %1 |)").arg(operandListString);
				}

				QString roleString;
				CRole* role = concept->getRole();
				if (role) {
					roleString = QString("%1").arg(role->getRoleTag());
					if (role->hasPropertyName()) {
						roleString += QString("~%1").arg(CIRIName::getRecentIRIName(role->getPropertyNameLinker()));
					}
				}
				QString addRoleString;
				if (!roleString.isEmpty()) {
					addRoleString = QString(" < %1 >").arg(roleString);
				}

				QString indiString;
				CIndividual* indi = concept->getNominalIndividual();
				if (indi) {
					indiString = QString("%1").arg(indi->getIndividualID());
					if (indi->hasIndividualName()) {
						indiString += QString("~%1").arg(CIRIName::getRecentIRIName(indi->getIndividualNameLinker()));
					}
				}
				QString addIndiString;
				if (!indiString.isEmpty()) {
					addIndiString = QString(" { %1 }").arg(indiString);
				}

				QString addParamString;
				cint64 param = concept->getParameter();
				if (param != 0) {
					addParamString = QString(" [ %1 ]").arg(param);
				}

				QString addReplacementString;
				CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
				if (conProData) {
					CReplacementData* repData = conProData->getReplacementData();
					if (repData) {
						QString addCommDisConString;
						for (CXNegLinker<CConcept*>* commDisConLinker = repData->getCommonDisjunctConceptLinker(); commDisConLinker; commDisConLinker = commDisConLinker->getNext()) {
							CConcept* con = commDisConLinker->getData();
							bool neg = commDisConLinker->getNegation();
							if (!addCommDisConString.isEmpty()) {
								addCommDisConString += QString(", ");
							}
							if (neg) {
								addCommDisConString += QString("-%1").arg(con->getConceptTag());
							} else {
								addCommDisConString += QString("%1").arg(con->getConceptTag());
							}
						}

						if (!addCommDisConString.isEmpty()) {
							addReplacementString += QString("Common Disjunction Concepts: %1\r\n").arg(addCommDisConString);
						}
					}

					CConceptRoleBranchingTrigger* conRoleBranchTriggers = conProData->getConceptRoleBranchTrigger();
					if (conRoleBranchTriggers) {
						QString addTriggerDisConString;
						for (CConceptRoleBranchingTrigger* conRoleBranchTriggersIt = conRoleBranchTriggers; conRoleBranchTriggersIt; conRoleBranchTriggersIt = conRoleBranchTriggersIt->getNext()) {
							if (conRoleBranchTriggersIt->isConceptTrigger()) {
								CConcept* con = conRoleBranchTriggersIt->getTriggerConcept();
								bool neg = conRoleBranchTriggersIt->getTriggerNegation();
								if (!addTriggerDisConString.isEmpty()) {
									addTriggerDisConString += QString(", ");
								}
								if (neg) {
									addTriggerDisConString += QString("Concept -%1").arg(con->getConceptTag());
								} else {
									addTriggerDisConString += QString("Concept %1").arg(con->getConceptTag());
								}
							} else if (conRoleBranchTriggersIt->isRoleTrigger()) {
								CRole* role = conRoleBranchTriggersIt->getTriggerRole();
								if (!addTriggerDisConString.isEmpty()) {
									addTriggerDisConString += QString(", ");
								}
								addTriggerDisConString += QString("Role %1").arg(role->getRoleTag());
							}
						}

						if (!addTriggerDisConString.isEmpty()) {
							addReplacementString += QString("Role-Concept-Triggers: %1\r\n").arg(addTriggerDisConString);
						}
					}

				}
				if (concept->getVariable()) {
					addReplacementString += QString("Referenced Variable: %1\r\n").arg(concept->getVariable()->getPathVariableID());
				}
				if (concept->getVariableLinker()) {
					QString variables;
					for (CSortedLinker<CVariable*>* varLinkIt = concept->getVariableLinker(); varLinkIt; varLinkIt = varLinkIt->getNext()) {
						if (!variables.isEmpty()) {
							variables.append(", ");
						}
						variables.append(QString::number(varLinkIt->getData()->getPathVariableID()));
					}
					addReplacementString += QString("Referenced Variables: %1\r\n").arg(variables);
				}


				cint64 opCode = concept->getOperatorCode();
				QString opCodeString("CCNONE");
				if (opCode == CCTOP) {
					opCodeString = QString("CCTOP");
				} else if (opCode == CCBOTTOM) {
					opCodeString = QString("CCBOTTOM");
				} else if (opCode == CCNOT) {
					opCodeString = QString("CCNOT");
				} else if (opCode == CCAND) {
					opCodeString = QString("CCAND");
				} else if (opCode == CCOR) {
					opCodeString = QString("CCOR");
				} else if (opCode == CCSOME) {
					opCodeString = QString("CCSOME");
				} else if (opCode == CCALL) {
					opCodeString = QString("CCALL");
				} else if (opCode == CCATMOST) {
					opCodeString = QString("CCATMOST");
				} else if (opCode == CCATLEAST) {
					opCodeString = QString("CCATLEAST");
				} else if (opCode == CCSELF) {
					opCodeString = QString("CCSELF");
				} else if (opCode == CCNOMINAL) {
					opCodeString = QString("CCNOMINAL");
				} else if (opCode == CCEQ) {
					opCodeString = QString("CCEQ");
				} else if (opCode == CCSUB) {
					opCodeString = QString("CCSUB");
				} else if (opCode == CCAQCHOOCE) {
					opCodeString = QString("CCAQCHOOCE");
				} else if (opCode == CCAQALL) {
					opCodeString = QString("CCAQALL");
				} else if (opCode == CCIMPLAQALL) {
					opCodeString = QString("CCIMPLAQALL");
				} else if (opCode == CCBRANCHAQALL) {
					opCodeString = QString("CCBRANCHAQALL");
				} else if (opCode == CCAQSOME) {
					opCodeString = QString("CCAQSOME");
				} else if (opCode == CCAQAND) {
					opCodeString = QString("CCAQAND");
				} else if (opCode == CCIMPLAQAND) {
					opCodeString = QString("CCIMPLAQAND");
				} else if (opCode == CCBRANCHAQAND) {
					opCodeString = QString("CCBRANCHAQAND");
				} else if (opCode == CCVALUE) {
					opCodeString = QString("CCVALUE");
				} else if (opCode == CCIMPL) {
					opCodeString = QString("CCIMPL");
				} else if (opCode == CCIMPLTRIG) {
					opCodeString = QString("CCIMPLTRIG");
				} else if (opCode == CCIMPLALL) {
					opCodeString = QString("CCIMPLALL");
				} else if (opCode == CCBRANCHALL) {
					opCodeString = QString("CCBRANCHALL");
				} else if (opCode == CCEQCAND) {
					opCodeString = QString("CCEQCAND");
				} else if (opCode == CCBRANCHTRIG) {
					opCodeString = QString("CCBRANCHTRIG");
				} else if (opCode == CCBRANCHIMPL) {
					opCodeString = QString("CCBRANCHIMPL");
				} else if (opCode == CCPBINDTRIG) {
					opCodeString = QString("CCPBINDTRIG");
				} else if (opCode == CCPBINDIMPL) {
					opCodeString = QString("CCPBINDIMPL");
				} else if (opCode == CCPBINDGROUND) {
					opCodeString = QString("CCPBINDGROUND");
				} else if (opCode == CCPBINDALL) {
					opCodeString = QString("CCPBINDALL");
				} else if (opCode == CCPBINDAND) {
					opCodeString = QString("CCPBINDAND");
				} else if (opCode == CCPBINDAQAND) {
					opCodeString = QString("CCPBINDAQAND");
				} else if (opCode == CCPBINDAQALL) {
					opCodeString = QString("CCPBINDAQALL");
				} else if (opCode == CCPBINDVARIABLE) {
					opCodeString = QString("CCPBINDVARIABLE");
				} else if (opCode == CCPBINDCYCLE) {
					opCodeString = QString("CCPBINDCYCLE");
				} else if (opCode == CCNOMTEMPLREF) {
					opCodeString = QString("CCNOMTEMPLREF");
				} else if (opCode == CCNOMVAR) {
					opCodeString = QString("CCNOMVAR");
				} else if (opCode == CCVARBINDTRIG) {
					opCodeString = QString("CCVARBINDTRIG");
				} else if (opCode == CCVARBINDIMPL) {
					opCodeString = QString("CCVARBINDIMPL");
				} else if (opCode == CCVARBINDGROUND) {
					opCodeString = QString("CCVARBINDGROUND");
				} else if (opCode == CCVARBINDALL) {
					opCodeString = QString("CCVARBINDALL");
				} else if (opCode == CCVARBINDAND) {
					opCodeString = QString("CCVARBINDAND");
				} else if (opCode == CCVARBINDAQAND) {
					opCodeString = QString("CCVARBINDAQAND");
				} else if (opCode == CCVARBINDAQALL) {
					opCodeString = QString("CCVARBINDAQALL");
				} else if (opCode == CCVARBINDVARIABLE) {
					opCodeString = QString("CCVARBINDVARIABLE");
				} else if (opCode == CCVARBINDJOIN) {
					opCodeString = QString("CCVARBINDJOIN");
				} else if (opCode == CCVARPBACKTRIG) {
					opCodeString = QString("CCVARPBACKTRIG");
				} else if (opCode == CCVARPBACKALL) {
					opCodeString = QString("CCVARPBACKALL");
				} else if (opCode == CCVARPBACKAQAND) {
					opCodeString = QString("CCVARPBACKAQAND");
				} else if (opCode == CCVARPBACKAQALL) {
					opCodeString = QString("CCVARPBACKAQALL");
				} else if (opCode == CCBACKACTIVTRIG) {
					opCodeString = QString("CCBACKACTIVTRIG");
				} else if (opCode == CCBACKACTIVIMPL) {
					opCodeString = QString("CCBACKACTIVIMPL");
				} else if (opCode == CCDATATYPE) {
					opCodeString = QString("CCDATATYPE");
				} else if (opCode == CCDATALITERAL) {
					opCodeString = QString("CCDATALITERAL");
				} else if (opCode == CCDATARESTRICTION) {
					opCodeString = QString("CCDATARESTRICTION");
				}
				QString addOpCodeString = QString(" %1").arg(opCodeString);

				QString conceptString = QString("%1:%2%3%4%5%6\r\n%7\r\n%8").arg(concept->getConceptTag()).arg(conceptNameString)
					.arg(addOpCodeString).arg(addRoleString).arg(addOperandListString).arg(addIndiString).arg(CConceptTextFormater::getConceptString(concept,false)).arg(addReplacementString);
				return conceptString;
			}




			QString COntologyTextFormater::getTBoxConceptsString(CConcreteOntology* ontology) {
				QStringList tboxStringList;

				CTBox* tbox = ontology->getTBox();
				CConceptVector* conceptVector = tbox->getConceptVector();
				cint64 conceptCount = conceptVector->getItemCount();
				for (cint64 i = 0; i < conceptCount; ++i) {
					CConcept* concept = conceptVector->getData(i);
					if (concept) {
						QString conceptString(getConceptString(concept,ontology));
						tboxStringList.append(conceptString);
					}
				}

				QString tboxConceptsString = tboxStringList.join("\r\n");
				return tboxConceptsString;
			}


			QString COntologyTextFormater::getRoleString(CRole* role, CConcreteOntology* ontology) {
				QString roleNameString;
				if (role->hasPropertyName()) {
					roleNameString = QString("%1:,%2:").arg(role->getRoleTag()).arg(CIRIName::getRecentIRIName(role->getPropertyNameLinker()));
				} else {
					roleNameString = QString("%1:").arg(role->getRoleTag());
				}
				QString posSuperRoleString;
				QString negSuperRoleString;
				QString inverseRoleString;
				QString domainConRoleString;
				QString rangeConRoleString;
				CSortedNegLinker<CRole*>* superRoleLinker = role->getIndirectSuperRoleList();
				CSortedNegLinker<CRole*>* superRoleLinkerIt = superRoleLinker;
				while (superRoleLinkerIt) {
					QString superRoleString;
					CRole* superRole = superRoleLinkerIt->getData();
					superRoleString = QString("%1").arg(superRole->getRoleTag());
					if (superRole->hasPropertyName()) {
						superRoleString += QString("~%1").arg(CIRIName::getRecentIRIName(superRole->getPropertyNameLinker()));
					}
					if (!superRoleLinkerIt->isNegated()) {
						posSuperRoleString += QString("+%1 ").arg(superRoleString);
					} else {
						negSuperRoleString += QString("-%1 ").arg(superRoleString);
						CSortedNegLinker<CRole*>* superRoleSuperRoleLinkerIt = superRole->getIndirectSuperRoleList();
						while (superRoleSuperRoleLinkerIt) {
							if (superRoleSuperRoleLinkerIt->isNegated() && superRoleSuperRoleLinkerIt->getData() == role) {
								inverseRoleString += QString("-%1 ").arg(superRoleString);
								break;
							}
							superRoleSuperRoleLinkerIt = superRoleSuperRoleLinkerIt->getNext();
						}
					}

					superRoleLinkerIt = superRoleLinkerIt->getNext();
				}

				for (CSortedNegLinker<CConcept*>* domConLinker = role->getDomainConceptList(); domConLinker; domConLinker = domConLinker->getNext()) {
					CConcept* domConcept = domConLinker->getData();
					bool domNegation = domConLinker->isNegated();
					if (!domainConRoleString.isEmpty()) {
						domainConRoleString += QString(", ");
					}
					domainConRoleString += QString("%1%2").arg(domNegation?"-":"+").arg(domConcept->getConceptTag());
				}
				for (CSortedNegLinker<CConcept*>* rangConLinker = role->getRangeConceptList(); rangConLinker; rangConLinker = rangConLinker->getNext()) {
					CConcept* rangConcept = rangConLinker->getData();
					bool rangNegation = rangConLinker->isNegated();
					if (!rangeConRoleString.isEmpty()) {
						rangeConRoleString += QString(", ");
					}
					rangeConRoleString += QString("%1%2").arg(rangNegation?"-":"+").arg(rangConcept->getConceptTag());
				}

				QString roleString = QString("%1\r\nsuper roles: %2\r\ninverse super roles: %3\r\ninverse roles: %4\r\ndomain concepts: %5\r\nrange concepts: %6\r\n").arg(roleNameString).arg(posSuperRoleString).arg(negSuperRoleString).arg(inverseRoleString).arg(domainConRoleString).arg(rangeConRoleString);
				return roleString;
			}

			QString COntologyTextFormater::getRBoxRolesString(CConcreteOntology* ontology) {
				QStringList rboxStringList;

				CRBox* rbox = ontology->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector();
				cint64 roleCount = roleVector->getItemCount();
				for (cint64 i = 0; i < roleCount; ++i) {
					CRole* role = roleVector->getData(i);
					if (role) {
						QString roleString(getRoleString(role,ontology));
						rboxStringList.append(roleString);
					}
				}

				QString rboxConceptsString = rboxStringList.join("\r\n");
				return rboxConceptsString;
			}







			QString COntologyTextFormater::getIndividualString(CIndividual* indi, CConcreteOntology* ontology) {
				QString indiNameString;
				if (indi->hasIndividualName()) {
					indiNameString = QString("%1:,%2:").arg(indi->getIndividualID()).arg(CIRIName::getRecentIRIName(indi->getIndividualNameLinker()));
				} else {
					indiNameString = QString("%1:").arg(indi->getIndividualID());
				}
				QString conceptAssertionString;
				QString roleAssertionString;
				CRoleAssertionLinker* roleAssertionLinker = indi->getAssertionRoleLinker();
				while (roleAssertionLinker) {
					QString roleAssString;
					CRole* role = roleAssertionLinker->getRole();
					QString roleString;
					if (role) {
						roleString = QString("%1").arg(role->getRoleTag());
						if (role->hasPropertyName()) {
							roleString += QString("~%1").arg(CIRIName::getRecentIRIName(role->getPropertyNameLinker()));
						}
					}
					QString addRoleString;
					if (!roleString.isEmpty()) {
						addRoleString = QString("< %1 >").arg(roleString);
					}
					CIndividual* otherIndi = roleAssertionLinker->getIndividual();
					roleAssString += QString("ROLE-ASSERTION %1. %2").arg(addRoleString).arg(otherIndi->getIndividualID());
					if (otherIndi->hasIndividualName()) {
						roleAssString += QString("~%1").arg(CIRIName::getRecentIRIName(otherIndi->getIndividualNameLinker()));
					}
					if (!roleAssertionString.isEmpty()) {
						roleAssertionString += QString(", ");
					}
					roleAssertionString += QString("%1").arg(roleAssString);
					roleAssertionLinker = roleAssertionLinker->getNext();
				}

				for (CConceptAssertionLinker* assConLinker = indi->getAssertionConceptLinker(); assConLinker; assConLinker = assConLinker->getNext()) {
					CConcept* concept = assConLinker->getData();
					bool negation = assConLinker->isNegated();
					if (!conceptAssertionString.isEmpty()) {
						conceptAssertionString += QString(", ");
					}
					conceptAssertionString += QString("%1%2").arg(negation?"-":"+").arg(concept->getConceptTag());
				}

				QString indiString = QString("%1\r\nasserted concepts: %2\r\nasserted role connections: %3\r\n").arg(indiNameString).arg(conceptAssertionString).arg(roleAssertionString);
				return indiString;
			}

			QString COntologyTextFormater::getABoxIndividualsString(CConcreteOntology* ontology) {
				QStringList indiStringList;

				CABox* abox = ontology->getABox();
				CIndividualVector* indiVector = abox->getIndividualVector();
				cint64 indiCount = indiVector->getItemCount();
				for (cint64 i = 0; i < indiCount; ++i) {
					CIndividual* indi = indiVector->getData(i);
					if (indi) {
						QString indiString(getIndividualString(indi,ontology));
						indiStringList.append(indiString);
					}
				}

				QString aboxConceptsString = indiStringList.join("\r\n");
				return aboxConceptsString;
			}









			QString COntologyTextFormater::getOntologyString(CConcreteOntology* ontology) {
				QString tBoxString = getTBoxConceptsString(ontology);
				QString rBoxString = getRBoxRolesString(ontology);
				QString aBoxString = getABoxIndividualsString(ontology);
				return QString("TBOX-CONCEPTS:\r\n\r\n%1\r\n\r\n\r\nRBOX-ROLES:\r\n\r\n%2\r\n\r\n\r\nABOX-INDIVIDUALS:\r\n\r\n%3\r\n\r\n\r\n").arg(tBoxString).arg(rBoxString).arg(aBoxString);
			}

			bool COntologyTextFormater::writeOntologyToFile(CConcreteOntology* ontology, const QString& filename) {
				QFile file(filename);
				if (filename.contains('/') || filename.contains('\\')) {
					int sepPos = qMax(filename.lastIndexOf('/'), filename.lastIndexOf('\\'));
					QString dirString = filename.mid(0, sepPos);
					QDir path;
					path.mkpath(dirString);
				}
				if (file.open(QIODevice::WriteOnly)) {
					QString ontoString = getOntologyString(ontology);
					file.write(ontoString.toLocal8Bit());
					file.close();
					return true;
				}
				return false;
			}





		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

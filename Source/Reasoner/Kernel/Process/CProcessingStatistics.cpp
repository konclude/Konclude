/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CProcessingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {




				CProcessingStatistics* CProcessingStatistics::mInstance = nullptr;
				cint64 CProcessingStatistics::statisticCount = 0;
				QMutex CProcessingStatistics::mInstanceMutex;

				CProcessingStatistics* registratorInstance = CProcessingStatistics::getProcessingStatistics();

				const CProcessingStatisticDescription CProcessingStatistics::statINDIVIDUALNODESWITCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-switch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKPROCESSCHANGECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-process-change-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKPROCESSPAUSECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-process-pause-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-creation-count");
				const CProcessingStatisticDescription CProcessingStatistics::statROOTTASKSATISFIABLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"root-task-satisfiable-calculated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statROOTTASKUNSATISFIABLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"root-task-unsatisfiable-calculated-count");


				const CProcessingStatisticDescription CProcessingStatistics::statCOMPLETIONGRAPHREUSECACHERETRIEVALCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"completion-graph-reuse-cache-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCOMPLETIONGRAPHREUSECACHERETRIEVALSUCCESSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"completion-graph-reuse-cache-retrieval-success-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCOMPLETIONGRAPHREUSECACHERETRIEVALFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"completion-graph-reuse-cache-retrieval-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCOMPLETIONGRAPHREUSECACHEREUSINGDETCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"completion-graph-reuse-cache-reusing-deterministic-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCOMPLETIONGRAPHREUSECACHEREUSINGNONDETCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"completion-graph-reuse-cache-reusing-nondeterministic-count");


				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALHASHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-hash-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALHASHSUCCESSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-hash-retrieval-success-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALHASHFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-hash-retrieval-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALPRECHECKCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-pretest-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALPRECHECKSUCCESSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-pretest-retrieval-success-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALPRECHECKDIRECTFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-pretest-retrieval-direct-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHERETRIEVALPRECHECKINSUFFICIENTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-pretest-retrieval-insufficient-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEUSEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-used-count");

				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGREQUSTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-request-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGREQUSTDIFFNODELEVELFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-request-different-individual-node-levels-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGREQUSTDIFFNOMINALFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-request-different-nominal-nodes-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGREQUSTATOMICCLASHFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-request-atomic-clash-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGTRYINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-trying-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGALREADYCACHEDTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-already-cached-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGALREADYCACHEDTESTPRECHECKFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-already-cached-pretest-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGALREADYCACHEDTESTHASHFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-already-cached-hash-test-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGALREADYCACHEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-already-cached-count");
				const CProcessingStatisticDescription CProcessingStatistics::statUNSATCACHEWRITINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"unsatisfiable-cache-writing-count");

				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALSUCCESSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-successful-retrieval-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALFOUNDEXPANSIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-found-expansion-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALFOUNDSATISFIABLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-found-satisfiable-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALCOMPATIBILITYTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-compatibility-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALCOMPATIBLESATCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-found-compatible-satisfiability-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALINCOMPATIBLESATCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-found-only-incompatible-satisfiability-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHERETRIEVALEXPANDEDCONCEPTSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-expanded-concepts-count");

				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGREQUESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-request-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHESATISFIABLEWRITINGREQUESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-satisfiable-request-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEEXPANSIONWRITINGREQUESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-expansion-request-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-already-cached-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDINCOMPATIBLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-already-incompatible-cached-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDSATADDEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-already-cached-satisfiable-added-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGNATURESATISFIABLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-satisfiable-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGNATUREEXPANSIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-expansion-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGSIGSATCRITICALBRANCHCONCEPTADDEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-signature-satisfiable-critical-branch-concepts-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGEXPANSIONDELAYEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-writing-expansion-delayed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATEXPCACHEWRITINGERRORCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-expander-cache-error-count");

				const CProcessingStatisticDescription CProcessingStatistics::statSATCACHEDABSORBEDDISJUNCTIONCONCEPTSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-cached-absorbed-disjunction-concepts-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATCACHEDABSORBEDMERGINGCONCEPTSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-cached-absorbed-merging-concepts-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATCACHEDABSORBEDGENERATINGCONCEPTSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"satisfiable-cached-absorbed-generating-concepts-count");


				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATURESAVINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-blocking-saving-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGREFRESHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-refresh-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGREFRESHLOSEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-refresh-lose-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGSEARCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-search-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGESTABLISHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-establish-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGADDFOLLOWINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-add-following-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGREMOVEFOLLOWINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-remove-following-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGCONCEPTEXPANSIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-concept-expansion-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-candidate-regarded-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINVALIDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-candidate-regarded-invalid-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINCOMPATIBLECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"signature-mirroring-blocking-candidate-regarded-incompatible-count");


				const CProcessingStatisticDescription CProcessingStatistics::statTASKBACKJUMPINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-backjumping-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKAWAYBACKJUMPINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-away-backjumping-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKROOTBACKJUMPINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-root-backjumping-count");

				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGSTARTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-started-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGSTEPSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-steps-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGPREVNODELEVELCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-previous-individual-node-level-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGPREVNODELEVELBRANCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-previous-individual-node-level-current-branch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGCURRNODELEVELBRANCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-current-individual-node-level-current-branch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKINGINDEPENDENTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-independent-reached-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKGNONDETERMINISTICCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-non-deterministic-initialization-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKGNONDETALREADYCLASHFAILEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-non-deterministic-already-clash-failed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKGNONDETCLOSEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"clashed-backtracking-non-deterministic-closed-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCLASHEDBACKTRACKEDDESCRIPTORSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"backtracked-clashed-descriptors-count");

				const CProcessingStatisticDescription CProcessingStatistics::statRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statAUTOMATERULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-AUTOMATE-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANDRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-AND-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSOMERULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-SOME-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statFUNCTIONALRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-FUNCTIONAL-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statATMOSTRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-ATMOST-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-OR-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSELFRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-SELF-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statATLEASTRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-ATLEAST-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statALLRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-ALL-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statNOMINALRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-NOMINAL-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVALUERULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-VALUE-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statIMPLICATIONRULEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-IMPLICATION-application-count");

				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGEINITCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-initialization-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEQUALIFYCHOOCECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-choose-qualify-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGENEWNOMINALINITCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-new-nominals-initialization-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESMERGEABLETESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-mergeable-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESMERGEABLEDISJOINTROLETESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-mergeable-disjoint-role-links-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESMERGEABLECONCEPTSETTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-mergeable-concept-sets-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESMERGEABLEDISTINCTTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-mergeable-distinct-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESMERGEABLETESTSUCCESSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-mergeable-test-success-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGECONCEPTSADDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-concepts-added-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGELINKSADDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-links-added-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEMERGEDISTINCTADDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-merging-distinct-added-count");

				const CProcessingStatisticDescription CProcessingStatistics::statIMPLICATIONTRIGGERINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"implication-triggering-count");
				const CProcessingStatisticDescription CProcessingStatistics::statIMPLICATIONEXECUTINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"implication-executing-count");

				const CProcessingStatisticDescription CProcessingStatistics::statINDINODENOMINALMERGECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-nominal-merge-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODENOMINALDISTINCTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-nominal-distinct-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODENOMINALCORRECTIDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-nodes-nominal-correct-ID-count");

				const CProcessingStatisticDescription CProcessingStatistics::statALLROLERESTRICTIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"executed-all-role-restriction-count");
				const CProcessingStatisticDescription CProcessingStatistics::statAUTOMATETRANSACTIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"executed-automate-transaction-count");
				const CProcessingStatisticDescription CProcessingStatistics::statAUTOMATESTATECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"executed-automate-state-count");
				const CProcessingStatisticDescription CProcessingStatistics::statAUTOMATEINITCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"executed-automate-initialization-count");

				const CProcessingStatisticDescription CProcessingStatistics::statSUCCESSORINDINODECREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"created-successor-individual-node-count");
				const CProcessingStatisticDescription CProcessingStatistics::statDISTINCTSUCCESSORINDINODECREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"created-distinct-successor-individual-node-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSEPARATESUCCESSORINDINODECREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"created-separate-successor-individual-node-count");
				const CProcessingStatisticDescription CProcessingStatistics::statNOMINALSUCCESSORINDINODECREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"created-nominal-successor-individual-node-count");

				const CProcessingStatisticDescription CProcessingStatistics::statTASKDISTINCTMERGEBRANCHCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-creation-distinct-merge-branch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKINDINODEMERGEBRANCHCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-creation-individual-node-merge-branch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKQUALIFYCHOOSEBRANCHCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-creation-qualify-choose-branch-count");
				const CProcessingStatisticDescription CProcessingStatistics::statTASKORBRANCHCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"task-creation-or-branch-count");

				const CProcessingStatisticDescription CProcessingStatistics::statOREXECUTIONINITCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-initialization-count");
				const CProcessingStatisticDescription CProcessingStatistics::statOREXECUTIONCONTINUECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-continuation-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORINSTANTCONTAINSEXECUTEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-instant-contains-branch-executing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORTRIGGEREDDELAYEDEXECUTEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-trigger-delayed-executing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORTRIGGERSEARCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-trigger-search-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORQUEUEDDELAYEDEXECUTEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-queued-delayed-executing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statOREXECUTIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-executing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORSINGLEBRANCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-single-branching-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORMULTIPLEBRANCHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-processing-multiple-branching-count");
				const CProcessingStatisticDescription CProcessingStatistics::statORREPLACEMENTIMPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"or-replacement-implication-count");

				const CProcessingStatisticDescription CProcessingStatistics::statCONCEPTTRIGGERINSTALLCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"install-concept-trigger-count");
				const CProcessingStatisticDescription CProcessingStatistics::statROLETRIGGERINSTALLCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"install-role-link-trigger-count");

				const CProcessingStatisticDescription CProcessingStatistics::statLABELCONCEPTSUBSETTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"concept-sub-set-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statLABELCONCEPTEQUALSETTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"concept-equal-set-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statLABELCONCEPTPAIRWISEEQUALSETTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"concept-pairwise-equal-set-test-count");

				const CProcessingStatisticDescription CProcessingStatistics::statOPTIMIZEDBLOCKINGB2AUTOMATETRANSACTIONTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"optimized-blocking-compatible-automate-transactions-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statOPTIMIZEDBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"optimized-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSUBSETBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"sub-set-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statEQUALSETBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"equal-set-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPAIRWISEEQUALSETBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"pairwise-equal-set-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statDETECTINDINODEBLOCKINGSTATUSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"detect-individual-node-blocking-status-count");
				const CProcessingStatisticDescription CProcessingStatistics::statDETECTANCINDINODEBLOCKINGSTATUSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"detect-ancestor-individual-node-blocking-status-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSUCCESSBLOCKINGSTATUSDETECTIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"blocking-status-detection-success-count");
				const CProcessingStatisticDescription CProcessingStatistics::statFAILEDBLOCKINGSTATUSDETECTIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"blocking-status-detection-failed-count");

				const CProcessingStatisticDescription CProcessingStatistics::statCONTINUEBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"continue-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANCESTORBLOCKINGSEARCHINDINODECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"ancestor-blocking-search-individual-node-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHEREBLOCKINGSEARCHINDINODECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-blocking-search-individual-node-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHEREBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-blocking-candidate-hash-search-individual-node-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHEREBLOCKINGCANDIDATEHASHUDATECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-blocking-candidate-hash-update-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-blocking-candidate-hash-update-individual-node-add-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHEREBLOCKINGCANDIDATEHASHUDATEREMOVECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-blocking-candidate-hash-update-individual-node-remove-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANYWHERECORECONCEPTBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"anywhere-core-concept-blocking-search-individual-node-test-count");

				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEEXPANSIONBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-expansion-blocking-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEPROCESSINGBLOCKINGTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-processing-blocking-test-count");

				const CProcessingStatisticDescription CProcessingStatistics::statLINKSCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-role-links-creation-count");
				const CProcessingStatisticDescription CProcessingStatistics::statDISTINCTCREATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-distinct-creation-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCONCEPTSADDEDINDINODELABELSETCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-concepts-added-set-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-concepts-added-processing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODESADDEDPROCESSINGQUEUECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-added-processing-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINSERTCONCEPTREAPPLICATIONSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-install-concept-reapplication-count");
				const CProcessingStatisticDescription CProcessingStatistics::statREAPPLIEDCONCEPTSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-concept-reapplication-count");
				const CProcessingStatisticDescription CProcessingStatistics::statCORECONCEPTSADDEDINDINODELABELSETCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-core-concepts-added-set-count");

				const CProcessingStatisticDescription CProcessingStatistics::statINDINODEUPDATELOADCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-update-load-count");
				const CProcessingStatisticDescription CProcessingStatistics::statINDINODELOCALIZEDLOADCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"individual-node-localization-count");


				const CProcessingStatisticDescription CProcessingStatistics::statPBINDVARIABLEBINDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-binded-variable-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDPROPAGATEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDPROPAGATEDFRESHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-fresh-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDPROPAGATEDINITIALCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-initial-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDGROUNDINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-grounding-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDCYCLEPROPAGATECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-cycle-propagation-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDCYCLEREAPPLYINSTALLCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-cycle-reapply-install-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULEANDAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-AND-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULEALLAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-ALL-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULEGROUNDINGAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-GROUNDING-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULECYCLEAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-CYCLE-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULEIMPLICATIONAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-IMPLICATION-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDRULEBINDNAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-binding-candidate-propagation-BINDING-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statPBINDREAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"binding-candidate-propagation-reapplication-count");




				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDVARIABLEBINDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-binded-variable-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDPROPAGATEDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDPROPAGATEDFRESHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-fresh-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDPROPAGATEDINITIALCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-initial-propagated-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEANDAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-AND-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEALLAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-ALL-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEBINDAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-BINDING-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEGROUNDINGAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-GROUNDING-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEIMPLICATIONAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-IMPLICATION-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDRULEJOINAPPLICATIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"rule-variable-binding-propagation-JOIN-application-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDJOINTRIGGERINSTALLCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-join-trigger-install-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDJOINTRIGGEREXECUTECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-join-trigger-execute-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDJOINCOMBINECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-join-combine-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDJOINCREATENEWCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-create-new-count");
				const CProcessingStatisticDescription CProcessingStatistics::statVARBINDGROUNDINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"variable-binding-propagation-grounding-count");



				const CProcessingStatisticDescription CProcessingStatistics::statTIMEBACKTRACING(registratorInstance,++CProcessingStatistics::statisticCount,"timing-clash-backtracking");
				const CProcessingStatisticDescription CProcessingStatistics::statTIMEUNSATCACHERETRIVAL(registratorInstance,++CProcessingStatistics::statisticCount,"timing-unsatisfiable-cache-retrieval");
				const CProcessingStatisticDescription CProcessingStatistics::statTIMECOMPLETIONGRAPHREUSERETRIVAL(registratorInstance,++CProcessingStatistics::statisticCount,"timing-completion-graph-reuse-cache-retrieval");



				const CProcessingStatisticDescription CProcessingStatistics::statANALYSESUBSUMERCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-subsumer-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSEPOSSIBLESUBSUMPTIONSCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-possible-subsumer-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSEPSEUDOMODELCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-create-pseudo-model-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVETESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-alternatives-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEMERGINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-alternatives-merging-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDCONCEPTSETMERGINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-concept-set-merging-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSORMERGINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-role-successor-merging-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEFOUNDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-alternatives-found-count");
				const CProcessingStatisticDescription CProcessingStatistics::statANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSOREXTENDEDMERGINGCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"analyse-candidate-concepts-saturated-role-successor-extended-merging-count");



				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCACHEESTABLISHCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-caching-establish-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCACHECONCEPTEXPANSIONCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-caching-concept-expansion-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCACHELOSECOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-caching-lose-count");


				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCRITICALTESTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-critical-concept-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCRITICALALLCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-critical-ALL-concept-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCRITICALORCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-critical-OR-concept-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCRITICALATMOSTCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-critical-ATMOST-concept-test-count");
				const CProcessingStatisticDescription CProcessingStatistics::statSATURATIONCRITICALADDCOUNT(registratorInstance,++CProcessingStatistics::statisticCount,"saturation-critical-add-count");




				CProcessingStatistics::CProcessingStatistics() {
				}


				CProcessingStatistics::~CProcessingStatistics() {
				}


				CProcessingStatisticRegistrator* CProcessingStatistics::registerProcessingStatisticDescription(CProcessingStatisticDescription* statDesc) {
					mStatDescHash.insert(statDesc->getStatisticDescriptionString(),statDesc);
					mStatDescList.append(statDesc);
					return this;
				}



				QList<CProcessingStatisticDescription*>* CProcessingStatistics::getProcessingStatisticDescriptionList() {
					return &mStatDescList;
				}


				CProcessingStatistics* CProcessingStatistics::getProcessingStatistics() {
					if (!mInstance) {
						mInstanceMutex.lock();
						if (!mInstance) {
							mInstance = new CProcessingStatistics();
						}
						mInstanceMutex.unlock();
					}
					return mInstance;
				}

				cint64 CProcessingStatistics::getIndexFromProcessingStatisticDescriptionName(const QString& statDesName) {
					CProcessingStatistics* procStat = getProcessingStatistics();
					CProcessingStatisticDescription* statDes = procStat->mStatDescHash.value(statDesName);
					if (statDes) {
						return statDes->getStatisticGatheringIndex();
					} else {
						return -1;
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "../support/ngap-r16.4.0/38413-g40.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER`
 */

#ifndef	_NGAP_InterSystemHandoverReportType_H_
#define	_NGAP_InterSystemHandoverReportType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NGAP_InterSystemHandoverReportType_PR {
	NGAP_InterSystemHandoverReportType_PR_NOTHING,	/* No components present */
	NGAP_InterSystemHandoverReportType_PR_tooearlyIntersystemHO,
	NGAP_InterSystemHandoverReportType_PR_intersystemUnnecessaryHO,
	NGAP_InterSystemHandoverReportType_PR_choice_Extensions
} NGAP_InterSystemHandoverReportType_PR;

/* Forward declarations */
struct NGAP_TooearlyIntersystemHO;
struct NGAP_IntersystemUnnecessaryHO;
struct NGAP_ProtocolIE_SingleContainer;

/* NGAP_InterSystemHandoverReportType */
typedef struct NGAP_InterSystemHandoverReportType {
	NGAP_InterSystemHandoverReportType_PR present;
	union NGAP_InterSystemHandoverReportType_u {
		struct NGAP_TooearlyIntersystemHO	*tooearlyIntersystemHO;
		struct NGAP_IntersystemUnnecessaryHO	*intersystemUnnecessaryHO;
		struct NGAP_ProtocolIE_SingleContainer	*choice_Extensions;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NGAP_InterSystemHandoverReportType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NGAP_InterSystemHandoverReportType;
extern asn_CHOICE_specifics_t asn_SPC_NGAP_InterSystemHandoverReportType_specs_1;
extern asn_TYPE_member_t asn_MBR_NGAP_InterSystemHandoverReportType_1[3];
extern asn_per_constraints_t asn_PER_type_NGAP_InterSystemHandoverReportType_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _NGAP_InterSystemHandoverReportType_H_ */
#include <asn_internal.h>

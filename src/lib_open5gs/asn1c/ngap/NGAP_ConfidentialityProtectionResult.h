/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "../support/ngap-r16.4.0/38413-g40.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER`
 */

#ifndef	_NGAP_ConfidentialityProtectionResult_H_
#define	_NGAP_ConfidentialityProtectionResult_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NGAP_ConfidentialityProtectionResult {
	NGAP_ConfidentialityProtectionResult_performed	= 0,
	NGAP_ConfidentialityProtectionResult_not_performed	= 1
	/*
	 * Enumeration is extensible
	 */
} e_NGAP_ConfidentialityProtectionResult;

/* NGAP_ConfidentialityProtectionResult */
typedef long	 NGAP_ConfidentialityProtectionResult_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_NGAP_ConfidentialityProtectionResult_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NGAP_ConfidentialityProtectionResult;
extern const asn_INTEGER_specifics_t asn_SPC_ConfidentialityProtectionResult_specs_1;
asn_struct_free_f ConfidentialityProtectionResult_free;
asn_struct_print_f ConfidentialityProtectionResult_print;
asn_constr_check_f ConfidentialityProtectionResult_constraint;
per_type_decoder_f ConfidentialityProtectionResult_decode_aper;
per_type_encoder_f ConfidentialityProtectionResult_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _NGAP_ConfidentialityProtectionResult_H_ */
#include <asn_internal.h>
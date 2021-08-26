/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "S1AP-CommonDataTypes"
 * 	found in "../support/s1ap-r16.4.0/36413-g40.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER`
 */

#ifndef	_S1AP_Criticality_H_
#define	_S1AP_Criticality_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum S1AP_Criticality {
	S1AP_Criticality_reject	= 0,
	S1AP_Criticality_ignore	= 1,
	S1AP_Criticality_notify	= 2
} e_S1AP_Criticality;

/* S1AP_Criticality */
typedef long	 S1AP_Criticality_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_S1AP_Criticality_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_S1AP_Criticality;
extern const asn_INTEGER_specifics_t asn_SPC_Criticality_specs_1;
asn_struct_free_f Criticality_free;
asn_struct_print_f Criticality_print;
asn_constr_check_f Criticality_constraint;
per_type_decoder_f Criticality_decode_aper;
per_type_encoder_f Criticality_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _S1AP_Criticality_H_ */
#include <asn_internal.h>

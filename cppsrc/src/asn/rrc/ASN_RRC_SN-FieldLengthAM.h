/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_SN_FieldLengthAM_H_
#define	_ASN_RRC_SN_FieldLengthAM_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ASN_RRC_SN_FieldLengthAM {
	ASN_RRC_SN_FieldLengthAM_size12	= 0,
	ASN_RRC_SN_FieldLengthAM_size18	= 1
} e_ASN_RRC_SN_FieldLengthAM;

/* ASN_RRC_SN-FieldLengthAM */
typedef long	 ASN_RRC_SN_FieldLengthAM_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ASN_RRC_SN_FieldLengthAM_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_SN_FieldLengthAM;
extern const asn_INTEGER_specifics_t asn_SPC_ASN_RRC_SN_FieldLengthAM_specs_1;
asn_struct_free_f ASN_RRC_SN_FieldLengthAM_free;
asn_struct_print_f ASN_RRC_SN_FieldLengthAM_print;
asn_constr_check_f ASN_RRC_SN_FieldLengthAM_constraint;
ber_type_decoder_f ASN_RRC_SN_FieldLengthAM_decode_ber;
der_type_encoder_f ASN_RRC_SN_FieldLengthAM_encode_der;
xer_type_decoder_f ASN_RRC_SN_FieldLengthAM_decode_xer;
xer_type_encoder_f ASN_RRC_SN_FieldLengthAM_encode_xer;
per_type_decoder_f ASN_RRC_SN_FieldLengthAM_decode_uper;
per_type_encoder_f ASN_RRC_SN_FieldLengthAM_encode_uper;
per_type_decoder_f ASN_RRC_SN_FieldLengthAM_decode_aper;
per_type_encoder_f ASN_RRC_SN_FieldLengthAM_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_SN_FieldLengthAM_H_ */
#include <asn_internal.h>

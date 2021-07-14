/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_CSI_RS_CellMobility_H_
#define	_ASN_RRC_CSI_RS_CellMobility_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ASN_RRC_PhysCellId.h"
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs {
	ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size24	= 0,
	ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size48	= 1,
	ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size96	= 2,
	ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size192	= 3,
	ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs_size264	= 4
} e_ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW__nrofPRBs;
typedef enum ASN_RRC_CSI_RS_CellMobility__density {
	ASN_RRC_CSI_RS_CellMobility__density_d1	= 0,
	ASN_RRC_CSI_RS_CellMobility__density_d3	= 1
} e_ASN_RRC_CSI_RS_CellMobility__density;

/* Forward declarations */
struct ASN_RRC_CSI_RS_Resource_Mobility;

/* ASN_RRC_CSI-RS-CellMobility */
typedef struct ASN_RRC_CSI_RS_CellMobility {
	ASN_RRC_PhysCellId_t	 cellId;
	struct ASN_RRC_CSI_RS_CellMobility__csi_rs_MeasurementBW {
		long	 nrofPRBs;
		long	 startPRB;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} csi_rs_MeasurementBW;
	long	*density;	/* OPTIONAL */
	struct ASN_RRC_CSI_RS_CellMobility__csi_rs_ResourceList_Mobility {
		A_SEQUENCE_OF(struct ASN_RRC_CSI_RS_Resource_Mobility) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} csi_rs_ResourceList_Mobility;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ASN_RRC_CSI_RS_CellMobility_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_nrofPRBs_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_density_11;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_CSI_RS_CellMobility;
extern asn_SEQUENCE_specifics_t asn_SPC_ASN_RRC_CSI_RS_CellMobility_specs_1;
extern asn_TYPE_member_t asn_MBR_ASN_RRC_CSI_RS_CellMobility_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_CSI_RS_CellMobility_H_ */
#include <asn_internal.h>

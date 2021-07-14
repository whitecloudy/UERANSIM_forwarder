/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#include "ASN_RRC_SlotFormatCombination.h"

static int
memb_NativeInteger_constraint_3(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 255)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ASN_RRC_slotFormats_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	/* Determine the number of elements */
	size = _A_CSEQUENCE_FROM_VOID(sptr)->count;
	
	if((size >= 1 && size <= 256)) {
		/* Perform validation of the inner elements */
		return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_ASN_RRC_Member_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  0,  255 }	/* (0..255) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_type_ASN_RRC_slotFormats_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 8,  8,  1,  256 }	/* (SIZE(1..256)) */,
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_ASN_RRC_slotFormats_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 8,  8,  1,  256 }	/* (SIZE(1..256)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_ASN_RRC_slotFormats_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_ASN_RRC_Member_constr_4,  memb_NativeInteger_constraint_3 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_ASN_RRC_slotFormats_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_ASN_RRC_slotFormats_specs_3 = {
	sizeof(struct ASN_RRC_SlotFormatCombination__slotFormats),
	offsetof(struct ASN_RRC_SlotFormatCombination__slotFormats, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ASN_RRC_slotFormats_3 = {
	"slotFormats",
	"slotFormats",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_ASN_RRC_slotFormats_tags_3,
	sizeof(asn_DEF_ASN_RRC_slotFormats_tags_3)
		/sizeof(asn_DEF_ASN_RRC_slotFormats_tags_3[0]) - 1, /* 1 */
	asn_DEF_ASN_RRC_slotFormats_tags_3,	/* Same as above */
	sizeof(asn_DEF_ASN_RRC_slotFormats_tags_3)
		/sizeof(asn_DEF_ASN_RRC_slotFormats_tags_3[0]), /* 2 */
	{ 0, &asn_PER_type_ASN_RRC_slotFormats_constr_3, SEQUENCE_OF_constraint },
	asn_MBR_ASN_RRC_slotFormats_3,
	1,	/* Single element */
	&asn_SPC_ASN_RRC_slotFormats_specs_3	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_ASN_RRC_SlotFormatCombination_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ASN_RRC_SlotFormatCombination, slotFormatCombinationId),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ASN_RRC_SlotFormatCombinationId,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"slotFormatCombinationId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ASN_RRC_SlotFormatCombination, slotFormats),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_ASN_RRC_slotFormats_3,
		0,
		{ 0, &asn_PER_memb_ASN_RRC_slotFormats_constr_3,  memb_ASN_RRC_slotFormats_constraint_1 },
		0, 0, /* No default value */
		"slotFormats"
		},
};
static const ber_tlv_tag_t asn_DEF_ASN_RRC_SlotFormatCombination_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ASN_RRC_SlotFormatCombination_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* slotFormatCombinationId */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* slotFormats */
};
asn_SEQUENCE_specifics_t asn_SPC_ASN_RRC_SlotFormatCombination_specs_1 = {
	sizeof(struct ASN_RRC_SlotFormatCombination),
	offsetof(struct ASN_RRC_SlotFormatCombination, _asn_ctx),
	asn_MAP_ASN_RRC_SlotFormatCombination_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ASN_RRC_SlotFormatCombination = {
	"SlotFormatCombination",
	"SlotFormatCombination",
	&asn_OP_SEQUENCE,
	asn_DEF_ASN_RRC_SlotFormatCombination_tags_1,
	sizeof(asn_DEF_ASN_RRC_SlotFormatCombination_tags_1)
		/sizeof(asn_DEF_ASN_RRC_SlotFormatCombination_tags_1[0]), /* 1 */
	asn_DEF_ASN_RRC_SlotFormatCombination_tags_1,	/* Same as above */
	sizeof(asn_DEF_ASN_RRC_SlotFormatCombination_tags_1)
		/sizeof(asn_DEF_ASN_RRC_SlotFormatCombination_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_ASN_RRC_SlotFormatCombination_1,
	2,	/* Elements count */
	&asn_SPC_ASN_RRC_SlotFormatCombination_specs_1	/* Additional specs */
};


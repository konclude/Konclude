/* OWL2FunctionalStyleParserTokenManager.cc */
#include "./OWL2FunctionalStyleParserTokenManager.h"
namespace Konclude {
namespace Parser {
namespace FunctionalJAVACC {
static const unsigned long long jjbitVec0[] = {
   0xfffe7000fffffff6ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x5e00000000ffffffULL
};
static const unsigned long long jjbitVec2[] = {
   0x0ULL, 0x0ULL, 0x0ULL, 0xff7fffffff7fffffULL
};
static const unsigned long long jjbitVec3[] = {
   0x0ULL, 0xbfff000000000000ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const unsigned long long jjbitVec4[] = {
   0x3000ULL, 0xffff000000000000ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const unsigned long long jjbitVec5[] = {
   0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffULL, 0x0ULL
};
static const unsigned long long jjbitVec6[] = {
   0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffULL
};
static const unsigned long long jjbitVec7[] = {
   0xfffffffffffffffeULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const unsigned long long jjbitVec8[] = {
   0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffff00000000ffffULL
};
static const unsigned long long jjbitVec9[] = {
   0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x3fffffffffffffffULL
};
static const unsigned long long jjbitVec10[] = {
   0x0ULL, 0x0ULL, 0x80000000000000ULL, 0xff7fffffff7fffffULL
};
static const unsigned long long jjbitVec11[] = {
   0xffffffffffffffffULL, 0xbfffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const unsigned long long jjbitVec12[] = {
   0x8000000000003000ULL, 0xffff000000000001ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const unsigned long long jjbitVec13[] = {
   0x0ULL, 0x0ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL
};
static const int jjnextStates[] = {
   6, 7, 8, 
};
static JAVACC_CHAR_TYPE jjstrLiteralChars_0[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_1[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_2[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_3[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_4[] = {0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_5[] = {0x28, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_6[] = {0x29, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_7[] = {0x3d, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_8[] = {0x5e, 0x5e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_9[] = {0x50, 0x72, 0x65, 0x66, 0x69, 0x78, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_10[] = {0x4f, 0x6e, 0x74, 0x6f, 0x6c, 0x6f, 0x67, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_11[] = {0x49, 0x6d, 0x70, 0x6f, 0x72, 0x74, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_12[] = {0x44, 0x4c, 0x53, 0x61, 0x66, 0x65, 0x52, 0x75, 0x6c, 0x65, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_13[] = {0x42, 0x6f, 0x64, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_14[] = {0x48, 0x65, 0x61, 0x64, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_15[] = {0x43, 0x6c, 0x61, 0x73, 0x73, 0x41, 0x74, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_16[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x74, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_17[] = {0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x74, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_18[] = {0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65, 0x6e, 0x74, 0x49, 0x6e, 0x64, 0x69, 0x76, 0x69, 0x64, 0x75, 0x61, 0x6c, 0x73, 0x41, 0x74, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_19[] = {0x42, 0x75, 0x69, 0x6c, 0x74, 0x49, 0x6e, 0x41, 0x74, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_20[] = {0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_21[] = {0x43, 0x6c, 0x61, 0x73, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_22[] = {0x44, 0x61, 0x74, 0x61, 0x74, 0x79, 0x70, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_23[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_24[] = {0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_25[] = {0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_26[] = {0x4e, 0x61, 0x6d, 0x65, 0x64, 0x49, 0x6e, 0x64, 0x69, 0x76, 0x69, 0x64, 0x75, 0x61, 0x6c, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_27[] = {0x44, 0x65, 0x63, 0x6c, 0x61, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_28[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x49, 0x6e, 0x76, 0x65, 0x72, 0x73, 0x65, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_29[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x43, 0x68, 0x61, 0x69, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_30[] = {0x43, 0x6f, 0x6d, 0x70, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_31[] = {0x4f, 0x6e, 0x65, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_32[] = {0x44, 0x61, 0x74, 0x61, 0x74, 0x79, 0x70, 0x65, 0x52, 0x65, 0x73, 0x74, 0x72, 0x69, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_33[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x73, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_34[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x55, 0x6e, 0x69, 0x6f, 0x6e, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_35[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x43, 0x6f, 0x6d, 0x70, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_36[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x4f, 0x6e, 0x65, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_37[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x53, 0x6f, 0x6d, 0x65, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x46, 0x72, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_38[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x41, 0x6c, 0x6c, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x46, 0x72, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_39[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x48, 0x61, 0x73, 0x56, 0x61, 0x6c, 0x75, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_40[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x48, 0x61, 0x73, 0x53, 0x65, 0x6c, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_41[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x4d, 0x69, 0x6e, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_42[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x4d, 0x61, 0x78, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_43[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x45, 0x78, 0x61, 0x63, 0x74, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_44[] = {0x44, 0x61, 0x74, 0x61, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x73, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_45[] = {0x44, 0x61, 0x74, 0x61, 0x55, 0x6e, 0x69, 0x6f, 0x6e, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_46[] = {0x44, 0x61, 0x74, 0x61, 0x43, 0x6f, 0x6d, 0x70, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_47[] = {0x44, 0x61, 0x74, 0x61, 0x4f, 0x6e, 0x65, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_48[] = {0x44, 0x61, 0x74, 0x61, 0x53, 0x6f, 0x6d, 0x65, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x46, 0x72, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_49[] = {0x44, 0x61, 0x74, 0x61, 0x41, 0x6c, 0x6c, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x46, 0x72, 0x6f, 0x6d, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_50[] = {0x44, 0x61, 0x74, 0x61, 0x48, 0x61, 0x73, 0x56, 0x61, 0x6c, 0x75, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_51[] = {0x44, 0x61, 0x74, 0x61, 0x4d, 0x69, 0x6e, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_52[] = {0x44, 0x61, 0x74, 0x61, 0x4d, 0x61, 0x78, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_53[] = {0x44, 0x61, 0x74, 0x61, 0x45, 0x78, 0x61, 0x63, 0x74, 0x43, 0x61, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x6c, 0x69, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_54[] = {0x53, 0x75, 0x62, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_55[] = {0x45, 0x71, 0x75, 0x69, 0x76, 0x61, 0x6c, 0x65, 0x6e, 0x74, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_56[] = {0x44, 0x69, 0x73, 0x6a, 0x6f, 0x69, 0x6e, 0x74, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_57[] = {0x44, 0x69, 0x73, 0x6a, 0x6f, 0x69, 0x6e, 0x74, 0x55, 0x6e, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_58[] = {0x53, 0x75, 0x62, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_59[] = {0x45, 0x71, 0x75, 0x69, 0x76, 0x61, 0x6c, 0x65, 0x6e, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_60[] = {0x44, 0x69, 0x73, 0x6a, 0x6f, 0x69, 0x6e, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_61[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x44, 0x6f, 0x6d, 0x61, 0x69, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_62[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x52, 0x61, 0x6e, 0x67, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_63[] = {0x49, 0x6e, 0x76, 0x65, 0x72, 0x73, 0x65, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_64[] = {0x46, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x6c, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_65[] = {0x49, 0x6e, 0x76, 0x65, 0x72, 0x73, 0x65, 0x46, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x6c, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_66[] = {0x52, 0x65, 0x66, 0x6c, 0x65, 0x78, 0x69, 0x76, 0x65, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_67[] = {0x49, 0x72, 0x72, 0x65, 0x66, 0x6c, 0x65, 0x78, 0x69, 0x76, 0x65, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_68[] = {0x53, 0x79, 0x6d, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_69[] = {0x41, 0x73, 0x79, 0x6d, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_70[] = {0x54, 0x72, 0x61, 0x6e, 0x73, 0x69, 0x74, 0x69, 0x76, 0x65, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_71[] = {0x53, 0x75, 0x62, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_72[] = {0x45, 0x71, 0x75, 0x69, 0x76, 0x61, 0x6c, 0x65, 0x6e, 0x74, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_73[] = {0x44, 0x69, 0x73, 0x6a, 0x6f, 0x69, 0x6e, 0x74, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69, 0x65, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_74[] = {0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x44, 0x6f, 0x6d, 0x61, 0x69, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_75[] = {0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x52, 0x61, 0x6e, 0x67, 0x65, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_76[] = {0x46, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x6c, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_77[] = {0x44, 0x61, 0x74, 0x61, 0x74, 0x79, 0x70, 0x65, 0x44, 0x65, 0x66, 0x69, 0x6e, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_78[] = {0x48, 0x61, 0x73, 0x4b, 0x65, 0x79, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_79[] = {0x53, 0x61, 0x6d, 0x65, 0x49, 0x6e, 0x64, 0x69, 0x76, 0x69, 0x64, 0x75, 0x61, 0x6c, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_80[] = {0x44, 0x69, 0x66, 0x66, 0x65, 0x72, 0x65, 0x6e, 0x74, 0x49, 0x6e, 0x64, 0x69, 0x76, 0x69, 0x64, 0x75, 0x61, 0x6c, 0x73, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_81[] = {0x43, 0x6c, 0x61, 0x73, 0x73, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_82[] = {0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_83[] = {0x4e, 0x65, 0x67, 0x61, 0x74, 0x69, 0x76, 0x65, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_84[] = {0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_85[] = {0x4e, 0x65, 0x67, 0x61, 0x74, 0x69, 0x76, 0x65, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_86[] = {0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_87[] = {0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_88[] = {0x53, 0x75, 0x62, 0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4f, 0x66, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_89[] = {0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x44, 0x6f, 0x6d, 0x61, 0x69, 0x6e, 0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_90[] = {0x41, 0x6e, 0x6e, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x52, 0x61, 0x6e, 0x67, 0x65, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_91[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_92[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_93[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_94[] = {0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_95[] = {0x5f, 0x3a, 0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_96[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_97[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_98[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_99[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_100[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_101[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_102[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_103[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_104[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_105[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_106[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_107[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_108[] = {0};

static JAVACC_CHAR_TYPE jjstrLiteralChars_109[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_110[] = {0};
static JAVACC_CHAR_TYPE jjstrLiteralChars_111[] = {0};
static const JAVACC_STRING_TYPE jjstrLiteralImages[] = {
jjstrLiteralChars_0, 
jjstrLiteralChars_1, 
jjstrLiteralChars_2, 
jjstrLiteralChars_3, 
jjstrLiteralChars_4, 
jjstrLiteralChars_5, 
jjstrLiteralChars_6, 
jjstrLiteralChars_7, 
jjstrLiteralChars_8, 
jjstrLiteralChars_9, 
jjstrLiteralChars_10, 
jjstrLiteralChars_11, 
jjstrLiteralChars_12, 
jjstrLiteralChars_13, 
jjstrLiteralChars_14, 
jjstrLiteralChars_15, 
jjstrLiteralChars_16, 
jjstrLiteralChars_17, 
jjstrLiteralChars_18, 
jjstrLiteralChars_19, 
jjstrLiteralChars_20, 
jjstrLiteralChars_21, 
jjstrLiteralChars_22, 
jjstrLiteralChars_23, 
jjstrLiteralChars_24, 
jjstrLiteralChars_25, 
jjstrLiteralChars_26, 
jjstrLiteralChars_27, 
jjstrLiteralChars_28, 
jjstrLiteralChars_29, 
jjstrLiteralChars_30, 
jjstrLiteralChars_31, 
jjstrLiteralChars_32, 
jjstrLiteralChars_33, 
jjstrLiteralChars_34, 
jjstrLiteralChars_35, 
jjstrLiteralChars_36, 
jjstrLiteralChars_37, 
jjstrLiteralChars_38, 
jjstrLiteralChars_39, 
jjstrLiteralChars_40, 
jjstrLiteralChars_41, 
jjstrLiteralChars_42, 
jjstrLiteralChars_43, 
jjstrLiteralChars_44, 
jjstrLiteralChars_45, 
jjstrLiteralChars_46, 
jjstrLiteralChars_47, 
jjstrLiteralChars_48, 
jjstrLiteralChars_49, 
jjstrLiteralChars_50, 
jjstrLiteralChars_51, 
jjstrLiteralChars_52, 
jjstrLiteralChars_53, 
jjstrLiteralChars_54, 
jjstrLiteralChars_55, 
jjstrLiteralChars_56, 
jjstrLiteralChars_57, 
jjstrLiteralChars_58, 
jjstrLiteralChars_59, 
jjstrLiteralChars_60, 
jjstrLiteralChars_61, 
jjstrLiteralChars_62, 
jjstrLiteralChars_63, 
jjstrLiteralChars_64, 
jjstrLiteralChars_65, 
jjstrLiteralChars_66, 
jjstrLiteralChars_67, 
jjstrLiteralChars_68, 
jjstrLiteralChars_69, 
jjstrLiteralChars_70, 
jjstrLiteralChars_71, 
jjstrLiteralChars_72, 
jjstrLiteralChars_73, 
jjstrLiteralChars_74, 
jjstrLiteralChars_75, 
jjstrLiteralChars_76, 
jjstrLiteralChars_77, 
jjstrLiteralChars_78, 
jjstrLiteralChars_79, 
jjstrLiteralChars_80, 
jjstrLiteralChars_81, 
jjstrLiteralChars_82, 
jjstrLiteralChars_83, 
jjstrLiteralChars_84, 
jjstrLiteralChars_85, 
jjstrLiteralChars_86, 
jjstrLiteralChars_87, 
jjstrLiteralChars_88, 
jjstrLiteralChars_89, 
jjstrLiteralChars_90, 
jjstrLiteralChars_91, 
jjstrLiteralChars_92, 
jjstrLiteralChars_93, 
jjstrLiteralChars_94, 
jjstrLiteralChars_95, 
jjstrLiteralChars_96, 
jjstrLiteralChars_97, 
jjstrLiteralChars_98, 
jjstrLiteralChars_99, 
jjstrLiteralChars_100, 
jjstrLiteralChars_101, 
jjstrLiteralChars_102, 
jjstrLiteralChars_103, 
jjstrLiteralChars_104, 
jjstrLiteralChars_105, 
jjstrLiteralChars_106, 
jjstrLiteralChars_107, 
jjstrLiteralChars_108, 
jjstrLiteralChars_109, 
jjstrLiteralChars_110, 
jjstrLiteralChars_111, 
};

/** Lexer state names. */
static const JAVACC_CHAR_TYPE lexStateNames_arr_0[] = 
{0x44, 0x45, 0x46, 0x41, 0x55, 0x4c, 0x54, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_1[] = 
{0x46, 0x75, 0x6c, 0x6c, 0x49, 0x72, 0x69, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_2[] = 
{0x46, 0x75, 0x6c, 0x6c, 0x49, 0x72, 0x69, 0x45, 0x6e, 0x64, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_3[] = 
{0x41, 0x66, 0x74, 0x65, 0x72, 0x50, 0x72, 0x65, 0x66, 0x69, 0x78, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_4[] = 
{0x51, 0x75, 0x6f, 0x74, 0x65, 0x64, 0x51, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_5[] = 
{0x51, 0x75, 0x6f, 0x74, 0x65, 0x64, 0x51, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x45, 0x6e, 0x64, 0};
static const JAVACC_CHAR_TYPE lexStateNames_arr_6[] = 
{0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0};
static const JAVACC_STRING_TYPE lexStateNames[] = {
lexStateNames_arr_0, 
lexStateNames_arr_1, 
lexStateNames_arr_2, 
lexStateNames_arr_3, 
lexStateNames_arr_4, 
lexStateNames_arr_5, 
lexStateNames_arr_6, 
};

/** Lex State array. */
static const int jjnewLexState[] = {
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, 3, -1, -1, -1, -1, 
   1, 4, 6, 2, 0, 0, 0, 5, 0, 0, 0, -1, 
};
static const unsigned long long jjtoToken[] = {
   0xffffffffffffffe1ULL, 0xa80bfffffffULL, 
};
static const unsigned long long jjtoSkip[] = {
   0x1eULL, 0xf57000000000ULL, 
};

void  OWL2FunctionalStyleParserTokenManager::setDebugStream(FILE *ds){ debugStream = ds; }

 int OWL2FunctionalStyleParserTokenManager::jjStopStringLiteralDfa_0(int pos, unsigned long long active0, unsigned long long active1){
   switch (pos)
   {
      case 0:
         if ((active0 & 0xfffffffffffffe00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 1:
         if ((active0 & 0xfffffffffffffe00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 2:
         if ((active0 & 0xfffffffffffffe00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 3:
         if ((active0 & 0xfffffffffffffe00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 4:
         if ((active0 & 0xffffffffffff9e00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 5:
         if ((active0 & 0xffffffff7fdf9e00ULL) != 0L || (active1 & 0x7ffffffULL) != 0L)
            return 9;
         return -1;
      case 6:
         if ((active0 & 0xffffffff7fdf9400ULL) != 0L || (active1 & 0x7ffbfffULL) != 0L)
            return 9;
         return -1;
      case 7:
         if ((active0 & 0xffffffff7fdf9400ULL) != 0L || (active1 & 0x7ffbfffULL) != 0L)
            return 9;
         return -1;
      case 8:
         if ((active0 & 0xffffffff7f8f9000ULL) != 0L || (active1 & 0x7ffbfffULL) != 0L)
            return 9;
         return -1;
      case 9:
         if ((active0 & 0xffff7fff7f8f1000ULL) != 0L || (active1 & 0x7ffbfffULL) != 0L)
            return 9;
         return -1;
      case 10:
         if ((active0 & 0xffbf7fff7f8f0000ULL) != 0L || (active1 & 0x7bfbfffULL) != 0L)
            return 9;
         return -1;
      case 11:
         if ((active0 & 0xffbf5fef77870000ULL) != 0L || (active1 & 0x7bfbfffULL) != 0L)
            return 9;
         return -1;
      case 12:
         if ((active0 & 0xffbb5fef36870000ULL) != 0L || (active1 & 0x7bfbfffULL) != 0L)
            return 9;
         return -1;
      case 13:
         if ((active0 & 0xfdbb5eeb36870000ULL) != 0L || (active1 & 0x7bfbfffULL) != 0L)
            return 9;
         return -1;
      case 14:
         if ((active0 & 0xfdbb5e6b36070000ULL) != 0L || (active1 & 0x7bd3fffULL) != 0L)
            return 9;
         return -1;
      case 15:
         if ((active0 & 0xfcbb5e6b22070000ULL) != 0L || (active1 & 0x7bd3fffULL) != 0L)
            return 9;
         return -1;
      case 16:
         if ((active0 & 0xfcbb1e6b22050000ULL) != 0L || (active1 & 0x7bd3fffULL) != 0L)
            return 9;
         return -1;
      case 17:
         if ((active0 & 0xfc391e6b22050000ULL) != 0L || (active1 & 0x7bd377fULL) != 0L)
            return 9;
         return -1;
      case 18:
         if ((active0 & 0xfc200e6320040000ULL) != 0L || (active1 & 0x7bd137fULL) != 0L)
            return 9;
         return -1;
      case 19:
         if ((active0 & 0xb8200e2200040000ULL) != 0L || (active1 & 0x73d137fULL) != 0L)
            return 9;
         return -1;
      case 20:
         if ((active0 & 0x9800080000040000ULL) != 0L || (active1 & 0x73c137fULL) != 0L)
            return 9;
         return -1;
      case 21:
         if ((active0 & 0x9800080000040000ULL) != 0L || (active1 & 0x72c137fULL) != 0L)
            return 9;
         return -1;
      case 22:
         if ((active0 & 0x9800000000040000ULL) != 0L || (active1 & 0x72c017fULL) != 0L)
            return 9;
         return -1;
      case 23:
         if ((active0 & 0x1800000000040000ULL) != 0L || (active1 & 0x228016bULL) != 0L)
            return 9;
         return -1;
      case 24:
         if ((active0 & 0x800000000000000ULL) != 0L || (active1 & 0x28000aULL) != 0L)
            return 9;
         return -1;
      case 25:
         if ((active0 & 0x800000000000000ULL) != 0L || (active1 & 0x280002ULL) != 0L)
            return 9;
         return -1;
      case 26:
         if ((active1 & 0x280002ULL) != 0L)
            return 9;
         return -1;
      case 27:
         if ((active1 & 0x280002ULL) != 0L)
            return 9;
         return -1;
      case 28:
         if ((active1 & 0x280002ULL) != 0L)
            return 9;
         return -1;
      case 29:
         if ((active1 & 0x80002ULL) != 0L)
            return 9;
         return -1;
      default :
         return -1;
   }
}

int  OWL2FunctionalStyleParserTokenManager::jjStartNfa_0(int pos, unsigned long long active0, unsigned long long active1){
   return jjMoveNfa_0(jjStopStringLiteralDfa_0(pos, active0, active1), pos + 1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjStopAtPos(int pos, int kind){
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   return pos + 1;
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_0(){
   switch(curChar)
   {
      case 34:
         return jjStopAtPos(0, 101);
      case 35:
         return jjStopAtPos(0, 102);
      case 40:
         return jjStopAtPos(0, 5);
      case 41:
         return jjStopAtPos(0, 6);
      case 60:
         return jjStopAtPos(0, 100);
      case 61:
         return jjStopAtPos(0, 7);
      case 65:
         return jjMoveStringLiteralDfa1_0(0x2000000ULL, 0x6c00020ULL);
      case 66:
         return jjMoveStringLiteralDfa1_0(0x82000ULL, 0x0ULL);
      case 67:
         return jjMoveStringLiteralDfa1_0(0x40208000ULL, 0x20000ULL);
      case 68:
         return jjMoveStringLiteralDfa1_0(0x133ff00109461000ULL, 0x112e00ULL);
      case 69:
         return jjMoveStringLiteralDfa1_0(0x880000000000000ULL, 0x100ULL);
      case 70:
         return jjMoveStringLiteralDfa1_0(0x0ULL, 0x1001ULL);
      case 72:
         return jjMoveStringLiteralDfa1_0(0x4000ULL, 0x4000ULL);
      case 73:
         return jjMoveStringLiteralDfa1_0(0x8000000000000800ULL, 0xaULL);
      case 78:
         return jjMoveStringLiteralDfa1_0(0x4000000ULL, 0x280000ULL);
      case 79:
         return jjMoveStringLiteralDfa1_0(0x60000ffeb0810400ULL, 0x40000ULL);
      case 80:
         return jjMoveStringLiteralDfa1_0(0x200ULL, 0x0ULL);
      case 82:
         return jjMoveStringLiteralDfa1_0(0x0ULL, 0x4ULL);
      case 83:
         return jjMoveStringLiteralDfa1_0(0x440000000000000ULL, 0x1008090ULL);
      case 84:
         return jjMoveStringLiteralDfa1_0(0x0ULL, 0x40ULL);
      case 86:
         return jjMoveStringLiteralDfa1_0(0x100000ULL, 0x0ULL);
      case 94:
         return jjMoveStringLiteralDfa1_0(0x100ULL, 0x0ULL);
      case 95:
         return jjMoveStringLiteralDfa1_0(0x0ULL, 0x80000000ULL);
      default :
         return jjMoveNfa_0(1, 0);
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa1_0(unsigned long long active0, unsigned long long active1){
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(0, active0, active1);
      return 1;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 58:
         if ((active1 & 0x80000000ULL) != 0L)
            return jjStopAtPos(1, 95);
         break;
      case 76:
         return jjMoveStringLiteralDfa2_0(active0, 0x1000ULL, active1, 0L);
      case 94:
         if ((active0 & 0x100ULL) != 0L)
            return jjStopAtPos(1, 8);
         break;
      case 97:
         return jjMoveStringLiteralDfa2_0(active0, 0x3ff00105520000ULL, active1, 0x10ec00ULL);
      case 98:
         return jjMoveStringLiteralDfa2_0(active0, 0x60000ffe30810000ULL, active1, 0x40000ULL);
      case 101:
         return jjMoveStringLiteralDfa2_0(active0, 0x8004000ULL, active1, 0x280004ULL);
      case 105:
         return jjMoveStringLiteralDfa2_0(active0, 0x1300000000040000ULL, active1, 0x10200ULL);
      case 108:
         return jjMoveStringLiteralDfa2_0(active0, 0x208000ULL, active1, 0x20000ULL);
      case 109:
         return jjMoveStringLiteralDfa2_0(active0, 0x800ULL, active1, 0L);
      case 110:
         return jjMoveStringLiteralDfa2_0(active0, 0x8000000082000400ULL, active1, 0x6c00002ULL);
      case 111:
         return jjMoveStringLiteralDfa2_0(active0, 0x40002000ULL, active1, 0L);
      case 113:
         return jjMoveStringLiteralDfa2_0(active0, 0x880000000000000ULL, active1, 0x100ULL);
      case 114:
         return jjMoveStringLiteralDfa2_0(active0, 0x200ULL, active1, 0x48ULL);
      case 115:
         return jjMoveStringLiteralDfa2_0(active0, 0L, active1, 0x20ULL);
      case 117:
         return jjMoveStringLiteralDfa2_0(active0, 0x440000000080000ULL, active1, 0x1001081ULL);
      case 121:
         return jjMoveStringLiteralDfa2_0(active0, 0L, active1, 0x10ULL);
      default :
         break;
   }
   return jjStartNfa_0(0, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa2_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(0, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(1, active0, active1);
      return 2;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 83:
         return jjMoveStringLiteralDfa3_0(active0, 0x1000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa3_0(active0, 0x20c000ULL, active1, 0x20040ULL);
      case 98:
         return jjMoveStringLiteralDfa3_0(active0, 0x440000000000000ULL, active1, 0x1000080ULL);
      case 99:
         return jjMoveStringLiteralDfa3_0(active0, 0x8000000ULL, active1, 0L);
      case 100:
         return jjMoveStringLiteralDfa3_0(active0, 0x2000ULL, active1, 0L);
      case 101:
         return jjMoveStringLiteralDfa3_0(active0, 0x80000200ULL, active1, 0L);
      case 102:
         return jjMoveStringLiteralDfa3_0(active0, 0x40000ULL, active1, 0x10004ULL);
      case 103:
         return jjMoveStringLiteralDfa3_0(active0, 0L, active1, 0x280000ULL);
      case 105:
         return jjMoveStringLiteralDfa3_0(active0, 0x80000ULL, active1, 0L);
      case 106:
         return jjMoveStringLiteralDfa3_0(active0, 0x60000ffe30810000ULL, active1, 0x40000ULL);
      case 109:
         return jjMoveStringLiteralDfa3_0(active0, 0x44000000ULL, active1, 0x8010ULL);
      case 110:
         return jjMoveStringLiteralDfa3_0(active0, 0x2000000ULL, active1, 0x6c01001ULL);
      case 112:
         return jjMoveStringLiteralDfa3_0(active0, 0x800ULL, active1, 0L);
      case 114:
         return jjMoveStringLiteralDfa3_0(active0, 0x100000ULL, active1, 0x8ULL);
      case 115:
         return jjMoveStringLiteralDfa3_0(active0, 0x1300000000000000ULL, active1, 0x4200ULL);
      case 116:
         return jjMoveStringLiteralDfa3_0(active0, 0x3ff00101420400ULL, active1, 0x102c00ULL);
      case 117:
         return jjMoveStringLiteralDfa3_0(active0, 0x880000000000000ULL, active1, 0x100ULL);
      case 118:
         return jjMoveStringLiteralDfa3_0(active0, 0x8000000000000000ULL, active1, 0x2ULL);
      case 121:
         return jjMoveStringLiteralDfa3_0(active0, 0L, active1, 0x20ULL);
      default :
         break;
   }
   return jjStartNfa_0(1, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa3_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(1, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(2, active0, active1);
      return 3;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x1000000ULL);
      case 67:
         return jjMoveStringLiteralDfa4_0(active0, 0x40000000000000ULL, active1, 0L);
      case 68:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x80ULL);
      case 75:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x4000ULL);
      case 79:
         return jjMoveStringLiteralDfa4_0(active0, 0x400000080000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa4_0(active0, 0x3ff00101421000ULL, active1, 0x382c00ULL);
      case 99:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x1001ULL);
      case 100:
         if ((active0 & 0x4000ULL) != 0L)
            return jjStartNfaWithStates_0(3, 14, 9);
         break;
      case 101:
         return jjMoveStringLiteralDfa4_0(active0, 0xe0000ffe34810000ULL, active1, 0x4800aULL);
      case 102:
         return jjMoveStringLiteralDfa4_0(active0, 0x40200ULL, active1, 0x10000ULL);
      case 105:
         return jjMoveStringLiteralDfa4_0(active0, 0x880000000100000ULL, active1, 0x100ULL);
      case 106:
         return jjMoveStringLiteralDfa4_0(active0, 0x1300000000000000ULL, active1, 0x200ULL);
      case 108:
         return jjMoveStringLiteralDfa4_0(active0, 0x8080000ULL, active1, 0x4ULL);
      case 109:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x30ULL);
      case 110:
         return jjMoveStringLiteralDfa4_0(active0, 0L, active1, 0x40ULL);
      case 111:
         return jjMoveStringLiteralDfa4_0(active0, 0x2000c00ULL, active1, 0x6c00000ULL);
      case 112:
         return jjMoveStringLiteralDfa4_0(active0, 0x40000000ULL, active1, 0L);
      case 115:
         return jjMoveStringLiteralDfa4_0(active0, 0x208000ULL, active1, 0x20000ULL);
      case 121:
         if ((active0 & 0x2000ULL) != 0L)
            return jjStartNfaWithStates_0(3, 13, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(2, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa4_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(2, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(3, active0, active1);
      return 4;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa5_0(active0, 0x2000000000000ULL, active1, 0L);
      case 67:
         return jjMoveStringLiteralDfa5_0(active0, 0x400000000000ULL, active1, 0L);
      case 69:
         return jjMoveStringLiteralDfa5_0(active0, 0x20000000000000ULL, active1, 0L);
      case 72:
         return jjMoveStringLiteralDfa5_0(active0, 0x4000000000000ULL, active1, 0L);
      case 73:
         return jjMoveStringLiteralDfa5_0(active0, 0x100000000000ULL, active1, 0x8000ULL);
      case 77:
         return jjMoveStringLiteralDfa5_0(active0, 0x18000000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa5_0(active0, 0x800000000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa5_0(active0, 0x1020000ULL, active1, 0x100c00ULL);
      case 83:
         return jjMoveStringLiteralDfa5_0(active0, 0x1000000000000ULL, active1, 0L);
      case 85:
         return jjMoveStringLiteralDfa5_0(active0, 0x200000000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa5_0(active0, 0x8100000ULL, active1, 0x80ULL);
      case 98:
         return jjMoveStringLiteralDfa5_0(active0, 0x400000000000000ULL, active1, 0L);
      case 99:
         return jjMoveStringLiteralDfa5_0(active0, 0x60000ffe30810000ULL, active1, 0x40000ULL);
      case 100:
         return jjMoveStringLiteralDfa5_0(active0, 0x4000000ULL, active1, 0L);
      case 101:
         return jjMoveStringLiteralDfa5_0(active0, 0x40000ULL, active1, 0x14014ULL);
      case 102:
         if ((active0 & 0x80000000ULL) != 0L)
            return jjStartNfaWithStates_0(4, 31, 9);
         return jjMoveStringLiteralDfa5_0(active0, 0x1000ULL, active1, 0x8ULL);
      case 105:
         return jjMoveStringLiteralDfa5_0(active0, 0x200ULL, active1, 0L);
      case 108:
         return jjMoveStringLiteralDfa5_0(active0, 0x40000040000400ULL, active1, 0L);
      case 109:
         return jjMoveStringLiteralDfa5_0(active0, 0L, active1, 0x20ULL);
      case 110:
         return jjMoveStringLiteralDfa5_0(active0, 0L, active1, 0x1000000ULL);
      case 111:
         return jjMoveStringLiteralDfa5_0(active0, 0x1300000000000000ULL, active1, 0x200ULL);
      case 114:
         return jjMoveStringLiteralDfa5_0(active0, 0x8000000000000800ULL, active1, 0x2ULL);
      case 115:
         if ((active0 & 0x200000ULL) != 0L)
         {
            jjmatchedKind = 21;
            jjmatchedPos = 4;
         }
         return jjMoveStringLiteralDfa5_0(active0, 0x8000ULL, active1, 0x20040ULL);
      case 116:
         return jjMoveStringLiteralDfa5_0(active0, 0x102480000ULL, active1, 0x6e83001ULL);
      case 118:
         return jjMoveStringLiteralDfa5_0(active0, 0x880000000000000ULL, active1, 0x100ULL);
      default :
         break;
   }
   return jjStartNfa_0(3, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa5_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(3, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(4, active0, active1);
      return 5;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa6_0(active0, 0x8000ULL, active1, 0x20000ULL);
      case 73:
         return jjMoveStringLiteralDfa6_0(active0, 0x4080000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa6_0(active0, 0x8d4000002000000ULL, active1, 0x6c00100ULL);
      case 98:
         return jjMoveStringLiteralDfa6_0(active0, 0x100000ULL, active1, 0L);
      case 101:
         return jjMoveStringLiteralDfa6_0(active0, 0x40001000ULL, active1, 0x20ULL);
      case 105:
         return jjMoveStringLiteralDfa6_0(active0, 0x1308000000000000ULL, active1, 0x281241ULL);
      case 106:
         return jjMoveStringLiteralDfa6_0(active0, 0x400000000000000ULL, active1, 0L);
      case 108:
         return jjMoveStringLiteralDfa6_0(active0, 0x2000000000000ULL, active1, 0x8ULL);
      case 110:
         return jjMoveStringLiteralDfa6_0(active0, 0xb00000000000ULL, active1, 0x1008000ULL);
      case 111:
         return jjMoveStringLiteralDfa6_0(active0, 0x1400000000400ULL, active1, 0L);
      case 114:
         return jjMoveStringLiteralDfa6_0(active0, 0x9060000ULL, active1, 0x110c00ULL);
      case 115:
         return jjMoveStringLiteralDfa6_0(active0, 0x8000000000000000ULL, active1, 0x2ULL);
      case 116:
         if ((active0 & 0x800ULL) != 0L)
            return jjStartNfaWithStates_0(5, 11, 9);
         return jjMoveStringLiteralDfa6_0(active0, 0x60000ffe30810000ULL, active1, 0x40090ULL);
      case 120:
         if ((active0 & 0x200ULL) != 0L)
            return jjStartNfaWithStates_0(5, 9, 9);
         return jjMoveStringLiteralDfa6_0(active0, 0x20000000000000ULL, active1, 0x4ULL);
      case 121:
         if ((active1 & 0x4000ULL) != 0L)
            return jjStartNfaWithStates_0(5, 78, 9);
         return jjMoveStringLiteralDfa6_0(active0, 0x100400000ULL, active1, 0x2000ULL);
      default :
         break;
   }
   return jjStartNfa_0(4, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa6_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(4, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(5, active0, active1);
      return 6;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa7_0(active0, 0x4000000000ULL, active1, 0L);
      case 67:
         return jjMoveStringLiteralDfa7_0(active0, 0x800000000ULL, active1, 0L);
      case 69:
         return jjMoveStringLiteralDfa7_0(active0, 0x80000000000ULL, active1, 0L);
      case 72:
         return jjMoveStringLiteralDfa7_0(active0, 0x18000000000ULL, active1, 0L);
      case 73:
         return jjMoveStringLiteralDfa7_0(active0, 0x210000000ULL, active1, 0L);
      case 77:
         return jjMoveStringLiteralDfa7_0(active0, 0x60000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa7_0(active0, 0x1000000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa7_0(active0, 0x6000000020810000ULL, active1, 0x40000ULL);
      case 82:
         return jjMoveStringLiteralDfa7_0(active0, 0x1000ULL, active1, 0L);
      case 83:
         return jjMoveStringLiteralDfa7_0(active0, 0x2000000000ULL, active1, 0L);
      case 85:
         return jjMoveStringLiteralDfa7_0(active0, 0x400000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa7_0(active0, 0x20000008000000ULL, active1, 0x80ULL);
      case 100:
         return jjMoveStringLiteralDfa7_0(active0, 0L, active1, 0x8000ULL);
      case 101:
         return jjMoveStringLiteralDfa7_0(active0, 0x8400800000040000ULL, active1, 0x1000aULL);
      case 103:
         return jjMoveStringLiteralDfa7_0(active0, 0x400ULL, active1, 0L);
      case 105:
         return jjMoveStringLiteralDfa7_0(active0, 0x200000000000ULL, active1, 0x4ULL);
      case 108:
         return jjMoveStringLiteralDfa7_0(active0, 0x882000000100000ULL, active1, 0x100ULL);
      case 109:
         return jjMoveStringLiteralDfa7_0(active0, 0x1400040000000ULL, active1, 0L);
      case 110:
         return jjMoveStringLiteralDfa7_0(active0, 0x1308000004080000ULL, active1, 0x200ULL);
      case 111:
         return jjMoveStringLiteralDfa7_0(active0, 0x1020000ULL, active1, 0x1101c01ULL);
      case 112:
         return jjMoveStringLiteralDfa7_0(active0, 0x100400000ULL, active1, 0x2000ULL);
      case 114:
         return jjMoveStringLiteralDfa7_0(active0, 0L, active1, 0x10ULL);
      case 115:
         return jjMoveStringLiteralDfa7_0(active0, 0x44000000000000ULL, active1, 0x20000ULL);
      case 116:
         return jjMoveStringLiteralDfa7_0(active0, 0x100002008000ULL, active1, 0x6c00060ULL);
      case 118:
         return jjMoveStringLiteralDfa7_0(active0, 0L, active1, 0x280000ULL);
      case 120:
         return jjMoveStringLiteralDfa7_0(active0, 0x10000000000000ULL, active1, 0L);
      default :
         break;
   }
   return jjStartNfa_0(5, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa7_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(5, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(6, active0, active1);
      return 7;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa8_0(active0, 0x80000ULL, active1, 0L);
      case 67:
         return jjMoveStringLiteralDfa8_0(active0, 0x18000000000000ULL, active1, 0L);
      case 70:
         return jjMoveStringLiteralDfa8_0(active0, 0L, active1, 0x2ULL);
      case 79:
         return jjMoveStringLiteralDfa8_0(active0, 0x8000800000000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa8_0(active0, 0L, active1, 0x80ULL);
      case 86:
         return jjMoveStringLiteralDfa8_0(active0, 0x6000000000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa8_0(active0, 0x58000000000ULL, active1, 0L);
      case 99:
         return jjMoveStringLiteralDfa8_0(active0, 0x420000000000000ULL, active1, 0L);
      case 100:
         return jjMoveStringLiteralDfa8_0(active0, 0x4000000ULL, active1, 0L);
      case 101:
         if ((active0 & 0x100000ULL) != 0L)
            return jjStartNfaWithStates_0(7, 20, 9);
         else if ((active0 & 0x400000ULL) != 0L)
         {
            jjmatchedKind = 22;
            jjmatchedPos = 7;
         }
         return jjMoveStringLiteralDfa8_0(active0, 0x881100140000000ULL, active1, 0x282100ULL);
      case 105:
         return jjMoveStringLiteralDfa8_0(active0, 0x20002000000ULL, active1, 0x6c08050ULL);
      case 108:
         return jjMoveStringLiteralDfa8_0(active0, 0x4000000000ULL, active1, 0L);
      case 110:
         return jjMoveStringLiteralDfa8_0(active0, 0x1610040000ULL, active1, 0x11001ULL);
      case 111:
         return jjMoveStringLiteralDfa8_0(active0, 0x202800008000ULL, active1, 0L);
      case 112:
         return jjMoveStringLiteralDfa8_0(active0, 0x400001020000ULL, active1, 0x100c00ULL);
      case 114:
         return jjMoveStringLiteralDfa8_0(active0, 0x6000000020810000ULL, active1, 0x40020ULL);
      case 115:
         return jjMoveStringLiteralDfa8_0(active0, 0x40000000000000ULL, active1, 0x20000ULL);
      case 116:
         return jjMoveStringLiteralDfa8_0(active0, 0x1300000008000000ULL, active1, 0x1000200ULL);
      case 117:
         return jjMoveStringLiteralDfa8_0(active0, 0x1000ULL, active1, 0L);
      case 118:
         return jjMoveStringLiteralDfa8_0(active0, 0L, active1, 0x4ULL);
      case 120:
         return jjMoveStringLiteralDfa8_0(active0, 0x80000000000ULL, active1, 0x8ULL);
      case 121:
         if ((active0 & 0x400ULL) != 0L)
            return jjStartNfaWithStates_0(7, 10, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(6, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa8_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(6, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(7, active0, active1);
      return 8;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 67:
         return jjMoveStringLiteralDfa9_0(active0, 0x100000000000000ULL, active1, 0L);
      case 68:
         return jjMoveStringLiteralDfa9_0(active0, 0L, active1, 0x202200ULL);
      case 79:
         return jjMoveStringLiteralDfa9_0(active0, 0x1040000000000000ULL, active1, 0x80000ULL);
      case 82:
         return jjMoveStringLiteralDfa9_0(active0, 0x100000000ULL, active1, 0L);
      case 85:
         return jjMoveStringLiteralDfa9_0(active0, 0x200000000000000ULL, active1, 0L);
      case 86:
         return jjMoveStringLiteralDfa9_0(active0, 0x1000000000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa9_0(active0, 0x1e080000000000ULL, active1, 0x1001001ULL);
      case 98:
         return jjMoveStringLiteralDfa9_0(active0, 0x8000000000000000ULL, active1, 0L);
      case 99:
         return jjMoveStringLiteralDfa9_0(active0, 0L, active1, 0x10ULL);
      case 101:
         return jjMoveStringLiteralDfa9_0(active0, 0x1001020000ULL, active1, 0x120c04ULL);
      case 102:
         if ((active0 & 0x800000000000ULL) != 0L)
            return jjStartNfaWithStates_0(8, 47, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa9_0(active0, 0x40c000000ULL, active1, 0x28ULL);
      case 108:
         return jjMoveStringLiteralDfa9_0(active0, 0x404000001000ULL, active1, 0L);
      case 109:
         if ((active0 & 0x8000ULL) != 0L)
            return jjStartNfaWithStates_0(8, 15, 9);
         return jjMoveStringLiteralDfa9_0(active0, 0x2800000000ULL, active1, 0L);
      case 110:
         return jjMoveStringLiteralDfa9_0(active0, 0x880220040000000ULL, active1, 0x100ULL);
      case 111:
         return jjMoveStringLiteralDfa9_0(active0, 0x6000000022810000ULL, active1, 0x6c40000ULL);
      case 114:
         return jjMoveStringLiteralDfa9_0(active0, 0x100000000000ULL, active1, 0x80ULL);
      case 115:
         return jjMoveStringLiteralDfa9_0(active0, 0x18000000000ULL, active1, 0L);
      case 116:
         return jjMoveStringLiteralDfa9_0(active0, 0x4200002000c0000ULL, active1, 0x10000ULL);
      case 117:
         return jjMoveStringLiteralDfa9_0(active0, 0L, active1, 0x2ULL);
      case 118:
         return jjMoveStringLiteralDfa9_0(active0, 0x10000000ULL, active1, 0x8040ULL);
      case 120:
         return jjMoveStringLiteralDfa9_0(active0, 0x40000000000ULL, active1, 0L);
      default :
         break;
   }
   return jjStartNfa_0(7, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa9_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(7, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(8, active0, active1);
      return 9;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 67:
         return jjMoveStringLiteralDfa10_0(active0, 0x20060000000000ULL, active1, 0L);
      case 73:
         return jjMoveStringLiteralDfa10_0(active0, 0x40000ULL, active1, 0x10000ULL);
      case 79:
         return jjMoveStringLiteralDfa10_0(active0, 0x201000000000ULL, active1, 0x14ULL);
      case 80:
         return jjMoveStringLiteralDfa10_0(active0, 0x400000000000000ULL, active1, 0L);
      case 83:
         return jjMoveStringLiteralDfa10_0(active0, 0x10000000000ULL, active1, 0L);
      case 86:
         return jjMoveStringLiteralDfa10_0(active0, 0xc000000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa10_0(active0, 0x1000000000000ULL, active1, 0x200200ULL);
      case 98:
         return jjMoveStringLiteralDfa10_0(active0, 0x1000000000000000ULL, active1, 0x80000ULL);
      case 99:
         return jjMoveStringLiteralDfa10_0(active0, 0x80000000000ULL, active1, 0x20ULL);
      case 101:
         if ((active0 & 0x1000ULL) != 0L)
            return jjStartNfaWithStates_0(9, 12, 9);
         return jjMoveStringLiteralDfa10_0(active0, 0x402310000000ULL, active1, 0x2040ULL);
      case 102:
         if ((active0 & 0x40000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(9, 54, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa10_0(active0, 0L, active1, 0x8000ULL);
      case 106:
         return jjMoveStringLiteralDfa10_0(active0, 0x8000000000000000ULL, active1, 0L);
      case 108:
         return jjMoveStringLiteralDfa10_0(active0, 0x106000000000000ULL, active1, 0x1001ULL);
      case 110:
         if ((active1 & 0x400000ULL) != 0L)
         {
            jjmatchedKind = 86;
            jjmatchedPos = 9;
         }
         return jjMoveStringLiteralDfa10_0(active0, 0x200000002000000ULL, active1, 0x6800002ULL);
      case 111:
         return jjMoveStringLiteralDfa10_0(active0, 0x408080000ULL, active1, 0x80ULL);
      case 112:
         return jjMoveStringLiteralDfa10_0(active0, 0x6000000820810000ULL, active1, 0x40000ULL);
      case 114:
         return jjMoveStringLiteralDfa10_0(active0, 0x18000001020000ULL, active1, 0x120c00ULL);
      case 115:
         return jjMoveStringLiteralDfa10_0(active0, 0x100000000000ULL, active1, 0L);
      case 116:
         return jjMoveStringLiteralDfa10_0(active0, 0x880000040000000ULL, active1, 0x1000100ULL);
      case 118:
         return jjMoveStringLiteralDfa10_0(active0, 0x4000000ULL, active1, 0x8ULL);
      default :
         break;
   }
   return jjStartNfa_0(8, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa10_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(8, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(9, active0, active1);
      return 10;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x800000ULL);
      case 67:
         return jjMoveStringLiteralDfa11_0(active0, 0x80000000000000ULL, active1, 0L);
      case 68:
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x1100ULL);
      case 79:
         return jjMoveStringLiteralDfa11_0(active0, 0x800000040000000ULL, active1, 0x61ULL);
      case 80:
         return jjMoveStringLiteralDfa11_0(active0, 0x2000000ULL, active1, 0x6000000ULL);
      case 86:
         return jjMoveStringLiteralDfa11_0(active0, 0x2000000000ULL, active1, 0L);
      case 97:
         return jjMoveStringLiteralDfa11_0(active0, 0x12006c000000000ULL, active1, 0L);
      case 98:
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x14ULL);
      case 99:
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x2ULL);
      case 100:
         return jjMoveStringLiteralDfa11_0(active0, 0x18000000000000ULL, active1, 0x8000ULL);
      case 101:
         return jjMoveStringLiteralDfa11_0(active0, 0xe000110020810000ULL, active1, 0x40008ULL);
      case 102:
         if ((active0 & 0x1000000000ULL) != 0L)
            return jjStartNfaWithStates_0(10, 36, 9);
         else if ((active0 & 0x200000000000ULL) != 0L)
            return jjStartNfaWithStates_0(10, 45, 9);
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x2000ULL);
      case 105:
         return jjMoveStringLiteralDfa11_0(active0, 0x200000004000000ULL, active1, 0x1000000ULL);
      case 106:
         return jjMoveStringLiteralDfa11_0(active0, 0x1000000000000000ULL, active1, 0x80000ULL);
      case 108:
         return jjMoveStringLiteralDfa11_0(active0, 0x1000800000000ULL, active1, 0L);
      case 109:
         if ((active0 & 0x80000ULL) != 0L)
            return jjStartNfaWithStates_0(10, 19, 9);
         return jjMoveStringLiteralDfa11_0(active0, 0x400000000000ULL, active1, 0L);
      case 110:
         if ((active0 & 0x8000000ULL) != 0L)
            return jjStartNfaWithStates_0(10, 27, 9);
         return jjMoveStringLiteralDfa11_0(active0, 0x400040000ULL, active1, 0x10000ULL);
      case 112:
         return jjMoveStringLiteralDfa11_0(active0, 0L, active1, 0x80ULL);
      case 114:
         return jjMoveStringLiteralDfa11_0(active0, 0x400000210000000ULL, active1, 0L);
      case 115:
         return jjMoveStringLiteralDfa11_0(active0, 0x100000000ULL, active1, 0L);
      case 116:
         return jjMoveStringLiteralDfa11_0(active0, 0x80001020000ULL, active1, 0x320e00ULL);
      case 117:
         return jjMoveStringLiteralDfa11_0(active0, 0x6000000000000ULL, active1, 0L);
      default :
         break;
   }
   return jjStartNfa_0(9, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa11_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(9, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(10, active0, active1);
      return 11;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 67:
         return jjMoveStringLiteralDfa12_0(active0, 0x80000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa12_0(active0, 0x400000000ULL, active1, 0x8ULL);
      case 97:
         return jjMoveStringLiteralDfa12_0(active0, 0x2000000000ULL, active1, 0x201300ULL);
      case 98:
         return jjMoveStringLiteralDfa12_0(active0, 0x800000000000000ULL, active1, 0x61ULL);
      case 99:
         return jjMoveStringLiteralDfa12_0(active0, 0x8000100000000000ULL, active1, 0L);
      case 100:
         return jjMoveStringLiteralDfa12_0(active0, 0x4040000ULL, active1, 0x10000ULL);
      case 101:
         if ((active0 & 0x4000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(11, 50, 9);
         return jjMoveStringLiteralDfa12_0(active0, 0x1002400800000000ULL, active1, 0x80080ULL);
      case 102:
         if ((active0 & 0x40000000ULL) != 0L)
            return jjStartNfaWithStates_0(11, 30, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa12_0(active0, 0x18000000000000ULL, active1, 0x22000ULL);
      case 106:
         return jjMoveStringLiteralDfa12_0(active0, 0L, active1, 0x14ULL);
      case 108:
         return jjMoveStringLiteralDfa12_0(active0, 0x8001c000000000ULL, active1, 0L);
      case 111:
         return jjMoveStringLiteralDfa12_0(active0, 0x600000000000000ULL, active1, 0x1000000ULL);
      case 114:
         return jjMoveStringLiteralDfa12_0(active0, 0x6020060022810000ULL, active1, 0x6040000ULL);
      case 115:
         return jjMoveStringLiteralDfa12_0(active0, 0x100000210000000ULL, active1, 0x800000ULL);
      case 116:
         return jjMoveStringLiteralDfa12_0(active0, 0x100000000ULL, active1, 0x2ULL);
      case 117:
         return jjMoveStringLiteralDfa12_0(active0, 0x1000000000000ULL, active1, 0x8000ULL);
      case 121:
         if ((active0 & 0x1000000ULL) != 0L)
         {
            jjmatchedKind = 24;
            jjmatchedPos = 11;
         }
         return jjMoveStringLiteralDfa12_0(active0, 0x20000ULL, active1, 0x100c00ULL);
      default :
         break;
   }
   return jjStartNfa_0(10, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa12_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(10, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(11, active0, active1);
      return 12;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa13_0(active0, 0x20000ULL, active1, 0x100000ULL);
      case 68:
         return jjMoveStringLiteralDfa13_0(active0, 0L, active1, 0x400ULL);
      case 80:
         return jjMoveStringLiteralDfa13_0(active0, 0L, active1, 0x200200ULL);
      case 82:
         return jjMoveStringLiteralDfa13_0(active0, 0L, active1, 0x800ULL);
      case 97:
         return jjMoveStringLiteralDfa13_0(active0, 0x80080000000000ULL, active1, 0x8000ULL);
      case 98:
         return jjMoveStringLiteralDfa13_0(active0, 0L, active1, 0x8ULL);
      case 99:
         return jjMoveStringLiteralDfa13_0(active0, 0x1000000000000000ULL, active1, 0x80000ULL);
      case 100:
         return jjMoveStringLiteralDfa13_0(active0, 0x20060000000000ULL, active1, 0L);
      case 101:
         return jjMoveStringLiteralDfa13_0(active0, 0x1000210000000ULL, active1, 0x14ULL);
      case 102:
         if ((active0 & 0x400000000ULL) != 0L)
            return jjStartNfaWithStates_0(12, 34, 9);
         else if ((active0 & 0x10000000000ULL) != 0L)
            return jjStartNfaWithStates_0(12, 40, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa13_0(active0, 0x40000ULL, active1, 0x10002ULL);
      case 106:
         return jjMoveStringLiteralDfa13_0(active0, 0x800000000000000ULL, active1, 0x61ULL);
      case 108:
         return jjMoveStringLiteralDfa13_0(active0, 0x2000000000ULL, active1, 0L);
      case 109:
         return jjMoveStringLiteralDfa13_0(active0, 0x800000000ULL, active1, 0L);
      case 110:
         if ((active0 & 0x200000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(12, 57, 9);
         return jjMoveStringLiteralDfa13_0(active0, 0x18400000000000ULL, active1, 0x1002000ULL);
      case 111:
         return jjMoveStringLiteralDfa13_0(active0, 0x2000000ULL, active1, 0x6020000ULL);
      case 112:
         return jjMoveStringLiteralDfa13_0(active0, 0x400000000000000ULL, active1, 0L);
      case 114:
         return jjMoveStringLiteralDfa13_0(active0, 0x100000000ULL, active1, 0x80ULL);
      case 115:
         return jjMoveStringLiteralDfa13_0(active0, 0x102000000000000ULL, active1, 0x800000ULL);
      case 116:
         return jjMoveStringLiteralDfa13_0(active0, 0xe000100020810000ULL, active1, 0x41100ULL);
      case 117:
         return jjMoveStringLiteralDfa13_0(active0, 0xc004000000ULL, active1, 0L);
      default :
         break;
   }
   return jjStartNfa_0(11, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa13_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(11, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(12, active0, active1);
      return 13;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 70:
         return jjMoveStringLiteralDfa14_0(active0, 0x2000000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa14_0(active0, 0x10000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa14_0(active0, 0x8000000000000000ULL, active1, 0x1000000ULL);
      case 97:
         return jjMoveStringLiteralDfa14_0(active0, 0x18000004000000ULL, active1, 0x1900ULL);
      case 99:
         return jjMoveStringLiteralDfa14_0(active0, 0x200000000ULL, active1, 0x14ULL);
      case 101:
         if ((active0 & 0x8000000000ULL) != 0L)
            return jjStartNfaWithStates_0(13, 39, 9);
         return jjMoveStringLiteralDfa14_0(active0, 0xd00004800000000ULL, active1, 0x800061ULL);
      case 105:
         return jjMoveStringLiteralDfa14_0(active0, 0x20160100000000ULL, active1, 0x2000ULL);
      case 106:
         return jjMoveStringLiteralDfa14_0(active0, 0L, active1, 0x8ULL);
      case 108:
         if ((active1 & 0x8000ULL) != 0L)
            return jjStartNfaWithStates_0(13, 79, 9);
         break;
      case 110:
         if ((active1 & 0x20000ULL) != 0L)
            return jjStartNfaWithStates_0(13, 81, 9);
         break;
      case 111:
         return jjMoveStringLiteralDfa14_0(active0, 0L, active1, 0x402ULL);
      case 112:
         return jjMoveStringLiteralDfa14_0(active0, 0x2000000ULL, active1, 0x6000000ULL);
      case 114:
         return jjMoveStringLiteralDfa14_0(active0, 0x80000000000ULL, active1, 0x200200ULL);
      case 115:
         return jjMoveStringLiteralDfa14_0(active0, 0x81000000000000ULL, active1, 0x100000ULL);
      case 116:
         return jjMoveStringLiteralDfa14_0(active0, 0x1000400000020000ULL, active1, 0x80080ULL);
      case 117:
         return jjMoveStringLiteralDfa14_0(active0, 0x2000000000ULL, active1, 0L);
      case 118:
         return jjMoveStringLiteralDfa14_0(active0, 0x40000ULL, active1, 0x10000ULL);
      case 121:
         if ((active0 & 0x800000ULL) != 0L)
         {
            jjmatchedKind = 23;
            jjmatchedPos = 13;
         }
         return jjMoveStringLiteralDfa14_0(active0, 0x6000000020010000ULL, active1, 0x40000ULL);
      default :
         break;
   }
   return jjStartNfa_0(12, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa14_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(12, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(13, active0, active1);
      return 14;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa15_0(active0, 0x10000ULL, active1, 0x40000ULL);
      case 67:
         return jjMoveStringLiteralDfa15_0(active0, 0x20000000ULL, active1, 0L);
      case 68:
         return jjMoveStringLiteralDfa15_0(active0, 0x2000000000000000ULL, active1, 0L);
      case 70:
         return jjMoveStringLiteralDfa15_0(active0, 0x1000000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa15_0(active0, 0x400000000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa15_0(active0, 0x1000000000000000ULL, active1, 0x81100ULL);
      case 82:
         return jjMoveStringLiteralDfa15_0(active0, 0x4000000000000000ULL, active1, 0L);
      case 99:
         return jjMoveStringLiteralDfa15_0(active0, 0x800000100000000ULL, active1, 0x61ULL);
      case 100:
         return jjMoveStringLiteralDfa15_0(active0, 0x80000000000ULL, active1, 0L);
      case 101:
         return jjMoveStringLiteralDfa15_0(active0, 0x2002000000ULL, active1, 0x6000008ULL);
      case 102:
         if ((active0 & 0x10000000ULL) != 0L)
            return jjStartNfaWithStates_0(14, 28, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa15_0(active0, 0x40000ULL, active1, 0x10000ULL);
      case 108:
         if ((active0 & 0x4000000ULL) != 0L)
            return jjStartNfaWithStates_0(14, 26, 9);
         return jjMoveStringLiteralDfa15_0(active0, 0x18000000000000ULL, active1, 0L);
      case 109:
         return jjMoveStringLiteralDfa15_0(active0, 0L, active1, 0x400ULL);
      case 110:
         return jjMoveStringLiteralDfa15_0(active0, 0x20060800000000ULL, active1, 0x802ULL);
      case 111:
         return jjMoveStringLiteralDfa15_0(active0, 0x100000020000ULL, active1, 0x200200ULL);
      case 114:
         return jjMoveStringLiteralDfa15_0(active0, 0x8402000000000000ULL, active1, 0x1800000ULL);
      case 115:
         if ((active0 & 0x100000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(14, 56, 9);
         return jjMoveStringLiteralDfa15_0(active0, 0x80004000000000ULL, active1, 0x100000ULL);
      case 116:
         return jjMoveStringLiteralDfa15_0(active0, 0x200000000ULL, active1, 0x2014ULL);
      case 121:
         return jjMoveStringLiteralDfa15_0(active0, 0L, active1, 0x80ULL);
      default :
         break;
   }
   return jjStartNfa_0(13, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa15_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(13, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(14, active0, active1);
      return 15;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 70:
         return jjMoveStringLiteralDfa16_0(active0, 0x4000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa16_0(active0, 0L, active1, 0x80ULL);
      case 80:
         return jjMoveStringLiteralDfa16_0(active0, 0L, active1, 0x14ULL);
      case 97:
         return jjMoveStringLiteralDfa16_0(active0, 0x4020060000000000ULL, active1, 0x402ULL);
      case 99:
         return jjMoveStringLiteralDfa16_0(active0, 0L, active1, 0x8ULL);
      case 100:
         return jjMoveStringLiteralDfa16_0(active0, 0x40000ULL, active1, 0x10000ULL);
      case 101:
         return jjMoveStringLiteralDfa16_0(active0, 0x80000000000000ULL, active1, 0x100000ULL);
      case 102:
         if ((active0 & 0x400000000000ULL) != 0L)
            return jjStartNfaWithStates_0(15, 46, 9);
         break;
      case 103:
         return jjMoveStringLiteralDfa16_0(active0, 0L, active1, 0x800ULL);
      case 104:
         return jjMoveStringLiteralDfa16_0(active0, 0x20000000ULL, active1, 0L);
      case 105:
         return jjMoveStringLiteralDfa16_0(active0, 0x18080200000000ULL, active1, 0x2000ULL);
      case 109:
         if ((active0 & 0x20000ULL) != 0L)
            return jjStartNfaWithStates_0(15, 17, 9);
         break;
      case 110:
         return jjMoveStringLiteralDfa16_0(active0, 0x100000000000ULL, active1, 0L);
      case 111:
         return jjMoveStringLiteralDfa16_0(active0, 0xa002000000000000ULL, active1, 0x1000000ULL);
      case 112:
         return jjMoveStringLiteralDfa16_0(active0, 0L, active1, 0x200200ULL);
      case 114:
         return jjMoveStringLiteralDfa16_0(active0, 0x1001000002000000ULL, active1, 0x6081100ULL);
      case 115:
         return jjMoveStringLiteralDfa16_0(active0, 0x2000000000ULL, active1, 0x40000ULL);
      case 116:
         return jjMoveStringLiteralDfa16_0(active0, 0xc00000900010000ULL, active1, 0x800061ULL);
      default :
         break;
   }
   return jjStartNfa_0(14, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa16_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(14, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(15, active0, active1);
      return 16;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 70:
         return jjMoveStringLiteralDfa17_0(active0, 0x2000000000ULL, active1, 0L);
      case 79:
         return jjMoveStringLiteralDfa17_0(active0, 0x100800000000ULL, active1, 0L);
      case 80:
         return jjMoveStringLiteralDfa17_0(active0, 0x800000000000000ULL, active1, 0x61ULL);
      case 97:
         return jjMoveStringLiteralDfa17_0(active0, 0x20000000ULL, active1, 0L);
      case 101:
         if ((active1 & 0x800ULL) != 0L)
            return jjStartNfaWithStates_0(16, 75, 9);
         return jjMoveStringLiteralDfa17_0(active0, 0L, active1, 0x200200ULL);
      case 102:
         if ((active1 & 0x80ULL) != 0L)
            return jjStartNfaWithStates_0(16, 71, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa17_0(active0, 0x100000000ULL, active1, 0x800400ULL);
      case 108:
         return jjMoveStringLiteralDfa17_0(active0, 0x20060000000000ULL, active1, 0x2ULL);
      case 109:
         if ((active0 & 0x2000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(16, 49, 9);
         return jjMoveStringLiteralDfa17_0(active0, 0x2000000000000000ULL, active1, 0L);
      case 110:
         return jjMoveStringLiteralDfa17_0(active0, 0x4000080000000000ULL, active1, 0L);
      case 111:
         return jjMoveStringLiteralDfa17_0(active0, 0x1001000200010000ULL, active1, 0x83100ULL);
      case 112:
         return jjMoveStringLiteralDfa17_0(active0, 0x8000000000000000ULL, active1, 0x1000000ULL);
      case 114:
         return jjMoveStringLiteralDfa17_0(active0, 0x4000000000ULL, active1, 0x100014ULL);
      case 115:
         if ((active0 & 0x80000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(16, 55, 9);
         return jjMoveStringLiteralDfa17_0(active0, 0L, active1, 0x40000ULL);
      case 116:
         return jjMoveStringLiteralDfa17_0(active0, 0x18000002000000ULL, active1, 0x6000008ULL);
      case 117:
         return jjMoveStringLiteralDfa17_0(active0, 0x40000ULL, active1, 0x10000ULL);
      case 121:
         return jjMoveStringLiteralDfa17_0(active0, 0x400000000000000ULL, active1, 0L);
      default :
         break;
   }
   return jjStartNfa_0(15, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa17_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(15, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(16, active0, active1);
      return 17;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 79:
         return jjMoveStringLiteralDfa18_0(active0, 0x400000000000000ULL, active1, 0x2ULL);
      case 80:
         return jjMoveStringLiteralDfa18_0(active0, 0L, active1, 0x8ULL);
      case 97:
         return jjMoveStringLiteralDfa18_0(active0, 0x2000080000040000ULL, active1, 0x10000ULL);
      case 101:
         return jjMoveStringLiteralDfa18_0(active0, 0x8000000000000000ULL, active1, 0x1040000ULL);
      case 102:
         if ((active0 & 0x800000000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 35, 9);
         else if ((active0 & 0x100000000000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 44, 9);
         break;
      case 103:
         return jjMoveStringLiteralDfa18_0(active0, 0x4000000000000000ULL, active1, 0L);
      case 105:
         return jjMoveStringLiteralDfa18_0(active0, 0x20060020000000ULL, active1, 0L);
      case 109:
         if ((active0 & 0x10000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 16, 9);
         else if ((active0 & 0x1000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 48, 9);
         break;
      case 110:
         if ((active1 & 0x400ULL) != 0L)
            return jjStartNfaWithStates_0(17, 74, 9);
         else if ((active1 & 0x2000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 77, 9);
         return jjMoveStringLiteralDfa18_0(active0, 0x200000000ULL, active1, 0L);
      case 111:
         return jjMoveStringLiteralDfa18_0(active0, 0x4100000000ULL, active1, 0x800014ULL);
      case 112:
         return jjMoveStringLiteralDfa18_0(active0, 0x1000000000000000ULL, active1, 0x81100ULL);
      case 114:
         return jjMoveStringLiteralDfa18_0(active0, 0x800002000000000ULL, active1, 0x200261ULL);
      case 116:
         return jjMoveStringLiteralDfa18_0(active0, 0L, active1, 0x100000ULL);
      case 121:
         if ((active0 & 0x2000000ULL) != 0L)
         {
            jjmatchedKind = 25;
            jjmatchedPos = 17;
         }
         else if ((active0 & 0x8000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 51, 9);
         else if ((active0 & 0x10000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(17, 52, 9);
         return jjMoveStringLiteralDfa18_0(active0, 0L, active1, 0x6000000ULL);
      default :
         break;
   }
   return jjStartNfa_0(16, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa18_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(16, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(17, active0, active1);
      return 18;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 68:
         return jjMoveStringLiteralDfa19_0(active0, 0L, active1, 0x2000000ULL);
      case 79:
         return jjMoveStringLiteralDfa19_0(active0, 0x200000000ULL, active1, 0L);
      case 82:
         return jjMoveStringLiteralDfa19_0(active0, 0L, active1, 0x4000000ULL);
      case 98:
         return jjMoveStringLiteralDfa19_0(active0, 0L, active1, 0x2ULL);
      case 101:
         if ((active0 & 0x4000000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 62, 9);
         return jjMoveStringLiteralDfa19_0(active0, 0x1000000000000000ULL, active1, 0x81100ULL);
      case 102:
         if ((active0 & 0x400000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 58, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa19_0(active0, 0x2000000000000000ULL, active1, 0x100000ULL);
      case 108:
         return jjMoveStringLiteralDfa19_0(active0, 0x80000040000ULL, active1, 0x10000ULL);
      case 109:
         if ((active0 & 0x4000000000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 38, 9);
         break;
      case 110:
         if ((active0 & 0x20000000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 29, 9);
         else if ((active0 & 0x100000000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 32, 9);
         else if ((active1 & 0x800000ULL) != 0L)
            return jjStartNfaWithStates_0(18, 87, 9);
         break;
      case 111:
         return jjMoveStringLiteralDfa19_0(active0, 0x800002000000000ULL, active1, 0x61ULL);
      case 112:
         return jjMoveStringLiteralDfa19_0(active0, 0L, active1, 0x14ULL);
      case 114:
         return jjMoveStringLiteralDfa19_0(active0, 0x8000000000000000ULL, active1, 0x1040008ULL);
      case 116:
         return jjMoveStringLiteralDfa19_0(active0, 0x20060000000000ULL, active1, 0x200200ULL);
      default :
         break;
   }
   return jjStartNfa_0(17, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa19_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(17, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(18, active0, active1);
      return 19;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 97:
         return jjMoveStringLiteralDfa20_0(active0, 0L, active1, 0x4000000ULL);
      case 101:
         return jjMoveStringLiteralDfa20_0(active0, 0L, active1, 0x14ULL);
      case 102:
         if ((active0 & 0x200000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 33, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa20_0(active0, 0x80000000000ULL, active1, 0x200ULL);
      case 106:
         return jjMoveStringLiteralDfa20_0(active0, 0L, active1, 0x2ULL);
      case 109:
         if ((active0 & 0x2000000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 37, 9);
         break;
      case 110:
         if ((active0 & 0x2000000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 61, 9);
         break;
      case 111:
         return jjMoveStringLiteralDfa20_0(active0, 0L, active1, 0x2100008ULL);
      case 112:
         return jjMoveStringLiteralDfa20_0(active0, 0x800000000000000ULL, active1, 0x61ULL);
      case 114:
         return jjMoveStringLiteralDfa20_0(active0, 0x1000000000000000ULL, active1, 0x81100ULL);
      case 115:
         if ((active1 & 0x10000ULL) != 0L)
         {
            jjmatchedKind = 80;
            jjmatchedPos = 19;
         }
         return jjMoveStringLiteralDfa20_0(active0, 0x40000ULL, active1, 0L);
      case 116:
         return jjMoveStringLiteralDfa20_0(active0, 0x8000000000000000ULL, active1, 0x1040000ULL);
      case 121:
         if ((active0 & 0x20000000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 41, 9);
         else if ((active0 & 0x40000000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 42, 9);
         else if ((active0 & 0x20000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(19, 53, 9);
         return jjMoveStringLiteralDfa20_0(active0, 0L, active1, 0x200000ULL);
      default :
         break;
   }
   return jjStartNfa_0(18, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa20_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(18, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(19, active0, active1);
      return 20;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa21_0(active0, 0x40000ULL, active1, 0x200000ULL);
      case 101:
         return jjMoveStringLiteralDfa21_0(active0, 0x800000000000000ULL, active1, 0x263ULL);
      case 105:
         return jjMoveStringLiteralDfa21_0(active0, 0x8000000000000000ULL, active1, 0x40000ULL);
      case 109:
         return jjMoveStringLiteralDfa21_0(active0, 0L, active1, 0x2000000ULL);
      case 110:
         if ((active1 & 0x100000ULL) != 0L)
            return jjStartNfaWithStates_0(20, 84, 9);
         return jjMoveStringLiteralDfa21_0(active0, 0L, active1, 0x4000000ULL);
      case 112:
         return jjMoveStringLiteralDfa21_0(active0, 0L, active1, 0x8ULL);
      case 114:
         return jjMoveStringLiteralDfa21_0(active0, 0L, active1, 0x14ULL);
      case 116:
         return jjMoveStringLiteralDfa21_0(active0, 0x1000080000000000ULL, active1, 0x81100ULL);
      case 121:
         return jjMoveStringLiteralDfa21_0(active0, 0L, active1, 0x1000000ULL);
      default :
         break;
   }
   return jjStartNfa_0(19, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa21_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(19, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(20, active0, active1);
      return 21;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 79:
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x1000000ULL);
      case 97:
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x2000000ULL);
      case 99:
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x2ULL);
      case 101:
         return jjMoveStringLiteralDfa22_0(active0, 0x8000000000000000ULL, active1, 0x8ULL);
      case 103:
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x4000000ULL);
      case 105:
         return jjMoveStringLiteralDfa22_0(active0, 0x1000000000000000ULL, active1, 0x100ULL);
      case 111:
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x40000ULL);
      case 114:
         return jjMoveStringLiteralDfa22_0(active0, 0x800000000000000ULL, active1, 0x61ULL);
      case 115:
         if ((active1 & 0x200ULL) != 0L)
            return jjStartNfaWithStates_0(21, 73, 9);
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x200000ULL);
      case 116:
         return jjMoveStringLiteralDfa22_0(active0, 0x40000ULL, active1, 0x14ULL);
      case 121:
         if ((active0 & 0x80000000000ULL) != 0L)
            return jjStartNfaWithStates_0(21, 43, 9);
         else if ((active1 & 0x1000ULL) != 0L)
            return jjStartNfaWithStates_0(21, 76, 9);
         return jjMoveStringLiteralDfa22_0(active0, 0L, active1, 0x80000ULL);
      default :
         break;
   }
   return jjStartNfa_0(20, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa22_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(20, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(21, active0, active1);
      return 22;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 65:
         return jjMoveStringLiteralDfa23_0(active0, 0L, active1, 0x80000ULL);
      case 101:
         if ((active1 & 0x4000000ULL) != 0L)
            return jjStartNfaWithStates_0(22, 90, 9);
         return jjMoveStringLiteralDfa23_0(active0, 0x1000000000000000ULL, active1, 0x100ULL);
      case 102:
         if ((active1 & 0x1000000ULL) != 0L)
            return jjStartNfaWithStates_0(22, 88, 9);
         break;
      case 105:
         return jjMoveStringLiteralDfa23_0(active0, 0L, active1, 0x2000000ULL);
      case 110:
         if ((active1 & 0x40000ULL) != 0L)
            return jjStartNfaWithStates_0(22, 82, 9);
         break;
      case 111:
         return jjMoveStringLiteralDfa23_0(active0, 0x40000ULL, active1, 0L);
      case 114:
         return jjMoveStringLiteralDfa23_0(active0, 0L, active1, 0x8ULL);
      case 115:
         if ((active0 & 0x8000000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(22, 63, 9);
         return jjMoveStringLiteralDfa23_0(active0, 0L, active1, 0x200000ULL);
      case 116:
         return jjMoveStringLiteralDfa23_0(active0, 0x800000000000000ULL, active1, 0x63ULL);
      case 121:
         if ((active1 & 0x4ULL) != 0L)
            return jjStartNfaWithStates_0(22, 66, 9);
         else if ((active1 & 0x10ULL) != 0L)
            return jjStartNfaWithStates_0(22, 68, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(21, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa23_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(21, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(22, active0, active1);
      return 23;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 80:
         return jjMoveStringLiteralDfa24_0(active0, 0L, active1, 0x2ULL);
      case 101:
         return jjMoveStringLiteralDfa24_0(active0, 0L, active1, 0x200000ULL);
      case 105:
         return jjMoveStringLiteralDfa24_0(active0, 0x800000000000000ULL, active1, 0L);
      case 109:
         if ((active0 & 0x40000ULL) != 0L)
            return jjStartNfaWithStates_0(23, 18, 9);
         break;
      case 110:
         if ((active1 & 0x2000000ULL) != 0L)
            return jjStartNfaWithStates_0(23, 89, 9);
         break;
      case 115:
         if ((active0 & 0x1000000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(23, 60, 9);
         else if ((active1 & 0x100ULL) != 0L)
            return jjStartNfaWithStates_0(23, 72, 9);
         return jjMoveStringLiteralDfa24_0(active0, 0L, active1, 0x80000ULL);
      case 116:
         return jjMoveStringLiteralDfa24_0(active0, 0L, active1, 0x8ULL);
      case 121:
         if ((active1 & 0x1ULL) != 0L)
            return jjStartNfaWithStates_0(23, 64, 9);
         else if ((active1 & 0x20ULL) != 0L)
            return jjStartNfaWithStates_0(23, 69, 9);
         else if ((active1 & 0x40ULL) != 0L)
            return jjStartNfaWithStates_0(23, 70, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(22, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa24_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(22, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(23, active0, active1);
      return 24;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 101:
         return jjMoveStringLiteralDfa25_0(active0, 0x800000000000000ULL, active1, 0L);
      case 114:
         return jjMoveStringLiteralDfa25_0(active0, 0L, active1, 0x200002ULL);
      case 115:
         return jjMoveStringLiteralDfa25_0(active0, 0L, active1, 0x80000ULL);
      case 121:
         if ((active1 & 0x8ULL) != 0L)
            return jjStartNfaWithStates_0(24, 67, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(23, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa25_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(23, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(24, active0, active1);
      return 25;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 101:
         return jjMoveStringLiteralDfa26_0(active0, 0L, active1, 0x80000ULL);
      case 111:
         return jjMoveStringLiteralDfa26_0(active0, 0L, active1, 0x2ULL);
      case 115:
         if ((active0 & 0x800000000000000ULL) != 0L)
            return jjStartNfaWithStates_0(25, 59, 9);
         break;
      case 116:
         return jjMoveStringLiteralDfa26_0(active0, 0L, active1, 0x200000ULL);
      default :
         break;
   }
   return jjStartNfa_0(24, active0, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa26_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1){
   if (((active0 &= old0) | (active1 &= old1)) == 0L)
      return jjStartNfa_0(24, old0, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(25, 0L, active1);
      return 26;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 105:
         return jjMoveStringLiteralDfa27_0(active1, 0x200000ULL);
      case 112:
         return jjMoveStringLiteralDfa27_0(active1, 0x2ULL);
      case 114:
         return jjMoveStringLiteralDfa27_0(active1, 0x80000ULL);
      default :
         break;
   }
   return jjStartNfa_0(25, 0L, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa27_0(unsigned long long old1, unsigned long long active1){
   if (((active1 &= old1)) == 0L)
      return jjStartNfa_0(25, 0L, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(26, 0L, active1);
      return 27;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 101:
         return jjMoveStringLiteralDfa28_0(active1, 0x2ULL);
      case 111:
         return jjMoveStringLiteralDfa28_0(active1, 0x200000ULL);
      case 116:
         return jjMoveStringLiteralDfa28_0(active1, 0x80000ULL);
      default :
         break;
   }
   return jjStartNfa_0(26, 0L, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa28_0(unsigned long long old1, unsigned long long active1){
   if (((active1 &= old1)) == 0L)
      return jjStartNfa_0(26, 0L, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(27, 0L, active1);
      return 28;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 105:
         return jjMoveStringLiteralDfa29_0(active1, 0x80000ULL);
      case 110:
         if ((active1 & 0x200000ULL) != 0L)
            return jjStartNfaWithStates_0(28, 85, 9);
         break;
      case 114:
         return jjMoveStringLiteralDfa29_0(active1, 0x2ULL);
      default :
         break;
   }
   return jjStartNfa_0(27, 0L, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa29_0(unsigned long long old1, unsigned long long active1){
   if (((active1 &= old1)) == 0L)
      return jjStartNfa_0(27, 0L, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(28, 0L, active1);
      return 29;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 111:
         return jjMoveStringLiteralDfa30_0(active1, 0x80000ULL);
      case 116:
         return jjMoveStringLiteralDfa30_0(active1, 0x2ULL);
      default :
         break;
   }
   return jjStartNfa_0(28, 0L, active1);
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa30_0(unsigned long long old1, unsigned long long active1){
   if (((active1 &= old1)) == 0L)
      return jjStartNfa_0(28, 0L, old1);
   if (input_stream->endOfInput()) {
      jjStopStringLiteralDfa_0(29, 0L, active1);
      return 30;
   }
   curChar = input_stream->readChar();
   switch(curChar)
   {
      case 110:
         if ((active1 & 0x80000ULL) != 0L)
            return jjStartNfaWithStates_0(30, 83, 9);
         break;
      case 121:
         if ((active1 & 0x2ULL) != 0L)
            return jjStartNfaWithStates_0(30, 65, 9);
         break;
      default :
         break;
   }
   return jjStartNfa_0(29, 0L, active1);
}

int OWL2FunctionalStyleParserTokenManager::jjStartNfaWithStates_0(int pos, int kind, int state){
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   if (input_stream->endOfInput()) { return pos + 1; }
   curChar = input_stream->readChar();
   return jjMoveNfa_0(state, pos + 1);
}

int OWL2FunctionalStyleParserTokenManager::jjMoveNfa_0(int startState, int curPos){
   int startsAt = 0;
   jjnewStateCnt = 9;
   int i = 1;
   jjstateSet[0] = startState;
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         unsigned long long l = 1ULL << curChar;
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 9:
                  if ((0x3ff600000000000ULL & l) != 0L)
                     { jjCheckNAddTwoStates(6, 7); }
                  else if (curChar == 58)
                  {
                     if (kind > 93)
                        kind = 93;
                  }
                  if ((0x3ff200000000000ULL & l) != 0L)
                     jjstateSet[jjnewStateCnt++] = 8;
                  break;
               case 1:
                  if ((0x3ff000000000000ULL & l) != 0L)
                  {
                     if (kind > 91)
                        kind = 91;
                     { jjCheckNAdd(0); }
                  }
                  else if (curChar == 58)
                  {
                     if (kind > 93)
                        kind = 93;
                  }
                  break;
               case 0:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 91)
                     kind = 91;
                  { jjCheckNAdd(0); }
                  break;
               case 3:
                  if (curChar == 45)
                     { jjCheckNAdd(4); }
                  break;
               case 4:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 92)
                     kind = 92;
                  { jjCheckNAddTwoStates(3, 4); }
                  break;
               case 6:
                  if ((0x3ff600000000000ULL & l) != 0L)
                     { jjCheckNAddTwoStates(6, 7); }
                  break;
               case 7:
                  if ((0x3ff200000000000ULL & l) != 0L)
                     jjstateSet[jjnewStateCnt++] = 8;
                  break;
               case 8:
                  if (curChar == 58)
                     kind = 93;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         unsigned long long l = 1ULL << (curChar & 077);
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 9:
                  if ((0x7fffffe87fffffeULL & l) != 0L)
                     { jjCheckNAdd(8); }
                  if ((0x7fffffe87fffffeULL & l) != 0L)
                     { jjCheckNAddTwoStates(6, 7); }
                  break;
               case 1:
                  if ((0x7fffffe07fffffeULL & l) != 0L)
                     { jjCheckNAddStates(0, 2); }
                  else if (curChar == 64)
                     { jjCheckNAdd(2); }
                  break;
               case 2:
                  if ((0x7fffffe07fffffeULL & l) == 0L)
                     break;
                  if (kind > 92)
                     kind = 92;
                  { jjCheckNAddTwoStates(2, 3); }
                  break;
               case 4:
                  if ((0x7fffffe07fffffeULL & l) == 0L)
                     break;
                  if (kind > 92)
                     kind = 92;
                  { jjCheckNAddTwoStates(3, 4); }
                  break;
               case 5:
                  if ((0x7fffffe07fffffeULL & l) != 0L)
                     { jjCheckNAddStates(0, 2); }
                  break;
               case 6:
                  if ((0x7fffffe87fffffeULL & l) != 0L)
                     { jjCheckNAddTwoStates(6, 7); }
                  break;
               case 7:
                  if ((0x7fffffe87fffffeULL & l) != 0L)
                     { jjCheckNAdd(8); }
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int hiByte = (curChar >> 8);
         int i1 = hiByte >> 6;
         unsigned long long l1 = 1ULL << (hiByte & 077);
         int i2 = (curChar & 0xff) >> 6;
         unsigned long long l2 = 1ULL << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 9:
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2))
                     { jjCheckNAddTwoStates(6, 7); }
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2))
                     { jjCheckNAdd(8); }
                  break;
               case 1:
                  if (jjCanMove_0(hiByte, i1, i2, l1, l2))
                     { jjCheckNAddStates(0, 2); }
                  break;
               case 6:
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2))
                     { jjCheckNAddTwoStates(6, 7); }
                  break;
               case 7:
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2))
                     { jjCheckNAdd(8); }
                  break;
               default : if (i1 == 0 || l1 == 0 || i2 == 0 ||  l2 == 0) break; else break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt), (jjnewStateCnt = startsAt), (i == (startsAt = 9 - startsAt)))
         return curPos;
      if (input_stream->endOfInput()) { return curPos; }
      curChar = input_stream->readChar();
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_3(){
   return jjMoveNfa_3(0, 0);
}

int OWL2FunctionalStyleParserTokenManager::jjMoveNfa_3(int startState, int curPos){
   int startsAt = 0;
   jjnewStateCnt = 3;
   int i = 1;
   jjstateSet[0] = startState;
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         unsigned long long l = 1ULL << curChar;
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0x3ff000000000000ULL & l) == 0L)
                     break;
                  if (kind > 105)
                     kind = 105;
                  { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 1:
                  if ((0x3ff600000000000ULL & l) != 0L)
                     { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 2:
                  if ((0x3ff200000000000ULL & l) != 0L && kind > 105)
                     kind = 105;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         unsigned long long l = 1ULL << (curChar & 077);
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0x7fffffe87fffffeULL & l) == 0L)
                     break;
                  if (kind > 105)
                     kind = 105;
                  { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 1:
                  if ((0x7fffffe87fffffeULL & l) != 0L)
                     { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 2:
                  if ((0x7fffffe87fffffeULL & l) != 0L && kind > 105)
                     kind = 105;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int hiByte = (curChar >> 8);
         int i1 = hiByte >> 6;
         unsigned long long l1 = 1ULL << (hiByte & 077);
         int i2 = (curChar & 0xff) >> 6;
         unsigned long long l2 = 1ULL << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if (!jjCanMove_0(hiByte, i1, i2, l1, l2))
                     break;
                  if (kind > 105)
                     kind = 105;
                  { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 1:
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2))
                     { jjCheckNAddTwoStates(1, 2); }
                  break;
               case 2:
                  if (jjCanMove_1(hiByte, i1, i2, l1, l2) && kind > 105)
                     kind = 105;
                  break;
               default : if (i1 == 0 || l1 == 0 || i2 == 0 ||  l2 == 0) break; else break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt), (jjnewStateCnt = startsAt), (i == (startsAt = 3 - startsAt)))
         return curPos;
      if (input_stream->endOfInput()) { return curPos; }
      curChar = input_stream->readChar();
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_5(){
   switch(curChar)
   {
      case 34:
         return jjStopAtPos(0, 108);
      default :
         return 1;
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_1(){
   return jjMoveNfa_1(0, 0);
}

int OWL2FunctionalStyleParserTokenManager::jjMoveNfa_1(int startState, int curPos){
   int startsAt = 0;
   jjnewStateCnt = 1;
   int i = 1;
   jjstateSet[0] = startState;
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         unsigned long long l = 1ULL << curChar;
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0xaffffffa00000000ULL & l) == 0L)
                     break;
                  kind = 103;
                  jjstateSet[jjnewStateCnt++] = 0;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         unsigned long long l = 1ULL << (curChar & 077);
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0xc7fffffeafffffffULL & l) == 0L)
                     break;
                  kind = 103;
                  jjstateSet[jjnewStateCnt++] = 0;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int hiByte = (curChar >> 8);
         int i1 = hiByte >> 6;
         unsigned long long l1 = 1ULL << (hiByte & 077);
         int i2 = (curChar & 0xff) >> 6;
         unsigned long long l2 = 1ULL << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if (!jjCanMove_2(hiByte, i1, i2, l1, l2))
                     break;
                  if (kind > 103)
                     kind = 103;
                  jjstateSet[jjnewStateCnt++] = 0;
                  break;
               default : if (i1 == 0 || l1 == 0 || i2 == 0 ||  l2 == 0) break; else break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt), (jjnewStateCnt = startsAt), (i == (startsAt = 1 - startsAt)))
         return curPos;
      if (input_stream->endOfInput()) { return curPos; }
      curChar = input_stream->readChar();
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_6(){
   switch(curChar)
   {
      case 10:
         return jjStopAtPos(0, 109);
      case 13:
         return jjStopAtPos(0, 110);
      default :
         return 1;
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_2(){
   switch(curChar)
   {
      case 62:
         return jjStopAtPos(0, 104);
      default :
         return 1;
   }
}

 int  OWL2FunctionalStyleParserTokenManager::jjMoveStringLiteralDfa0_4(){
   return jjMoveNfa_4(3, 0);
}

int OWL2FunctionalStyleParserTokenManager::jjMoveNfa_4(int startState, int curPos){
   int startsAt = 0;
   jjnewStateCnt = 3;
   int i = 1;
   jjstateSet[0] = startState;
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         unsigned long long l = 1ULL << curChar;
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 3:
               case 0:
                  if ((0xfffffffbffffffffULL & l) == 0L)
                     break;
                  if (kind > 107)
                     kind = 107;
                  { jjCheckNAddTwoStates(0, 1); }
                  break;
               case 2:
                  if ((0x8400000000ULL & l) == 0L)
                     break;
                  if (kind > 107)
                     kind = 107;
                  { jjCheckNAddTwoStates(0, 1); }
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         unsigned long long l = 1ULL << (curChar & 077);
         (void)l;
         do
         {
            switch(jjstateSet[--i])
            {
               case 3:
                  if ((0xffffffffefffffffULL & l) != 0L)
                  {
                     if (kind > 107)
                        kind = 107;
                     { jjCheckNAddTwoStates(0, 1); }
                  }
                  else if (curChar == 92)
                     jjstateSet[jjnewStateCnt++] = 2;
                  break;
               case 0:
                  if ((0xffffffffefffffffULL & l) == 0L)
                     break;
                  kind = 107;
                  { jjCheckNAddTwoStates(0, 1); }
                  break;
               case 1:
                  if (curChar == 92)
                     jjstateSet[jjnewStateCnt++] = 2;
                  break;
               case 2:
                  if (curChar != 92)
                     break;
                  if (kind > 107)
                     kind = 107;
                  { jjCheckNAddTwoStates(0, 1); }
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int hiByte = (curChar >> 8);
         int i1 = hiByte >> 6;
         unsigned long long l1 = 1ULL << (hiByte & 077);
         int i2 = (curChar & 0xff) >> 6;
         unsigned long long l2 = 1ULL << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 3:
               case 0:
                  if (!jjCanMove_2(hiByte, i1, i2, l1, l2))
                     break;
                  if (kind > 107)
                     kind = 107;
                  { jjCheckNAddTwoStates(0, 1); }
                  break;
               default : if (i1 == 0 || l1 == 0 || i2 == 0 ||  l2 == 0) break; else break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt), (jjnewStateCnt = startsAt), (i == (startsAt = 3 - startsAt)))
         return curPos;
      if (input_stream->endOfInput()) { return curPos; }
      curChar = input_stream->readChar();
   }
}

bool OWL2FunctionalStyleParserTokenManager::jjCanMove_0(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2){
   switch(hiByte)
   {
      case 0:
         return ((jjbitVec2[i2] & l2) != 0L);
      case 3:
         return ((jjbitVec3[i2] & l2) != 0L);
      case 32:
         return ((jjbitVec4[i2] & l2) != 0L);
      case 33:
         return ((jjbitVec5[i2] & l2) != 0L);
      case 47:
         return ((jjbitVec6[i2] & l2) != 0L);
      case 48:
         return ((jjbitVec7[i2] & l2) != 0L);
      case 253:
         return ((jjbitVec8[i2] & l2) != 0L);
      case 255:
         return ((jjbitVec9[i2] & l2) != 0L);
      default :
         if ((jjbitVec0[i1] & l1) != 0L)
            return true;
         return false;
   }
}

bool OWL2FunctionalStyleParserTokenManager::jjCanMove_1(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2){
   switch(hiByte)
   {
      case 0:
         return ((jjbitVec10[i2] & l2) != 0L);
      case 3:
         return ((jjbitVec11[i2] & l2) != 0L);
      case 32:
         return ((jjbitVec12[i2] & l2) != 0L);
      case 33:
         return ((jjbitVec5[i2] & l2) != 0L);
      case 47:
         return ((jjbitVec6[i2] & l2) != 0L);
      case 48:
         return ((jjbitVec7[i2] & l2) != 0L);
      case 253:
         return ((jjbitVec8[i2] & l2) != 0L);
      case 255:
         return ((jjbitVec9[i2] & l2) != 0L);
      default :
         if ((jjbitVec0[i1] & l1) != 0L)
            return true;
         return false;
   }
}

bool OWL2FunctionalStyleParserTokenManager::jjCanMove_2(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2){
   switch(hiByte)
   {
      case 0:
         return ((jjbitVec13[i2] & l2) != 0L);
      default :
         if ((jjbitVec7[i1] & l1) != 0L)
            return true;
         return false;
   }
}

/** Token literal values. */

Token * OWL2FunctionalStyleParserTokenManager::jjFillToken(){
   Token *t;
   JAVACC_STRING_TYPE curTokenImage;
   int beginLine;
   int endLine;
   int beginColumn;
   int endColumn;
   if (jjmatchedPos < 0)
   {
       curTokenImage = image.c_str();
   if (input_stream->getTrackLineColumn()) {
      beginLine = endLine = input_stream->getEndLine();
      beginColumn = endColumn = input_stream->getEndColumn();
   }
   }
   else
   {
      JAVACC_STRING_TYPE im = jjstrLiteralImages[jjmatchedKind];
      curTokenImage = (im.length() == 0) ? input_stream->GetImage() : im;
   if (input_stream->getTrackLineColumn()) {
      beginLine = input_stream->getBeginLine();
      beginColumn = input_stream->getBeginColumn();
      endLine = input_stream->getEndLine();
      endColumn = input_stream->getEndColumn();
   }
   }
   t = Token::newToken(jjmatchedKind, curTokenImage);
   t->specialToken = NULL;
   t->next = NULL;

   if (input_stream->getTrackLineColumn()) {
   t->beginLine = beginLine;
   t->endLine = endLine;
   t->beginColumn = beginColumn;
   t->endColumn = endColumn;
   }

   return t;
}
const int defaultLexState = 0;
/** Get the next Token. */

Token * OWL2FunctionalStyleParserTokenManager::getNextToken(){
  Token *matchedToken;
  int curPos = 0;

  for (;;)
  {
   EOFLoop: 
   if (input_stream->endOfInput())
   {
      jjmatchedKind = 0;
      jjmatchedPos = -1;
      matchedToken = jjFillToken();
      return matchedToken;
   }
   curChar = input_stream->BeginToken();
   image = jjimage;
   image.clear();
   jjimageLen = 0;

   switch(curLexState)
   {
     case 0:
       { input_stream->backup(0);
          while (curChar <= 32 && (0x100002600ULL & (1ULL << curChar)) != 0L)
       {
       if (input_stream->endOfInput()) { goto EOFLoop; }
       curChar = input_stream->BeginToken();
       }
       }
       jjmatchedKind = 0x7fffffff;
       jjmatchedPos = 0;
       curPos = jjMoveStringLiteralDfa0_0();
       break;
     case 1:
       jjmatchedKind = 103;
       jjmatchedPos = -1;
       curPos = 0;
       curPos = jjMoveStringLiteralDfa0_1();
       break;
     case 2:
       jjmatchedKind = 0x7fffffff;
       jjmatchedPos = 0;
       curPos = jjMoveStringLiteralDfa0_2();
       break;
     case 3:
       jjmatchedKind = 106;
       jjmatchedPos = -1;
       curPos = 0;
       curPos = jjMoveStringLiteralDfa0_3();
       break;
     case 4:
       jjmatchedKind = 107;
       jjmatchedPos = -1;
       curPos = 0;
       curPos = jjMoveStringLiteralDfa0_4();
       break;
     case 5:
       jjmatchedKind = 0x7fffffff;
       jjmatchedPos = 0;
       curPos = jjMoveStringLiteralDfa0_5();
       break;
     case 6:
       jjmatchedKind = 0x7fffffff;
       jjmatchedPos = 0;
       curPos = jjMoveStringLiteralDfa0_6();
       if (jjmatchedPos == 0 && jjmatchedKind > 111)
       {
          jjmatchedKind = 111;
       }
       break;
   }
     if (jjmatchedKind != 0x7fffffff)
     {
        if (jjmatchedPos + 1 < curPos)
           input_stream->backup(curPos - jjmatchedPos - 1);
        if ((jjtoToken[jjmatchedKind >> 6] & (1ULL << (jjmatchedKind & 077))) != 0L)
        {
           matchedToken = jjFillToken();
           TokenLexicalActions(matchedToken);
       if (jjnewLexState[jjmatchedKind] != -1)
         curLexState = jjnewLexState[jjmatchedKind];
           return matchedToken;
        }
        else
        {
           SkipLexicalActions(NULL);
         if (jjnewLexState[jjmatchedKind] != -1)
           curLexState = jjnewLexState[jjmatchedKind];
           goto EOFLoop;
        }
     }
     int error_line = input_stream->getEndLine();
     int error_column = input_stream->getEndColumn();
     JAVACC_STRING_TYPE error_after;
     bool EOFSeen = false;
     if (input_stream->endOfInput()) {
        EOFSeen = true;
        error_after = curPos <= 1 ? EMPTY : input_stream->GetImage();
        if (curChar == '\n' || curChar == '\r') {
           error_line++;
           error_column = 0;
        }
        else
           error_column++;
     }
     if (!EOFSeen) {
        error_after = curPos <= 1 ? EMPTY : input_stream->GetImage();
     }
     errorHandler->lexicalError(EOFSeen, curLexState, error_line, error_column, error_after, curChar, this);
  }
}


void  OWL2FunctionalStyleParserTokenManager::SkipLexicalActions(Token *matchedToken){
   switch(jjmatchedKind)
   {
      default :
         break;
   }
}

void  OWL2FunctionalStyleParserTokenManager::TokenLexicalActions(Token *matchedToken){
   switch(jjmatchedKind)
   {
      default :
         break;
   }
}
  /** Reinitialise parser. */
  void OWL2FunctionalStyleParserTokenManager::ReInit(JAVACC_CHARSTREAM *stream, int lexState, OWL2FunctionalStyleParser *parserArg) {
    if (input_stream) delete input_stream;
    jjmatchedPos = jjnewStateCnt = 0;
    curLexState = lexState;
    input_stream = stream;
    ReInitRounds();
    debugStream = stdout; // init
    SwitchTo(lexState);
    parser = parserArg;
    errorHandler = new TokenManagerErrorHandler();
    errorHandlerCreated = true;
  }

  void OWL2FunctionalStyleParserTokenManager::ReInitRounds() {
    int i;
    jjround = 0x80000001;
    for (i = 9; i-- > 0;)
      jjrounds[i] = 0x80000000;
  }

  /** Switch to specified lex state. */
  void OWL2FunctionalStyleParserTokenManager::SwitchTo(int lexState) {
    if (lexState >= 7 || lexState < 0)
      assert(false);
      //throw 1;//new TokenMgrError("Error: Ignoring invalid lexical state : " + lexState + ". State unchanged.", TokenMgrError.INVALID_LEXICAL_STATE);
    else
      curLexState = lexState;
  }

  /** Constructor. */
  OWL2FunctionalStyleParserTokenManager::OWL2FunctionalStyleParserTokenManager (JAVACC_CHARSTREAM *stream, int lexState, OWL2FunctionalStyleParser *parserArg)
  : CTokenManager()
  {
    input_stream = NULL;
    ReInit(stream, lexState, parserArg);
  }

  // Destructor
  OWL2FunctionalStyleParserTokenManager::~OWL2FunctionalStyleParserTokenManager () {
    if (input_stream) delete input_stream;
    if (errorHandlerCreated) delete errorHandler;
  }

}
}
}

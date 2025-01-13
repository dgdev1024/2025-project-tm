/// @file TMM.Keyword.hpp

#pragma once
#include <TMM.Common.hpp>

namespace tmm
{

    enum class KeywordType
    {
        None = 0,
        Language,
        Section,
        Register,
        Condition,
        Instruction
    };

    enum LanguageType : tmc::Int32
    {
        LT_SECTION,
        LT_FUNCTION,
        LT_LET,
        LT_CONST
    };

    enum SectionType : tmc::Int32
    {
        ST_METADATA,
        ST_RST_0,
        ST_RST_1,
        ST_RST_2,
        ST_RST_3,
        ST_RST_4,
        ST_RST_5,
        ST_RST_6,
        ST_RST_7,
        ST_RST_8,
        ST_RST_9,
        ST_RST_A,
        ST_RST_B,
        ST_RST_C,
        ST_RST_D,
        ST_RST_E,
        ST_RST_F,
        ST_INT_0,
        ST_INT_1,
        ST_INT_2,
        ST_INT_3,
        ST_INT_4,
        ST_INT_5,
        ST_INT_6,
        ST_INT_7,
        ST_INT_8,
        ST_INT_9,
        ST_INT_A,
        ST_INT_B,
        ST_INT_C,
        ST_INT_D,
        ST_INT_E,
        ST_INT_F,
        ST_PROGRAM,
        ST_RAM,
        ST_QRAM
    };

    enum RegisterType : tmc::Int32
    {
        RT_A, 
        RT_AW, 
        RT_AH, 
        RT_AL,
        RT_B, 
        RT_BW, 
        RT_BH, 
        RT_BL,
        RT_C, 
        RT_CW, 
        RT_CH, 
        RT_CL,
        RT_D, 
        RT_DW, 
        RT_DH, 
        RT_DL
    };

    enum ConditionType : tmc::Int32
    {
        CT_N,
        CT_CS, 
        CT_CC,
        CT_ZS, 
        CT_ZC,
        CT_OS,
        CT_US
    };

    enum InstructionType : tmc::Int32
    {
        IT_NOP,
        IT_STOP,
        IT_HALT,
        IT_SEC,
        IT_CEC,
        IT_DI,
        IT_EI,
        IT_DAL,
        IT_DAW,
        IT_DAB,
        IT_CPL,
        IT_CPW,
        IT_CPB,
        IT_SCF,
        IT_CCF,
        IT_LD,
        IT_LDQ,
        IT_LDH,
        IT_ST,
        IT_STQ,
        IT_STH,
        IT_MV,
        IT_PUSH,
        IT_POP,
        IT_JMP,
        IT_JPB,
        IT_CALL,
        IT_RST,
        IT_RET,
        IT_RETI,
        IT_JPS,
        IT_INC,
        IT_DEC,
        IT_ADD,
        IT_ADC,
        IT_SUB,
        IT_SBC,
        IT_AND,
        IT_OR,
        IT_XOR,
        IT_CMP,
        IT_SLA,
        IT_SRA,
        IT_SRL,
        IT_RL,
        IT_RLC,
        IT_RR,
        IT_RRC,
        IT_BIT,
        IT_SET,
        IT_RES,
        IT_SWAP
    };

    struct Keyword
    {
        KeywordType mType       = KeywordType::None;
        tmc::Int32  mParamOne   = 0;
        tmc::Int32  mParamTwo   = 0;

    public:
        static const Keyword& Lookup (const tmc::String& pKeyword);

    };

}

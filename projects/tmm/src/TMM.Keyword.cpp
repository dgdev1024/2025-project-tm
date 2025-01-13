/// @file TMM.Keyword.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Keyword.hpp>

namespace tmm
{

    /* Static Constants - Keyword Lookup Table ****************************************************/

    static const tmc::Dictionary<Keyword> KEYWORD_LOOKUP = {
        { "", { KeywordType::None } },

        { "SECTION", { KeywordType::Language, LanguageType::LT_SECTION } },
        { "FUNCTION", { KeywordType::Language, LanguageType::LT_FUNCTION } },
        { "MACRO", { KeywordType::Language, LanguageType::LT_FUNCTION } },
        { "LET", { KeywordType::Language, LanguageType::LT_LET } },
        { "CONST", { KeywordType::Language, LanguageType::LT_CONST } },

        { "METADATA", { KeywordType::Section, SectionType::ST_METADATA } },
        { "RST0", { KeywordType::Section, SectionType::ST_RST_0 } },
        { "RST1", { KeywordType::Section, SectionType::ST_RST_1 } },
        { "RST2", { KeywordType::Section, SectionType::ST_RST_2 } },
        { "RST3", { KeywordType::Section, SectionType::ST_RST_3 } },
        { "RST4", { KeywordType::Section, SectionType::ST_RST_4 } },
        { "RST5", { KeywordType::Section, SectionType::ST_RST_5 } },
        { "RST6", { KeywordType::Section, SectionType::ST_RST_6 } },
        { "RST7", { KeywordType::Section, SectionType::ST_RST_7 } },
        { "RST8", { KeywordType::Section, SectionType::ST_RST_8 } },
        { "RST9", { KeywordType::Section, SectionType::ST_RST_9 } },
        { "RSTA", { KeywordType::Section, SectionType::ST_RST_A } },
        { "RSTB", { KeywordType::Section, SectionType::ST_RST_B } },
        { "RSTC", { KeywordType::Section, SectionType::ST_RST_C } },
        { "RSTD", { KeywordType::Section, SectionType::ST_RST_D } },
        { "RSTE", { KeywordType::Section, SectionType::ST_RST_E } },
        { "RSTF", { KeywordType::Section, SectionType::ST_RST_F } },
        { "INT0", { KeywordType::Section, SectionType::ST_INT_0 } },
        { "INT1", { KeywordType::Section, SectionType::ST_INT_1 } },
        { "INT2", { KeywordType::Section, SectionType::ST_INT_2 } },
        { "INT3", { KeywordType::Section, SectionType::ST_INT_3 } },
        { "INT4", { KeywordType::Section, SectionType::ST_INT_4 } },
        { "INT5", { KeywordType::Section, SectionType::ST_INT_5 } },
        { "INT6", { KeywordType::Section, SectionType::ST_INT_6 } },
        { "INT7", { KeywordType::Section, SectionType::ST_INT_7 } },
        { "INT8", { KeywordType::Section, SectionType::ST_INT_8 } },
        { "INT9", { KeywordType::Section, SectionType::ST_INT_9 } },
        { "INTA", { KeywordType::Section, SectionType::ST_INT_A } },
        { "INTB", { KeywordType::Section, SectionType::ST_INT_B } },
        { "INTC", { KeywordType::Section, SectionType::ST_INT_C } },
        { "INTD", { KeywordType::Section, SectionType::ST_INT_D } },
        { "INTE", { KeywordType::Section, SectionType::ST_INT_E } },
        { "INTF", { KeywordType::Section, SectionType::ST_INT_F } },
        { "PROGRAM", { KeywordType::Section, SectionType::ST_PROGRAM } },
        { "RAM", { KeywordType::Section, SectionType::ST_RAM } },
        { "QRAM", { KeywordType::Section, SectionType::ST_QRAM } },

        { "A", { KeywordType::Register, RegisterType::RT_A } },
        { "AW", { KeywordType::Register, RegisterType::RT_AW } },
        { "AH", { KeywordType::Register, RegisterType::RT_AH } },
        { "AL", { KeywordType::Register, RegisterType::RT_AL } },
        { "B", { KeywordType::Register, RegisterType::RT_B } },
        { "BW", { KeywordType::Register, RegisterType::RT_BW } },
        { "BH", { KeywordType::Register, RegisterType::RT_BH } },
        { "BL", { KeywordType::Register, RegisterType::RT_BL } },
        { "C", { KeywordType::Register, RegisterType::RT_C } },
        { "CW", { KeywordType::Register, RegisterType::RT_CW } },
        { "CH", { KeywordType::Register, RegisterType::RT_CH } },
        { "CL", { KeywordType::Register, RegisterType::RT_CL } },
        { "D", { KeywordType::Register, RegisterType::RT_D } },
        { "DW", { KeywordType::Register, RegisterType::RT_DW } },
        { "DH", { KeywordType::Register, RegisterType::RT_DH } },
        { "DL", { KeywordType::Register, RegisterType::RT_DL } },

        { "N", { KeywordType::Condition, ConditionType::CT_N } },
        { "CS", { KeywordType::Condition, ConditionType::CT_CS } },
        { "CC", { KeywordType::Condition, ConditionType::CT_CC } },
        { "ZS", { KeywordType::Condition, ConditionType::CT_ZS } },
        { "ZC", { KeywordType::Condition, ConditionType::CT_ZC } },
        { "OS", { KeywordType::Condition, ConditionType::CT_OS } },
        { "US", { KeywordType::Condition, ConditionType::CT_US } },

        { "NOP", { KeywordType::Instruction, InstructionType::IT_NOP } },
        { "STOP", { KeywordType::Instruction, InstructionType::IT_STOP } },
        { "HALT", { KeywordType::Instruction, InstructionType::IT_HALT } },
        { "SEC", { KeywordType::Instruction, InstructionType::IT_SEC } },
        { "CEC", { KeywordType::Instruction, InstructionType::IT_CEC } },
        { "DI", { KeywordType::Instruction, InstructionType::IT_DI } },
        { "EI", { KeywordType::Instruction, InstructionType::IT_EI } },
        { "DAL", { KeywordType::Instruction, InstructionType::IT_DAL } },
        { "DAW", { KeywordType::Instruction, InstructionType::IT_DAW } },
        { "DAB", { KeywordType::Instruction, InstructionType::IT_DAB } },
        { "CPL", { KeywordType::Instruction, InstructionType::IT_CPL } },
        { "CPW", { KeywordType::Instruction, InstructionType::IT_CPW } },
        { "CPB", { KeywordType::Instruction, InstructionType::IT_CPB } },
        { "SCF", { KeywordType::Instruction, InstructionType::IT_SCF } },
        { "CCF", { KeywordType::Instruction, InstructionType::IT_CCF } },
        { "LD", { KeywordType::Instruction, InstructionType::IT_LD } },
        { "LDQ", { KeywordType::Instruction, InstructionType::IT_LDQ } },
        { "LDH", { KeywordType::Instruction, InstructionType::IT_LDH } },
        { "ST", { KeywordType::Instruction, InstructionType::IT_ST } },
        { "STQ", { KeywordType::Instruction, InstructionType::IT_STQ } },
        { "STH", { KeywordType::Instruction, InstructionType::IT_STH } },
        { "MV", { KeywordType::Instruction, InstructionType::IT_MV } },
        { "PUSH", { KeywordType::Instruction, InstructionType::IT_PUSH } },
        { "POP", { KeywordType::Instruction, InstructionType::IT_POP } },
        { "JMP", { KeywordType::Instruction, InstructionType::IT_JMP } },
        { "JPB", { KeywordType::Instruction, InstructionType::IT_JPB } },
        { "CALL", { KeywordType::Instruction, InstructionType::IT_CALL } },
        { "RST", { KeywordType::Instruction, InstructionType::IT_RST } },
        { "RET", { KeywordType::Instruction, InstructionType::IT_RET } },
        { "RETI", { KeywordType::Instruction, InstructionType::IT_RETI } },
        { "JPS", { KeywordType::Instruction, InstructionType::IT_JPS } },
        { "INC", { KeywordType::Instruction, InstructionType::IT_INC } },
        { "DEC", { KeywordType::Instruction, InstructionType::IT_DEC } },
        { "ADD", { KeywordType::Instruction, InstructionType::IT_ADD } },
        { "ADC", { KeywordType::Instruction, InstructionType::IT_ADC } },
        { "SUB", { KeywordType::Instruction, InstructionType::IT_SUB } },
        { "SBC", { KeywordType::Instruction, InstructionType::IT_SBC } },
        { "AND", { KeywordType::Instruction, InstructionType::IT_AND } },
        { "OR", { KeywordType::Instruction, InstructionType::IT_OR } },
        { "XOR", { KeywordType::Instruction, InstructionType::IT_XOR } },
        { "CMP", { KeywordType::Instruction, InstructionType::IT_CMP } },
        { "SLA", { KeywordType::Instruction, InstructionType::IT_SLA } },
        { "SRA", { KeywordType::Instruction, InstructionType::IT_SRA } },
        { "SRL", { KeywordType::Instruction, InstructionType::IT_SRL } },
        { "RL", { KeywordType::Instruction, InstructionType::IT_RL } },
        { "RLC", { KeywordType::Instruction, InstructionType::IT_RLC } },
        { "RR", { KeywordType::Instruction, InstructionType::IT_RR } },
        { "RRC", { KeywordType::Instruction, InstructionType::IT_RRC } },
        { "BIT", { KeywordType::Instruction, InstructionType::IT_BIT } },
        { "SET", { KeywordType::Instruction, InstructionType::IT_SET } },
        { "RES", { KeywordType::Instruction, InstructionType::IT_RES } },
        { "SWAP", { KeywordType::Instruction, InstructionType::IT_SWAP } }
    };

    /* Public Methods *****************************************************************************/

    const Keyword& Keyword::Lookup (const tmc::String& pKeyword)
    {
        auto lIter = KEYWORD_LOOKUP.find(pKeyword);
        return (lIter != KEYWORD_LOOKUP.end()) ? lIter->second : KEYWORD_LOOKUP.at("");
    }

}

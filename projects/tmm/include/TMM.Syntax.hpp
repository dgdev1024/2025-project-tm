/// @file TMM.Syntax.hpp

#pragma once

#include <TMM.Token.hpp>

namespace tmm
{

    /* Syntax Type Enumeration ********************************************************************/

    enum class SyntaxType
    {
        // Statements
        Program,
        SectionStatement,
        LabelStatement,
        DataStatement,
        InstructionStatement,

        // Expressions
        BinaryExpression,
        UnaryExpression,
        AddressExpression,

        // Primary Expressions
        Identifier,
        RegisterLiteral,
        ConditionLiteral,
        StringLiteral,
        NumericLiteral,
        PlaceholderLiteral

    };

    /* Statement Syntax Base Class ****************************************************************/

    class Statement
    {
    public:
        using Ptr   = tmc::Shared<Statement>;
        using Body  = tmc::SharedList<Statement>;

    protected:

        inline Statement (const SyntaxType& pType) :
            mType   { pType }
        {}

    public:

        template <typename T, typename... As>
        inline static tmc::Shared<T> Make (As&&... pArgs)
        {
            static_assert(std::is_base_of_v<Statement, T>, "'T' must derive from 'tmm::Statement'.");
            return tmc::MakeShared<T>(std::forward<As>(pArgs)...);
        }

        template <typename T>
        inline static tmc::Shared<T> Cast (const Statement::Ptr& pSyntaxPtr)
        {
            static_assert(std::is_base_of_v<Statement, T>, "'T' must derive from 'tmm::Statement'.");
            return std::static_pointer_cast<T>(pSyntaxPtr);
        }

    public:

        inline const SyntaxType& GetType () const
        {
            return mType;
        }

    protected:
        SyntaxType mType;

    };  

    /* Expression Syntax Base Class ***************************************************************/

    class Expression : public Statement
    {
    public:
        using Ptr   = tmc::Shared<Expression>;
        using Body  = tmc::SharedList<Expression>;

    protected:

        inline Expression (const SyntaxType& pType) :
            Statement   { pType }
        {}

    public:

        template <typename T, typename... As>
        inline static tmc::Shared<T> Make (As&&... pArgs)
        {
            static_assert(std::is_base_of_v<Expression, T>, "'T' must derive from 'tmm::Expression'.");
            return tmc::MakeShared<T>(std::forward<As>(pArgs)...);
        }

        template <typename T>
        inline static tmc::Shared<T> Cast (const Expression::Ptr& pSyntaxPtr)
        {
            static_assert(std::is_base_of_v<Expression, T>, "'T' must derive from 'tmm::Expression'.");
            return std::static_pointer_cast<T>(pSyntaxPtr);
        }

    };

    /* Program Syntax Class ***********************************************************************/

    class Program : public Statement
    {
    public:
        using Ptr   = tmc::Shared<Program>;

    public:

        inline Program () :
            Statement { SyntaxType::Program }
        {}

    public:

        inline void Push (const Statement::Ptr& pSyntaxPtr)
        {
            mBody.push_back(pSyntaxPtr);
        }

    public:
        inline const Statement::Body& GetBody () const { return mBody; }

    private:
        Statement::Body mBody;
        
    };

    /* Statement Syntax Classes *******************************************************************/

    class SectionStatement : public Statement
    {
    public:
        using Ptr = tmc::Shared<SectionStatement>;

    public:

        inline SectionStatement (
            const tmc::Int32& pSectionType
        ) :
            Statement               { SyntaxType::SectionStatement },
            mSectionType            { pSectionType }
        {}

    public:

        inline const tmc::Int32&        GetSectionType () const { return mSectionType; }
        inline tmc::Boolean             IsRAM () const { return mSectionType >= SectionType::ST_RAM; }

    private:
        tmc::Int32          mSectionType;

    };

    class LabelStatement : public Statement
    {
    public:
        using Ptr = tmc::Shared<LabelStatement>;

    public:

        inline LabelStatement (
            const Expression::Ptr& pExpression
        ) :
            Statement       { SyntaxType::LabelStatement },
            mExpression     { pExpression }
        {}

    public:

        inline const Expression::Ptr&   GetExpression () const { return mExpression; }

    private:
        Expression::Ptr     mExpression = nullptr;

    };

    class DataStatement : public Statement
    {
    public:
        using Ptr = tmc::Shared<DataStatement>;

    public:

        inline DataStatement (
            const tmc::Int32& pDataType
        ) :
            Statement               { SyntaxType::DataStatement },
            mDataType               { pDataType }
        {}

    public:

        inline void PushExpression (const Expression::Ptr& pExpression)
        {
            mExpressionBody.push_back(pExpression);
        }

    public:

        inline const tmc::Int32&            GetDataType () const { return mDataType; }
        inline const Expression::Body&      GetExpressionBody () const { return mExpressionBody; }

    private:
        tmc::Int32          mDataType;
        Expression::Body    mExpressionBody;

    };

    class InstructionStatement : public Statement
    {
    public:
        using Ptr = tmc::Shared<InstructionStatement>;

    public:

        inline InstructionStatement (
            const tmc::Int32& pInstructionType,
            const Expression::Ptr& pFirstOperandExpression = nullptr,
            const Expression::Ptr& pSecondOperandExpression = nullptr
        ) :
            Statement                       { SyntaxType::InstructionStatement },
            mInstructionType                { pInstructionType },
            mFirstOperandExpression         { pFirstOperandExpression },
            mSecondOperandExpression        { pSecondOperandExpression }
        {}

    public:

        inline const tmc::Int32&            GetInstructionType () const { return mInstructionType; }
        inline const Expression::Ptr&       GetFirstOperandExpression () const { return mFirstOperandExpression; }
        inline const Expression::Ptr&       GetSecondOperandExpression () const { return mSecondOperandExpression; }

    private:
        tmc::Int32          mInstructionType;
        Expression::Ptr     mFirstOperandExpression = nullptr;
        Expression::Ptr     mSecondOperandExpression = nullptr;

    };

    /* Expression Syntax Classes ******************************************************************/

    class BinaryExpression : public Expression
    {
    public:
        using Ptr = tmc::Shared<BinaryExpression>;

    public:
        inline BinaryExpression (
            const Expression::Ptr& pLefthandExpression,
            const Expression::Ptr& pRighthandExpression,
            const Token& pOperatorToken
        ) :
            Expression              { SyntaxType::BinaryExpression },
            mLefthandExpression     { pLefthandExpression },
            mRighthandExpression    { pRighthandExpression },
            mOperatorToken          { pOperatorToken }
        {}

    public:
        inline const Expression::Ptr&   GetLefthandExpression () const { return mLefthandExpression; }
        inline const Expression::Ptr&   GetRighthandExpression () const { return mRighthandExpression; }
        inline const Token&             GetOperatorToken () const { return mOperatorToken; }

    private:
        Expression::Ptr mLefthandExpression   = nullptr;
        Expression::Ptr mRighthandExpression  = nullptr;
        Token           mOperatorToken;

    };

    class UnaryExpression : public Expression
    {
    public:
        using Ptr = tmc::Shared<UnaryExpression>;

    public:
        inline UnaryExpression (
            const Expression::Ptr& pRighthandExpression,
            const Token& pOperatorToken
        ) :
            Expression              { SyntaxType::UnaryExpression },
            mRighthandExpression    { pRighthandExpression },
            mOperatorToken          { pOperatorToken }
        {}

    public:
        inline const Expression::Ptr&   GetRighthandExpression () const { return mRighthandExpression; }
        inline const Token&             GetOperatorToken () const { return mOperatorToken; }

    private:
        Expression::Ptr mRighthandExpression  = nullptr;
        Token           mOperatorToken;

    };

    class AddressExpression : public Expression
    {
    public:
        using Ptr = tmc::Shared<AddressExpression>;

    public:
        inline AddressExpression (
            const Expression::Ptr& pInnerExpression
        ) :
            Expression          { SyntaxType::AddressExpression },
            mInnerExpression    { pInnerExpression }
        {}

    public:
        inline const Expression::Ptr&   GetInnerExpression () const { return mInnerExpression; }

    private:
        Expression::Ptr mInnerExpression  = nullptr;

    };

    /* Primary Expression Syntax Classes **********************************************************/

    class RegisterLiteral : public Expression
    {
    public:
        inline RegisterLiteral (const tmc::Int32& pRegisterType) :
            Expression      { SyntaxType::RegisterLiteral },
            mRegisterType   { pRegisterType }
        {}

    public:
        inline const tmc::Int32& GetRegisterType () const { return mRegisterType; }

    private:
        tmc::Int32  mRegisterType;

    };

    class ConditionLiteral : public Expression
    {
    public:
        inline ConditionLiteral (const tmc::Int32& pConditionType) :
            Expression      { SyntaxType::ConditionLiteral },
            mConditionType   { pConditionType }
        {}

    public:
        inline const tmc::Int32& GetConditionType () const { return mConditionType; }

    private:
        tmc::Int32  mConditionType;

    };

    class Identifier : public Expression
    {
    public:
        using Ptr = tmc::Shared<Identifier>;

    public:
        inline Identifier (const tmc::String& pSymbol) :
            Expression  { SyntaxType::Identifier },
            mSymbol     { pSymbol }
        {}

    public:
        inline const tmc::String&   GetSymbol () const  { return mSymbol; }

    private:
        tmc::String     mSymbol = "";

    };

    class StringLiteral : public Expression
    {
    public:
        using Ptr = tmc::Shared<StringLiteral>;

    public:
        inline StringLiteral (const tmc::String& pValue) :
            Expression  { SyntaxType::StringLiteral },
            mValue      { pValue }
        {}

    public:
        inline const tmc::String&   GetValue () const  { return mValue; }

    private:
        tmc::String     mValue = "";

    };

    class NumericLiteral : public Expression
    {
    public:
        using Ptr = tmc::Shared<NumericLiteral>;

    public:
        inline NumericLiteral (const tmc::Float64& pValue) :
            Expression  { SyntaxType::NumericLiteral },
            mValue      { pValue }
        {}

    public:
        inline const tmc::Float64&  GetValue () const   { return mValue; }

    private:
        tmc::Float64    mValue = 0.0;

    };

    class PlaceholderLiteral : public Expression
    {
    public:
        inline PlaceholderLiteral (const tmc::Uint32& pSlot) :
            Expression  { SyntaxType::PlaceholderLiteral },
            mSlot       { pSlot }
        {}

    public:
        inline const tmc::Uint32&  GetSlot () const     { return mSlot; }

    private:
        tmc::Uint32 mSlot = 0;

    };

}

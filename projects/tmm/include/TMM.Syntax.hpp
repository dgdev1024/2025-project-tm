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
        InstructionStatement,
        FunctionStatement,

        // Expressions
        FunctionCall,
        BinaryExpression,
        UnaryExpression,

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

    private:
        tmc::SharedList<Statement> mBody;
        
    };

    /* Statement Syntax Classes *******************************************************************/

    class SectionStatement : public Statement
    {
    public:
        inline SectionStatement (
            const Token& pSection,
            const Expression::Ptr& pOffset = nullptr
        ) :
            Statement   { SyntaxType::SectionStatement },
            mSection    { pSection },
            mOffset     { pOffset }
        {}

    public:
        inline const Token&             GetSection () const { return mSection; }
        inline const Expression::Ptr&   GetOffset () const { return mOffset; }

    private:
        Token           mSection;
        Expression::Ptr mOffset = nullptr;

    };

    class LabelStatement : public Statement
    {
    public:
        inline LabelStatement (
            const Expression::Ptr& pSymbol
        ) :
            Statement   { SyntaxType::LabelStatement },
            mSymbol     { pSymbol }
        {}

    public:
        inline const Expression::Ptr& GetSymbol () const { return mSymbol; }

    private:
        Expression::Ptr mSymbol = nullptr;

    };

    class InstructionStatement : public Statement
    {
    public:
        inline InstructionStatement (
            const tmc::Int32& pMnemonic,
            const Expression::Ptr& pOperandOne = nullptr,
            const Expression::Ptr& pOperandTwo = nullptr

        ) :
            Statement       { SyntaxType::InstructionStatement },
            mMnemonic       { pMnemonic },
            mOperandOne     { pOperandOne },
            mOperandTwo     { pOperandTwo }
        {}

    public:
        const tmc::Int32&   GetMnemonic () const { return mMnemonic; }

    private:
        tmc::Int32      mMnemonic;
        Expression::Ptr mOperandOne = nullptr;
        Expression::Ptr mOperandTwo = nullptr;

    };

    class FunctionStatement : public Statement
    {
    public:
        inline FunctionStatement (const Expression::Ptr& pName) :
            Statement   { SyntaxType::FunctionStatement },
            mName       { pName }
        {}

    public:
        inline void Push (const Statement::Ptr& pSyntaxPtr)
        {
            mBody.push_back(pSyntaxPtr);
        }

        inline const Expression::Ptr& GetName () const
        {
            return mName;
        }

        inline const tmc::SharedList<Statement>& GetBody () const
        {
            return mBody;
        }

    private:
        Expression::Ptr             mName = nullptr;
        tmc::SharedList<Statement>  mBody;

    };

    /* Expression Syntax Classes ******************************************************************/

    class FunctionCall : public Expression
    {
    public:
        inline FunctionCall (const Expression::Ptr& pName) :
            Expression  { SyntaxType::FunctionCall },
            mName       { pName }
        {}

    public:

        inline void PushArgument (const Expression::Ptr& pSyntaxPtr)
        {
            mArguments.push_back(pSyntaxPtr);
        }

        inline const Expression::Ptr& GetName () const
        {
            return mName;
        }

    private:
        Expression::Ptr             mName = nullptr;
        tmc::SharedList<Expression> mArguments;

    };

    class BinaryExpression : public Expression
    {
    public:
        inline BinaryExpression (
            const Expression::Ptr& pLefthand,
            const Expression::Ptr& pRighthand,
            const Token& pOperator
        ) :
            Expression  { SyntaxType::BinaryExpression },
            mLefthand   { pLefthand },
            mRighthand  { pRighthand },
            mOperator   { pOperator }
        {}

    public:
        inline const Expression::Ptr&   GetLefthand () const    { return mLefthand; }
        inline const Expression::Ptr&   GetRighthand () const   { return mRighthand; }
        inline const Token&             GetOperator () const    { return mOperator; }

    private:
        Expression::Ptr mLefthand   = nullptr;
        Expression::Ptr mRighthand  = nullptr;
        Token           mOperator;

    };

    class UnaryExpression : public Expression
    {
    public:
        inline UnaryExpression (
            const Expression::Ptr& pRighthand,
            const Token& pOperator
        ) :
            Expression  { SyntaxType::UnaryExpression },
            mRighthand  { pRighthand },
            mOperator   { pOperator }
        {}

    public:
        inline const Expression::Ptr&   GetRighthand () const   { return mRighthand; }
        inline const Token&             GetOperator () const    { return mOperator; }

    private:
        Expression::Ptr mRighthand  = nullptr;
        Token           mOperator;

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

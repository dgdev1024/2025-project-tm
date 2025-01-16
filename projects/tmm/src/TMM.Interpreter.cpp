/// @file TMM.Interpreter.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Interpreter.hpp>

namespace tmm
{

    /* Public Constructors and Destructor *********************************************************/

    Interpreter::Interpreter (Lexer& pLexer, Parser& pParser, Object& pObject) :
        mLexer      { pLexer },
        mParser     { pParser },
        mObject     { pObject }
    {

    }

    /* Public Methods *****************************************************************************/

    tmc::Boolean Interpreter::Run (const Program::Ptr& pProgram)
    {
        auto lResult = Evaluate(pProgram);

        return (lResult != nullptr);
    }

    /* Private Methods - Statement Evaluation *****************************************************/

    RuntimeValue::Ptr Interpreter::Evaluate (const Statement::Ptr& pStatement)
    {
        switch (pStatement->GetType())
        {
            default:
                std::cerr << "[Interpreter] Un-implemented syntax node encountered." << std::endl;
                return nullptr;
        }
    }

}

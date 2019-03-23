#include "pfx.hpp"
#include "common_pfx.hpp"
#include "math.h"

#include <stdio.h>
#include <memory>
#include <typeinfo>
#include <vector>

struct PrintCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto node = iter.evaluateNext();

        printf("%s", node->toString().c_str());

        return pfx::NullNode::instance;
    }
};

struct PrintLnCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto node = iter.evaluateNext();

        printf("%s\n", node->toString().c_str());

        return pfx::NullNode::instance;
    }
};

struct AddCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() + arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createFloat(arg1->toDouble() + arg2->toDouble());
        default:
            return pfx::createString(arg1->toString() + arg2->toString());
        }
    }
};

struct SubCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() - arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createFloat(arg1->toDouble() - arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct MultiplyCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() * arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createFloat(arg1->toDouble() * arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct DivideCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() / arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createFloat(arg1->toDouble() / arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct LessCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() < arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createInteger(arg1->toDouble() < arg2->toDouble());
        case pfx::NodeType::String:
            return pfx::createInteger(arg1->toString() < arg2->toString());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct EqualCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg1 = iter.evaluateNext();
        auto arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return pfx::createInteger(arg1->toInteger() == arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return pfx::createInteger(arg1->toDouble() == arg2->toDouble());
        case pfx::NodeType::String:
            return pfx::createInteger(arg1->toString() == arg2->toString());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct SqrtCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg = iter.evaluateNext();

        return pfx::createFloat(sqrt(arg->toDouble()));
    }
};

struct WhileCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto conditionNode = iter.fetchNext();
        auto statementNode = iter.fetchNext();

        while (conditionNode->evaluate()->toInteger())
        {
            statementNode->evaluate();
        }

        return pfx::NullNode::instance;
    }
};

struct IfCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto cond = iter.evaluateNext();
        auto thenPart = iter.fetchNext();
        auto elsePart = iter.fetchNext();

        auto &thePart = cond->toInteger() ? thenPart : elsePart;

        return thePart->evaluate();
    }
};

static bool isWhitespace(char c)
{
    return ((9 <= c) && (c <= 13)) || (c == ' ');
}

std::string readword()
{
    std::string word;

    // Eat whitespace
    int c;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (!isWhitespace(c))
        {
            ungetc(c, stdin);
            break;
        }
    }

    // Get the characters.
    while ((c = fgetc(stdin)) != EOF)
    {
        if (isWhitespace(c))
        {
            ungetc(c, stdin);
            break;
        }

        word.push_back(c);
    }

    return word;
}

std::string readline()
{
    std::string line;

    int c;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (c == '\n')
        {
            ungetc(c, stdin);
            break;
        }
        line.push_back(c);
    }

    return line;
}

struct ReadWordCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &) override
    {
        return pfx::createString(readword());
    }
};

struct ReadLineCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &) override
    {
        return pfx::createString(readline());
    }
};

struct CommentCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        iter.fetchNext();

        return pfx::NullNode::instance;
    }
};

struct DumpCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto n = iter.evaluateNext();

        n->dump();
        puts("");

        return pfx::NullNode::instance;
    }
};

struct MapCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto mapPos = iter.getPosition();
        auto mapNode = iter.evaluateNext()->asCommand();
        auto groupPos = iter.getPosition();
        auto groupNode = iter.evaluateNext()->asGroup();

        if (!mapNode)
        {
            mapPos.raiseErrorHere("Command node expected.");
        }
        if (!groupNode)
        {
            groupPos.raiseErrorHere("Group node expected");
        }

        auto newGroup = pfx::createGroup();

        auto groupIter = groupNode->getIterator();

        while (!groupIter.ended())
        {
            newGroup->nodes.push_back(mapNode->evaluate(groupIter));
        }

        return newGroup;
    }
};

int main()
{
    try
    {
        pfx::Input input("hw.txt");
        pfx::Context ctx;

        cpfx::applyCommonPfx(ctx);

        ctx.setCommand("print", std::make_shared<PrintCommand>());
        ctx.setCommand("println", std::make_shared<PrintLnCommand>());
        ctx.setCommand("dump", std::make_shared<DumpCommand>());

        ctx.setCommand("readword", std::make_shared<ReadWordCommand>());
        ctx.setCommand("readline", std::make_shared<ReadLineCommand>());

        ctx.setCommand("while", std::make_shared<WhileCommand>());
        ctx.setCommand("if", std::make_shared<IfCommand>());

        ctx.setCommand("+", std::make_shared<AddCommand>());
        ctx.setCommand("-", std::make_shared<SubCommand>());
        ctx.setCommand("*", std::make_shared<MultiplyCommand>());
        ctx.setCommand("/", std::make_shared<DivideCommand>());
        ctx.setCommand("sqrt", std::make_shared<SqrtCommand>());

        ctx.setCommand("<", std::make_shared<LessCommand>());
        ctx.setCommand("=", std::make_shared<EqualCommand>());

        ctx.setCommand("map", std::make_shared<MapCommand>());

        ctx.setCommand("//", std::make_shared<CommentCommand>());

        pfx::NodeRef gn = ctx.compileCode(input);
        gn->evaluate();

        return 0;
    }
    catch (pfx::Error e)
    {
        printf("Error: %s\n", e.toString().c_str());
        return 1;
    }
}
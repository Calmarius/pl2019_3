#include "pfx.hpp"
#include "math.h"

#include <stdio.h>
#include <memory>

struct PrintCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef node = iter.evaluateNext();

        printf("%s", node->toString().c_str());

        return pfx::NullNode::instance;
    }
};

struct PrintLnCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef node = iter.evaluateNext();

        printf("%s\n", node->toString().c_str());

        return pfx::NullNode::instance;
    }
};

struct AddCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.evaluateNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return std::make_shared<pfx::IntegerNode>(arg1->toInteger() + arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return std::make_shared<pfx::FloatNode>(arg1->toDouble() + arg2->toDouble());
        default:
            return std::make_shared<pfx::StringNode>(arg1->toString() + arg2->toString());
        }
    }
};

struct SubCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.evaluateNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return std::make_shared<pfx::IntegerNode>(arg1->toInteger() - arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return std::make_shared<pfx::FloatNode>(arg1->toDouble() - arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct MultiplyCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.evaluateNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return std::make_shared<pfx::IntegerNode>(arg1->toInteger() * arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return std::make_shared<pfx::FloatNode>(arg1->toDouble() * arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct DivideCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.evaluateNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return std::make_shared<pfx::IntegerNode>(arg1->toInteger() / arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return std::make_shared<pfx::FloatNode>(arg1->toDouble() / arg2->toDouble());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct LessCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.evaluateNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        if (arg1->getType() != arg2->getType())
        {
            return pfx::NullNode::instance;
        }

        switch (arg1->getType())
        {
        case pfx::NodeType::Integer:
            return std::make_shared<pfx::IntegerNode>(arg1->toInteger() < arg2->toInteger());
        case pfx::NodeType::FloatingPoint:
            return std::make_shared<pfx::IntegerNode>(arg1->toDouble() < arg2->toDouble());
        case pfx::NodeType::String:
            return std::make_shared<pfx::IntegerNode>(arg1->toString() < arg2->toString());
        default:
            return pfx::NullNode::instance;
        }
    }
};

struct ToIntCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.evaluateNext();

        return std::make_shared<pfx::IntegerNode>(arg->toInteger());
    }
};

struct ToFloatCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.evaluateNext();

        return std::make_shared<pfx::FloatNode>(arg->toDouble());
    }
};

struct ToStringCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.evaluateNext();

        return std::make_shared<pfx::StringNode>(arg->toString());
    }
};

struct ListCommand : pfx::Command
{
    pfx::Context *ctx;

    ListCommand(pfx::Context *ctx) : ctx(ctx) {}

    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.evaluateNext();

        return ctx->evaluateGroup(*dynamic_cast<pfx::GroupNode *>(arg.get()));
    }
};

void registerNodeCommand(pfx::Context *ctx, std::string name, pfx::NodeRef node)
{
    struct NodeCommand : pfx::Command
    {
        pfx::NodeRef ref;

        NodeCommand(pfx::NodeRef ref) : ref(ref) {}

        pfx::NodeRef execute(pfx::ArgIterator &) override
        {
            return ref;
        }
    };

    ctx->registerCommand(name, std::make_shared<NodeCommand>(node));
}

struct LetCommand : pfx::Command
{
    pfx::Context *ctx;

    LetCommand(pfx::Context *ctx) : ctx(ctx) {}

    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg1 = iter.fetchNext();
        pfx::NodeRef arg2 = iter.evaluateNext();

        registerNodeCommand(ctx, arg1->toString(), arg2);

        return pfx::NullNode::instance;
    }
};

struct SqrtCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.evaluateNext();

        return std::make_shared<pfx::FloatNode>(sqrt(arg->toDouble()));
    }
};

struct WhileCommand : pfx::Command
{
    pfx::Context *ctx;

    WhileCommand(pfx::Context *ctx) : ctx(ctx) {}

    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef conditionNode = iter.evaluateNext();
        pfx::NodeRef statementNode = iter.evaluateNext();

        pfx::GroupNode *condition = dynamic_cast<pfx::GroupNode *>(conditionNode.get());
        pfx::GroupNode *statement = dynamic_cast<pfx::GroupNode *>(statementNode.get());

        if (!condition || !statement || !condition->nodes.size())
        {
            return pfx::NullNode::instance;
        }

        while (ctx->executeGroup(*condition)->toInteger())
        {
            ctx->executeGroup(*statement);
        }

        return pfx::NullNode::instance;
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
    while (!feof(stdin))
    {
        char c = fgetc(stdin);

        if (!isWhitespace(c))
        {
            ungetc(c, stdin);
            break;
        }
    }

    // Get the characters.
    while (!feof(stdin))
    {
        char c = fgetc(stdin);

        if (isWhitespace(c))
        {
            ungetc(c, stdin);
            break;
        }

        word.push_back(c);
    }

    return word;
}

struct ReadWordCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &) override
    {
        return std::make_shared<pfx::StringNode>(readword());
    }
};

int main()
{
    try
    {
        pfx::Input input("hw.txt");
        pfx::Context ctx;
        pfx::Position error;

        ctx.registerCommand("print", std::make_shared<PrintCommand>());
        ctx.registerCommand("println", std::make_shared<PrintLnCommand>());

        ctx.registerCommand("int", std::make_shared<ToIntCommand>());
        ctx.registerCommand("float", std::make_shared<ToFloatCommand>());
        ctx.registerCommand("string", std::make_shared<ToStringCommand>());

        ctx.registerCommand("list", std::make_shared<ListCommand>(&ctx));

        ctx.registerCommand("readword", std::make_shared<ReadWordCommand>());

        ctx.registerCommand("while", std::make_shared<WhileCommand>(&ctx));

        ctx.registerCommand("+", std::make_shared<AddCommand>());
        ctx.registerCommand("-", std::make_shared<SubCommand>());
        ctx.registerCommand("*", std::make_shared<MultiplyCommand>());
        ctx.registerCommand("/", std::make_shared<DivideCommand>());
        ctx.registerCommand("sqrt", std::make_shared<SqrtCommand>());

        ctx.registerCommand("<", std::make_shared<LessCommand>());

        ctx.registerCommand("let", std::make_shared<LetCommand>(&ctx));

        std::shared_ptr<pfx::GroupNode> gn = ctx.compileCode(input, error);
        ctx.executeGroup(*gn);

        return 0;
    }
    catch (pfx::Error e)
    {
        printf("Error: %s\n", e.toString().c_str());
        return 1;
    }
}
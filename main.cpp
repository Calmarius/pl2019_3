#include "pfx.hpp"

#include <memory>

struct PrintCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef node = iter.evaluateNext();

        printf("%s", node->toString().c_str());

        return std::make_shared<pfx::NullNode>();
    }
};

struct PrintLnCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef node = iter.evaluateNext();

        printf("%s\n", node->toString().c_str());

        return std::make_shared<pfx::NullNode>();
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
        ctx.registerCommand("+", std::make_shared<AddCommand>());
        ctx.registerCommand("-", std::make_shared<SubCommand>());

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
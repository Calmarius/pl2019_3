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

        return std::make_shared<pfx::IntegerNode>(arg1->toInteger() + arg2->toInteger());
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
        ctx.registerCommand("+", std::make_shared<AddCommand>());

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
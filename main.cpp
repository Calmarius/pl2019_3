#include "pfx.hpp"

#include <memory>

struct PrintCommand : pfx::Command
{
    std::shared_ptr<pfx::Node> execute(pfx::ArgIterator &iter) override
    {
        std::shared_ptr<pfx::Node> node = iter.next();

        printf("%s", node->toString().c_str());

        return std::make_shared<pfx::NullNode>();
    }
};

struct PrintLnCommand : pfx::Command
{
    std::shared_ptr<pfx::Node> execute(pfx::ArgIterator &iter) override
    {
        std::shared_ptr<pfx::Node> node = iter.next();

        printf("%s\n", node->toString().c_str());

        return std::make_shared<pfx::NullNode>();
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
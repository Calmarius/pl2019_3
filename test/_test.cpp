#include "pfx.hpp"

#undef NDEBUG
#include <assert.h>

int main()
{
    /* I have to admit I added only regressions here. Because I was too lazy to
     * test the obvious...
     */
    {
        printf("Non existing file.\n");
        bool exceptionThrown = false;
        try
        {
            pfx::Input input("Not exist");
        }
        catch (pfx::error::FailedToOpenFile)
        {
            exceptionThrown = true;
        }

        assert(exceptionThrown);
    }
    {
        printf("Empty input.\n");
        pfx::Input input("stuff.txt", "");
        pfx::Context ctx;
        pfx::NodeRef gn = ctx.compileCode(input);
    }

    {
        printf("Dummy iterator\n");
        pfx::ArgIterator iter;

        assert(dynamic_cast<pfx::NullNode *>(iter.fetchNext().get()));
    }

    {
        printf("Dummy iterator next\n");
        pfx::ArgIterator iter;

        assert(dynamic_cast<pfx::NullNode *>(iter.next().get()));
    }

    {
        printf("Test the node types.\n");

        pfx::Input input(
            "", R"( "string" 42 42.5 "42" "42.3" x)"); //< This also tests the
                                                       // case where there are
                                                       // no whitespace at the
                                                       // end of file.
        pfx::Context ctx;
        pfx::NodeRef n = ctx.compileCode(input);
        pfx::GroupNode *gn = dynamic_cast<pfx::GroupNode *>(n.get());

        assert(gn);
        assert(gn->nodes.size() == 6);
        assert(gn->nodes[0].node->getType() == pfx::NodeType::String);
        assert(gn->nodes[0].node->toString() == "string");

        assert(gn->nodes[1].node->getType() == pfx::NodeType::Integer);
        assert(gn->nodes[1].node->toInteger() == 42);

        assert(gn->nodes[2].node->getType() == pfx::NodeType::FloatingPoint);
        assert(gn->nodes[2].node->toDouble() == 42.5);

        assert(gn->nodes[3].node->getType() == pfx::NodeType::String);
        assert(gn->nodes[3].node->toString() == "42");

        assert(gn->nodes[4].node->getType() == pfx::NodeType::String);
        assert(gn->nodes[4].node->toString() == "42.3");

        assert(gn->nodes[5].node->getType() == pfx::NodeType::Command);
        assert(gn->nodes[5].node->toString() == "x");
    }

    {
        printf("Test if I got the tab stop right...\n");
        pfx::Context ctx;
        pfx::Input input = pfx::Input("", "\tx");
        pfx::NodeRef n = ctx.compileCode(input);
        pfx::GroupNode *gn = dynamic_cast<pfx::GroupNode *>(n.get());
        assert(gn);
        assert(gn->nodes.size() == 1);
        assert(gn->nodes[0].start.column == 5);

        input = pfx::Input("", " \tx");
        n = ctx.compileCode(input);
        gn = dynamic_cast<pfx::GroupNode *>(n.get());
        assert(gn);
        assert(gn->nodes.size() == 1);
        assert(gn->nodes[0].start.column == 5);

        input = pfx::Input("", "  \tx");
        n = ctx.compileCode(input);
        gn = dynamic_cast<pfx::GroupNode *>(n.get());
        assert(gn);
        assert(gn->nodes.size() == 1);
        assert(gn->nodes[0].start.column == 5);

        input = pfx::Input("", "   \tx");
        n = ctx.compileCode(input);
        gn = dynamic_cast<pfx::GroupNode *>(n.get());
        assert(gn);
        assert(gn->nodes.size() == 1);
        assert(gn->nodes[0].start.column == 5);

        input = pfx::Input("", "    \tx");
        n = ctx.compileCode(input);
        gn = dynamic_cast<pfx::GroupNode *>(n.get());
        assert(gn);
        assert(gn->nodes.size() == 1);
        assert(gn->nodes[0].start.column == 9);
    }

    {
        printf("Escaped quotes.\n");
        pfx::Input input("", R"( "Quoted string ""like this""." )");

        pfx::Token t;
        assert(pfx::readWord(input, t));
        assert(t.word == R"(Quoted string "like this".)");
    }
}
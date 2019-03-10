#include "pfx.hpp"

#undef NDEBUG
#include <assert.h>

int main()
{
    {
        printf("Nonexisting file.\n");
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
}
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
}
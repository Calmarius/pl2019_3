#include "../common_pfx.hpp"

namespace cpfx
{
struct ContainerCommand : pfx::Command
{
    pfx::NodeRef ref;

    ContainerCommand() : ContainerCommand(pfx::NullNode::instance)
    {
    }

    ContainerCommand(pfx::NodeRef ref) : ref(ref)
    {
    }

    pfx::NodeRef execute(pfx::ArgIterator &) override
    {
        return ref;
    }
};

void let(pfx::CommandNode &cmd, pfx::NodeRef value)
{
    ContainerCommand *varContainer =
        dynamic_cast<ContainerCommand *>(cmd.command.get());
    if (!varContainer)
    {
        auto tmp = std::make_shared<ContainerCommand>();
        cmd.command = tmp;
        varContainer = tmp.get();
    }
    varContainer->ref = value;
}

struct LetCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto pos = iter.getPosition();
        auto variable = iter.fetchNext()->asCommand();
        auto value = iter.evaluateNext();

        if (!variable) pos.raiseErrorHere("This is not a variable!");

        let(*variable, value);

        return value;
    }
};

struct ListCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto gn = iter.fetchNext()->asGroup();

        if (gn) return gn->evaluateAll();

        return gn;
    }
};

struct TRecRequest
{
    pfx::NodeRef group;
};

struct FunctionRunner : pfx::Command
{
    std::vector<pfx::CommandRef> parameters;
    std::vector<pfx::CommandRef> locals;
    pfx::GroupRef body;

    FunctionRunner(pfx::GroupRef parameters, pfx::GroupRef locals,
                   pfx::GroupRef body)
        : body(body)
    {
        for (auto x : parameters->nodes)
        {
            this->parameters.push_back(x.node->asCommand());
        }

        for (auto x : locals->nodes)
        {
            this->locals.push_back(x.node->asCommand());
        }
    }

    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        std::vector<pfx::CommandCallbackRef> savedVariables;
        std::vector<pfx::CommandCallbackRef> savedLocals;
        std::vector<pfx::NodeRef> args;

        int nParams = parameters.size();
        while (nParams-- > 0)
        {
            args.push_back(iter.evaluateNext());
        }

        // Save previous meanings of the formal arguments and locals and
        // override them with new meanings.

        int i = 0;
        for (auto &x : parameters)
        {
            auto cn = x->asCommand();
            savedVariables.push_back(cn->command);
            cn->command = std::make_shared<ContainerCommand>(args[i++]);
        }

        for (auto &x : locals)
        {
            auto cn = x->asCommand();
            savedLocals.push_back(cn->command);
            cn->command = std::make_shared<ContainerCommand>();
        }

        // Execute the body
        pfx::NodeRef currentBody = body;
        pfx::NodeRef result;
        for (;;)
        {
            try
            {
                result = currentBody->evaluate();
                break;
            }
            catch (const TRecRequest &trecRequest)
            {
                /** The tail recursion request can come from many layers deep.
                 * Throwing an exception allows us to unwind those layers and
                 * return to nearest executing function. And do the body swap
                 * there.
                 */
                currentBody = trecRequest.group;
            }
        }

        // Restore the meanings of the overridden stuff.
        i = 0;
        for (auto &x : parameters)
        {
            auto cn = x->asCommand();
            cn->command = savedVariables[i++];
        }

        i = 0;
        for (auto &x : locals)
        {
            auto cn = x->asCommand();
            cn->command = savedLocals[i++];
        }

        // Done.
        return result;
    }
};

std::shared_ptr<pfx::Command> createLambda(pfx::ArgIterator &iter)
{
    pfx::Position pos = iter.getPosition();
    pfx::GroupRef argsGroup = iter.fetchNext()->asGroup();
    if (!argsGroup)
    {
        pos.raiseErrorHere("Group node expected (for arguments)");
    }

    pos = iter.getPosition();
    pfx::GroupRef locals = iter.fetchNext()->asGroup();
    if (!locals)
    {
        pos.raiseErrorHere("Group node expected (for locals) ");
    }

    pos = iter.getPosition();
    pfx::GroupRef body = iter.fetchNext()->asGroup();
    if (!body)
    {
        pos.raiseErrorHere("Group node expected (for function body)");
    }

    for (auto arg : argsGroup->nodes)
    {
        if (!arg.node->asCommand())
        {
            arg.start.raiseErrorHere("Identifier expected.");
        }
    }
    for (auto arg : locals->nodes)
    {
        if (!arg.node->asCommand())
        {
            arg.start.raiseErrorHere("Identifier expected.");
        }
    }

    return std::make_shared<FunctionRunner>(argsGroup, locals, body);
}

struct LambdaCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        return pfx::createCommand(createLambda(iter));
    }
};

struct FetchCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        return iter.fetchNext();
    }
};

struct ToIntCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg = iter.evaluateNext();

        return pfx::createInteger(arg->toInteger());
    }
};

struct ToFloatCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg = iter.evaluateNext();

        return pfx::createFloat(arg->toDouble());
    }
};

struct ToStringCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto arg = iter.evaluateNext();

        return pfx::createString(arg->toString());
    }
};

struct BindCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto pos = iter.getPosition();
        auto bindeeCmd = iter.fetchNext()->asCommand();

        if (!bindeeCmd)
        {
            pos.raiseErrorHere("Command expected.");
        }

        pos = iter.getPosition();
        auto toBindCmd = iter.evaluateNext()->asCommand();
        if (!toBindCmd)
        {
            pos.raiseErrorHere("Command expected.");
        }

        bindeeCmd->command = toBindCmd->command;

        return pfx::NullNode::instance;
    };
};

struct TRecCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        auto pos = iter.getPosition();
        auto cmd = iter.fetchNext()->asCommand();

        if (!cmd)
        {
            pos.raiseErrorHere("Command node expected.");
        }

        auto *fr = dynamic_cast<FunctionRunner *>(cmd->command.get());
        if (!fr)
        {
            pos.raiseErrorHere("Runnable function expected..");
        }

        std::vector<pfx::NodeRef> args;
        auto nParams = fr->parameters.size();

        while (nParams-- > 0)
        {
            args.push_back(iter.evaluateNext());
        }

        auto i = int(0);
        for (auto &x : fr->parameters)
        {
            let(*x->asCommand(), args[i++]);
        }

        throw TRecRequest{fr->body};
    }
};


void applyCommonPfx(pfx::Context &ctx)
{
    /**
     * Prefixes:
     *
     * %: integer
     * "text": string
     * ^: float
     * >: command
     * (name): group
     *
     * *: indicates that the node is fetched, not evaluated.
     *
     * Unprefixed can be anything.
     */

    /**
     * bind >*var >command --> null
     *
     * Binds the command to var. So now we van use 'var' instead of 'command'.
     */
    ctx.setCommand("bind", std::make_shared<BindCommand>());

    /**
     * fetch *node --> node
     *
     * Just returns the node without evaluation.
     * This command can be used at places where evaluation would happen.
     */
    ctx.setCommand("fetch", std::make_shared<FetchCommand>());

    /**
     * float node --> ^value
     *
     * Coerces the evaluated node's value to float.
     */
    ctx.setCommand("float", std::make_shared<ToFloatCommand>());

    /**
     * int node --> %value
     *
     * Coerces the evaluated node's value to integer.
     */
    ctx.setCommand("int", std::make_shared<ToIntCommand>());

    /**
     * lambda *(>arg1 >arg2 ...) *(>local1 >local2 ...) *(function-body) -->
     * >function-runner
     *
     * Creates a function expression.
     * The arguments are the formal arguments to be passed to the function and
     * determine the number of arguments to run. The locals are the variables to
     * be used locally in the lambda. The function body will be evaluated when
     * the lambda is invoked.
     *
     * All the arguments and locals value is saved before the evaluation to
     * shadow global variables.
     */
    ctx.setCommand("lambda", std::make_shared<LambdaCommand>());

    /**
     * let >*varName value  --> value
     *
     * Assign the given variable to the value.
     */
    ctx.setCommand("let", std::make_shared<LetCommand>());

    /**
     * list *(group) --> (evaluated-group)
     * list any --> any
     *
     * Evaluates all nodes in the group and returns their results in a result
     * group.
     */
    ctx.setCommand("list", std::make_shared<ListCommand>());

    /**
     * string node --> "stringValue"
     *
     * Coerces the evaluated node's value to string.
     */
    ctx.setCommand("string", std::make_shared<ToStringCommand>());

    /**
     * trec >functionNode --> null
     *
     * Tail recursive call to the given function. Sort of.
     * The argument node must be a command node and must be bound to a function.
     *
     * It then evaluates the required amount of arguments, alters the given
     * variables (without saving them!) and then sets the current argument
     * iterator to point to its function body. This way the function can be
     * executed without recursion and without the risk of running out of stack.
     *
     */
    ctx.setCommand("trec", std::make_shared<TRecCommand>());
}

} // namespace cpfx

#ifdef UNITTEST

#undef NDEBUG

#include <assert.h>
#include <stdarg.h>

std::string ssprintfv(const char *format, va_list args)
{
    va_list args2;
    va_copy(args2, args);

    int n = vsnprintf(nullptr, 0, format, args);
    auto c_str = std::make_unique<char[]>(n + 1);
    vsnprintf(c_str.get(), n + 1, format, args2);
    va_end(args2);

    return std::string(c_str.get());
}

std::string ssprintf(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    std::string ret = ssprintfv(format, args);
    va_end(args);

    return ret;
}


struct AssertCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::Position pos = iter.getPosition();
        pfx::NodeRef a = iter.evaluateNext();
        pfx::NodeRef b = iter.evaluateNext();

        if (a->getType() != b->getType())
        {
            pos.raiseErrorHere(ssprintf("Assertion failed. %s != %s\n",
                                        a->toString().c_str(),
                                        b->toString().c_str()));
        }
        switch (a->getType())
        {
        case pfx::NodeType::FloatingPoint:
            if (a->toDouble() != b->toDouble())
            {
                pos.raiseErrorHere(ssprintf("Assertion failed. %s != %s\n",
                                            a->toString().c_str(),
                                            b->toString().c_str()));
            }
            break;
        case pfx::NodeType::Integer:
            if (a->toInteger() != b->toInteger())
            {
                pos.raiseErrorHere(ssprintf("Assertion failed. %s != %s\n",
                                            a->toString().c_str(),
                                            b->toString().c_str()));
            }
            break;
        case pfx::NodeType::String:
            if (a->toString() != b->toString())
            {
                pos.raiseErrorHere(ssprintf("Assertion failed. %s != %s\n",
                                            a->toString().c_str(),
                                            b->toString().c_str()));
            }
            break;
        default:
            assert(false);
        }

        return pfx::NullNode::instance;
    }
};

struct MulCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef a = iter.evaluateNext();
        pfx::NodeRef b = iter.evaluateNext();

        return std::make_shared<pfx::FloatNode>(a->toDouble() * b->toDouble());
    }
};


void run(std::string str)
{
    pfx::Input input("", str);
    pfx::Context ctx;

    cpfx::applyCommonPfx(ctx);
    ctx.setCommand("assert", std::make_shared<AssertCommand>());
    ctx.setCommand("*", std::make_shared<MulCommand>());

    ctx.compileCode(input)->evaluate();
}


int main()
{
    try
    {
        printf("Test 1\n");
        run(R"(let x 42 assert x 42)");

        printf("Test 2\n");
        run(R"(assert string list ( 42 "foo" "bar" ) "42foobar" )");

        printf("Test 3\n");
        run(R"(
            bind square lambda ( x ) ( ) ( * x x ) assert square 7.0 49.0
        )");

        printf("Test 4\n");
        run(R"(
            assert string 7 "7"
            assert float 7 7.0
            assert int 7 7

            assert string 7.0 "7"
            assert float 7.0 7.0
            assert int 7.0 7

            assert string "7" "7"
            assert float "7" 7.0
            assert int "7" 7
        )");
    }
    catch (const pfx::Error &e)
    {
        printf("%s\n", e.toString().c_str());
        assert(false);
    }


    return 0;
}


#endif
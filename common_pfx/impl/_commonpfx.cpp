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
        pfx::NodeRef variable = iter.fetchNext();
        pfx::NodeRef value = iter.evaluateNext();

        pfx::Position pos = iter.getPosition();
        pfx::CommandNode *cmd =
            dynamic_cast<pfx::CommandNode *>(variable.get());
        if (!cmd) pos.raiseErrorHere("This is not a variable!");

        let(*cmd, value);

        return value;
    }
};

struct ListCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::NodeRef arg = iter.fetchNext();

        pfx::GroupNode *gn = dynamic_cast<pfx::GroupNode *>(arg.get());

        if (gn) return gn->evaluateAll();

        return arg;
    }
};

struct TRecRequest
{
    pfx::NodeRef group;
};

struct FunctionRunner : pfx::Command
{
    std::vector<pfx::NodeRef> parameters;
    std::vector<pfx::NodeRef> locals;
    pfx::NodeRef body;

    FunctionRunner(pfx::NodeRef parameters, pfx::NodeRef locals,
                   pfx::NodeRef body)
        : body(body)
    {
        for (auto x : dynamic_cast<pfx::GroupNode *>(parameters.get())->nodes)
        {
            this->parameters.push_back(x.node);
        }

        for (auto x : dynamic_cast<pfx::GroupNode *>(locals.get())->nodes)
        {
            this->locals.push_back(x.node);
        }
    }

    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        std::vector<pfx::CommandRef> savedVariables;
        std::vector<pfx::CommandRef> savedLocals;
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
            pfx::CommandNode *cn = dynamic_cast<pfx::CommandNode *>(x.get());
            savedVariables.push_back(cn->command);
            cn->command = std::make_shared<ContainerCommand>(args[i++]);
        }

        for (auto &x : locals)
        {
            pfx::CommandNode *cn = dynamic_cast<pfx::CommandNode *>(x.get());
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
            pfx::CommandNode *cn = dynamic_cast<pfx::CommandNode *>(x.get());
            cn->command = savedVariables[i++];
        }

        i = 0;
        for (auto &x : locals)
        {
            pfx::CommandNode *cn = dynamic_cast<pfx::CommandNode *>(x.get());
            cn->command = savedLocals[i++];
        }

        // Done.
        return result;
    }
};

std::shared_ptr<pfx::Command> createLambda(pfx::ArgIterator &iter)
{
    pfx::Position pos = iter.getPosition();
    pfx::NodeRef argsGroupRef = iter.fetchNext();
    pfx::GroupNode *argsGroup =
        dynamic_cast<pfx::GroupNode *>(argsGroupRef.get());
    if (!argsGroup)
    {
        pos.raiseErrorHere("Group node expected (for arguments)");
    }

    pos = iter.getPosition();
    pfx::NodeRef localsRef = iter.fetchNext();
    pfx::GroupNode *locals = dynamic_cast<pfx::GroupNode *>(localsRef.get());
    if (!locals)
    {
        pos.raiseErrorHere("Group node expected (for locals) ");
    }

    pos = iter.getPosition();
    pfx::NodeRef bodyRef = iter.fetchNext();
    pfx::GroupNode *body = dynamic_cast<pfx::GroupNode *>(bodyRef.get());
    if (!body)
    {
        pos.raiseErrorHere("Group node expected (for function body)");
    }

    for (auto arg : argsGroup->nodes)
    {
        if (!dynamic_cast<pfx::CommandNode *>(arg.node.get()))
        {
            arg.start.raiseErrorHere("Identifier expected.");
        }
    }
    for (auto arg : locals->nodes)
    {
        if (!dynamic_cast<pfx::CommandNode *>(arg.node.get()))
        {
            arg.start.raiseErrorHere("Identifier expected.");
        }
    }

    return std::make_shared<FunctionRunner>(argsGroupRef, localsRef, bodyRef);
}

struct LambdaCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        return std::make_shared<pfx::CommandNode>(createLambda(iter));
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

struct BindCommand : pfx::Command
{
    pfx::NodeRef execute(pfx::ArgIterator &iter) override
    {
        pfx::Position pos = iter.getPosition();
        pfx::NodeRef bindee = iter.fetchNext();

        pfx::CommandNode *bindeeCmd =
            dynamic_cast<pfx::CommandNode *>(bindee.get());
        if (!bindeeCmd)
        {
            pos.raiseErrorHere("Command expected.");
        }

        pos = iter.getPosition();
        pfx::NodeRef toBind = iter.evaluateNext();
        pfx::CommandNode *toBindCmd =
            dynamic_cast<pfx::CommandNode *>(toBind.get());
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
        pfx::Position pos = iter.getPosition();
        pfx::NodeRef fn = iter.fetchNext();

        pfx::CommandNode *cmd = dynamic_cast<pfx::CommandNode *>(fn.get());
        if (!fn)
        {
            pos.raiseErrorHere("Command node expected.");
        }
        FunctionRunner *fr = dynamic_cast<FunctionRunner *>(cmd->command.get());
        if (!fr)
        {
            pos.raiseErrorHere("Runnable function expected..");
        }

        std::vector<pfx::NodeRef> args;
        int nParams = fr->parameters.size();

        while (nParams-- > 0)
        {
            args.push_back(iter.evaluateNext());
        }

        int i = 0;
        for (auto &x : fr->parameters)
        {
            let(*dynamic_cast<pfx::CommandNode *>(x.get()), args[i++]);
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

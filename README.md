# PL 2019/3

## Introduction

My motivation was to create the simplest language that can be used for something,
easy to implement and integrate.

The name comes from the year and the fact that this is the 3rd programming language I try to make this year.

## Quick build guide

Only Linux builds available.

But I kept is as simple as possible: just compile the _.cpp in libpfx as a library to get the corresponding object file.

Just issue "make" and install the packages it complain about missing. It should work then.

It needs valgrind to run the unit tests and check memory stuff.
And needs doxygen to generate the docs.

That's it.

---------

The description of the language and short tutorial follows.

## Input character set

The input files are treated as binary, that is a sequence of bytes, so the implementer don't need to care about the encoding stuff and things like that.
The main character class is whitespace: the space character itself (ASCII 32), and characters between ASCII 9 and 13. That is the \t, \n, \v, \f, \r characters.
The rest of the characters are part of the language without exception.

## The tokens

One higher level the language is comprised of tokens.
A token is a sequence on non-whitespace characters like `foo`, or a sequence of non-quote characters between quotes, eg. `"foo bar"`.
In this quoted form the quotes themshelves are escaped as doubling the quotes, eg. `"Quote in string ""like this"". "`
And this is the only escape sequence in the langauge.

## The nodes

Then the tokens are arranged into a tree structure as nodes.
There are several kind of nodes.

First of all quoted tokens are always turned into string nodes.
Non-quoted tokens are interpreted as follows:

- If the token can fully be interpreted as an integer, then an integer node is created with the parsed value.
- If the token can fully be interpreted as a floating point number, a floating point node is created with the given floating point value.
- The `(` token is reserved as a group node start mark, similarly the `)` reserved as a group end mark. This would allow defining the so called group nodes like this: `( 3 "foo" "bar" )`.
- Everything else is treated as a command word, which needs to be implemented by the user, this basically covers everything, every symbol and stuff that is not interpreted as a number.

I deliberately not defined how integers and floats are parsed, as you can use your favorite language's integer and floating point parsing functions.
For the C++ code in this repo I used strtol for the integers strtod for the floats.
Also I didn't define the ranges of the integer and float to allow the implementers use their own

## Types of nodes and their operations.

There are 4 basic operations on the nodes:

- ToInteger: gets the integer value out of the node.
- ToFloat: gets the floating point value from the node.
- ToString: gets the string value of the node.
- Evaluate(argumentIterator): evaluates the node, creates a new node in the process. This operation also has access to an iterator the allows the node to gets its arguments.

These operations behave separately for each node type.

### Integer node

This node is immuatable.

- ToInteger: just returns the stored value.
- ToFloat: just retuns the stored value converted to float.
- ToString: turns the stored value into string. (In the C++ code I used the %d printf format).
- Evaluate: Returns a reference itself.

### Float node

This node is immuatable.

- ToInteger: just returns the stored value converted to integer.
- ToFloat: just retuns the stored value.
- ToString: turns the stored value into string. (In the C++ code I used the %g printf format).
- Evaluate: Returns a reference itself.

### String node

This node is immuatable.

- ToInteger: just returns the stored value converted to integer. (In the C++ code I used strtol)
- ToFloat: just retuns the stored value converted to float. ((In the C++ code I used strtod))
- ToString: returns the stored value.
- Evaluate: Returns a reference itself.

### Command node

This node contains an user defined callback object that manages the execution of this command.
Before the parsing the user must register these callbacks for each command word so during parsing the parser can bind them to the actual command nodes.

- ToInteger: returns 0.
- ToFloat: returns 0.
- ToString: returns the token text.
- Evaluate: Executes the user provided calback an returns the new node it creates.

During parsing each identical command word will be made to refer to the same command callback.
The command callback can be stateful, and this fact can be abused to introduce variables to the language.

### Null node

This kind of node arises only during the executiong typically indicating the case when no meaningful result can be provided.

- ToInteger: returns 0.
- ToFloat: returns 0-0.
- ToString: returns empty string.
- Evaluate: Returns itself.

### Group node

- ToInteger: returns 0.
- ToFloat: returns 0.0.
- ToString: converts each child node to string and the concatenates these strings.
- Evaluate: iterates over child nodes and evaluate each of them. The evaluation of the last node will be the result. However the trick is that each of the nodes are being evaluated receives the loop iterator, so they can fetch or evalute more nodes (their arguments basically).
- Evaluate all: this is an operation specific to the group nodes. It does something similar that the Evaluate method does, but instead of just returning the result of the last evaluation it returns an new group node that contains the results of all evaluations that took place during the iteration.

## Argument iterators

When evaluating a group an argument iterator is created.
This argument iterator has 2 main operations:

- fetchNext: it just fetches the next node without evaluating.
- evaluateNext: it fetches the next node and evaluate it. It passes itself as an argument to the evaluation, so the evaluated node can read more nodes.

The evaluateNext should be used when you expect to read simple values like numbers or strings. If you want to read group nodes or commands, use fetchNext and check the type.

## Executing the code

After parsing we get the root group node. And the execution of the program is done by calling the Evaluate method of this root group node.

As we have written previously the executing starts by creating an iterator that traverses the group node's child nodes.
Then recursively evaluate them to, passing them a reference of the argument iterator to fetch or evaluate furhet nodes in the group.
Then it returns with a result and the next child node the iterator points to is evaluated and so on, until we reach the end.

## Implementing the features

We must point out that the language itself is empty. It doesn't come with features. it's the developer's task to add these to the language in a form of command node callbacks.
Most thing can be implemented quite simply.

The full documentation is available

### Hello world

As usual in programming language we would like to print stuff first. So we can implement it as follows:

    struct PrintLnCommand : pfx::Command
    {
        pfx::NodeRef execute(pfx::ArgIterator &iter) override
        {
            pfx::NodeRef node = iter.evaluateNext();

            printf("%s\n", node->toString().c_str());

            return pfx::NullNode::instance;
        }
    };

As we can see, it evaluates the next node, and just print it when we convert it to string. It returns the null node, because it has nothing meaningful to return.

Then in the main program we write:

    pfx::Input input("hw.txt"); // Input is the class that keeps track of line number and column information and stuff like that.
    pfx::Context ctx; // The execution context.

    ctx.setCommand("println", std::make_shared<PrintLnCommand>()); // We register the command.
    pfx::NodeRef gn = ctx.compileCode(input);
    gn->evaluate(); // This is where our code runs.

    return 0;

Now we can write our first program like this:

    println "Hello world!"
    println "Hello again!"

And if compile the C++ program and run it, it should print these two lines by executing the two println commands.

### Aritmetic

Arithmetic can be realized using the Polish notation.
Just like the reverse polish one, it doesn't need parenthesis and precedence as it's encoded in the notation.
And we can implement commands for the operations.
Here is one for the addition:

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
                return std::make_shared<pfx::IntegerNode>(arg1->toInteger() +
                                                        arg2->toInteger());
            case pfx::NodeType::FloatingPoint:
                return std::make_shared<pfx::FloatNode>(arg1->toDouble() +
                                                        arg2->toDouble());
            default:
                return std::make_shared<pfx::StringNode>(arg1->toString() +
                                                        arg2->toString());
            }
        }
    };

The corresponding main program registration is like this:

    ctx.setCommand("+", std::make_shared<AddCommand>());

Now we can write things like:

    println + 3 4

Which should print 7.

And similarly we can add the other arithmetic operators and various mathematical functions we might need.

### Exposing `evaluateAll`

Groups have this operation which is very useful when dealing with data sets or when printing stuff.

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

It calls the evaluateAll when its argument really a group node. Just returns the argument when it's not.
In the main program we can register this under the name "list".

Now we can write:

    println list ( "The answer is: " + 20 22 )

The result of the list command is the group `( "The answer is: " 42 )`.
Which will be passed to the println which converts it to string.
We know that converting a group to string causes all its child nodes converted to string and then concatenated.
So it should print `"The answer is: 42"`.

### Adding variables

This is a tricky part. It's introduced like this:

    // Stateful command to contain the value.
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
            // It really does nothing, just returns the stored value.
            return ref;
        }
    };

    struct LetCommand : pfx::Command
    {
        pfx::NodeRef execute(pfx::ArgIterator &iter) override
        {
            pfx::NodeRef variable = iter.fetchNext();
            pfx::NodeRef value = iter.evaluateNext();

            // Check if it's really a variable.
            pfx::Position pos = iter.getPosition();
            pfx::CommandNode *cmd =
                dynamic_cast<pfx::CommandNode *>(variable.get());
            if (!cmd) pos.raiseErrorHere("This is not a variable!");

            // If it doesn't contain a container already, make it so.
            ContainerCommand *varContainer =
                dynamic_cast<ContainerCommand *>(cmd->command.get());
            if (!varContainer)
            {
                auto tmp = std::make_shared<ContainerCommand>();
                cmd->command = tmp;
                varContainer = tmp.get();
            }
            // At this point it contains a container, so store our value there.
            varContainer->ref = value;

            return pfx::NullNode::instance;
        }
    };

It exploits the fact that all command nodes of the same name are identical.
Change the command of one, the meaning of all changes.

This command is registered as the "let" command from now ion.

Now we can write things like:

    let a 2
    let b 3
    let c + a b
    println list ( "The c is: " c )

It will write `The c is: 5`.

### Adding comments

You probably noticed that there are no comments in this language.
But they can be added as a command like this:

    struct CommentCommand : pfx::Command
    {
        pfx::NodeRef execute(pfx::ArgIterator &iter) override
        {
            iter.fetchNext();

            return pfx::NullNode::instance;
        }
    };

As it can be seen, it fetches the next node and just ignores it.
Assing this to the `//` command word, then you can comment stuff like this:

    // ( Stuff to
     comment out )

This exploit the fact the group nodes are single nodes.


### Adding if statement

The if statement functions as the conditional operator as well. It's implemented like this:

    struct IfCommand : pfx::Command
    {
        pfx::NodeRef execute(pfx::ArgIterator &iter) override
        {
            pfx::NodeRef cond = iter.evaluateNext();
            pfx::NodeRef thenPart = iter.fetchNext();
            pfx::NodeRef elsePart = iter.fetchNext();

            pfx::NodeRef &thePart = cond->toInteger() ? thenPart : elsePart;

            return thePart->evaluate();
        }
    };

It evaluates the condition, then fetches the next two nodes.
If the condition, when converted to integer, evaluates to non-zero it will evaluate the "then part", otherwise the "else part".

Now we can write a program that solves the quadratic equation.

    let d - * b b * * 4.0 a c
    println list ( "Discriminant is: " d )
    if < d 0.0
    (
        println "No solution"
        list ( )
    )
    (
        let x1 / + - 0.0 b sqrt d * 2.0 a
        let x2 / - - 0.0 b sqrt d * 2.0 a
        println list ( "Solution 1: " x1 )
        println list ( "Solution 2: " x2 )
        list ( x1 x2 )
    )

## Conclusion

These examples were just the tip of the iceberg.
You can add all sorts of stuff using command nodes: like functions, lambdas, higher order functions and more (You can find these in the example program).
Whatever to fit your scripting needs.

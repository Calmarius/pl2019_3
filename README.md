# PL 2019/3

## Introduction

My motivation was to create the simplest language that can be used for something,
easy to implement and integrate.

The name comes from the year and the fact that this is the 3rd programming language I try to make this year.

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
- Evaluate: evaluates the node, creates a new node in the process. This operation also has access to an iterator the allows the node to gets its arguments.

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
- Evaluate: creates and iterator that enumerates the child nodes, then enters a loop that evaluates the child node the iterator points to, the child node's evaluate function will receive the iterator so it can fetch further arguments.
    Repeat this until the iterator reaches the end. The result is the result of the last evaluation.
- Evaluate all: this is an operation specific to the group nodes. It does something similar that the Evaluate method does, but instead of just returning the result of the last evaluation it returns an new group node that contains the results of all evaluations that took place during the iteration.

## Executing the code

After parsing we get the root group node. And the execution of the program is done by calling the Evaluate method of this root group node.

As we have written previously the executing starts by creating an iterator that traverses the group node's child nodes.
Then recursively evaluate them to, passing them a reference of the argument iterator to fetch or evaluate furhet nodes in the group.
Then it returns with a result and the next child node the iterator points to is evaluated and so on, until we reach the end.

## Implementing the features

We must point out that the language itself is empty. It doesn't come with features. it's the developer's task to add these to the language in a form of command node callbacks.

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

As we can see, we evaluate the next node, and just print it when we convert it to string. Then in the main program we write:

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




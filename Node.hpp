#pragma once

namespace pfx
{
using NodeRef = std::shared_ptr<Node>;

struct Node
{
    Position start;
    Position end;

    Node()
    {
    }

    void setToken(Token t)
    {
        start = t.start;
        end = t.end;
    }

    virtual ~Node() {}

    void dumpIndent(int indent)
    {
        printf("%*s", indent * 4, "");
    }

    void dump(int indent = 0)
    {
        dumpIndent(indent);
        dumpPart(indent);
    }

    virtual void dumpPart(int /*indent*/) { printf("No dump for this node.\n"); }

    std::shared_ptr<Node> evaluate()
    {
        ArgIterator tmp;
        return evaluate(tmp);
    }

    virtual std::shared_ptr<Node> evaluate(ArgIterator &) const = 0;

    virtual std::string toString() = 0;
    virtual int toInteger() = 0;
    virtual double toDouble() = 0;

    virtual NodeType getType() = 0;
};

struct IntegerNode : Node
{
    using Node::evaluate;

    const int value;

    IntegerNode(int value) : value(value) {}
    void dumpPart(int /*indent*/) override
    {
        printf("Integer: %d\n", value);
    }

    std::string toString() override;
    int toInteger() override { return value; }
    double toDouble() override { return value; }

    virtual std::shared_ptr<Node> evaluate(ArgIterator &) const override
    {
        return std::make_shared<IntegerNode>(*this);
    }

    NodeType getType() override
    {
        return NodeType::Integer;
    };
};

struct FloatNode : Node
{
    using Node::evaluate;
    const double value;

    FloatNode(double value) : value(value) {}

    void dumpPart(int /*indent*/) override
    {
        printf("Float: %g\n", value);
    }

    std::string toString() override;
    int toInteger() override { return value; }
    double toDouble() override { return value; }

    virtual std::shared_ptr<Node> evaluate(ArgIterator &) const override
    {
        return std::make_shared<FloatNode>(*this);
    }

    virtual NodeType getType()
    {
        return NodeType::FloatingPoint;
    };
};

int stringToInteger(const std::string &str);
double stringToDouble(const std::string &str);

struct CommandNode : Node
{
    using Node::evaluate;
    std::shared_ptr<Command> command;
    std::string prettyName;

    CommandNode(std::shared_ptr<Command> cmd) : command(cmd) {}

    CommandNode(std::shared_ptr<Command> cmd, std::string name) : CommandNode(cmd)
    {
        prettyName = name;
    }

    void dumpPart(int /*indent*/) override
    {
        printf("Command: %s\n", prettyName.c_str());
    }

    std::string toString() override { return prettyName; }
    int toInteger() override { return stringToInteger(prettyName); }
    double toDouble() override { return stringToDouble(prettyName); }

    std::shared_ptr<Node> evaluate(ArgIterator &) const override;

    virtual NodeType getType()
    {
        return NodeType::Command;
    };
};

struct StringNode : Node
{
    using Node::evaluate;
    const std::string value;
    StringNode(std::string value) : value(value) {}

    std::string toString() override { return value; }
    int toInteger() override { return stringToInteger(value); }
    double toDouble() override { return stringToDouble(value); }

    void dumpPart(int /*indent*/) override
    {
        printf("Quoted string: %s\n", value.c_str());
    }

    std::shared_ptr<Node> evaluate(ArgIterator &) const
    {
        return std::make_shared<StringNode>(*this);
    }

    virtual NodeType getType()
    {
        return NodeType::String;
    };
};

struct GroupNode : Node
{
    using Node::evaluate;
    std::vector<std::shared_ptr<Node>> nodes;

    std::string toString() override;
    int toInteger() override { return stringToInteger(toString()); }
    double toDouble() override { return stringToDouble(toString()); }

    void close(Token t)
    {
        end = t.end;
    }

    void dumpPart(int indent) override;

    std::shared_ptr<Node> evaluate(ArgIterator &) const override;

    std::shared_ptr<GroupNode> evaluateAll() const;

    virtual NodeType getType()
    {
        return NodeType::Group;
    };
};

struct NullNode : Node
{
    using Node::evaluate;
    std::string toString() override { return "null"; }
    int toInteger() override { return 0; }
    double toDouble() override { return 0; }

    static std::shared_ptr<Node> instance;

    void dumpPart(int /*indent*/) override
    {
        printf("Null\n");
    }

    std::shared_ptr<Node> evaluate(ArgIterator &) const
    {
        return std::make_shared<NullNode>(*this);
    }

    virtual NodeType getType()
    {
        return NodeType::Null;
    };
};

} // namespace pfx
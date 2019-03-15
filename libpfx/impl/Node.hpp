/// @file Node.hpp Contains definition of Node and its descendants.

namespace pfx
{
struct Node;
/// Shorthand for the node references.
using NodeRef = std::shared_ptr<Node>;

/// Defines a node, the basic building block of the language.
class Node
{
protected:
    /** Used internally to dump indents.
     *
     * @param [in] indent Prints 4 × indent spaces.
     */
    void dumpIndent(int indent)
    {
        printf("%*s", indent * 4, "");
    }

public:
    /// Default constructor does nothing.
    Node()
    {
    }

    /// Default destructor does nothing.
    virtual ~Node()
    {
    }

    /**
     * Dumps the contents of the node, for debugging purposes.
     *
     * @param [in] indent specifies the indent level (for multi line dumps
     * only).
     */
    virtual void dump(int indent = 0)
    {
        (void)indent;
        printf("(none)");
    };

    /**
     * Node evaluation without iterators.
     *
     * @return Reference to the result node.
     *
     * @remarks
     * When called on a node that do uses an iterator. The iterator will return
     * references to NullNode.
     */
    std::shared_ptr<Node> evaluate()
    {
        ArgIterator tmp;
        return evaluate(tmp);
    }

    /**
     * Evaluates a node.
     *
     * @param [in] iterator An iterator the implementation may use to read
     * further arguments.
     *
     * @return The node representing the result.
     */
    virtual std::shared_ptr<Node> evaluate(ArgIterator &iterator) = 0;

    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual std::string toString() = 0;

    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual int toInteger() = 0;

    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual double toDouble() = 0;

    /**
     * @return The type of the node.
     */
    virtual NodeType getType() = 0;
};

/// This node represents an immutable integer value.
struct IntegerNode : Node
{
    using Node::evaluate;

    /// The stored value.
    const int value;

    /**
     * Creates an integer node with the given value.
     *
     * @param [in] value The value to store.
     */
    IntegerNode(int value) : value(value)
    {
    }


    void dump(int /*indent*/) override
    {
        printf("Integer: %d", value);
    }

    /**
     *  Converts the integer value to a string. Using the %%d format specifier.
     *
     * @return The string representation.
     */
    std::string toString() override;

    /**
     * @return The value itself.
     */
    int toInteger() override
    {
        return value;
    }

    /**
     * @return The value itself converted to double.
     */
    double toDouble() override
    {
        return value;
    }

    /**
     * Returns a copy of itself.
     */
    virtual std::shared_ptr<Node> evaluate(ArgIterator &) override
    {
        return std::make_shared<IntegerNode>(*this);
    }

    /**
     * @return NodeType::Integer
     */
    NodeType getType() override
    {
        return NodeType::Integer;
    };
};

/// This node represents an immutable floating point value
struct FloatNode : Node
{
    using Node::evaluate;

    /// The stored value itself.
    const double value;

    /**
     * Constructs a new float node.
     *
     * @param [in] value The value to store.
     */
    FloatNode(double value) : value(value)
    {
    }

    void dump(int /*indent*/) override
    {
        printf("Float: %g", value);
    }

    /**
     * @return The string representation of the stored value. It is generated
     * according to the %%g printf format.
     *
     */
    std::string toString() override;

    /**
     *  @return The value converted to integer.
     */
    int toInteger() override
    {
        return value;
    }

    /**
     * @return The stored value itself.
     */
    double toDouble() override
    {
        return value;
    }

    /**
     * @return a copy of this node.
     */
    virtual std::shared_ptr<Node> evaluate(ArgIterator &) override
    {
        return std::make_shared<FloatNode>(*this);
    }

    /**
     * @return NodeType::FloatingPoint
     */
    virtual NodeType getType()
    {
        return NodeType::FloatingPoint;
    };
};

struct CommandNode : Node
{
    using Node::evaluate;
    std::shared_ptr<Command> command;
    std::string prettyName;

    CommandNode(std::shared_ptr<Command> cmd) : command(std::move(cmd))
    {
    }

    CommandNode(std::shared_ptr<Command> cmd, std::string name)
        : CommandNode(std::move(cmd))
    {
        prettyName = std::move(name);
    }

    void dump(int /*indent*/) override
    {
        printf("Command: %s", prettyName.c_str());
    }

    std::string toString() override
    {
        return prettyName;
    }
    int toInteger() override
    {
        return 0;
    }
    double toDouble() override
    {
        return 0.0;
    }

    std::shared_ptr<Node> evaluate(ArgIterator &) override;

    virtual NodeType getType()
    {
        return NodeType::Command;
    };
};

/// Represents a string value.
struct StringNode : Node
{
    using Node::evaluate;

    /// The stored value.
    const std::string value;

    /// Creates string node.
    StringNode(std::string value) : value(std::move(value))
    {
    }

    /// @return The stored value.
    std::string toString() override
    {
        return value;
    }

    /// @return the value converted to integer using strtol.
    int toInteger() override
    {
        return stringToInteger(value);
    }

    /// @return The value converted to double using strtod.
    double toDouble() override
    {
        return stringToDouble(value);
    }

    void dump(int /*indent*/) override
    {
        printf("Quoted string: %s", value.c_str());
    }

    /// @return itself.
    std::shared_ptr<Node> evaluate(ArgIterator &)
    {
        return std::make_shared<StringNode>(*this);
    }

    /// @return NodeType::String
    virtual NodeType getType()
    {
        return NodeType::String;
    };
};

/// Represents a node that can contain more child nodes
struct GroupNode : Node
{
    using Node::evaluate;
    /// Contains references to nodes and their metadata.
    std::vector<NodeInfo> nodes;

    /**
     * Gets the string representation of all child nodes and concatenate them.
     *
     * @return The concatenated result.
     */
    std::string toString() override;

    /**
     * @return 0
     */
    int toInteger() override
    {
        return 0;
    }

    /**
     * @return 0.0
     */
    double toDouble() override
    {
        return 0.0;
    }

    void dump(int indent) override;

    /**
     * Evaluates each node.
     *
     * @return the evaluation result of the last evaluation.
     *
     * @remarks
     *  The iterators during the evaluation is passed to the nodes being
     * evaluated. So they can recursively fetch or evaluate their other
     * arguments.
     */
    std::shared_ptr<Node> evaluate(ArgIterator &) override;

    /**
     * Evaluate all nodes in the group.
     *
     * @return A new group node containing the evaluation result of each
     * evaluation.
     *
     * @remarks
     * This works similarly to the evaluate, except that all evaluation results
     * are kept.
     */
    std::shared_ptr<GroupNode> evaluateAll();

    /**
     * @return NodeType::Group;
     */
    virtual NodeType getType()
    {
        return NodeType::Group;
    };
};

/// Represent the null node. Used when no meaningful result available.
struct NullNode : Node
{
    using Node::evaluate;

    /// @return the string "null".
    std::string toString() override
    {
        return "null";
    }

    /// @return 0
    int toInteger() override
    {
        return 0;
    }

    /// @return 0.0
    double toDouble() override
    {
        return 0;
    }

    /// Singleton instance so you don't need to create these.
    static std::shared_ptr<Node> instance;

    void dump(int /*indent*/) override
    {
        printf("Null");
    }

    /// @return itself.
    std::shared_ptr<Node> evaluate(ArgIterator &)
    {
        return std::make_shared<NullNode>(*this);
    }

    /// @return NodeType::Null
    virtual NodeType getType()
    {
        return NodeType::Null;
    };
};

} // namespace pfx
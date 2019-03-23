/// @file Node.hpp Contains definition of Node and its descendants.

namespace pfx
{
/// Defines a node, the basic building block of the language.
class Node : public std::enable_shared_from_this<Node>
{
    // Owned via references, do not copy.
    Node(const Node &) = delete;
    Node &operator=(const Node &) = delete;

protected:
    /** Used internally to dump indents.
     *
     * @param [in] indent Prints 4 × indent spaces.
     */
    void dumpIndent(int indent) const
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
    virtual void dump(int indent = 0) const
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
    NodeRef evaluate() const
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
     *
     * @remarks
     *  The default behavior is returning itself.
     */
    virtual NodeRef evaluate(ArgIterator &iterator) const
    {
        (void)iterator;
        return std::const_pointer_cast<Node>(shared_from_this());
    }


    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual std::string toString() const = 0;

    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual int toInteger() const = 0;

    /**
     * @return string value from the node. The behavior is implementation
     * defined.
     */
    virtual double toDouble() const = 0;

    /**
     * @return The type of the node.
     */
    virtual NodeType getType() const = 0;
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


    void dump(int /*indent*/) const override
    {
        printf("Integer: %d", value);
    }

    /**
     *  Converts the integer value to a string. Using the %%d format specifier.
     *
     * @return The string representation.
     */
    std::string toString() const override;

    /**
     * @return The value itself.
     */
    int toInteger() const override
    {
        return value;
    }

    /**
     * @return The value itself converted to double.
     */
    double toDouble() const override
    {
        return value;
    }

    /**
     * @return Itself.
     */
    /**
     * @return NodeType::Integer
     */
    NodeType getType() const override
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

    void dump(int /*indent*/) const override
    {
        printf("Float: %g", value);
    }

    /**
     * @return The string representation of the stored value. It is generated
     * according to the %%g printf format.
     *
     */
    std::string toString() const override;

    /**
     *  @return The value converted to integer.
     */
    int toInteger() const override
    {
        return value;
    }

    /**
     * @return The stored value itself.
     */
    double toDouble() const override
    {
        return value;
    }

    /**
     * @return NodeType::FloatingPoint
     */
    NodeType getType() const override
    {
        return NodeType::FloatingPoint;
    };
};

/// Represents an user defined command.
struct CommandNode : Node
{
    using Node::evaluate;

    /// Reference to the command.
    std::shared_ptr<Command> command;

    /// The name of the command. This is stored during the parsing.
    std::string prettyName;

    /**
     * Constructs a command node for the given command.
     *
     * @param [in] command The command this node represents.
     */
    CommandNode(std::shared_ptr<Command> command) : command(std::move(command))
    {
    }

    /**
     * Constructs a command node for the given command.
     *
     * @param [in] command The command this node represents.
     * @param [in] name The name of the command refer to.
     */
    CommandNode(std::shared_ptr<Command> command, std::string name)
        : CommandNode(std::move(command))
    {
        prettyName = std::move(name);
    }

    void dump(int /*indent*/) const override
    {
        printf("Command: %s", prettyName.c_str());
    }

    std::string toString() const override
    {
        return prettyName;
    }
    int toInteger() const override
    {
        return 0;
    }
    double toDouble() const override
    {
        return 0.0;
    }

    /**
     * Evaluates the node by running the passed in command.
     *
     * @param [in,out] iterator The argument iterator the command use the fetch
     * more arguments.
     *
     * @return The result of the command.
     */
    NodeRef evaluate(ArgIterator &iterator) const override;

    /// @return NodeType::Command
    NodeType getType() const override
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

    /**
     * Creates string node.
     *
     * @param [in] value The string value to store.
     */
    StringNode(std::string value) : value(std::move(value))
    {
    }

    /// @return The stored value.
    std::string toString() const override
    {
        return value;
    }

    /// @return the value converted to integer using strtol.
    int toInteger() const override
    {
        return stringToInteger(value);
    }

    /// @return The value converted to double using strtod.
    double toDouble() const override
    {
        return stringToDouble(value);
    }

    void dump(int /*indent*/) const override
    {
        printf("Quoted string: %s", value.c_str());
    }

    /// @return NodeType::String
    virtual NodeType getType() const
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
    std::string toString() const override;

    /**
     * @return 0
     */
    int toInteger() const override
    {
        return 0;
    }

    /**
     * @return 0.0
     */
    double toDouble() const override
    {
        return 0.0;
    }

    void dump(int indent) const override;

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
    NodeRef evaluate(ArgIterator &) const override;

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
    std::shared_ptr<GroupNode> evaluateAll() const;

    /**
     * @return NodeType::Group;
     */
    virtual NodeType getType() const
    {
        return NodeType::Group;
    };
};

/// Represent the null node. Used when no meaningful result available.
struct NullNode : Node
{
    using Node::evaluate;

    /// @return the string "null".
    std::string toString() const override
    {
        return "null";
    }

    /// @return 0
    int toInteger() const override
    {
        return 0;
    }

    /// @return 0.0
    double toDouble() const override
    {
        return 0;
    }

    /// Singleton instance so you don't need to create these.
    static NodeRef instance;

    void dump(int /*indent*/) const override
    {
        printf("Null");
    }

    /// @return NodeType::Null
    virtual NodeType getType() const
    {
        return NodeType::Null;
    };
};

/**
 * creates an integer node.
 *
 * @param [in] value The value the node represents.
 *
 * @return The node.
 */
inline NodeRef createInteger(int value)
{
    return std::make_shared<IntegerNode>(value);
}

/**
 * creates a float node.
 *
 * @param [in] value The value the node represents.
 *
 * @return The node.
 */
inline NodeRef createFloat(double value)
{
    return std::make_shared<FloatNode>(value);
}

/**
 * creates a string node.
 *
 * @param [in] value The value the node represents.
 *
 * @return The node.
 */
inline NodeRef createString(std::string value)
{
    return std::make_shared<StringNode>(value);
}

/**
 * creates a group node.
 *
 * @return The node.
 */
inline GroupRef createGroup()
{
    return std::make_shared<GroupNode>();
}

/**
 * creates a command node.
 *
 * @param [in] command The command callback it represents.
 *
 * @return The node.
 */
inline CommandRef createCommand(CommandCallbackRef command)
{
    return std::make_shared<CommandNode>(command);
}

} // namespace pfx
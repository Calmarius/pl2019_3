/// @file ArgIterator.hpp Argument iterator.

namespace pfx
{

struct Node;
struct NodeInfo;

/// This class is used to iterate on the command node's arguments.
class ArgIterator
{
private:
    typedef std::vector<NodeInfo>::const_iterator IteratorType;
    IteratorType &current;
    IteratorType end;

    static IteratorType dummy;

public:
    /**
     * Default constructor creates a dummy iterator. That's immediately on the
     * end and returns null nodes.
     */
    ArgIterator() : current(dummy), end(dummy)
    {
    }

    /**
     * Sets up and argument iterator.
     *
     * @param [in] current The iterator the element where the iteration start.
     * @param [in] end The iterator the points one after the last element.
     */
    ArgIterator(IteratorType &current, IteratorType end)
        : current(current), end(end)
    {
    }

    /**
     * @return The position of the current node. If the iterator is finished, it
     * returns a default Position() object.
     */
    Position getPosition();

    /**
     * @return Reference to the next node. If the iterator is at the end it
     * returns a pointer to a NullNode.
     */
    std::shared_ptr<Node> next();

    /**
     * Evaluates the next node.
     *
     * @return The result of the evaluation.
     *
     * @remarks
     *  If the iterator is at the end it returns a reference to a NullNode.
     * The evaluation may cause more nodes to be read if the node is a
     * CommandNode.
     */
    std::shared_ptr<Node> evaluateNext();

    /**
     * Reads the next node then moves the iterator forward.
     *
     * @return The reference to the next to, if the iterator is at the end. It
     * returns a reference to a NullNode.
     *
     * @remarks
     * Compared to the evaluateNext, this function doesn't evaluate the
     * next node. Just reads the node without evaluation.
     */
    std::shared_ptr<Node> fetchNext();

    /**
     * @return True if the iterator is reached the the end, false otherwise.
     */
    bool ended()
    {
        return current == end;
    }
};
} // namespace pfx
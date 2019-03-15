namespace pfx
{
/** Contains a reference to a node and some metadata
 *
 * @remarks
 * The reason this structure exists, is that the same node may exist in multiple
 * places in the source (as in command nodes). So there must be a way to specify
 * their positions separately.
 */
struct NodeInfo
{
    /// The character position where the corresponding token begins.
    Position start = Position();

    /// The character position where the corresponding token ends.
    Position end = Position();

    /// The reference to the pointed node.
    std::shared_ptr<Node> node;

    /**
     * Simple constructor for nodes that created on the fly.
     *
     * @param [in] n The node to contain.
     */
    NodeInfo(std::shared_ptr<Node> n) : node(std::move(n))
    {
    }

    /**
     * Full constructor that also specifies the corresponding token.
     *
     * @param [in] n The node to contain.
     * @param [in] t The token.
     */
    NodeInfo(std::shared_ptr<Node> n, const Token &t)
        : start(t.start), end(t.end), node(std::move(n))
    {
    }

    /**
     * Sets the positions according to the token.
     *
     * @param [in] t The token.
     */
    void setToken(const Token &t)
    {
        start = t.start;
        end = t.end;
    }

    /**
     * Just sets the end position based on the token.
     *
     * @param [in] t The token.
     *
     * @remarks
     *      This is used to set the end position of the group nodes.
     */
    void close(const Token &t)
    {
        end = t.end;
    }
};

} // namespace pfx
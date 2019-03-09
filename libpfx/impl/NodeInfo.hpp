namespace pfx
{
struct NodeInfo
{
    Position start = Position();
    Position end = Position();
    std::shared_ptr<Node> node;

    NodeInfo(std::shared_ptr<Node> n) : node(n) {}

    NodeInfo(std::shared_ptr<Node> n, Token t) : start(t.start), end(t.end), node(n) {}

    void setToken(Token t)
    {
        start = t.start;
        end = t.end;
    }

    void close(Token t)
    {
        end = t.end;
    }
};

}
namespace pfx
{

struct Node;
struct NodeInfo;

class ArgIterator
{
  private:
    typedef std::vector<NodeInfo>::const_iterator IteratorType;
    IteratorType &current;
    IteratorType end;

    static IteratorType dummy;

  public:
    ArgIterator() : current(dummy), end(dummy) {}
    ArgIterator(IteratorType &current, IteratorType end) : current(current), end(end) {}

    Position getPosition();
    std::shared_ptr<Node> next();
    std::shared_ptr<Node> evaluateNext();
    std::shared_ptr<Node> fetchNext();
    bool ended() { return current == end; }
};
}
namespace pfx
{
    class Node;
    class ArgIterator;
    class Command;
    using CommandRef = std::shared_ptr<Command>;

    struct Command
    {
        virtual std::shared_ptr<Node> execute(ArgIterator &) = 0;
        virtual ~Command() {}
    };
}
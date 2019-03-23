/// @file declarations.hpp contains forward declarations of types only.

namespace pfx
{
class Node;
class ArgIterator;
class Command;
struct Node;
struct GroupNode;
struct CommandNode;

/// Shorthand for the Command reference.
using CommandCallbackRef = std::shared_ptr<Command>;
/// Shorthand for the node references.
using NodeRef = std::shared_ptr<Node>;

/// Type for command node references
using CommandRef = std::shared_ptr<CommandNode>;

/// Type for group node references.
using GroupRef = std::shared_ptr<GroupNode>;
}
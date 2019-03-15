/// @file NodeType.hpp Contains the NodeType class.

#pragma once

namespace pfx
{
/// Represents the types of the possible nodes.
enum class NodeType
{
    Integer,       ///< Integer literal
    FloatingPoint, ///< Floating point literal
    String,        ///< String literal
    Command,       ///< Command
    Group,         ///< Group of nodes
    Null           ///< Unknown node
};
}
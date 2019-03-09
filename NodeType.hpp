#pragma once

namespace pfx
{
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
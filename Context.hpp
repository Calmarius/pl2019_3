#pragma once

namespace pfx
{
class CommandNode;

class GroupNode;

class Context
{
  std::string currentErrorString;
  void *userPtr = nullptr;
  std::map<std::string, std::shared_ptr<CommandNode>> commands;

public:
  void registerCommand(std::string name, std::shared_ptr<Command> command);
  std::shared_ptr<GroupNode> compileCode(Input &input, Position &errorPosition);
  std::shared_ptr<GroupNode> evaluateGroup(const GroupNode &groupNode);
};
} // namespace pfx

#pragma once

namespace pfx
{
struct Error
{
    Position position;
    std::string reason;

    Error(Position pos, std::string reason) : position(pos), reason(reason) {}
    std::string toString();
};

namespace error
{
#define DECLARE_ERROR(Typename, message)            \
    struct Typename : Error                         \
    {                                               \
        Typename(Position p) : Error(p, message) {} \
    };

#define DECLARE_REASON_ERROR(Typename)            \
    struct Typename : Error                         \
    {                                               \
        Typename(Position p, std::string msg) : Error(p, msg) {} \
    };

DECLARE_ERROR(NonExecutableNode, "This node cannot be executed.")
DECLARE_ERROR(NotImplemented, "This functionality is not implemented for this class.")
DECLARE_ERROR(InvalidOperation, "This operation shouldn't run on this class.")
DECLARE_ERROR(ClosingBraceWithoutOpeningOne, "You have a ')' but not the corresponding (.");
DECLARE_ERROR(ClosingBraceExpected, "For forgot to close a '('.");
DECLARE_ERROR(UndefinedCommand, "This command is undefined.");

DECLARE_REASON_ERROR(RuntimeError);
DECLARE_REASON_ERROR(FailedToOpenFile);

#undef DECLARE_ERROR
#undef DECLARE_REASON_ERROR

} // namespace error

}
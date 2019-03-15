/// @file Position.hpp Defines the Position class.

namespace pfx
{
/// Represents a character position in the source code.
struct Position
{
    const char *fn; ///< The file the position is in
    int column;     ///< The column in the file.
    int line;       ///< The line in the file.

    /// @return A string representation of this structure.
    std::string toString();

    /**
     * Raises an exception in this position
     *
     * @param [in] errorMessage The message to pass.
     *
     * @throw error::RuntimeError This is the exception thrown.
     */
    void raiseErrorHere(std::string errorMessage);
};

} // namespace pfx
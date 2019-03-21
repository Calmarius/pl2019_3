/// @file Input.hpp Contains the Input class.

namespace pfx
{
/// Keeps track of the character position while reading characters.
class Input
{
public:
    // Can be a file stream, string stream or whatever.
    std::unique_ptr<std::istream> inputStream;

    const char *fn; // Filename is just stored to know what to report.

    int column = 0; // Zero based but we add +1 when turning it to string.
    /* The general idea is to determine the line count by counting carriage
     * returns and line feeds. Whichever has more it determines the count of
     * lines, this works well with \n and \r\n and \r line endings.
     */

    int crCount = 0;
    int lfCount = 0;
    int tabSize;

public:
    /// @return True if the creation failed for some reason, false otherwise.
    bool fail()
    {
        return inputStream->fail();
    }

    /**
     * Creates the input from a string.
     *
     * @param [in] filename The name of the text file to show.
     * @param [in] source The source code to test.
     * @param [in] tabSize The number of spaces in a tab stop.
     */
    Input(const char *filename, const std::string &source, int tabSize = 4)
        : tabSize(tabSize)
    {
        inputStream = std::make_unique<std::stringstream>(source);
        fn = filename;
    }

    /**
     * Creates the input by reading the source from a file.
     *
     * @param [in] file The file to open.
     * @param [in] tabSize The number of spaces in a tab stop.
     */
    Input(const char *file, int tabSize = 4);

    /**
     * @return True if we are at the end of file.
     *
     * @remarks It just calls the underlying stream's eof() method.
     */
    bool eof()
    {
        return inputStream->eof();
    }

    /**
     * @return The next character in the input.
     *
     * @remarks It just calls the underlying stream's peek() method.
     */
    int peek()
    {
        return inputStream->peek();
    }

    /**
     * @return The next character and consume it.
     */
    int get();

    /**
     * @return The current character position.
     */
    Position getPosition()
    {
        return Position{fn, column + 1,
                        crCount > lfCount ? crCount + 1 : lfCount + 1};
    }
};
} // namespace pfx
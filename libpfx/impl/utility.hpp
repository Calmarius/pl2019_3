/// @file utility.hpp Some utility functions used by the lib.
namespace pfx
{
/**
 *  @param [in] c The character to test.
 *
 * @return True if it's a whitespace character.
 */
bool isWhitespace(char c);

/**
 * Converts a string to integer.
 *
 * @param [in] string The string to convert
 *
 * @return The integer representation.
 */
int stringToInteger(const std::string &string);
/**
 * Converts a string to floating point.
 *
 * @param [in] string The string to convert.
 *
 * @return The floating point representation.
 */
double stringToDouble(const std::string &string);


class Input;
class Token;
/**
 * Reads a word from the input.
 *
 * @param [in,out] input The input to read the word from.
 * @param [out] token The token read.
 *
 * @return True if a word is read, false otherwise.
 */
bool readWord(Input &input, Token &token);
} // namespace pfx
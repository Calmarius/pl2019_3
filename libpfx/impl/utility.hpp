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
} // namespace pfx
namespace pfx
{
struct Position
{
    const char *fn; ///< The file the position is in
    int column;     ///< The column in the file.
    int line;       ///< The line in the file.

    std::string toString();

    void raiseErrorHere(std::string errorMsg);
};

}
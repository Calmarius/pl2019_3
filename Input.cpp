namespace pfx
{
    class Input
    {
        std::ifstream fileStream;
        std::istream *inputStream;
        std::stringstream sstream;
        const char *fn;
        int column = 0;
        int crCount = 0;
        int lfCount = 0;
        int tabSize;

      public:
        bool fail()
        {
            return inputStream->fail();
        }

        Input(const char *filename, std::string source, int tabSize = 4) : sstream(source), tabSize(tabSize)
        {
            fn = filename;
            inputStream = &sstream;
        }

        Input(const char *file, int tabSize = 4) : fileStream(file), tabSize(tabSize)
        {
            inputStream = &fileStream;
            fn = file;
        }

        bool eof() { return inputStream->eof(); }

        int peek() { return inputStream->peek(); }

        int get();

        Position getPosition()
        {
            return Position{fn, column + 1, crCount > lfCount ? crCount + 1 : lfCount + 1};
        }
    };
}
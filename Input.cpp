namespace pfx
{
    int Input::get()
    {
        int c = inputStream->get();

        switch (c)
        {
        case '\t':
        {
            column++;
            column /= tabSize;
            column *= tabSize;
        }
        break;
        case '\r':
        {
            crCount++;
            column = 0;
        }
        break;
        case '\n':
        {
            lfCount++;
            column = 0;
        }
        break;
        default:
            column++;
        }

        return c;
    }
}
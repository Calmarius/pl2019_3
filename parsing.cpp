namespace pfx
{
    bool isWhitespace(char c)
    {
        return ((9 <= c) && (c <= 13)) || (c == ' ');
    }

    bool isNumeric(char c)
    {
        return ('0' <= c) && (c <= '9');
    }

    /*std::string PFX_PositionToString(PFX_Position x)
    {
        return ssprintf("%s:%d:%d", x.fn, x.line, x.column);
    }*/


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


    bool readWord(Input &input, Token &token)
    {
        token = Token();
        token.quoted = false;
        // Consume whitespace.
        while (isWhitespace(input.peek()))
        {
            input.get();
        }
        if (input.eof())
        {
            return false;
        }
        // Read the word
        token.start = input.getPosition();
        if (input.peek() == '"')
        {
            // Quoted string mode.
            while (input.peek() == '"')
            {
                input.get();
                while (!input.eof() && input.peek() != '"')
                {
                    token.word.push_back(input.get());
                }
                input.get();
                if (input.peek() == '"')
                {
                    // It was an escaped "
                    token.word.push_back('"');
                }
            }
            token.quoted = true;
        }
        else
        {
            // Non-quoted
            while (!input.eof() && !isWhitespace(input.peek()))
            {
                token.word.push_back(input.get());
            }
        }
        token.end = input.getPosition();

        return true;
    }
}


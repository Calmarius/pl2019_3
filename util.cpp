namespace pfx
{

    std::string ssprintfv(const char *format, va_list args)
    {
        va_list args2;
        va_copy(args2, args);

        int n = vsnprintf(nullptr, 0, format, args);
        auto c_str = std::make_unique<char[]>(n + 1);
        vsnprintf(c_str.get(), n + 1, format, args2);
        va_end(args2);

        return std::string(c_str.get());
    }

    std::string ssprintf(const char *format, ...)
    {
        va_list args;

        va_start(args, format);
        std::string ret = ssprintfv(format, args);
        va_end(args);

        return ret;
    }

    template <class T, class ...Args>
    std::shared_ptr<T>* createSharedReference(Args&&... args)
    {
        return new std::shared_ptr<T>(new T(args...));
    }

    template <class T>
    std::shared_ptr<T>* resolveSharedReference(void *ptr)
    {
        return static_cast<std::shared_ptr<T>*>(ptr);
    }

    template <class T>
    std::shared_ptr<T>* duplicateSharedReference(void *ptr)
    {
        return new std::shared_ptr<T>(*resolveSharedReference<T>(ptr));
    }

    template <class T>
    T* resolveShared(void *ptr)
    {
        return resolveSharedReference<T>(ptr)->get();
    }
}

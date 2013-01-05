#ifndef H_SINGLETON
#define H_SINGLETON

template<class T>
class Singleton
{
    static T* instance;

    public:
        ~Singleton() { delete instance; }

        static T* Instance()
        {
            if (instance != nullptr)
                return instance;

            return instance = new T();
        }
};

template<class T>
T* Singleton<T>::instance = nullptr;

#define SINGLETON(a) friend class Singleton<a>; \
                          a(); \
                          a(a&);

#endif

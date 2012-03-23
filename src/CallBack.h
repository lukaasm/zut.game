/*#################################################
# Copyright (C) 2006 Arash Partow <http://partow.net/>
#################################################*/

#ifndef H_CALLBACK
#define H_CALLBACK

template < class Class, typename ReturnType, typename Parameter1 = void, typename Parameter2 = void>
class CallBack
{
    public:
        typedef ReturnType (Class::*Method)(Parameter1, Parameter2);

        CallBack(Class* instance, Method method)
        {
            _method = method;
            _instance = instance;
        };

        ReturnType operator()(Parameter1 p1, Parameter2 p2)
        {
            return (_instance->*_method)(p1, p2);
        };

    private:
        Class* _instance;
        Method  _method;
};

template < class Class, typename ReturnType, typename Parameter1>
class CallBack < Class, ReturnType, Parameter1 >
{
    public:
        typedef ReturnType (Class::*Method)(Parameter1);

        CallBack(Class* instance, Method method)
        {
            _method = method;
            _instance = instance;
        };

        ReturnType operator()(Parameter1 p1)
        {
           return (_instance->*_method)(p1);
        };

    private:
        Class* _instance;
        Method  _method;
};

template < class Class, typename ReturnType>
class CallBack < Class, ReturnType >
{
    public:
        typedef ReturnType (Class::*Method)();

        CallBack(Class* instance, Method method)
        {
            _method = method;
            _instance = instance;
        };

        ReturnType operator()()
        {
            return (_instance->*_method)();
        };

    private:
        Class* _instance;
        Method  _method;
};

#endif

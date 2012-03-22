/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CALLBACK
#define H_CALLBACK

template < class Class, typename ReturnType, typename Parameter1, typename Parameter2 >
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

#endif

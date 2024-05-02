#pragma once
#include <iostream>

template <typename T>
class SingletonBase
{
public:

    virtual ~SingletonBase() {
        std::cout << "singleton des" << std::endl;
        //delete instance;
        //instance = nullptr;
    }

    SingletonBase(const SingletonBase&) = delete;

    SingletonBase& operator=(const SingletonBase&) = delete;

    static T& get_instance() {
        static T instance;

        return instance;
    }

protected:

    
    SingletonBase() {
        std::cout << "singleton cons" << std::endl;
    }
};

//template<typename T>
//T* SingletonBase<T>::instance = nullptr;

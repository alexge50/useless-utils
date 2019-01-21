//requires concepts and constraints

#include <type_traits>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <iostream>

template<typename, typename = void>
struct HasBegin: std::false_type {};

template<typename T>
struct HasBegin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type{};

template<typename, typename = void>
struct HasEnd: std::false_type {};

template<typename T>
struct HasEnd<T, std::void_t<decltype(std::declval<T>().end())>> : std::true_type{};

template<typename T>
concept Iterable = HasBegin<T>::value && HasEnd<T>::value;


template<template<typename> class T, typename... Args>
concept Iterables = (... && HasBegin<T<Args>>::value ) && (... && HasEnd<T<Args>>::value);

//template<typename... T>
//concept Iterables = (... && HasBegin<T>::value ) && (... && HasEnd<T>::value);

template<typename Functor, typename... Iterators>
void helper(Functor functor, std::pair<Iterators, Iterators>... iterators)
{
    while(((iterators.first != iterators.second) && ...))
        functor((*(iterators.first++))...);
}

template<typename Functor, template<typename> class iterable, typename... Args>
 requires Iterables<iterable, Args...>
void join(Functor functor, const iterable<Args>&... containers)
{
    helper(functor, std::make_pair(containers.begin(), containers.end())...);
}

void f()
{
    using namespace std::literals::string_literals;

    std::deque<int> d = {1, 2, 3};
    std::deque<std::string> s = {"1"s, "2"s, "3"s};
    std::deque<float> f = {1.f, 2.f, 3.f};

    join([](int d, std::string s, float f){
        std::cout << d << ' ' << s << ' ' << f << '\n';
    }, d, s, f);
}

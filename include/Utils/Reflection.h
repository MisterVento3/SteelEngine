#pragma once

#include "tuple"
#include "string"

namespace SteelEngine {

	struct IReflectionData;

	template<int...> struct index_tuple {};

	template<int I, typename IndexTuple, typename... Types>
	struct make_indexes_impl;

	template<int I, int... Indexes, typename T, typename... Types>
	struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
	{
		typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type;
	};

	template<int I, int... Indexes>
	struct make_indexes_impl<I, index_tuple<Indexes...>>
	{
		typedef index_tuple<Indexes...> type;
	};

	template<typename... Types>
	struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>
	{

	};

	template<class Ret, class... Args, int... Indexes>
	Ret apply_helper(Ret(*pf)(Args...), index_tuple<Indexes...>, std::tuple<Args...>&& tup)
	{
		return pf(std::forward<Args>(std::get<Indexes>(tup))...);
	}

	template<class Ret, class... Args>
	Ret apply(Ret(*pf)(Args...), const std::tuple<Args...>& tup)
	{
		return apply_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
	}

	template<class Ret, class... Args>
	Ret apply(Ret(*pf)(Args...), std::tuple<Args...>&& tup)
	{
		return apply_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
	}

	template<class... Args, int... Indexes>
	void apply_helper(void(*pf)(Args...), index_tuple<Indexes...>, std::tuple<Args...>&& tup)
	{
		pf(std::forward<Args>(std::get<Indexes>(tup))...);
	}

	template<class... Args>
	void apply(void(*pf)(Args...), const std::tuple<Args...>& tup)
	{
		apply_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
	}

	template<class... Args>
	void apply(void(*pf)(Args...), std::tuple<Args...>&& tup)
	{
		apply_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
	}

	template<typename T, typename... Args, int... Indexes>
	void apply_helper(T* obj, void(T::*pf)(Args...), index_tuple<Indexes...>, std::tuple<Args...>&& tup)
	{
		(obj->*pf)(std::forward<Args>(std::get<Indexes>(tup))...);
	}

	template<typename T, typename... Args>
	void apply(T* obj, void(T::*pf)(Args...), const std::tuple<Args...>& tup)
	{
		apply_helper(obj, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
	}

	template<typename T, typename... Args>
	void apply(T* obj, void(T::*pf)(Args...), std::tuple<Args...>&& tup)
	{
		apply_helper(obj, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
	}

	template<typename T, typename Ret, typename... Args, int... Indexes>
	Ret apply_helper(T* obj, Ret(T::*pf)(Args...), index_tuple<Indexes...>, std::tuple<Args...>&& tup)
	{
		return (obj->*pf)(std::forward<Args>(std::get<Indexes>(tup))...);
	}

	template<typename T, typename Ret, typename... Args>
	Ret apply(T* obj, Ret(T::*pf)(Args...), const std::tuple<Args...>& tup)
	{
		return apply_helper(obj, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
	}

	template<typename T, typename Ret, typename... Args>
	Ret apply(T* obj, Ret(T::*pf)(Args...), std::tuple<Args...>&& tup)
	{
		return apply_helper(obj, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
	}

	std::string getFullTypename(const IReflectionData* type);

}
//
//  Iterator.hpp
//  DataCpp
//
//  Created by Luis Finke on 7/18/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <iterator>
#include <tuple>

namespace fgl {
	template <typename T,
		typename TIter = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::end(std::declval<T>()))>
	struct enumerate_iterator {
		size_t i;
		TIter iter;
		bool operator==(const enumerate_iterator& other) const {
			return iter == other.iter;
		}
		bool operator!=(const enumerate_iterator& other) const {
			return iter != other.iter;
		}
		enumerate_iterator& operator++() {
			iter++;
			i++;
			return *this;
		}
		enumerate_iterator operator++(int) {
			enumerate_iterator prevIt = *this;
			iter++;
			i++;
			return prevIt;
		}
		enumerate_iterator& operator--() {
			iter--;
			i--;
			return *this;
		}
		enumerate_iterator operator--(int) {
			enumerate_iterator prevIt = *this;
			iter--;
			i--;
			return prevIt;
		}
		auto operator*() const {
			return std::tie(i, *iter);
		}
	};

	template <typename T,
		typename TIter = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::end(std::declval<T>()))>
	struct enumerate_iterable_wrapper {
		T iterable;
		auto begin() {
			return enumerate_iterator<T,TIter>{ 0, std::begin(iterable) };
		}
		auto end() {
			return enumerate_iterator<T,TIter>{ 0, std::end(iterable) };
		}
	};

	template <typename T,
		typename TIter = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::end(std::declval<T>()))>
	constexpr auto enumerate(T&& iterable) {
		return enumerate_iterable_wrapper<T,TIter>{ std::forward<T>(iterable) };
	}
}

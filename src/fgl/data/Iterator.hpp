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
#include <memory>
#include <tuple>
#include <cmath>

namespace fgl {
	template <typename Iterable,
		typename Iterator = decltype(std::begin(std::declval<Iterable>())),
		typename = decltype(std::end(std::declval<Iterable>()))>
	constexpr auto enumerate(Iterable&& iterable);

	template<typename Value>
	constexpr auto range(Value stop);
	template<typename Value>
	constexpr auto range(Value start, Value stop);
	template<typename Value, typename Step>
	constexpr auto range(Value start, Value stop, Step step);



#pragma mark Iterator implementation

	template <typename InnerIterator>
	struct enumerate_iterator {
		size_t i;
		InnerIterator iter;
		
		inline bool operator==(const enumerate_iterator& other) const {
			return iter == other.iter;
		}
		inline bool operator!=(const enumerate_iterator& other) const {
			return iter != other.iter;
		}
		inline enumerate_iterator& operator++() {
			increment();
			return *this;
		}
		inline enumerate_iterator operator++(int) {
			enumerate_iterator prevIt = *this;
			increment();
			return prevIt;
		}
		inline enumerate_iterator& operator--() {
			decrement();
			return *this;
		}
		inline enumerate_iterator operator--(int) {
			enumerate_iterator prevIt = *this;
			decrement();
			return prevIt;
		}
		inline auto operator*() const {
			return std::tuple<size_t,decltype(*iter)>{ i, *iter };
		}
		
		inline void increment() {
			iter++;
			i++;
		}
		inline void decrement() {
			iter--;
			i--;
		}
	};

	template <typename Iterable,
		typename Iterator = decltype(std::begin(std::declval<Iterable>())),
		typename = decltype(std::end(std::declval<Iterable>()))>
	struct enumerate_iterable_wrapper {
		Iterable& iterable;
		inline auto begin() {
			return enumerate_iterator<Iterator>{ 0, std::begin(iterable) };
		}
		inline auto end() {
			return enumerate_iterator<Iterator>{ 0, std::end(iterable) };
		}
	};

	template <typename Iterable, typename Iterator, typename _>
	constexpr auto enumerate(Iterable&& iterable) {
		return enumerate_iterable_wrapper<Iterable,Iterator>{ std::forward<Iterable>(iterable) };
	}



	template<typename Value, typename Step = Value>
	struct valuerange_iterator {
		Value index;
		Step step;
		
		inline bool operator==(const valuerange_iterator& other) const {
			return index == other.index;
		}
		inline bool operator!=(const valuerange_iterator& other) const {
			return index != other.index;
		}
		inline valuerange_iterator& operator++() {
			increment();
			return *this;
		}
		inline valuerange_iterator operator++(int) {
			valuerange_iterator prevIt = *this;
			increment();
			return prevIt;
		}
		inline valuerange_iterator& operator--() {
			decrement();
			return *this;
		}
		inline valuerange_iterator operator--(int) {
			valuerange_iterator prevIt = *this;
			decrement();
			return prevIt;
		}
		inline auto operator*() const {
			return index;
		}
		inline void increment() {
			index += step;
		}
		inline void decrement() {
			index -= step;
		}
	};

	template<typename Value, typename Step>
	struct valuerange_iterable {
		Value start;
		Value stop;
		Step step;
		inline auto begin() const {
			FGL_ASSERT(!std::is_arithmetic<Step>::value || step != 0, "step cannot be 0!");
			return valuerange_iterator<Value,Step>{ start, step };
		}
		inline auto end() const {
			FGL_ASSERT(!std::is_arithmetic<Step>::value || step != 0, "step cannot be 0!");
			auto endValue = stop;
			if((step > 0 && endValue < start) || (step < 0 && endValue > start)) {
				endValue = start;
			}
			else if(std::fmod((stop - start), step) != 0) {
				size_t count = (size_t)std::floor((stop - start) / step);
				if((start + (step * (Step)count)) != stop) {
					count++;
					endValue = start + (step * (Step)count);
					FGL_ASSERT((step > 0 && (endValue > stop || endValue < start)) || (step < 0 && (endValue < stop || endValue > start)), "Invalid range causes overflow");
				}
			}
			return valuerange_iterator<Value,Step>{ endValue, step };
		}
	};

	template<typename Value>
	constexpr auto range(Value stop) {
		return valuerange_iterable<Value,size_t>{ 0, stop, 1 };
	}

	template<typename Value>
	constexpr auto range(Value start, Value stop) {
		return valuerange_iterable<Value,size_t>{ start, stop, 1 };
	}

	template<typename Value, typename Step>
	constexpr auto range(Value start, Value stop, Step step) {
		return valuerange_iterable<Value,Step>{ start, stop, step };
	}
}

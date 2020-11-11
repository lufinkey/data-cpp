//
//  Iterator.hpp
//  DataCpp
//
//  Created by Luis Finke on 7/18/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Traits.hpp>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <cmath>

namespace fgl {
	template<typename Iterable>
	using BeginIteratorOf = decltype(std::begin(std::declval<Iterable>()));
	template<typename Iterable>
	using EndIteratorOf = decltype(std::end(std::declval<Iterable>()));


	template<typename Iterable>
	using InstBeginIteratorOf = BeginIteratorOf<add_lref_if_nonref<Iterable>>;
	template<typename Iterable>
	using InstEndIteratorOf = EndIteratorOf<add_lref_if_nonref<Iterable>>;


	template<typename Iterable>
	using TransferredBeginIteratorOf = BeginIteratorOf<make_lref_if_rref_or_nonref<Iterable>>;
	template<typename Iterable>
	using TransferredEndIteratorOf = EndIteratorOf<make_lref_if_rref_or_nonref<Iterable>>;


	template <typename Iterable,
		typename = TransferredBeginIteratorOf<Iterable>,
		typename = TransferredEndIteratorOf<Iterable>>
	constexpr auto enumerate(Iterable&& iterable);


	template<typename Value>
	constexpr auto range(Value stop);
	template<typename Value>
	constexpr auto range(Value start, Value stop);
	template<typename Value, typename Step>
	constexpr auto range(Value start, Value stop, Step step);


	template <typename Iterable,
		typename = TransferredBeginIteratorOf<Iterable>,
		typename = TransferredEndIteratorOf<Iterable>>
	constexpr auto reversed(Iterable&& iterable);




#pragma mark Iterator implementation
	


	#pragma mark enumerate

	template <typename InnerIterator>
	struct enumerate_iterator {
		using difference_type = typename std::iterator_traits<InnerIterator>::difference_type;
		using value_type = std::tuple<size_t,decltype(*std::declval<InnerIterator>())>;
		using pointer = void;
		using reference = value_type;
		using iterator_category = typename std::iterator_traits<InnerIterator>::iterator_category;
		
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
		typename = InstBeginIteratorOf<Iterable>,
		typename = InstEndIteratorOf<Iterable>>
	struct enumerate_iterable_wrapper {
		using InnerBeginIterator = InstBeginIteratorOf<Iterable>;
		using InnerEndIterator = InstEndIteratorOf<Iterable>;
		Iterable iterable;
		inline auto distance() {
			using IterableType = std::remove_const_t<std::remove_reference_t<Iterable>>;
			if constexpr(has_memberfunc_size<IterableType,size_t>::value) {
				return std::size(iterable);
			} else {
				return std::distance(std::begin(iterable),std::end(iterable));
			}
		}
		inline auto begin() {
			return enumerate_iterator<InnerBeginIterator>{ 0, std::begin(iterable) };
		}
		inline auto end() {
			return enumerate_iterator<InnerEndIterator>{ distance(), std::end(iterable) };
		}
	};

	template <typename Iterable, typename _, typename _2>
	constexpr auto enumerate(Iterable&& iterable) {
		using ForwardedType = decltype(std::forward<Iterable>(iterable));
		if constexpr(std::is_rvalue_reference_v<ForwardedType>) {
			using IterableType = std::remove_const_t<std::remove_reference_t<Iterable>>;
			return enumerate_iterable_wrapper<IterableType>{ std::forward<Iterable>(iterable) };
		} else {
			return enumerate_iterable_wrapper<ForwardedType>{ std::forward<Iterable>(iterable) };
		}
	}



	#pragma mark range

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
				if(static_cast<Value>(start + (step * (Step)count)) != stop) {
					count++;
					endValue = static_cast<Value>(start + (step * (Step)count));
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



	#pragma mark reverse

	template <typename Iterable,
		typename = InstBeginIteratorOf<Iterable>,
		typename = InstEndIteratorOf<Iterable>>
	struct reversed_iterator_wrapper {
		using InnerBeginIterator = InstBeginIteratorOf<Iterable>;
		using InnerEndIterator = InstEndIteratorOf<Iterable>;
		Iterable iterable;
		inline auto begin() {
			return std::make_reverse_iterator<InnerEndIterator>(iterable.end());
		}
		inline auto end() {
			return std::make_reverse_iterator<InnerBeginIterator>(iterable.begin());
		}
	};

	template <typename Iterable, typename _, typename _2>
	constexpr auto reversed(Iterable&& iterable) {
		using ForwardedType = decltype(std::forward<Iterable>(iterable));
		if constexpr(std::is_rvalue_reference_v<ForwardedType>) {
			using IterableType = std::remove_const_t<std::remove_reference_t<Iterable>>;
			return reversed_iterator_wrapper<IterableType>{ std::forward<Iterable>(iterable) };
		} else {
			return reversed_iterator_wrapper<ForwardedType>{ std::forward<Iterable>(iterable) };
		}
	}
}

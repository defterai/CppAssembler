#pragma once

#include <array>
#include <utility>
#include "asm/defs.h"

namespace CppAsm::Os
{
	template<class T, class T2 = void>
	struct is_byte_ordered {
		static constexpr bool value = true;
	};

	template<class T>
	struct is_byte_ordered<T, typename std::enable_if<sizeof(T) == 1>::type> {
		static constexpr bool value = false;
	};

	template <class T>
	struct is_any_array {
		static constexpr bool value = std::is_array<T>::value;
	};

	template<class T, Size N>
	struct is_any_array<std::array<T, N>> {
		static constexpr bool value = true;
	};

	template<class T>
	struct any_array_remove_extent;

	template<class T>
	struct any_array_remove_extent {
		typedef typename std::remove_extent<T>::type type;
	};

	template<class T, Size N>
	struct any_array_remove_extent<std::array<T, N>> {
		typedef T type;
	};

	template<class T>
	struct any_array_extent {
		static constexpr Size value = std::extent<T>::value;
	};

	template<class T, Size N>
	struct any_array_extent<std::array<T, N> > {
		static constexpr Size value = N;
	};

	template<class T, class T2 = void>
	struct byte_order_split {
		typedef T element_type;

		static constexpr Size count = is_byte_ordered<T>::value ? 1 : sizeof(T);
		static constexpr Size size = is_byte_ordered<T>::value ? sizeof(T) : 1;

		static_assert((size * count) == sizeof(T), "Wrong byte_order_split template");
	};

	template<class T>
	struct byte_order_split<T, typename std::enable_if<is_any_array<T>::value>::type> {
		typedef typename any_array_remove_extent<T>::type sub_type;
		typedef typename byte_order_split<sub_type>::element_type element_type;

		static constexpr Size count = any_array_extent<T>::value * byte_order_split<sub_type>::count;
		static constexpr Size size = byte_order_split<sub_type>::size;

		static_assert((size * count) == sizeof(T), "Wrong byte_order_split template");
	};

	template<endian SRC_ORDER, endian DST_ORDER, Size SIZE>
	struct raw_byte_order_changer;

	template<>
	struct raw_byte_order_changer<endian::little, endian::big, 2> {
		static void execute(char* data) {
			std::swap(data[0], data[1]);
		}
	};

	template<>
	struct raw_byte_order_changer<endian::little, endian::big, 4> {
		static void execute(char* data) {
			std::swap(data[0], data[3]);
			std::swap(data[1], data[2]);
		}
	};

	template<>
	struct raw_byte_order_changer<endian::little, endian::big, 8> {
		static void execute(char* data) {
			std::swap(data[0], data[7]);
			std::swap(data[1], data[6]);
			std::swap(data[2], data[5]);
			std::swap(data[3], data[4]);
		}
	};

	template<>
	struct raw_byte_order_changer<endian::big, endian::little, 2> {
		static void execute(char* data) {
			std::swap(data[0], data[1]);
		}
	};

	template<>
	struct raw_byte_order_changer<endian::big, endian::little, 4> {
		static void execute(char* data) {
			std::swap(data[0], data[3]);
			std::swap(data[1], data[2]);
		}
	};

	template<>
	struct raw_byte_order_changer<endian::big, endian::little, 8> {
		static void execute(char* data) {
			std::swap(data[0], data[7]);
			std::swap(data[1], data[6]);
			std::swap(data[2], data[5]);
			std::swap(data[3], data[4]);
		}
	};

	template<endian SRC_ORDER, endian DST_ORDER, class T>
	struct byte_order_changer {
		static void execute(T& data) {
			char* cdata = reinterpret_cast<char*>(&data);
			raw_byte_order_changer<SRC_ORDER, DST_ORDER, sizeof(T)>::execute(cdata);
		}
	};

	template<class T, endian SRC_ORDER, endian DST_ORDER>
	typename std::enable_if<SRC_ORDER != DST_ORDER && is_byte_ordered<T>::value>::type change_byte_order(void* data, Size count) {
		typedef typename byte_order_split<T>::element_type element_type;
		element_type* cdata = static_cast<element_type*>(data);
		for (Size i = 0; i < byte_order_split<T>::count * count; i++, cdata++) {
			byte_order_changer<SRC_ORDER, DST_ORDER, element_type>::execute(*cdata);
		}
	}

	template<class T, endian SRC_ORDER, endian DST_ORDER>
	typename std::enable_if<SRC_ORDER == DST_ORDER || !is_byte_ordered<T>::value>::type change_byte_order(void* data, Size count) {
		// type is not byte ordered or byte order equal - no need convert
	}
}

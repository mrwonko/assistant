#pragma once

#include <cstddef> // size_t
#include <type_traits> // enable_if, is_same
#include <string> // string
#include <utility> // forward

#include <boost/uuid/uuid.hpp>

// Type-Safe SQL Statements
namespace sql {
	using uuid = boost::uuids::uuid;
	template< typename T >
	constexpr const bool is_uuid() {
		return std::is_same< T, uuid >::value;
	}

	using varchar = std::string; // TODO use std::string_view once C++17 is out
	template< typename T >
	constexpr const bool is_varchar() {
		return std::is_same< T, varchar >::value;
	}

	using integer = int;
	template< typename T >
	constexpr const bool is_integer() {
		return std::is_same< T, integer >::value;
	}

	/// Helper for checking that the given type is one of the basic SQL types.
	template< typename T >
	constexpr const bool is_basic_sql_type() {
		return is_uuid< T >() || is_varchar< T >() || is_integer< T >();
	}

	/// to_sql_type converts a given value to its SQL representation. By default it works for any types that define implicit conversions.
	template< typename From, typename To >
	struct cast {
		static const To apply( From&& x ) {
			return To{ std::forward< From >( x ) }; // if this fails, you probably need to specialize sql::cast for your type
		}
	};

	struct create {
		str_const name;
	};

	template< std::size_t size >
	using creates = const std::array< create, size >;
}

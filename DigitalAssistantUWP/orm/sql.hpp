#pragma once

#include <cstddef> // std::size_t

// Type-Safe SQL Statements
namespace sql {
	struct create {
		str_const name;
	};

	template< std::size_t size >
	using creates = const std::array< create, size >;
}

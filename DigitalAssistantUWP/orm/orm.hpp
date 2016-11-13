#pragma once

#include <string>
#include <array>
#include <algorithm>

/*
# Concept

The goal is to be able to be able to succinctly describe a POD to allow for compile-time reflection such that on-demand ORM is possible.

Use cases:
* Create database schema (create tables)
* Do schema migration (versioning)
* Insert statement from POD, filling in the POD's ID/PK
* Update statement from before and after POD with minimal diff (to allow 3-way merging)
* Delete statement from POD
* Select statement with Member-Pointer-Where

A lot of this can be done nicely through template specialization/function overloading, but then we have no list of all tables so we can't create or migrate the schema.

We probably need to have an object of a type which contains all this, but then how do we do the insert etc. functions?

Probably combine the two. Have one object with template arguments of all the classes/tables for which there are definitions. Maybe this is passed into all the select etc. functions. That way we can also have different objects for different subsets of the tables, but we must then have a distinct version & migration for each version, not a global database version.

A table is defined by:
* name
* primary key
* fields
* maybe constraints later
*/

/// @author Scott Schurr, Willi Schinmeyer
class str_const { // constexpr string
public:
	template< std::size_t N >
	constexpr str_const( const char( &a )[ N ] )
		: _data( a )
		, _size( N - 1 ) {
	}
	constexpr const char operator[]( std::size_t n ) const {
		return n < _size ? _data[ n ] :
			throw std::out_of_range( "" );
	}
	constexpr const std::size_t size() const {
		return _size;
	}
	constexpr const bool operator==( const str_const& rhs ) const {
		return size() == rhs.size() && ( size() == 0 || ( _data[ 0 ] == rhs[ 0 ] && str_const{ _data + 1, _size - 1 } == str_const{ rhs._data + 1, rhs._size - 1 } ) );
	}
private:
	constexpr str_const( const char* const data, const std::size_t size )
		: _data( data )
		, _size( size ) {
	}

	const char* const _data;
	const std::size_t _size;
};

// Type-Safe SQL Statements
namespace sql {
	struct create {
		str_const name;
	};

	template< std::size_t size >
	using creates = const std::array< create, size >;
}

struct table_definition {
	const str_const name;
};

template< typename TableType >
constexpr const table_definition get_table_definition() {
	static_assert( false, "no table definition defined for this type" );
}

// struct used like a namespace so we can inherit from private classes, hiding implementation details
struct orm {
private:
	/// Variadic Template end-of-list tag
	struct eol {
	};

	// Create Table

	template< typename TableType >
	static constexpr const sql::create create() {
		return {
			get_table_definition< TableType >().name,
		};
	}

	template< typename... TableTypes >
	static constexpr const sql::creates< sizeof...( TableTypes ) > create_all() {
		return { create< TableTypes >()... };
	}

	// Per-Type 

	template< typename... InvalidArguments >
	struct mapper_impl {
		mapper_impl() {
			static_assert( false, "logic error: template not fully specialized" );
		}
	};

	template<>
	struct mapper_impl< eol > {
	};

	template< typename TableType, typename... TableTypes >
	struct mapper_impl< TableType, TableTypes... >
		: mapper_impl< TableTypes... > {
	};

public:

	template< typename... TableTypes >
	struct mapper
		: mapper_impl< TableTypes..., eol > {
		constexpr const auto create() const {
			return create_all< TableTypes... >();
		}
	};
};

struct Person {
	std::size_t id;
	std::string name;
	std::size_t father;
};

template<>
constexpr const table_definition get_table_definition< Person >() {
	return table_definition{
		"person"
	};
}

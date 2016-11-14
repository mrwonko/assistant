#pragma once

#include <string>
#include <array>
#include <algorithm>
#include <tuple>
#include <cstddef> // std::size_t

#include "str_const.hpp"
#include "sql.hpp"

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

template< typename TableType, typename ColumnType >
struct column_definition {
	/// type of the class/table this is a member/column of
	using table_type = TableType;
	/// type of the value in the column
	using type = ColumnType;
	/// type of pointer to member/column in class/table
	using pointer_type = const ColumnType TableType::*;
	/// name of the column in the database
	const str_const name;
	/// pointer to member/column in class/table
	const pointer_type pointer;
};

template< typename TableType, typename... ColumnTypes >
using column_definitions = std::tuple< column_definition< TableType, ColumnTypes >... >;

template< typename TableType, typename... ColumnTypes >
struct table_definition {
	using type = TableType;
	const str_const name;
	const column_definitions< TableType, ColumnTypes... > columns;
};

/// table_definition_type defines the type of table definition associated with a given type.
///
/// In order to be able to have template-dependent return types in get_table_definition(), we need this separate definition users can specialize.
template< typename TableType >
struct table_definition_type {
	using type = table_definition< TableType >;
};

/// get_table_definition returns the table definition for a given type. Users should specialize this for types they want to persist.
/// 
/// You'll probably want to specialize table_definition_type as well.
template< typename TableType >
constexpr const typename table_definition_type< TableType >::type get_table_definition() {
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
		return{
			get_table_definition< TableType >().name,
		};
	}

	template< typename... TableTypes >
	static constexpr const sql::creates< sizeof...( TableTypes ) > create_all() {
		return{ create< TableTypes >()... };
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

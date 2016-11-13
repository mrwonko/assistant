#pragma once

#include <string>

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

namespace detail {
	// FIXME: this should be a lot simpler with fold expressions
	template< typename = void, typename... >
	void create_tables();
	template<>
	void create_tables< void >() {
	}
	template< typename Table, typename... Tables >
	void create_tables() {
		static_assert( table_definition< Table >::is_valid, "missing table definition" ); // getting this error means you need to specialize table_definition for your type using make_table_definition
		create_tables< Tables... >();
	}
}

template< typename... Tables >
class mapper {
public:
	void create_tables() {
		detail::create_tables< Tables... >();
	}
};

template< typename T >
struct table_definition {
	static constexpr bool is_valid = false;
};

struct make_table_definition {
	static constexpr bool is_valid = true;
	// more info goes here, coming in via future template parameters
};


struct Person {
	std::size_t id;
	std::string name;
	std::size_t father;
};

template<>
struct table_definition<Person> : make_table_definition {
};

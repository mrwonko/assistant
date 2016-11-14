#include "orm.hpp"
#include <string>

struct Person {
	std::size_t id;
	std::string name;
	std::size_t father;
};

template<>
struct table_definition_type< Person > {
	using type = table_definition< Person, std::size_t, std::string, std::size_t >;
};

template<>
constexpr const table_definition_type< Person >::type get_table_definition< Person >() {
	return{
		"person",
		{
			{ "id", &Person::id },
			{ "name", &Person::name },
			{ "father", &Person::father },
		},
	};
}

void syntaxTest() {
	orm::mapper< Person > mapper;
	auto creates = mapper.create();
	static_assert( orm::mapper< Person >{}.create().at( 0 ).name == "person", "unexpected table name" );

	sql::cast< sql::integer, sql::integer >::apply( 42 ); // cast from basic type to itself is always defined
	//sql::cast< int, sql::varchar >::apply( 12 ); // fails due to undefined cast
}

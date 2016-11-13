#include "orm.hpp"

void syntaxTest() {
	orm::mapper< Person > mapper;
	auto creates = mapper.create();
	static_assert( orm::mapper< Person >{}.create().at( 0 ).name == "person", "unexpected table name" );
}

#include "orm.hpp"

void syntaxTest() {
	mapper<Person>().create_tables();
	mapper<int>().create_tables();
}

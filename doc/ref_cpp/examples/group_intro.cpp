// @@Example: ex_cpp_group_intro @@
// @@Fold@@
#include <iostream>
#include <tightdb.hpp>
#include <tightdb/util/file.hpp>

using namespace std;
using namespace realm;

// @@EndFold@@
REALM_TABLE_2(PeopleTable,
                name, String,
                age, Int)

void func()
{
    Group g("people.tightdb", 0, Group::mode_ReadWrite);

    PeopleTable::Ref table = g.add_table<PeopleTable>("people");

    table->add("Mary", 14);
    table->add("Joe", 17);
    table->add("Jack", 22);

    g.commit();
}
// @@Fold@@

int main()
{
    func();
    util::File::remove("people.tightdb");
}
// @@EndFold@@
// @@EndExample@@

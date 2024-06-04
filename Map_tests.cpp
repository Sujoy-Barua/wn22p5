// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "unit_test_framework.h"

using std::pair;
using std::string;
using std::vector;

TEST(map_comprehensive_test) {
    // A map stores two types, key and value
    Map<string, double> words;
    ASSERT_EQUAL(words.size(), 0);
    ASSERT_EQUAL(words.find("pi"), words.end());

    // One way to use a map is like an array
    words["hello"] = 1;
    ASSERT_EQUAL(words["hello"], 1);
    ASSERT_EQUAL(words.size(), 1);

    // Maps store a std::pair type, which "glues" one key to one value.
    // The CS term is Tuple, a fixed-size heterogeneous container.
    pair<string, double> tuple;
    tuple.first = "world";
    tuple.second = 2;
    words.insert(tuple);

    ASSERT_EQUAL(words["world"], 2);

    // Here's the C++11 way to insert a pair
    words.insert({"pi", 3.14159});
    ASSERT_ALMOST_EQUAL(words["pi"], 3.14159, 0.00001);
    ASSERT_EQUAL(words.find("li"), words.end());

    vector<string> expected_keys = { "hello", "pi", "world" };
    vector<double> expected_values = { 1, 3.14159, 2 };
    vector<string> actual_keys;
    vector<double> actual_values;
    // Iterate over map contents using a C++11 range-for loop
    // This is the equivalent without C++11:
    // for (Map<string, double>::Iterator it = words.begin();
    //      it != words.end(); ++it) {
    //   pair<string, double> &p = *it;
    for (auto &p : words) {
    auto word = p.first; //key
    auto number = p.second; //value
    actual_keys.push_back(word);
    actual_values.push_back(number);
    }
    ASSERT_EQUAL(expected_keys, actual_keys);
    ASSERT_EQUAL(expected_values, actual_values);

    // Check if a key is in the map.  find() returns an iterator.
    auto found_it = words.find("pi");
    ASSERT_NOT_EQUAL(found_it, words.end());
    auto &word = (*found_it).first; //key
    auto number = (*found_it).second; //value
    ASSERT_EQUAL(word, "pi");
    ASSERT_ALMOST_EQUAL(number, 3.14159, 0.00001);

    // When using the [] notation. An element not found is automatically created.
    // If the value type of the map is numeric, it will always be 0 "by default".
    ASSERT_EQUAL(words["bleh"], 0.0);

    ASSERT_EQUAL(words.size(), 4);

    Map<string, double> words_2;
    ASSERT_EQUAL(words_2.size(), 0);
    words_2["map"] = 10;
    ASSERT_EQUAL(words["map"], 0);

    pair<string, double> tuple2;
    tuple2.first = "world";
    tuple2.second = 20;
    pair<Map<string, double>::Iterator, bool> 
        iterbool2 = words_2.insert(tuple2);
    ASSERT_EQUAL((iterbool2.first)->second, 20);
    ASSERT_EQUAL(iterbool2.second, true);
    
    

    ASSERT_EQUAL(words_2.find("world")->second, 20);

    pair<string, double> tuple3;
    tuple3.first = "world";
    tuple3.second = 30;
    pair<Map<string, double>::Iterator, bool> 
        iterbool = words_2.insert(tuple3);
    ASSERT_EQUAL(words_2.begin()->second, 10);
    ASSERT_EQUAL((iterbool.first)->second, 20);
    ASSERT_EQUAL(iterbool.second, false);

    Map<string, double>::Iterator 
        iter2 = words_2.find("world");
    ASSERT_EQUAL(words_2.begin()->second, 10);
    ASSERT_EQUAL(iter2->second, 20);
}

TEST_MAIN()

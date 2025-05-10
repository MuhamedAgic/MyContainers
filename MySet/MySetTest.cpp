
#include <gtest/gtest.h>
#include "UnorderedSet.h"

using namespace MyContainers;

using ::testing::Types;

// 1. Define the list of types you want to test
typedef Types<
        char,
        signed char,
        unsigned char,
        short,
        unsigned short,
        int,
        unsigned int,
        double,
        long double,
        float,
        long,
        long long,
        unsigned long,
        unsigned long long,
        long int,
        long long int,
        std::string> MySetTypes;

// 2. Define a test fixture template
// This creates a test fixture template, so you can write tests that are generic over type T.
// Within each test, TypeParam will refer to the current type being tested
template <typename T>
class MySetTypedTest : public ::testing::Test {
protected:

    // To generate sample values for unit tests
    static T sample_value() {
        if constexpr (std::is_same_v<T, std::string>) {
            return "a";
        }
        else {
            return T(42);
        }
    }
};

// This tells Google Test:
// “For the test fixture MySetTypedTest, run the tests for every type in MySetTypes.”
// So, if you write TYPED_TEST(MySetTypedTest, InsertAddsElement), it will generate n tests, one for every type in MySetTypes
TYPED_TEST_SUITE(MySetTypedTest, MySetTypes);

// 3. Write tests using TYPED_TEST
TYPED_TEST(MySetTypedTest, InsertAddsElement) {
    UnorderedSet<TypeParam> s; // TypeParam is a special name provided by Google Test inside typed tests; it refers to the current type being tested
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    EXPECT_TRUE(s.contains(val));
    EXPECT_EQ(s.size(), 1);
}

TYPED_TEST(MySetTypedTest, InsertDuplicateDoesNotIncreaseSize) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    EXPECT_FALSE(s.add(val));
    EXPECT_TRUE(s.contains(val));
    EXPECT_EQ(s.size(), 1);
}

TYPED_TEST(MySetTypedTest, RemoveDecreasesElement) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    EXPECT_TRUE(s.contains(val));
    EXPECT_EQ(s.size(), 1);
    EXPECT_TRUE(s.remove(val));
    EXPECT_FALSE(s.contains(val));
    EXPECT_EQ(s.size(), 0);
}

TYPED_TEST(MySetTypedTest, EmptySetBehavesCorrectly) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_EQ(s.size(), 0);
    EXPECT_FALSE(s.contains(val));
    EXPECT_FALSE(s.remove(val));
}

TYPED_TEST(MySetTypedTest, InsertMultipleElements) {
    UnorderedSet<TypeParam> s;
    TypeParam val1 = TestFixture::sample_value();
    TypeParam val2 = val1;
    if constexpr (std::is_same_v<TypeParam, std::string>) {
        val2 = "b";
    } else {
        val2 = TypeParam(43);
    }
    EXPECT_TRUE(s.add(val1));
    EXPECT_TRUE(s.add(val2));
    EXPECT_TRUE(s.contains(val1));
    EXPECT_TRUE(s.contains(val2));
    EXPECT_EQ(s.size(), 2);
}

TYPED_TEST(MySetTypedTest, RemoveNonexistentElementDoesNothing) {
    UnorderedSet<TypeParam> s;
    TypeParam val1 = TestFixture::sample_value();
    TypeParam val2 = val1;
    if constexpr (std::is_same_v<TypeParam, std::string>) {
        val2 = "b";
    } else {
        val2 = TypeParam(43);
    }
    EXPECT_TRUE(s.add(val1));
    EXPECT_FALSE(s.remove(val2));
    EXPECT_TRUE(s.contains(val1));
    EXPECT_FALSE(s.contains(val2));
    EXPECT_EQ(s.size(), 1);
}

TYPED_TEST(MySetTypedTest, HandlesManyElements) {
    UnorderedSet<TypeParam> s;
    // Insert more elements than the default capacity
    for (std::size_t i = 0; i < 20; ++i) {
        TypeParam val;
        if constexpr (std::is_same_v<TypeParam, std::string>) {
            val = std::string(1, 'a' + (i % 26));
        } else {
            val = TypeParam(100 + i);
        }
        EXPECT_TRUE(s.add(val));
        EXPECT_TRUE(s.contains(val));
    }
    EXPECT_EQ(s.size(), 20);
}

TYPED_TEST(MySetTypedTest, CopyConstructorCreatesEqualSet) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    UnorderedSet<TypeParam> copy(s);
    EXPECT_TRUE(copy.contains(val));
    EXPECT_EQ(copy.size(), s.size());
    EXPECT_EQ(copy, s);
}

TYPED_TEST(MySetTypedTest, AssignmentOperatorCreatesEqualSet) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    UnorderedSet<TypeParam> copy;
    copy = s;
    EXPECT_TRUE(copy.contains(val));
    EXPECT_EQ(copy.size(), s.size());
    EXPECT_EQ(copy, s);
}

TYPED_TEST(MySetTypedTest, SelfAssignmentDoesNotChangeSet) {
    UnorderedSet<TypeParam> s;
    TypeParam val = TestFixture::sample_value();
    EXPECT_TRUE(s.add(val));
    s = s; // Self-assignment
    EXPECT_TRUE(s.contains(val));
    EXPECT_EQ(s.size(), 1);
}

TYPED_TEST(MySetTypedTest, InequalityOperatorWorks) {
    UnorderedSet<TypeParam> s1, s2;
    TypeParam val1 = TestFixture::sample_value();
    TypeParam val2 = val1;
    if constexpr (std::is_same_v<TypeParam, std::string>) {
        val2 = "b";
    } else {
        val2 = TypeParam(43);
    }
    s1.add(val1);
    s2.add(val2);
    EXPECT_NE(s1, s2);
}

TYPED_TEST(MySetTypedTest, EqualityOperatorWorks) {
    UnorderedSet<TypeParam> s1, s2;
    TypeParam val1 = TestFixture::sample_value();
    s1.add(val1);
    s2.add(val1);
    EXPECT_EQ(s1, s2);
}




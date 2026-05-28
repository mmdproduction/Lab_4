#include <gtest/gtest.h>
#include "Optional.hpp"
#include "Exception.hpp"

TEST(Optional, NoneHasNoValue) {
    auto opt = Optional<int>::none();
    EXPECT_FALSE(opt.has_value());
}
 
TEST(Optional, SomeHasValue) {
    auto opt = Optional<int>::some(42);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), 42);
}
 
TEST(Optional, ValueOnNoneThrows) {
    auto opt = Optional<int>::none();
    EXPECT_THROW(opt.value(), OptionalHasNotValue);
}
 
TEST(Optional, CopyOfSomeHasValue) {
    auto opt = Optional<int>::some(7);
    auto copy = opt;
    EXPECT_TRUE(copy.has_value());
    EXPECT_EQ(copy.value(), 7);
}
 
TEST(Optional, CopyOfNoneHasNoValue) {
    auto opt = Optional<int>::none();
    auto copy = opt;
    EXPECT_FALSE(copy.has_value());
}
 
TEST(Optional, AssignSomeToNone) {
    auto opt = Optional<int>::none();
    opt = Optional<int>::some(99);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), 9);
}
 
TEST(Optional, AssignNoneToSome) {
    auto opt = Optional<int>::some(5);
    opt = Optional<int>::none();
    EXPECT_FALSE(opt.has_value());
}
 
TEST(Optional, WorksWithNonTrivialType) {
    auto opt = Optional<std::string>::some("hello");
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), "hello");
}


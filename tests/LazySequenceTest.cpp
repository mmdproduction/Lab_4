#include "LazySequence.hpp"
#include <gtest/gtest.h>

TEST(LazySequence, DefaultConstructorIsEmpty) {
    LazySequence<int> seq;
    EXPECT_EQ(seq.get_materialized_count(), 0);
    EXPECT_FALSE(seq.is_infinite());
}
 
TEST(LazySequence, ConstructFromArray) {
    int items[] = {1, 2, 3};
    LazySequence<int> seq(items, 3);
    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 3);
}
 
TEST(LazySequence, ConstructFromSequence) {
    ArraySequence<int> s;
    s.append(10); s.append(20); s.append(30);
    LazySequence<int> seq(s);
    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(1), 20);
    EXPECT_EQ(seq.get(2), 30);
}
 
TEST(LazySequence, ConstructInfiniteRecurrent) {

    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    LazySequence<int> fib(
        [](const Sequence<int>& w) { return w.get(0) + w.get(1); },
        seed, 2
    );
    EXPECT_TRUE(fib.is_infinite());
}
 
TEST(LazySequence, CopyConstructorCopiesCache) {
    int items[] = {1, 2, 3};
    LazySequence<int> original(items, 3);
    LazySequence<int> copy(original);
    EXPECT_EQ(copy.get(0), 1);
    EXPECT_EQ(copy.get(1), 2);
    EXPECT_EQ(copy.get(2), 3);
}
 
TEST(LazySequence, CopyConstructorIsIndependent) {
    int items[] = {1, 2, 3};
    LazySequence<int> original(items, 3);
    LazySequence<int> copy(original);
    copy.append(99);
    EXPECT_EQ(original.get(0), 1);
}

TEST(LazySequence, GetFirstElement) {
    int items[] = {42, 1, 2};
    LazySequence<int> seq(items, 3);
    EXPECT_EQ(seq.get_first(), 42);
}
 
TEST(LazySequence, GetLastElement) {
    int items[] = {1, 2, 99};
    LazySequence<int> seq(items, 3);
    EXPECT_EQ(seq.get_last(), 99);
}
 
TEST(LazySequence, GetFirstOnEmptyThrows) {
    LazySequence<int> seq;
    EXPECT_THROW(seq.get_first(), EmptySequence);
}
 
TEST(LazySequence, GetLastOnEmptyThrows) {
    LazySequence<int> seq;
    EXPECT_THROW(seq.get_last(), EmptySequence);
}
 
TEST(LazySequence, GetOutOfRangeThrows) {
    int items[] = {1, 2, 3};
    LazySequence<int> seq(items, 3);
    EXPECT_THROW(seq.get(5), IndexOutOfRange);
}

TEST(LazySequence, FiniteSequenceHasFiniteLength) {
    int items[] = {1, 2, 3};
    LazySequence<int> seq(items, 3);
    Cardinal len = seq.get_length();
    EXPECT_FALSE(len.is_infinite());
    EXPECT_EQ(len.get_value(), 3);
}
 
TEST(LazySequence, EmptySequenceHasFiniteLengthZero) {
    LazySequence<int> seq;
    Cardinal len = seq.get_length();
    EXPECT_FALSE(len.is_infinite());
    EXPECT_EQ(len.get_value(), 0);
}
 
TEST(LazySequence, InfiniteSequenceHasInfiniteLength) {
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    LazySequence<int> fib(
        [](const Sequence<int>& w) { return w.get(0) + w.get(1); },
        seed, 2
    );
    Cardinal len = fib.get_length();
    EXPECT_TRUE(len.is_infinite());
}
 
TEST(LazySequence, FiniteLengthsAreEqual) {
    int a[] = {1, 2, 3};
    int b[] = {4, 5, 6};
    LazySequence<int> seqA(a, 3);
    LazySequence<int> seqB(b, 3);
    EXPECT_TRUE(seqA.get_length() == seqB.get_length());
}
 
TEST(LazySequence, InfiniteLengthsAreEqual) {
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    LazySequence<int> fib1(
        [](const Sequence<int>& w) { return w.get(0) + w.get(1); },
        seed, 2
    );
    LazySequence<int> fib2(
        [](const Sequence<int>& w) { return w.get(0) + w.get(1); },
        seed, 2
    );
    EXPECT_TRUE(fib1.get_length() == fib2.get_length());
}
 
TEST(LazySequence, FiniteAndInfiniteLengthsAreNotEqual) {
    int items[] = {1, 2, 3};
    LazySequence<int> finite(items, 3);
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    LazySequence<int> infinite(
        [](const Sequence<int>& w) { return w.get(0) + w.get(1); },
        seed, 2
    );
    EXPECT_FALSE(finite.get_length() == infinite.get_length());
}
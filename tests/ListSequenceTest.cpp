#include <gtest/gtest.h>
#include "Sequence.hpp"



TEST(ListSequenceTest, DefaultConstructor){
    ListSequence<int> list;
    EXPECT_EQ(list.getLength(), 0);
}

TEST(ListSequenceTest, ConstructorWithArray){
    int a[] = {1, 2, 3, 4};
    ListSequence<int> list(a, 4);
    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(3), 4);
}

TEST(ListSequenceTest, ConstructorWithInitializeList){
    ListSequence<int> list({1, 2, 3, 4});
    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(3), 4);
}

TEST(ListSequenceTest, AppendAndGet) {
    ListSequence<int> list;
    list.append(42);
    list.append(100);
    
    EXPECT_EQ(list.get(0), 42);
    EXPECT_EQ(list.get(1), 100);
}

TEST(ListSequenceTest, PrependAndGet) {
    ListSequence<int> list;
    list.prepend(42);
    list.prepend(100);
    
    EXPECT_EQ(list.get(1), 42);
    EXPECT_EQ(list.get(0), 100);
}

TEST(ListSequenceTest, AppendAndOperator) {
    ListSequence<int> list;
    list.append(42);
    list.append(100);
    
    EXPECT_EQ(list[0], 42);
    EXPECT_EQ(list[1], 100);
}

TEST(ListSequenceTest, PrependAndOperator) {
    ListSequence<int> list;
    list.prepend(42);
    list.prepend(100);
    
    EXPECT_EQ(list[1], 42);
    EXPECT_EQ(list[0], 100);
}

TEST(ListSequenceTest, OutOfRange) {
    ListSequence<int> list;
    
    EXPECT_THROW(list.get(3), IndexOutOfRange);
}

TEST(ListSequenceTest, CopyConstructor) {
    ListSequence<int> original;
    original.append(1);
    original.append(12);
    original.append(23);
    
    ListSequence<int> copy(original);
    
    EXPECT_EQ(copy.getLength(), 3);
    EXPECT_EQ(copy.get(0), 1);
    EXPECT_EQ(copy.get(1), 12);
    EXPECT_EQ(copy.get(2), 23);
}

TEST(ListSequenceTest, GetFirstAndGetLast) {
    ListSequence<int> arraySeq;
    arraySeq.append(1);
    arraySeq.append(12);
    arraySeq.append(23);

    
    EXPECT_EQ(arraySeq.getLength(), 3);
    EXPECT_EQ(arraySeq.getFirst(), 1);
    EXPECT_EQ(arraySeq.getLast(), 23);
    
}

TEST(ListSequenceTest, StringDefaultConstructor){
    ListSequence<std::string> list;
    EXPECT_EQ(list.getLength(), 0);
}

TEST(ListSequenceTest, StringConstructorWithArray){
    std::string a[] = {"1", "2", "3", "4"};
    ListSequence<std::string> list(a, 4);
    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(3), "4");
}

TEST(ListSequenceTest, StringConstructorWithInitializeList){
    ListSequence<std::string> list({"1", "2", "3", "4"});
    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(3), "4");
}

TEST(ListSequenceTest, StringAppendAndGet) {
    ListSequence<std::string> list;
    list.append("42");
    list.append("100");
    
    EXPECT_EQ(list.get(0), "42");
    EXPECT_EQ(list.get(1), "100");
}

TEST(ListSequenceTest, StringPrependAndGet) {
    ListSequence<std::string> list;
    list.prepend("42");
    list.prepend("100");
    
    EXPECT_EQ(list.get(1), "42");
    EXPECT_EQ(list.get(0), "100");
}

TEST(ListSequenceTest, StringAppendAndOperator) {
    ListSequence<std::string> list;
    list.append("42");
    list.append("100");
    
    EXPECT_EQ(list[0], "42");
    EXPECT_EQ(list[1], "100");
}

TEST(ListSequenceTest, StringPrependAndOperator) {
    ListSequence<std::string> list;
    list.prepend("42");
    list.prepend("100");
    
    EXPECT_EQ(list[1], "42");
    EXPECT_EQ(list[0], "100");
}

TEST(ListSequenceTest, StringOutOfRange) {
    ListSequence<std::string> list;
    
    EXPECT_THROW(list.get(3), IndexOutOfRange);
}

TEST(ListSequenceTest, StringCopyConstructor) {
    ListSequence<std::string> original;
    original.append("1");
    original.append("12");
    original.append("23");
    
    ListSequence<std::string> copy(original);
    
    EXPECT_EQ(copy.getLength(), 3);
    EXPECT_EQ(copy.get(0), "1");
    EXPECT_EQ(copy.get(1), "12");
    EXPECT_EQ(copy.get(2), "23");
}

TEST(ListSequenceTest, StringGetFirstAndgetLast) {
    ListSequence<std::string> arraySeq;
    arraySeq.append("1");
    arraySeq.append("12");
    arraySeq.append("23");

    
    EXPECT_EQ(arraySeq.getLength(), 3);
    EXPECT_EQ(arraySeq.getFirst(), "1");
    EXPECT_EQ(arraySeq.getLast(), "23");
    
}

TEST(ListSequenceTest, mapIntTest) {
    ListSequence<int> list;
    list.append(42);
    list.append(100);

    auto mList = list.map<int>([](int x){ return 2*x;});

    
    EXPECT_EQ(mList->getFirst(), 84);
    EXPECT_EQ(mList->get(1), 200);
}
#include <gtest/gtest.h>
#include "Generator.hpp"
#include "Sequence.hpp"


TEST(FiniteGenerator, HasNextFalseOnEmptySequence) {
    ArraySequence<int> seq;
    FiniteGenerator<int> gen(seq);
    EXPECT_FALSE(gen.has_next());
}
 
TEST(FiniteGenerator, ReturnsElementsInOrder) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(2); seq.append(3);
    FiniteGenerator<int> gen(seq);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 3);
}
 
TEST(FiniteGenerator, HasNextFalseAfterAllConsumed) {
    ArraySequence<int> seq;
    seq.append(1);
    FiniteGenerator<int> gen(seq);
    gen.get_next();
    EXPECT_FALSE(gen.has_next());
}
 
TEST(FiniteGenerator, GetNextOnExhaustedThrows) {
    ArraySequence<int> seq;
    seq.append(1);
    FiniteGenerator<int> gen(seq);
    gen.get_next();
    EXPECT_THROW(gen.get_next(), InvalidNextValue);
}
 
TEST(FiniteGenerator, TryGetNextReturnsSome) {
    ArraySequence<int> seq;
    seq.append(42);
    FiniteGenerator<int> gen(seq);
    auto result = gen.try_get_next();
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 42);
}
 
TEST(FiniteGenerator, TryGetNextReturnsNoneWhenExhausted) {
    ArraySequence<int> seq;
    seq.append(1);
    FiniteGenerator<int> gen(seq);
    gen.get_next();
    EXPECT_FALSE(gen.try_get_next().has_value());
}
 
TEST(FiniteGenerator, CloneIsIndependent) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(2); seq.append(3);
    FiniteGenerator<int> gen(seq);
    gen.get_next(); // pos=1
    auto* clone = gen.clone();
    EXPECT_EQ(clone->get_next(), 2); 
    EXPECT_EQ(gen.get_next(), 2);    
    delete clone;
}

TEST(DifferenceGenerator, EmptySeedThrows) {
    ArraySequence<double> seed;
    EXPECT_THROW(DifferenceGenerator<double>(seed), InvalidArgument);
}
 
TEST(DifferenceGenerator, HasNextAlwaysTrue) {
    ArraySequence<double> seed;
    seed.append(1); seed.append(2);
    DifferenceGenerator<double> gen(seed);
    for (int i = 0; i < 100; ++i)
        EXPECT_TRUE(gen.has_next());
}
 
TEST(DifferenceGenerator, ConstantSequence) {
    
    ArraySequence<double> seed;
    seed.append(5); seed.append(5); seed.append(5);
    DifferenceGenerator<double> gen(seed);
    EXPECT_DOUBLE_EQ(gen.get_next(), 5);
    EXPECT_DOUBLE_EQ(gen.get_next(), 5);
    EXPECT_DOUBLE_EQ(gen.get_next(), 5);
}
 
TEST(DifferenceGenerator, LinearSequence) {
    
    ArraySequence<double> seed;
    seed.append(1); seed.append(3); seed.append(5); seed.append(7);
    DifferenceGenerator<double> gen(seed);
    EXPECT_DOUBLE_EQ(gen.get_next(), 9);
    EXPECT_DOUBLE_EQ(gen.get_next(), 11);
    EXPECT_DOUBLE_EQ(gen.get_next(), 13);
}
 
TEST(DifferenceGenerator, QuadraticSequence) {
    
    ArraySequence<double> seed;
    seed.append(1); seed.append(4); seed.append(9); seed.append(16);
    DifferenceGenerator<double> gen(seed);
    EXPECT_DOUBLE_EQ(gen.get_next(), 25);
    EXPECT_DOUBLE_EQ(gen.get_next(), 36);
    EXPECT_DOUBLE_EQ(gen.get_next(), 49);
}
 
TEST(DifferenceGenerator, CubicSequence) {
    
    ArraySequence<double> seed;
    seed.append(1); seed.append(8); seed.append(27);
    seed.append(64); seed.append(125);
    DifferenceGenerator<double> gen(seed);
    EXPECT_DOUBLE_EQ(gen.get_next(), 216);
    EXPECT_DOUBLE_EQ(gen.get_next(), 343);
}
 
TEST(DifferenceGenerator, SingleElementRepeats) {
    ArraySequence<double> seed;
    seed.append(7);
    DifferenceGenerator<double> gen(seed);
    EXPECT_DOUBLE_EQ(gen.get_next(), 7);
    EXPECT_DOUBLE_EQ(gen.get_next(), 7);
}
 
TEST(DifferenceGenerator, CloneHasSameState) {
    ArraySequence<double> seed;
    seed.append(1); seed.append(4); seed.append(9); seed.append(16);
    DifferenceGenerator<double> gen(seed);
    gen.get_next(); // 25
    auto* clone = gen.clone();
    EXPECT_DOUBLE_EQ(clone->get_next(), gen.get_next()); 
    delete clone;
}

TEST(ReccurentGenerator, WrongSeedSizeThrows) {
    ArraySequence<int> seed;
    seed.append(1); 
    EXPECT_THROW(
        ReccurentGenerator<int>([](const Sequence<int>& w){ return w.get(0) + w.get(1); }, seed, 2),
        InvalidArgument
    );
}
 
TEST(ReccurentGenerator, HasNextAlwaysTrue) {
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    ReccurentGenerator<int> gen(
        [](const Sequence<int>& w){ return w.get(0) + w.get(1); }, seed, 2
    );
    for (int i = 0; i < 100; ++i)
        EXPECT_TRUE(gen.has_next());
}
 
TEST(ReccurentGenerator, FibonacciSequence) {
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    ReccurentGenerator<int> gen(
        [](const Sequence<int>& w){ return w.get(0) + w.get(1); }, seed, 2
    );
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(gen.get_next(), 5);
    EXPECT_EQ(gen.get_next(), 8);
    EXPECT_EQ(gen.get_next(), 13);
}
 
TEST(ReccurentGenerator, WindowSizeOne) {
    ArraySequence<int> seed;
    seed.append(1);
    ReccurentGenerator<int> gen(
        [](const Sequence<int>& w){ return w.get(0) * 2; }, seed, 1
    );
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 4);
    EXPECT_EQ(gen.get_next(), 8);
}
 
TEST(ReccurentGenerator, CloneIsIndependent) {
    ArraySequence<int> seed;
    seed.append(1); seed.append(1);
    ReccurentGenerator<int> gen(
        [](const Sequence<int>& w){ return w.get(0) + w.get(1); }, seed, 2
    );
    gen.get_next(); // 2
    gen.get_next(); // 3
    auto* clone = gen.clone();
    EXPECT_EQ(clone->get_next(), gen.get_next()); // оба дают 5
    delete clone;
}

TEST(MapGenerator, MapsEachElement) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(2); seq.append(3);
    auto* gen = new MapGenerator<int, int>(
        new FiniteGenerator<int>(seq), [](int x){ return x * 2; }
    );
    EXPECT_EQ(gen->get_next(), 2);
    EXPECT_EQ(gen->get_next(), 4);
    EXPECT_EQ(gen->get_next(), 6);
    delete gen;
}
 
TEST(MapGenerator, ChangesType) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(0);
    auto* gen = new MapGenerator<int, bool>(
        new FiniteGenerator<int>(seq), [](int x){ return x != 0; }
    );
    EXPECT_TRUE(gen->get_next());
    EXPECT_FALSE(gen->get_next());
    delete gen;
}
 
TEST(MapGenerator, HasNextFalseWhenSourceExhausted) {
    ArraySequence<int> seq;
    seq.append(1);
    auto* gen = new MapGenerator<int, int>(
        new FiniteGenerator<int>(seq), [](int x){ return x; }
    );
    gen->get_next();
    EXPECT_FALSE(gen->has_next());
    delete gen;
}

TEST(WhereGenerator, FiltersOddNumbers) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(2); seq.append(3); seq.append(4);
    auto* gen = new WhereGenerator<int>(
        new FiniteGenerator<int>(seq), [](int x){ return x % 2 == 0; }
    );
    EXPECT_EQ(gen->get_next(), 2);
    EXPECT_EQ(gen->get_next(), 4);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 
TEST(WhereGenerator, HasNextFalseWhenNonePass) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(3);
    auto* gen = new WhereGenerator<int>(
        new FiniteGenerator<int>(seq), [](int x){ return x % 2 == 0; }
    );
    EXPECT_FALSE(gen->has_next());
    delete gen;
}

TEST(ConcatGenerator, ReturnsFirstThenSecond) {
    ArraySequence<int> a, b;
    a.append(1); a.append(2);
    b.append(3); b.append(4);
    auto* gen = new ConcatGenerator<int>(
        new FiniteGenerator<int>(a), new FiniteGenerator<int>(b)
    );
    EXPECT_EQ(gen->get_next(), 1);
    EXPECT_EQ(gen->get_next(), 2);
    EXPECT_EQ(gen->get_next(), 3);
    EXPECT_EQ(gen->get_next(), 4);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 
TEST(ConcatGenerator, EmptyFirst) {
    ArraySequence<int> a, b;
    b.append(1); b.append(2);
    auto* gen = new ConcatGenerator<int>(
        new FiniteGenerator<int>(a), new FiniteGenerator<int>(b)
    );
    EXPECT_EQ(gen->get_next(), 1);
    EXPECT_EQ(gen->get_next(), 2);
    delete gen;
}

TEST(PrependGenerator, PrependedElementComesFirst) {
    ArraySequence<int> seq;
    seq.append(2); seq.append(3);
    auto* gen = new PrependGenerator<int>(new FiniteGenerator<int>(seq), 1);
    EXPECT_EQ(gen->get_next(), 1);
    EXPECT_EQ(gen->get_next(), 2);
    EXPECT_EQ(gen->get_next(), 3);
    delete gen;
}
 
TEST(PrependGenerator, PrependToEmpty) {
    ArraySequence<int> seq;
    auto* gen = new PrependGenerator<int>(new FiniteGenerator<int>(seq), 42);
    EXPECT_TRUE(gen->has_next());
    EXPECT_EQ(gen->get_next(), 42);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}

TEST(AppendGenerator, AppendedElementComesLast) {
    ArraySequence<int> seq;
    seq.append(1); seq.append(2);
    auto* gen = new AppendGenerator<int>(new FiniteGenerator<int>(seq), 3);
    EXPECT_EQ(gen->get_next(), 1);
    EXPECT_EQ(gen->get_next(), 2);
    EXPECT_EQ(gen->get_next(), 3);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 
TEST(AppendGenerator, AppendToEmpty) {
    ArraySequence<int> seq;
    auto* gen = new AppendGenerator<int>(new FiniteGenerator<int>(seq), 99);
    EXPECT_TRUE(gen->has_next());
    EXPECT_EQ(gen->get_next(), 99);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 

TEST(ZipGenerator, ZipsEqualLengthSequences) {
    ArraySequence<int> a, b;
    a.append(1); a.append(2);
    b.append(3); b.append(4);
    auto* gen = new ZipGenerator<int, int>(
        new FiniteGenerator<int>(a), new FiniteGenerator<int>(b)
    );
    auto p1 = gen->get_next();
    EXPECT_EQ(p1.first, 1); EXPECT_EQ(p1.second, 3);
    auto p2 = gen->get_next();
    EXPECT_EQ(p2.first, 2); EXPECT_EQ(p2.second, 4);
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 
TEST(ZipGenerator, StopsAtShorterSequence) {
    ArraySequence<int> a, b;
    a.append(1); a.append(2); a.append(3);
    b.append(10);
    auto* gen = new ZipGenerator<int, int>(
        new FiniteGenerator<int>(a), new FiniteGenerator<int>(b)
    );
    gen->get_next();
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
 
TEST(ZipGenerator, EmptySequenceProducesNoPairs) {
    ArraySequence<int> a, b;
    a.append(1);
    auto* gen = new ZipGenerator<int, int>(
        new FiniteGenerator<int>(a), new FiniteGenerator<int>(b)
    );
    EXPECT_FALSE(gen->has_next());
    delete gen;
}
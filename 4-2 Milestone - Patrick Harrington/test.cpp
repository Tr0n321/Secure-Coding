// Uncomment the next line to use precompiled headers in Visual Studio.
// #include "pch.h"
// Uncomment the next line if you do not use precompiled headers.
#include "gtest/gtest.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <vector>

// The global test environment setup and tear down.
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        // Initialize random seed.
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// Register the test environment before RUN_ALL_TESTS executes.
::testing::Environment* const globalEnvironment =
    ::testing::AddGlobalTestEnvironment(new Environment);

// Create our test class to house shared data between tests.
class CollectionTest : public ::testing::Test
{
protected:
    // Create a smart pointer to hold our collection.
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    {
        // Create a new collection to be used in each test.
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    {
        // Erase all elements in the collection, if any remain.
        collection->clear();

        // Free the pointer.
        collection.reset(nullptr);
    }

    // Helper function to add random values from 0 to 99 count times to the collection.
    void add_entries(int count)
    {
        assert(count > 0);

        for (auto i = 0; i < count; ++i)
        {
            collection->push_back(rand() % 100);
        }
    }
};

// Test that a collection smart pointer is created successfully.
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // The fixture must create the collection before the test can continue.
    ASSERT_TRUE(collection);
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // A newly created vector should be empty and have a size of zero.
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running.
 * Uncomment this test to see a failure in the test explorer.
TEST_F(CollectionTest, AlwaysFail)
{
    FAIL();
}
*/

// Verify adding a single value to an empty collection.
TEST_F(CollectionTest, CanAddSingleValueToEmptyVector)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 1);
}

// Verify adding five values to the collection.
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    add_entries(5);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 5);
}

// Verify that max_size is greater than or equal to size for 0, 1, 5, and 10 entries.
TEST_F(CollectionTest, MaxSizeIsGreaterThanOrEqualToSize)
{
    const std::vector<int>::size_type entryCounts[] = { 0, 1, 5, 10 };

    for (auto count : entryCounts)
    {
        collection->clear();

        if (count > 0)
        {
            add_entries(static_cast<int>(count));
        }

        EXPECT_GE(collection->max_size(), collection->size())
            << "max_size should be at least size when vector contains " << count << " entries.";
    }
}

// Verify that capacity is greater than or equal to size for 0, 1, 5, and 10 entries.
TEST_F(CollectionTest, CapacityIsGreaterThanOrEqualToSize)
{
    const std::vector<int>::size_type entryCounts[] = { 0, 1, 5, 10 };

    for (auto count : entryCounts)
    {
        collection->clear();

        if (count > 0)
        {
            add_entries(static_cast<int>(count));
        }

        EXPECT_GE(collection->capacity(), collection->size())
            << "capacity should be at least size when vector contains " << count << " entries.";
    }
}

// Verify resizing increases the collection size.
TEST_F(CollectionTest, ResizeIncreasesCollectionSize)
{
    ASSERT_TRUE(collection->empty());

    collection->resize(10);

    EXPECT_EQ(collection->size(), 10);
    EXPECT_FALSE(collection->empty());
}

// Verify resizing decreases the collection size.
TEST_F(CollectionTest, ResizeDecreasesCollectionSize)
{
    add_entries(10);
    ASSERT_EQ(collection->size(), 10);

    collection->resize(5);

    EXPECT_EQ(collection->size(), 5);
    EXPECT_FALSE(collection->empty());
}

// Verify resizing decreases the collection to zero.
TEST_F(CollectionTest, ResizeDecreasesCollectionToZero)
{
    add_entries(10);
    ASSERT_EQ(collection->size(), 10);

    collection->resize(0);

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Verify clear erases the collection.
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(10);
    ASSERT_FALSE(collection->empty());

    collection->clear();

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Verify erase(begin, end) erases the full collection.
TEST_F(CollectionTest, EraseBeginEndErasesCollection)
{
    add_entries(10);
    ASSERT_EQ(collection->size(), 10);

    auto nextIterator = collection->erase(collection->begin(), collection->end());

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
    EXPECT_EQ(nextIterator, collection->end());
}

// Verify reserve increases capacity but does not change the collection size.
TEST_F(CollectionTest, ReserveIncreasesCapacityButDoesNotChangeSize)
{
    const auto originalSize = collection->size();
    const auto originalCapacity = collection->capacity();
    const std::vector<int>::size_type requestedCapacity = 100;

    collection->reserve(requestedCapacity);

    EXPECT_EQ(collection->size(), originalSize);
    EXPECT_GE(collection->capacity(), requestedCapacity);
    EXPECT_GE(collection->capacity(), originalCapacity);
}

// Negative test: verify std::out_of_range is thrown when at() uses an index out of bounds.
TEST_F(CollectionTest, AtThrowsOutOfRangeWhenIndexIsOutOfBounds)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5);

    EXPECT_THROW(collection->at(5), std::out_of_range);
}

// Student-defined positive test: verify inserted values maintain their order.
TEST_F(CollectionTest, MaintainsInsertionOrder)
{
    collection->push_back(10);
    collection->push_back(20);
    collection->push_back(30);

    ASSERT_EQ(collection->size(), 3);
    EXPECT_EQ(collection->at(0), 10);
    EXPECT_EQ(collection->at(1), 20);
    EXPECT_EQ(collection->at(2), 30);
}

// Student-defined negative test: verify reserve rejects a capacity larger than max_size.
TEST_F(CollectionTest, ReserveThrowsLengthErrorWhenCapacityExceedsMaxSize)
{
    const auto invalidCapacity = collection->max_size() + 1;

    EXPECT_THROW(collection->reserve(invalidCapacity), std::length_error);
}

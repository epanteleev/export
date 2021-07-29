#include <gtest/gtest.h>
#include <Env.h>

class Prepare : public ::testing::Test {
protected:
    Prepare() : env(Environment::open()) {}

    void SetUp() override {
        env.set("key", "value");
        env.set("key1", "value1");
        env.set("key2", "");
    }

    void TearDown() override {
        env.del("key");
        env.del("key1");
        env.del("key2");
    }

    Environment env;
};

TEST_F(Prepare, test1) {
    ASSERT_EQ(env.get("key"), "value;");
}

TEST_F(Prepare, test2) {
    ASSERT_EQ(env.get("key1"), "value1;");
}

TEST_F(Prepare, test3) {
    ASSERT_EQ(env.get("key2"), ";");
}

TEST_F(Prepare, test4) {
    ASSERT_THROW(env.get("key3"), VariableNotFound);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

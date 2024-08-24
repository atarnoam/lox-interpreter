#include <gtest/gtest.h>

#include "src/semantics/environment.h"

TEST(EnvironmentTest, Copy) {
    Environment global{};
    Environment son(&global);
    EXPECT_EQ(son.enclosing, &global);
    Environment son2 = son;
    EXPECT_EQ(son2.enclosing, &global);
    EXPECT_EQ(global.enclosing, nullptr);
}

TEST(EnvironmentTest, MoveConstructor) {
    Environment global{};
    Environment son(&global);
    EXPECT_EQ(son.enclosing, &global);
    Environment son2 = std::move(son);
    EXPECT_EQ(son2.enclosing, &global);
    EXPECT_EQ(global.enclosing, nullptr);
}

TEST(EnvironmentTest, MoveAssignment) {
    Environment global{};
    Environment son(&global);
    EXPECT_EQ(son.enclosing, &global);
    Environment son2(&global);
    son2 = std::move(son);
    EXPECT_EQ(son2.enclosing, &global);
    EXPECT_EQ(global.enclosing, nullptr);
}

TEST(EnvironmentTreeTest, AddEnvironmentNull) {
    EnvironmentTree env_tree{};
    Environment &globals = env_tree.globals();
    EXPECT_EQ(globals.enclosing, nullptr);
    Environment &new_env = *env_tree.add_environment(nullptr);
    EXPECT_EQ(globals.enclosing, nullptr);
    EXPECT_EQ(new_env.enclosing, nullptr);
}

TEST(EnvironmentTreeTest, AddEnvironmentGlobals) {
    EnvironmentTree env_tree{};
    Environment &globals = env_tree.globals();
    EXPECT_EQ(globals.enclosing, nullptr);
    Environment &new_env = *env_tree.add_environment(&globals);
    EXPECT_EQ(new_env.enclosing, &globals);
    EXPECT_EQ(globals.enclosing, nullptr);
}
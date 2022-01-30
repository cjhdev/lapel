#include "lapel_get_opt.h"
#include "gtest/gtest.h"

using namespace Lapel;

#define SIZE(X) (sizeof(X)/sizeof(*X))

using Option = GetOpt::Option;
using Result = GetOpt::Result;

static const Option opts1[] = {

    {.s="none",      .c='n', .arg=GetOpt::NONE},
    {.s="optional",  .c='o', .arg=GetOpt::OPTIONAL},
    {.s="required",  .c='r', .arg=GetOpt::REQUIRED},
};

TEST(GetOptTest, it_end_when_nothing)
{
    const char *argv[] = {};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_FALSE(inst.next());

    ASSERT_EQ(GetOpt::END, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());
    ASSERT_EQ(nullptr, inst.get_option());

    ASSERT_EQ(nullptr, inst.get_arg());
}

TEST(GetOptTest, it_ok_when_none_and_no_arg)
{
    const char *argv[] = {"--none"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());
}


TEST(GetOptTest, it_ok_when_short_none_and_no_arg)
{
    const char *argv[] = {"-n"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_optional_and_no_arg)
{
    const char *argv[] = {"--optional"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());

    // next will give end
    ASSERT_EQ(GetOpt::END, inst.get_result());
}


TEST(GetOptTest, it_ok_when_short_optional_and_no_arg)
{
    const char *argv[] = {"-o"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_optional_and_arg_as_next_token)
{
    const char *argv[] = {"--optional", "hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_short_optional_and_arg_as_next_token)
{
    const char *argv[] = {"-o", "hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_optional_and_arg_as_same_token)
{
    const char *argv[] = {"--optional=hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}


TEST(GetOptTest, it_ok_when_short_optional_and_arg_as_same_token)
{
    const char *argv[] = {"-ohello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_missing_when_required_and_no_arg)
{
    const char *argv[] = {"--required"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_FALSE(inst.next());

    ASSERT_EQ(GetOpt::MISSING, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());
    ASSERT_TRUE(inst.get_option() != nullptr);

    ASSERT_FALSE(inst.next());
}


TEST(GetOptTest, it_missing_when_short_required_and_no_arg)
{
    const char *argv[] = {"-r"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_FALSE(inst.next());

    ASSERT_EQ(GetOpt::MISSING, inst.get_result());
    ASSERT_EQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_required_and_arg_as_next_token)
{
    const char *argv[] = {"--required", "hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}


TEST(GetOptTest, it_ok_when_short_required_and_arg_as_next_token)
{
    const char *argv[] = {"-r", "hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_required_and_arg_as_same_token)
{
    const char *argv[] = {"--required=hello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_ok_when_short_required_and_arg_as_same_token)
{
    const char *argv[] = {"-rhello"};

    GetOpt inst(SIZE(argv), argv, opts1, SIZE(opts1));

    ASSERT_TRUE(inst.next());

    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ("hello", inst.get_arg());

    ASSERT_FALSE(inst.next());
}

TEST(GetOptTest, it_supports_chain_of_short_none)
{
    static const Option opts[] = {

        {.s=nullptr, .c='h', .arg=GetOpt::NONE},
        {.s=nullptr, .c='e', .arg=GetOpt::NONE},
        {.s=nullptr, .c='l', .arg=GetOpt::NONE},
        {.s=nullptr, .c='o', .arg=GetOpt::NONE},
    };

    const char *argv[] = {"-hello"};

    GetOpt inst(SIZE(argv), argv, opts, SIZE(opts));

    // -h
    ASSERT_TRUE(inst.next());
    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ(nullptr, inst.get_arg());

    // -e
    ASSERT_TRUE(inst.next());
    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ(nullptr, inst.get_arg());

    // -l
    ASSERT_TRUE(inst.next());
    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ(nullptr, inst.get_arg());

    // -l
    ASSERT_TRUE(inst.next());
    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ(nullptr, inst.get_arg());

    // -o
    ASSERT_TRUE(inst.next());
    ASSERT_EQ(GetOpt::OK, inst.get_result());
    ASSERT_STREQ(nullptr, inst.get_arg());

    ASSERT_FALSE(inst.next());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

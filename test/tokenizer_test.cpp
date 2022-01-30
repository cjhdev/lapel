#include "lapel_tokenizer.h"
#include "gtest/gtest.h"

TEST(TokenizerTest, one_token)
{
    char *argv[20];
    size_t argc;

    char input[] = "test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
}

TEST(TokenizerTest, many_token)
{
    char *argv[20];
    size_t argc;

    char input[] = "test test test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(3, argc);

    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
    ASSERT_TRUE(strcmp("test", argv[1]) == 0);
    ASSERT_TRUE(strcmp("test", argv[2]) == 0);
}

TEST(TokenizerTest, leading_whitespace)
{
    char *argv[20];
    size_t argc;

    char input[] = "  test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
}

TEST(TokenizerTest, trailing_whitespace)
{
    char *argv[20];
    size_t argc;

    char input[] = "test  ";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
}

TEST(TokenizerTest, separating_whitespace)
{
    char *argv[20];
    size_t argc;

    char input[] = "   test  test ";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(2, argc);

    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
    ASSERT_TRUE(strcmp("test", argv[1]) == 0);
}

TEST(TokenizerTest, single_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "'test  '";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test  ", argv[0]) == 0);
}

TEST(TokenizerTest, double_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "\"test  \"";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test  ", argv[0]) == 0);
}

TEST(TokenizerTest, open_single_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "'test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(0, argc);
}

TEST(TokenizerTest, open_double_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "\"test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(0, argc);
}

TEST(TokenizerTest, quote_within_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "'\"test  \"'";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("\"test  \"", argv[0]) == 0);
}

TEST(TokenizerTest, escaped_space)
{
    char *argv[20];
    size_t argc;

    char input[] = "test\\ test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test test", argv[0]) == 0);
}

TEST(TokenizerTest, escape_before_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "test\\\"test";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test\"test", argv[0]) == 0);
}

TEST(TokenizerTest, escape_within_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "'te\\\"st'";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("te\\\"st", argv[0]) == 0);
}

TEST(TokenizerTest, trailing_escape_1)
{
    char *argv[20];
    size_t argc;

    char input[] = "\\";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(0, argc);
}

TEST(TokenizerTest, trailing_escape_2)
{
    char *argv[20];
    size_t argc;

    char input[] = "test\\";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(0, argc);
}

TEST(TokenizerTest, trailing_escape_3)
{
    char *argv[20];
    size_t argc;

    char input[] = "test \\";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("test", argv[0]) == 0);
}

TEST(TokenizerTest, empty_quote)
{
    char *argv[20];
    size_t argc;

    char input[] = "''";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("", argv[0]) == 0);
}

TEST(TokenizerTest, quote_with_leading_text)
{
    char *argv[20];
    size_t argc;

    char input[] = "test'ing'";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("testing", argv[0]) == 0);
}

TEST(TokenizerTest, quote_with_trailing_text)
{
    char *argv[20];
    size_t argc;

    char input[] = "'test'ing";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("testing", argv[0]) == 0);
}

TEST(TokenizerTest, escaped_quote_pair)
{
    char *argv[20];
    size_t argc;

    char input[] = "\\\"test\\\"";

    argc = Lapel::Tokenizer::tokenize(input, argv, sizeof(argv)/sizeof(*argv));

    ASSERT_EQ(1, argc);
    ASSERT_TRUE(strcmp("\"test\"", argv[0]) == 0);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

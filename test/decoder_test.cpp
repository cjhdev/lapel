#include "lapel_decoder.h"
#include "gtest/gtest.h"

using namespace Lapel;

size_t decode(Decoder &decoder, const char *input, bool &decoded, DecoderSymbol &s)
{
    size_t retval = 0;

    for(retval=0,decoded=false; !decoded && (input[retval] != 0); retval++){

        decoded = decoder.process(input[retval], s);
    }

    return retval;
}

TEST(DecoderTest, it_decodes_home)
{
    Decoder decoder;
    DecoderSymbol s;
    bool decoded;
    const char input[] = "\e[1~";

    size_t parsed = decode(decoder, input, decoded, s);

    ASSERT_TRUE(decoded);
    ASSERT_EQ(strlen(input), parsed);
    ASSERT_EQ(DecoderSymbol::TYPE_HOME, s.type);
}

TEST(DecoderTest, it_decodes_end)
{
    Decoder decoder;
    DecoderSymbol s;
    bool decoded;
    const char input[] = "\e[1~";

    size_t parsed = decode(decoder, input, decoded, s);

    ASSERT_TRUE(decoded);
    ASSERT_EQ(strlen(input), parsed);
    ASSERT_EQ(DecoderSymbol::TYPE_HOME, s.type);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

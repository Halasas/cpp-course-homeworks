 #include<algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include "gtest/gtest.h"
#include "huffman/packer.h"
#include <sstream>

namespace {
	uint32_t BUF_SIZE = 4096;
	std::string encode_decode_str(std::string str) {
		std::ofstream test_writer("encode.txt", std::ios::binary);
		test_writer.write(&str[0], str.size());
		test_writer.close();

		packer::pack("encode.txt", "tmp");
		packer::unpack("tmp", "decode.txt");

		std::ifstream out_test("decode.txt", std::ios::binary);
		std::string check((std::istreambuf_iterator<char>(out_test)), std::istreambuf_iterator<char>());
		out_test.close();
		return check;
	}

	bool random_test(size_t size) {
		std::ofstream in_test("encode.txt", std::ios::binary);
		string in;
		in.resize(size);
		for (size_t i = 0; i < size; ++i) {
			in[i] = (char)rand();
		}
		in_test << in;
		in_test.close();

		packer::pack("encode.txt", "tmp");
		packer::unpack("tmp", "decode.txt");

		char *buf1 = new char[BUF_SIZE];
		char *buf2 = new char[BUF_SIZE];

		std::ifstream f1("encode.txt");
		std::ifstream f3("decode.txt");
		bool f = true;

		while (true) {
			f1.read(buf1, BUF_SIZE);
			int gc1 = (int)f1.gcount();
			f3.read(buf2, BUF_SIZE);
			int gc3 = (int)f3.gcount();
			if (gc1 != gc3)
				f = false;
			for (int i = 0; i < gc1; ++i)
				if (buf1[i] != buf2[i])
					f = false;
			if (f1.eof())
				break;
		}
		delete[] buf1;
		delete[] buf2;
		f1.close();
		f3.close();
		return f;
	}
	
	bool equal(std::string file1, std::string file2)
	{
		packer::pack(file1, "tmp");
		packer::unpack("tmp", file2);
		char *buf1 = new char[BUF_SIZE];
		char *buf2 = new char[BUF_SIZE];

		std::fstream f1(file1);
		std::fstream f3(file2);
		bool f = true;
		while (true) {
			f1.read(buf1, BUF_SIZE);
			int gc1 = (int)f1.gcount();
			f3.read(buf2, BUF_SIZE);
			int gc3 = (int)f3.gcount();
			if (gc1 != gc3)
				f = false;
			for (int i = 0; i < gc1; ++i)
				if (buf1[i] != buf2[i])
					f = false;
			if (f1.eof())
				break;
		}
		delete[] buf1;
		delete[] buf2;
		f1.close();
		f3.close();
		return f;
	}
}


TEST(correctness, hello_world)
{
    std::string str = "hello world";
    std::string check = encode_decode_str(str);
    EXPECT_EQ(str, check);
}

TEST(correctness, empty)
{
    std::string str = "";
    std::string check = encode_decode_str(str);
    EXPECT_EQ(str, check);
}

TEST(correctness, one_letter)
{
	std::string str = "a";
	std::string check = encode_decode_str(str);
	EXPECT_EQ(str, check);

	str = "aaaaaaa";
	check = encode_decode_str(str);
	EXPECT_EQ(str, check);
}

namespace {

}

TEST(correctness, big_random)
{
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_TRUE(random_test(10));
    }
}

TEST(correctness, buffer) {
	EXPECT_TRUE(random_test(BUF_SIZE * 8 + BUF_SIZE/3));
}

TEST(correctness, jpg1) {
	EXPECT_TRUE(equal("tree.JPG", "out2.jpg"));
}

TEST(correctness, Ready_Player_One) {
	EXPECT_TRUE(equal("Ready Player One.txt", "Ready Player One out.txt"));
}


TEST(correctness, 10m_dd) {
	EXPECT_TRUE(equal("10m_dd", "out"));
}

TEST(correctness, 10m_truncate) {
	EXPECT_TRUE(equal("10m_truncate", "out"));
}
/*
TEST(correctness, 300m_dd) {
	EXPECT_TRUE(equal("300m_dd", "out"));
}
*/

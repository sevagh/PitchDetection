#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>

TEST(MpmTest, Violin_A4_44100)
{
	auto data = vec_from_file("./sample/A4_44100_violin.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Piano_B4_44100)
{
	auto data = vec_from_file("./sample/B4_44100_piano.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Fiddle_BB4_44100)
{
	auto data = vec_from_file("./sample/BB4_44100_fiddle.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 466.2;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Piano_D4_44100)
{
	auto data = vec_from_file("./sample/D4_44100_piano.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Acoustic_E2_44100)
{
	auto data = vec_from_file("./sample/E2_44100_acousticguitar.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Classical_FSharp4_48000)
{
	auto data = vec_from_file("./sample/F-4_48000_classicalguitar.txt");
	double pitch = get_pitch_mpm(data, 48000);
	double expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmTest, Violin_G3_44100)
{
	auto data = vec_from_file("./sample/G3_44100_violin.txt");
	double pitch = get_pitch_mpm(data, 44100);
	double expected = 196.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

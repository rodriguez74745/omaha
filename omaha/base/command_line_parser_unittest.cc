// Copyright 2008-2009 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================

#include "omaha/base/command_line_parser.h"
#include "omaha/testing/unit_test.h"

namespace omaha {

// This will succeed since the CommandLineToArgvW function returns the
// path to the current executable file if it's passed the empty string.
TEST(CommandLineParserTest, ParseFromString_NullString) {
  CommandLineParser parser;
  EXPECT_SUCCEEDED(parser.ParseFromString(NULL));
  EXPECT_EQ(0, parser.GetRequiredSwitchCount());
}

// This will succeed since the CommandLineToArgvW function returns the
// path to the current executable file if it's passed the empty string.
TEST(CommandLineParserTest, ParseFromString_EmptyString) {
  CommandLineParser parser;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("")));
  EXPECT_EQ(0, parser.GetRequiredSwitchCount());
}

// This will succeed since the CommandLineToArgvW function returns the
// path to the current executable file if it's passed the empty string.
TEST(CommandLineParserTest, ParseFromString_SpacesOnlyString) {
  CommandLineParser parser;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("    ")));
}

TEST(CommandLineParserTest, ParseFromArgv_NullArgv) {
  CommandLineParser parser;
  EXPECT_FAILED(parser.ParseFromArgv(0, NULL));
}

TEST(CommandLineParserTest, CallFunctionsBeforeParse) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_FALSE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_EQ(0, parser.GetRequiredSwitchCount());
  EXPECT_FAILED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_FAILED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 0, &arg_value));
}

TEST(CommandLineParserTest, ParseFromString_ProgramNameOnly) {
  CommandLineParser parser;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe")));
  EXPECT_EQ(0, parser.GetRequiredSwitchCount());
}

TEST(CommandLineParserTest, ValidateSwitchMixedCase) {
  CommandLineParser parser;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /FooP")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foop")));
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("FooP")));
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("fOOp")));
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("FOOP")));
  EXPECT_FALSE(parser.HasRequiredSwitch(_T("blAH")));
}

TEST(CommandLineParserTest, ParseFromString_OneSwitchNoArgs) {
  CommandLineParser parser;
  size_t arg_count = 0;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /foo")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(0, arg_count);
}

TEST(CommandLineParserTest, ParseFromString_OneSwitchOneArg) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /foo bar")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(1, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 0, &arg_value));
  EXPECT_STREQ(_T("bar"), arg_value);
}

TEST(CommandLineParserTest, ParseFromString_OneSwitchTwoArgs) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /foo bar baz")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(2, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 0, &arg_value));
  EXPECT_STREQ(_T("bar"), arg_value);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 1, &arg_value));
  EXPECT_STREQ(_T("baz"), arg_value);
}

TEST(CommandLineParserTest, ParseFromString_TwoSwitchesNoArgs) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /foo /bar")));
  EXPECT_EQ(2, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("bar")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(0, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("bar"), &arg_count));
  EXPECT_EQ(0, arg_count);
}

TEST(CommandLineParserTest, ParseFromString_TwoSwitchesOneArgNoArg) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("myprog.exe /foo blech /bar")));
  EXPECT_EQ(2, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("bar")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(1, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 0, &arg_value));
  EXPECT_STREQ(_T("blech"), arg_value);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("bar"), &arg_count));
  EXPECT_EQ(0, arg_count);
}

TEST(CommandLineParserTest, ParseFromString_ArgInQuotesWithLeadingSlash) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(parser.ParseFromString(_T("f.exe /pi \"arg\" \"/sw x\"")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("pi")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("pi"), &arg_count));
  EXPECT_EQ(2, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("pi"), 0, &arg_value));
  EXPECT_STREQ(_T("arg"), arg_value);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("pi"), 1, &arg_value));
  EXPECT_STREQ(_T("/sw x"), arg_value);
}

// Paths with spaces and no enclosing quotes is not supported. Thus, there is
// no test for it.
TEST(CommandLineParserTest, ParseFromString_SpaceInPathWithQuotes) {
  CommandLineParser parser;
  size_t arg_count = 0;
  CString arg_value;
  EXPECT_SUCCEEDED(
      parser.ParseFromString(_T("\"C:\\Space In Path\\myprog.exe\" /foo bar")));
  EXPECT_EQ(1, parser.GetRequiredSwitchCount());
  EXPECT_TRUE(parser.HasRequiredSwitch(_T("foo")));
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentCount(_T("foo"), &arg_count));
  EXPECT_EQ(1, arg_count);
  EXPECT_SUCCEEDED(parser.GetRequiredSwitchArgumentValue(_T("foo"), 0, &arg_value));
  EXPECT_STREQ(_T("bar"), arg_value);
}

}  // namespace omaha

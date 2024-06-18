// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers)

#include <catch2/catch_test_macros.hpp>

#include <Vandior/vandior.hpp>

static inline constexpr std::size_t t_line = 5;
static inline constexpr std::size_t t_line2 = 42;
static inline constexpr std::size_t t_line3 = 100;
static inline constexpr std::size_t t_line4 = 10;
static inline constexpr std::size_t t_colum = 6;
static inline constexpr std::size_t t_colum2 = 5;
static inline constexpr std::size_t t_colum3 = 7;
static inline constexpr std::size_t t_colum4 = 8;
static inline constexpr std::size_t t_colum5 = 9;
static inline constexpr std::size_t t_colum6 = t_line4;
static inline constexpr std::size_t t_colum8 = 11;
static inline constexpr std::size_t t_colum10 = 15;
static inline constexpr std::size_t t_colum11 = 13;
static inline constexpr std::size_t t_colum13 = 17;
static inline constexpr std::size_t t_colum14 = 12;
static inline constexpr auto identf = vnd::TokenType::IDENTIFIER;
static inline constexpr auto inte = vnd::TokenType::INTEGER;
static inline constexpr auto doub = vnd::TokenType::DOUBLE;
#ifdef _WIN32  // Windows
static inline constexpr std::string_view filename = R"(.\unknown.vn)";
#else
static inline constexpr std::string_view filename = R"(./unknown.vn)";
#endif
static inline constexpr std::string_view filename2 = "example.cpp";
static inline constexpr std::string_view filename3 = "new_file.cpp";
static inline constexpr std::string_view filename4 = "unknown";
static inline constexpr std::string_view timerName = "My Timer";
static inline constexpr std::string_view timerBigs = "-----------";
static inline constexpr std::string_view timerTime1 = "ms";
static inline constexpr std::string_view timerTime2 = "ns";

static inline constexpr long long int timerSleap = 12;
static inline constexpr long long int timerSleap2 = 5;
static inline constexpr std::size_t timerCicles = 1000000;
static inline constexpr long double timerResolution = 5.0L;
#define REQ_FORMAT(type, string) REQUIRE(FORMAT("{}", type) == (string));  // NOLINT(*-macro-usage)

TEST_CASE("std::filesystem::path formater", "[FMT]") { REQ_FORMAT(std::filesystem::path("../ssss"), "../ssss"); }
TEST_CASE("glm::vec formater", "[FMT]") {
    REQ_FORMAT(glm::vec2(0.0F, 0.0F), "vec2(0, 0)");
    REQ_FORMAT(glm::dvec2(0.0, 0.0), "dvec2(0, 0)");
    REQ_FORMAT(glm::ldvec2(0.0, 0.0), "ldvec2(0, 0)");
    REQ_FORMAT(glm::vec3(0.0F, 0.0F, 0.0F), "vec3(0, 0, 0)");
    REQ_FORMAT(glm::dvec3(0.0, 0.0, 0.0), "dvec3(0, 0, 0)");
    REQ_FORMAT(glm::ldvec3(0.0, 0.0, 0.0F), "ldvec3(0, 0, 0)");
    REQ_FORMAT(glm::vec4(0.0F, 0.0, 0.0, 0.0), "vec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::dvec4(0.0, 0.0, 0.0, 0.0), "dvec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::ldvec4(0.0, 0.0, 0.0F, 0.0), "ldvec4(0, 0, 0, 0)");
}

// clang-format off
TEST_CASE("glm::mat formater", "[FMT]") {
    REQ_FORMAT(glm::mat2(0.0F, 0.0F, 0.0F, 0.0F), "mat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::dmat2(0.0, 0.0, 0.0, 0.0), "dmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::ldmat2(0.0, 0.0, 0.0, 0.0), "ldmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::mat3(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::dmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "dmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::ldmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::mat4(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQ_FORMAT(glm::dmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "dmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQ_FORMAT(glm::ldmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
}


TEST_CASE("std::complex formater", "[FMT]") {
    std::complex<float> complexFloat(1.0F, 2.0F);
    std::complex<double> complexDouble(3.0, 4.0);
    std::complex<long double> complexLongDouble(5.0L, 6.0L);

    REQ_FORMAT(complexFloat, "(1, 2)");
    REQ_FORMAT(complexDouble, "(3, 4)");
    REQ_FORMAT(complexLongDouble, "(5, 6)");
}

TEST_CASE("std::complex formater 2", "[FMT]") {
    std::complex<float> complexFloat(1.1F, 2.1F);
    std::complex<double> complexDouble(3.1, 4.1);
    std::complex<long double> complexLongDouble(5.1L, 6.1L);

    REQ_FORMAT(complexFloat, "(1.1, 2.1)");
    REQ_FORMAT(complexDouble, "(3.1, 4.1)");
    REQ_FORMAT(complexLongDouble, "(5.1, 6.1)");
}
// clang-format on

TEST_CASE("Timer: MSTimes", "[timer]") {
    vnd::Timer timer{timerName.data()};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    std::string output = timer.to_string();
    std::string new_output = (timer / timerCicles).to_string();
    REQUIRE(output.find(timerName.data()) != std::string::npos);
    REQUIRE(output.find(timerTime1) != std::string::npos);
    REQUIRE(new_output.find(timerTime2) != std::string::npos);
}

TEST_CASE("Timer: MSTimes FMT", "[timer]") {
    vnd::Timer timer{timerName.data()};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    std::string output = FORMAT("{}", timer);
    std::string new_output = FORMAT("{}", (timer / timerCicles));
    REQUIRE(output.find(timerName.data()) != std::string::npos);
    REQUIRE(output.find(timerTime1) != std::string::npos);
    REQUIRE(new_output.find(timerTime2) != std::string::npos);
}

TEST_CASE("Timer: BigTimer", "[timer]") {
    vnd::Timer timer{timerName.data(), vnd::Timer::Big};
    std::string output = timer.to_string();
    REQUIRE(output.find(timerName.data()) != std::string::npos);
    REQUIRE(output.find(timerBigs) != std::string::npos);
}

TEST_CASE("Timer: BigTimer FMT", "[timer]") {
    vnd::Timer timer{timerName.data(), vnd::Timer::Big};
    std::string output = FORMAT("{}", timer);
    REQUIRE(output.find(timerName.data()) != std::string::npos);
    REQUIRE(output.find(timerBigs) != std::string::npos);
}

TEST_CASE("Timer: AutoTimer", "[timer]") {
    vnd::Timer timer;
    std::string output = timer.to_string();
    REQUIRE(output.find("Timer") != std::string::npos);
}

TEST_CASE("Timer: PrintTimer", "[timer]") {
    std::stringstream out;
    vnd::Timer timer;
    out << timer;
    std::string output = out.str();
    REQUIRE(output.find("Timer") != std::string::npos);
}

TEST_CASE("Timer: PrintTimer FMT", "[timer]") {
    vnd::Timer timer;
    std::string output = FORMAT("{}", timer);
    REQUIRE(output.find("Timer") != std::string::npos);
}

TEST_CASE("Timer: TimeItTimer", "[timer]") {
    vnd::Timer timer;
    std::string output = timer.time_it([]() { std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap2)); }, timerResolution);
    REQUIRE(output.find(timerTime1) != std::string::npos);
}

TEST_CASE("CodeSourceLocation default constructor sets default values", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location;
    REQUIRE(location.getFileName().empty());
    REQUIRE(location.getLine() == 0);
    REQUIRE(location.getColumn() == 0);
}

TEST_CASE("CodeSourceLocation constructor sets values correctly", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location(filename2, t_line2, t_colum6);
    REQUIRE(location.getFileName() == filename2);
    REQUIRE(location.getLine() == t_line2);
    REQUIRE(location.getColumn() == t_colum6);
}

TEST_CASE("CodeSourceLocation setters update values", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location;
    location.setFileName(filename3);
    location.setLine(t_line3);
    location.setColumn(t_colum6 * 2);
    REQUIRE(location.getFileName() == filename3);
    REQUIRE(location.getLine() == t_line3);
    REQUIRE(location.getColumn() == t_colum6 * 2);
}

TEST_CASE("CodeSourceLocation unknown() creates object with default values", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location = vnd::CodeSourceLocation::unknown();
    REQUIRE(location.getFileName() == filename4);
    REQUIRE(location.getLine() == 0);
    REQUIRE(location.getColumn() == 0);
}

TEST_CASE("CodeSourceLocation equality and inequality operators work correctly", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location1("file1.cpp", t_line4, t_colum2);
    vnd::CodeSourceLocation location2("file1.cpp", t_line4, t_colum2);
    vnd::CodeSourceLocation location3("file2.cpp", t_line4, t_colum2);

    REQUIRE(location1 == location2);
    REQUIRE(location1 != location3);
}

TEST_CASE("CodeSourceLocation toString() produces expected string", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location(filename2, t_line2, t_colum6);
    REQUIRE(location.toString() == "(file:example.cpp, line:42, column:10)");
}

TEST_CASE("corrected format for Tokentype", "[token_type]") {
    using enum vnd::TokenType;
    REQ_FORMAT(INTEGER, "INTEGER")
    REQ_FORMAT(DOUBLE, "DOUBLE")
    REQ_FORMAT(BOOLEAN, "BOOLEAN")
    REQ_FORMAT(PLUS, "PLUS_OPERATOR")
    REQ_FORMAT(MINUS, "MINUS_OPERATOR")
    REQ_FORMAT(EQUAL, "EQUAL_OPERATOR")
    REQ_FORMAT(DOT, "DOT_OPERATOR")
    REQ_FORMAT(STAR, "STAR_OPERATOR")
    REQ_FORMAT(DIVIDE, "DIVIDE_OPERATOR")
    REQ_FORMAT(XOR, "XOR_OPERATOR")
    REQ_FORMAT(PERCENT, "PERCENT_OPERATOR")
    REQ_FORMAT(OR, "OR_OPERATOR")
    REQ_FORMAT(AND, "AND_OPERATOR")
    REQ_FORMAT(LESS, "LESS_OPERATOR")
    REQ_FORMAT(GREATER, "GREATER_OPERATOR")
    REQ_FORMAT(PLUSPLUS, "PLUSPLUS_OPERATOR")
    REQ_FORMAT(MINUSMINUS, "MINUSMINUS_OPERATOR")
    REQ_FORMAT(PLUSEQUAL, "PLUSEQUAL_OPERATOR")
    REQ_FORMAT(MINUSEQUAL, "MINUSEQUAL_OPERATOR")
    REQ_FORMAT(NOTEQUAL, "NOTEQUAL_OPERATOR")
    REQ_FORMAT(STAREQUAL, "STAREQUAL_OPERATOR")
    REQ_FORMAT(DIVIDEEQUAL, "DIVIDEEQUAL_OPERATOR")
    REQ_FORMAT(XOREQUAL, "XOREQUAL_OPERATOR")
    REQ_FORMAT(PERCENTEQUAL, "PERCENTEQUAL_OPERATOR")
    REQ_FORMAT(OROR, "OROR_OPERATOR")
    REQ_FORMAT(ANDAND, "ANDAND_OPERATOR")
    REQ_FORMAT(EQUALEQUAL, "EQUALEQUAL_OPERATOR")
    REQ_FORMAT(LESSEQUAL, "LESSEQUAL_OPERATOR")
    REQ_FORMAT(GREATEREQUAL, "GREATEREQUAL_OPERATOR")
    REQ_FORMAT(IDENTIFIER, "IDENTIFIER")
    REQ_FORMAT(CHAR, "CHAR")
    REQ_FORMAT(STRING, "STRING")
    REQ_FORMAT(EOFT, "EOF")
    REQ_FORMAT(K_MAIN, "K_MAIN")
    REQ_FORMAT(K_VAR, "K_VAR")
    REQ_FORMAT(K_IF, "K_IF")
    REQ_FORMAT(K_WHILE, "K_WHILE")
    REQ_FORMAT(K_ELSE, "K_ELSE")
    REQ_FORMAT(K_FOR, "K_FOR")
    REQ_FORMAT(K_BREAK, "BREAK")
    REQ_FORMAT(K_FUN, "K_FUN")
    REQ_FORMAT(K_RETURN, "K_RETURN")
    REQ_FORMAT(K_NULLPTR, "K_NULLPTR")
    REQ_FORMAT(OPEN_PARENTESIS, "OPEN_PARENTESIS")
    REQ_FORMAT(OPEN_SQ_PARENTESIS, "OPEN_SQ_PARENTESIS")
    REQ_FORMAT(OPEN_CUR_PARENTESIS, "OPEN_CUR_PARENTESIS")
    REQ_FORMAT(CLOSE_PARENTESIS, "CLOSE_PARENTESIS")
    REQ_FORMAT(CLOSE_SQ_PARENTESIS, "CLOSE_SQ_PARENTESIS")
    REQ_FORMAT(CLOSE_CUR_PARENTESIS, "CLOSE_CUR_PARENTESIS")
    REQ_FORMAT(NOT, "NOT_OPERATOR")
    REQ_FORMAT(COMMA, "COMMA")
    REQ_FORMAT(COLON, "COLON")
    REQ_FORMAT(COMMENT, "COMMENT")
    REQ_FORMAT(UNKNOWN, "UNKNOWN")
}

TEST_CASE("corrected format for Tokentype compat to string", "[token_type]") {
    using enum vnd::TokenType;
    REQ_FORMAT(comp_tokType(INTEGER), "INT")
    REQ_FORMAT(comp_tokType(DOUBLE), "DBL")
    REQ_FORMAT(comp_tokType(BOOLEAN), "BOOL")
    REQ_FORMAT(comp_tokType(PLUS), "PLUS_OP")
    REQ_FORMAT(comp_tokType(MINUS), "MINUS_OP")
    REQ_FORMAT(comp_tokType(EQUAL), "EQUAL_OP")
    REQ_FORMAT(comp_tokType(DOT), "DOT_OP")
    REQ_FORMAT(comp_tokType(STAR), "STAR_OP")
    REQ_FORMAT(comp_tokType(DIVIDE), "DIVIDE_OP")
    REQ_FORMAT(comp_tokType(XOR), "XOR_OP")
    REQ_FORMAT(comp_tokType(PERCENT), "PERCENT_OP")
    REQ_FORMAT(comp_tokType(OR), "OR_OP")
    REQ_FORMAT(comp_tokType(AND), "AND_OP")
    REQ_FORMAT(comp_tokType(LESS), "LESS_OP")
    REQ_FORMAT(comp_tokType(GREATER), "GREATER_OP")
    REQ_FORMAT(comp_tokType(PLUSPLUS), "PLUSPLUS_OP")
    REQ_FORMAT(comp_tokType(MINUSMINUS), "MINUSMINUS_OP")
    REQ_FORMAT(comp_tokType(PLUSEQUAL), "PLUSEQUAL_OP")
    REQ_FORMAT(comp_tokType(MINUSEQUAL), "MINUSEQUAL_OP")
    REQ_FORMAT(comp_tokType(NOTEQUAL), "NOTEQUAL_OP")
    REQ_FORMAT(comp_tokType(STAREQUAL), "STAREQUAL_OP")
    REQ_FORMAT(comp_tokType(DIVIDEEQUAL), "DIVIDEEQUAL_OP")
    REQ_FORMAT(comp_tokType(XOREQUAL), "XOREQUAL_OP")
    REQ_FORMAT(comp_tokType(PERCENTEQUAL), "PERCENTEQUAL_OP")
    REQ_FORMAT(comp_tokType(OROR), "OROR_OP")
    REQ_FORMAT(comp_tokType(ANDAND), "ANDAND_OP")
    REQ_FORMAT(comp_tokType(EQUALEQUAL), "EQUALEQUAL_OP")
    REQ_FORMAT(comp_tokType(LESSEQUAL), "LESSEQUAL_OP")
    REQ_FORMAT(comp_tokType(GREATEREQUAL), "GREATEREQUAL_OP")
    REQ_FORMAT(comp_tokType(IDENTIFIER), "IDENT")
    REQ_FORMAT(comp_tokType(CHAR), "CH")
    REQ_FORMAT(comp_tokType(STRING), "STR")
    REQ_FORMAT(comp_tokType(EOFT), "EOF")
    REQ_FORMAT(comp_tokType(K_MAIN), "K_MAIN")
    REQ_FORMAT(comp_tokType(K_VAR), "K_VAR")
    REQ_FORMAT(comp_tokType(K_IF), "K_IF")
    REQ_FORMAT(comp_tokType(K_WHILE), "K_WHILE")
    REQ_FORMAT(comp_tokType(K_ELSE), "K_ELSE")
    REQ_FORMAT(comp_tokType(K_FOR), "K_FOR")
    REQ_FORMAT(comp_tokType(K_BREAK), "BREAK")
    REQ_FORMAT(comp_tokType(K_FUN), "K_FUN")
    REQ_FORMAT(comp_tokType(K_RETURN), "K_RETURN")
    REQ_FORMAT(comp_tokType(K_NULLPTR), "K_NULLPTR")
    REQ_FORMAT(comp_tokType(OPEN_PARENTESIS), "OPEN_PAR")
    REQ_FORMAT(comp_tokType(OPEN_SQ_PARENTESIS), "OPEN_SQ_PAR")
    REQ_FORMAT(comp_tokType(OPEN_CUR_PARENTESIS), "OPEN_CUR_PAR")
    REQ_FORMAT(comp_tokType(CLOSE_PARENTESIS), "CLOSE_PAR")
    REQ_FORMAT(comp_tokType(CLOSE_SQ_PARENTESIS), "CLOSE_SQ_PAR")
    REQ_FORMAT(comp_tokType(CLOSE_CUR_PARENTESIS), "CLOSE_CUR_PAR")
    REQ_FORMAT(comp_tokType(NOT), "NOT_OP")
    REQ_FORMAT(comp_tokType(COMMA), "COMMA")
    REQ_FORMAT(comp_tokType(COLON), "COLON")
    REQ_FORMAT(comp_tokType(COMMENT), "COMMENT")
    REQ_FORMAT(comp_tokType(UNKNOWN), "UNKNOWN")
}

namespace {
    void modifyToken(vnd::Token &token) {
        token.setType(vnd::TokenType::INTEGER);
        token.setValue("assss");
        token.setFileName(filename);
        token.setLine(1);
        token.setColumn(1);
    }
}  // namespace

TEST_CASE("default constructed token", "[token]") {
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
}

TEST_CASE("default constructed token toString", "[token]") {
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
}

TEST_CASE("default constructed token format", "[token]") {
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQ_FORMAT(token, "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    REQUIRE(token.compat_to_string() == "(typ: UNKNOWN, val: '', sl:(f:unknown, l:0, c:0))");
}

TEST_CASE("default constructed token set propriety", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}

TEST_CASE("default constructed token isType", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.isType(CHAR) == false);
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.isType(INTEGER) == true);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}
TEST_CASE("default constructed token set propriety tostring", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
#ifdef _WIN32  // Windows
    REQUIRE(token.to_string() == R"((type: INTEGER, value: 'assss', source location:(file:.\unknown.vn, line:1, column:1)))");
    REQUIRE(token.compat_to_string() == R"((typ: INT, val: 'assss', sl:(f:.\unknown.vn, l:1, c:1)))");
#else
    REQUIRE(token.to_string() == R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
    REQUIRE(token.compat_to_string() == R"((typ: INT, val: 'assss', sl:(f:./unknown.vn, l:1, c:1)))");
#endif
}

TEST_CASE("construct a empty value token", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{UNKNOWN, vnd::CodeSourceLocation{}};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.getFileName().empty() == true);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:, line:0, column:0))");
    REQUIRE(token.compat_to_string() == "(typ: UNKNOWN, val: '', sl:(f:, l:0, c:0))");
}

TEST_CASE("FolderCreationResult Constructor") {
    SECTION("Default constructor") {
        vnd::FolderCreationResult result;
        REQUIRE(result.success() == false);
        REQUIRE(result.path().empty());
    }

    SECTION("Parameterized constructor") {
        vnd::FolderCreationResult result(true, fs::path("/test/path"));
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == fs::path("/test/path"));
    }
}

TEST_CASE("FolderCreationResult Setters") {
    vnd::FolderCreationResult result;

    SECTION("Set success") {
        result.set_success(true);
        REQUIRE(result.success() == true);
    }

    SECTION("Set path") {
        fs::path testPath("/test/path");
        REQUIRE(result.path().empty());
        result.set_path(testPath);
        REQUIRE(result.path() == testPath);
    }

    SECTION("Set path with empty string") { REQUIRE_THROWS_AS(result.set_path(fs::path()), std::invalid_argument); }
}

TEST_CASE("FolderCreationResult Folder Creation Functions") {
    // Create a temporary directory for testing
    auto tempDir = fs::temp_directory_path() / "vnd_test";
    fs::create_directories(tempDir);

    SECTION("Create folder with valid parameters") {
        std::string folderName = "test_folder";
        vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(folderName, tempDir);
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == tempDir / folderName);
        // Clean up
        fs::remove_all(tempDir / folderName);
    }

    SECTION("Create folder with empty folder name") {
        std::string emptyFolderName;
        vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(emptyFolderName, tempDir);
        REQUIRE(result.success() == false);
        REQUIRE(result.path().empty());
    }

    SECTION("Create folder in non-existent parent directory") {
        fs::path nonExistentParentDir = tempDir / "non_existent_dir";
        std::string folderName = "test_folder";
        vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(folderName, nonExistentParentDir);
        REQUIRE(result.success() == true);
        REQUIRE(!result.path().empty());
    }

    SECTION("Create folder next to non-existent file") {
        fs::path nonExistentFilePath = tempDir / "non_existent_file.txt";
        std::string folderName = "test_folder";
        vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolderNextToFile(nonExistentFilePath, folderName);
        REQUIRE(result.success() == true);
        REQUIRE(!result.path().empty());
    }

    SECTION("Create folder next to existing file") {
        // Create a file in the temporary directory
        fs::path filePath = tempDir / "test_file.txt";
        std::ofstream ofs(filePath);
        ofs.close();

        std::string folderName = "test_folder";
        vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolderNextToFile(filePath, folderName);
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == tempDir / folderName);

        // Clean up
        fs::remove(filePath);
        fs::remove_all(tempDir / folderName);
    }

    // Clean up the temporary directory
    fs::remove_all(tempDir);
}

TEST_CASE("FolderCreationResult Serialization") {
    vnd::FolderCreationResult result(true, fs::path("/test/path"));
    nlohmann::json jsn = result;
    REQUIRE(jsn["success"] == true);
    REQUIRE(jsn["path"] == "/test/path");

    vnd::FolderCreationResult deserializedResult;
    jsn.get_to(deserializedResult);
    REQUIRE(deserializedResult.success() == true);
    REQUIRE(deserializedResult.path() == fs::path("/test/path"));
}

TEST_CASE("default constructed token set propriety format", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(FORMAT("{}", token) == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
#ifdef _WIN32  // Windows
    REQ_FORMAT(token, R"((type: INTEGER, value: 'assss', source location:(file:.\unknown.vn, line:1, column:1)))");
#else
    REQ_FORMAT(token, R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
#endif
}

TEST_CASE("Token Comparison Equality", "[Token]") {
    vnd::Token token1(vnd::TokenType::PLUS, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
    vnd::Token token2(vnd::TokenType::PLUS, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
    REQUIRE(token1 == token2);
}

TEST_CASE("Token Comparison Inequality", "[Token]") {
    vnd::Token token1(identf, "variable", vnd::CodeSourceLocation(filename, t_line, t_colum));
    vnd::Token token2(identf, "variable2", vnd::CodeSourceLocation(filename, t_line, t_colum));
    REQUIRE(token1 != token2);
}

TEST_CASE("tokenizer emit identifier token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"a a_ a0 a000_ _a", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(identf, "a0", vnd::CodeSourceLocation(filename, 1, 6)));
    REQUIRE(tokens[3] == vnd::Token(identf, "a000_", vnd::CodeSourceLocation(filename, 1, t_colum5)));
    REQUIRE(tokens[4] == vnd::Token(identf, "_a", vnd::CodeSourceLocation(filename, 1, t_colum10)));
}

TEST_CASE("tokenizer emit identifier token new line", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"a a_\na0 a000_ _a", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(identf, "a0", vnd::CodeSourceLocation(filename, 2, 1)));
    REQUIRE(tokens[3] == vnd::Token(identf, "a000_", vnd::CodeSourceLocation(filename, 2, 4)));
    REQUIRE(tokens[4] == vnd::Token(identf, "_a", vnd::CodeSourceLocation(filename, 2, 10)));
}

TEST_CASE("tokenizer emit integer token for hexadecimals numbers", "[tokenizer]") {
    // hexadecimals 0xhexnum a-f A-F 0-9
    vnd::Tokenizer tokenizer{"#0 #23 #24 #ff #7f", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(inte, "#0", vnd::CodeSourceLocation(filename, 1, 0)));
    REQUIRE(tokens[1] == vnd::Token(inte, "#23", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(inte, "#24", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(inte, "#ff", vnd::CodeSourceLocation(filename, 1, t_colum8)));
    REQUIRE(tokens[4] == vnd::Token(inte, "#7f", vnd::CodeSourceLocation(filename, 1, t_colum10)));
}

TEST_CASE("tokenizer emit exception on  malformed exadecimal number or octal number", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"#", filename};
    REQUIRE_THROWS_AS(tokenizer.tokenize(), std::runtime_error);
}

TEST_CASE("tokenizer emit integer token for octal numbers", "[tokenizer]") {
    // octal 0oOctnum 0-7
    vnd::Tokenizer tokenizer{"#o0 #o23 #o24", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(inte, "#o0", vnd::CodeSourceLocation(filename, 1, 0)));
    REQUIRE(tokens[1] == vnd::Token(inte, "#o23", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(inte, "#o24", vnd::CodeSourceLocation(filename, 1, t_colum5)));
}

TEST_CASE("tokenizer emit integer token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"42 333 550 34000000", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(inte, "550", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[3] == vnd::Token(inte, "34000000", vnd::CodeSourceLocation(filename, 1, t_colum14)));
}

TEST_CASE("tokenizer emit integer token new line", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"42 333\n550 34000000", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(inte, "550", vnd::CodeSourceLocation(filename, 2, 1)));
    REQUIRE(tokens[3] == vnd::Token(inte, "34000000", vnd::CodeSourceLocation(filename, 2, 5)));
}

TEST_CASE("tokenizer emit double token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"1. 1.0 1e+1 1E+1 1.1e+1 1.1E+1 1e-1 1E-1 1.1e-1 1.1E-1 .4e12 4i 5.4if .7f", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 15);
    REQUIRE(tokens[0] == vnd::Token(doub, "1.", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(doub, "1.0", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(doub, "1e+1", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[3] == vnd::Token(doub, "1E+1", vnd::CodeSourceLocation(filename, 1, t_colum11)));
    REQUIRE(tokens[4] == vnd::Token(doub, "1.1e+1", vnd::CodeSourceLocation(filename, 1, 18)));
    REQUIRE(tokens[5] == vnd::Token(doub, "1.1E+1", vnd::CodeSourceLocation(filename, 1, 25)));
    REQUIRE(tokens[6] == vnd::Token(doub, "1e-1", vnd::CodeSourceLocation(filename, 1, 32)));
    REQUIRE(tokens[7] == vnd::Token(doub, "1E-1", vnd::CodeSourceLocation(filename, 1, 37)));
    REQUIRE(tokens[8] == vnd::Token(doub, "1.1e-1", vnd::CodeSourceLocation(filename, 1, 42)));
    REQUIRE(tokens[9] == vnd::Token(doub, "1.1E-1", vnd::CodeSourceLocation(filename, 1, 49)));
    REQUIRE(tokens[10] == vnd::Token(doub, ".4e12", vnd::CodeSourceLocation(filename, 1, 56)));
    REQUIRE(tokens[11] == vnd::Token(doub, "4i", vnd::CodeSourceLocation(filename, 1, 62)));
    REQUIRE(tokens[12] == vnd::Token(doub, "5.4if", vnd::CodeSourceLocation(filename, 1, 65)));
    REQUIRE(tokens[13] == vnd::Token(doub, ".7f", vnd::CodeSourceLocation(filename, 1, 71)));
}

TEST_CASE("tokenizer emit operator token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"* / = , : < > ! | & + - ^ . %", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 16);
    REQUIRE(tokens[0] == vnd::Token(STAR, "*", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(DIVIDE, "/", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(EQUAL, "=", vnd::CodeSourceLocation(filename, 1, 5)));
    REQUIRE(tokens[3] == vnd::Token(COMMA, ",", vnd::CodeSourceLocation(filename, 1, t_colum)));
    REQUIRE(tokens[4] == vnd::Token(COLON, ":", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[5] == vnd::Token(LESS, "<", vnd::CodeSourceLocation(filename, 1, t_colum8)));
    REQUIRE(tokens[6] == vnd::Token(GREATER, ">", vnd::CodeSourceLocation(filename, 1, t_colum11)));
    REQUIRE(tokens[7] == vnd::Token(NOT, "!", vnd::CodeSourceLocation(filename, 1, t_colum10)));
    REQUIRE(tokens[8] == vnd::Token(OR, "|", vnd::CodeSourceLocation(filename, 1, t_colum13)));
    REQUIRE(tokens[9] == vnd::Token(AND, "&", vnd::CodeSourceLocation(filename, 1, 19)));
    REQUIRE(tokens[10] == vnd::Token(PLUS, "+", vnd::CodeSourceLocation(filename, 1, 21)));
    REQUIRE(tokens[11] == vnd::Token(MINUS, "-", vnd::CodeSourceLocation(filename, 1, 23)));
    REQUIRE(tokens[12] == vnd::Token(XOR, "^", vnd::CodeSourceLocation(filename, 1, 25)));
    REQUIRE(tokens[13] == vnd::Token(DOT, ".", vnd::CodeSourceLocation(filename, 1, 27)));
    REQUIRE(tokens[14] == vnd::Token(PERCENT, "%", vnd::CodeSourceLocation(filename, 1, 29)));
}

TEST_CASE("tokenizer emit operationEqual token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"+= -= *= /= %=", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(PLUSEQUAL, "+=", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(MINUSEQUAL, "-=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(STAREQUAL, "*=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(DIVIDEEQUAL, "/=", vnd::CodeSourceLocation(filename, 1, 10)));
    REQUIRE(tokens[4] == vnd::Token(PERCENTEQUAL, "%=", vnd::CodeSourceLocation(filename, 1, 13)));
}

TEST_CASE("tokenizer emit boolean operator token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"== >= <= !=", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(EQUALEQUAL, "==", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(GREATEREQUAL, ">=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(LESSEQUAL, "<=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(NOTEQUAL, "!=", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit logical operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"&& ||", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::ANDAND, "&&", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::OROR, "||", vnd::CodeSourceLocation(filename, 1, 4)));
}

TEST_CASE("tokenizer emit unary operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"++ --", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::PLUSPLUS, "++", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::MINUSMINUS, "--", vnd::CodeSourceLocation(filename, 1, 4)));
}

TEST_CASE("tokenizer emit parenthesis token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"( )", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_PARENTESIS, "(", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_PARENTESIS, ")", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("tokenizer emit square parenthesis token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"[ ]", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_SQ_PARENTESIS, "[", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_SQ_PARENTESIS, "]", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("Tokenizer emit square curly token", "[Tokenizer]") {
    vnd::Tokenizer tokenizer{"{ }", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_CUR_PARENTESIS, "{", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_CUR_PARENTESIS, "}", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("Tokenizer emit char token", "[Tokenizer]") {
    using enum vnd::TokenType;
    constexpr std::string_view code2 = R"('a' '\\' '')";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(CHAR, "a", vnd::CodeSourceLocation(filename, 1, 2)));
    REQUIRE(tokens[1] == vnd::Token(CHAR, R"(\\)", vnd::CodeSourceLocation(filename, 1, t_colum)));
    REQUIRE(tokens[2] == vnd::Token(CHAR, "", vnd::CodeSourceLocation(filename, 1, t_colum8)));
}

TEST_CASE("Tokenizer emit exception for unknown char", "[Tokenizer]") {
    vnd::Tokenizer tokenizer{";", filename};
    REQUIRE_THROWS_AS(tokenizer.tokenize(), std::runtime_error);
}

TEST_CASE("Tokenizer emit string token", "[Tokenizer]") {
    using enum vnd::TokenType;
    constexpr std::string_view code2 = R"("a" "\\" "")";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(STRING, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(STRING, R"(\\)", vnd::CodeSourceLocation(filename, 1, 5)));
    REQUIRE(tokens[2] == vnd::Token(STRING, "", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit unknown token on non closed char token", "[tokenizer]") {
    constexpr std::string_view code2 = R"('a")";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::UNKNOWN, R"(a")", vnd::CodeSourceLocation(filename, 1, 2)));
}

TEST_CASE("tokenizer emit comment token", "[tokenizer]") {
    constexpr std::string_view code2 = R"(// line comment)";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::COMMENT, "// line comment", vnd::CodeSourceLocation(filename, 1, 1)));
}

TEST_CASE("tokenizer emit multiline comment token", "[tokenizer]") {
    constexpr std::string_view code2 = R"(/*multi\nline\ncomment*/)";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::COMMENT, R"(/*multi\nline\ncomment*/)", vnd::CodeSourceLocation(filename, 1, 1)));
}  // namespace

TEST_CASE("ASTNode type conversion using as<T>()", "[ast]") {
    vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    vnd::VariableNode dummyNode("id", token);

    // Test conversion to base type (should work)
    vnd::ASTNode *basePtr = &dummyNode;
    auto *dummyPtr = basePtr->as<vnd::VariableNode>();
    REQUIRE(dummyPtr != nullptr);

    // Test conversion to derived type (should fail)
    auto *anotherPtr = basePtr->as<vnd::Token>();
    REQUIRE(anotherPtr == nullptr);

    // Test const conversion
    const vnd::ASTNode *constBasePtr = &dummyNode;
    const auto *constDummyPtr = constBasePtr->as<vnd::VariableNode>();
    REQUIRE(constDummyPtr != nullptr);

    const auto *constAnotherPtr = constBasePtr->as<vnd::Token>();
    REQUIRE(constAnotherPtr == nullptr);
}

TEST_CASE("ASTNode get token", "[ast]") {
    vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    vnd::VariableNode dummyNode("id", token);
    REQUIRE(dummyNode.get_token() == token);
}

TEST_CASE("Parser emit boolean literal node", "[parser]") {
    vnd::Parser parser("true", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Boolean);
    const auto *number = ast->as<vnd::LiteralNode<bool>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == true);
}

TEST_CASE("boolean node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::BOOLEAN, "true", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::BOOLEAN, "false", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<bool> boola{true, token1, NodeType::Boolean};
    vnd::LiteralNode<bool> boolb{false, token2, NodeType::Boolean};
    REQUIRE(boola.get_value() == true);
    REQUIRE(boolb.get_value() == false);
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == false);
    REQUIRE(boolb.get_value() == true);
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}

TEST_CASE("Parser emit string literal node", "[parser]") {
    vnd::Parser parser("\"tr\"", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::String);
    const auto *number = ast->as<vnd::LiteralNode<std::string_view>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == "tr");
}

TEST_CASE("string node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::STRING, "tr", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::STRING, "sr", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<std::string_view> boola{"tr", token1, NodeType::String};
    vnd::LiteralNode<std::string_view> boolb{"sr", token2, NodeType::String};
    REQUIRE(boola.get_value() == "tr");
    REQUIRE(boolb.get_value() == "sr");
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == "sr");
    REQUIRE(boolb.get_value() == "tr");
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}

TEST_CASE("Parser emit char literal node", "[parser]") {
    vnd::Parser parser("'c'", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Char);
    const auto *number = ast->as<vnd::LiteralNode<char>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 'c');
}

TEST_CASE("char node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::CHAR, "c", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::CHAR, "d", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<char> boola{'c', token1, NodeType::Char};
    vnd::LiteralNode<char> boolb{'d', token2, NodeType::Char};
    REQUIRE(boola.get_value() == 'c');
    REQUIRE(boolb.get_value() == 'd');
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == 'd');
    REQUIRE(boolb.get_value() == 'c');
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}
TEST_CASE("Parser emit integer number node", "[parser]") {
    vnd::Parser parser("1", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
}

TEST_CASE("Parser emit complex number node", "[parser]") {
    vnd::Parser parser("1i", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<std::complex<double>>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == std::complex<double>(0, 1));
}

TEST_CASE("integer number node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::INTEGER, "2", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{vnd::TokenType::INTEGER, "3", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::NumberNode<int> inta{2, token1, NumberNodeType::Integer};
    vnd::NumberNode<int> intb{3, token2, NumberNodeType::Integer};
    REQUIRE(inta.get_value() == 2);
    REQUIRE(intb.get_value() == 3);
    REQUIRE(inta.get_token() == token1);
    REQUIRE(intb.get_token() == token2);
    std::swap(inta, intb);
    REQUIRE(inta.get_value() == 3);
    REQUIRE(intb.get_value() == 2);
    REQUIRE(inta.get_token() == token2);
    REQUIRE(intb.get_token() == token1);
}

TEST_CASE("Imaginary number node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::DOUBLE, "2if", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{vnd::TokenType::DOUBLE, "3if", vnd::CodeSourceLocation{filename, t_line, t_colum3}};
    vnd::NumberNode<std::complex<float>> cmpa{std::complex<float>(0, 2), token1, NumberNodeType::ImaginaryFloat};
    vnd::NumberNode<std::complex<float>> cmpb{std::complex<float>(0, 3), token2, NumberNodeType::ImaginaryFloat};
    REQUIRE(cmpa.get_value() == std::complex<float>(0, 2));
    REQUIRE(cmpb.get_value() == std::complex<float>(0, 3));
    REQUIRE(cmpa.get_token() == token1);
    REQUIRE(cmpb.get_token() == token2);
    std::swap(cmpa, cmpb);
    REQUIRE(cmpa.get_value() == std::complex<float>(0, 3));
    REQUIRE(cmpb.get_value() == std::complex<float>(0, 2));
    REQUIRE(cmpa.get_token() == token2);
    REQUIRE(cmpb.get_token() == token1);
}

TEST_CASE("double number node swap", "[parser]") {
    vnd::NumberNode<double> douba{2.3, vnd::Token{vnd::TokenType::DOUBLE, "2.3", vnd::CodeSourceLocation{filename, t_line, t_colum}},
                                  NumberNodeType::Double};
    vnd::NumberNode<double> doubb{3.3, vnd::Token{vnd::TokenType::DOUBLE, "3.3", vnd::CodeSourceLocation{filename, t_line, t_colum2}},
                                  NumberNodeType::Double};
    REQUIRE(douba.get_value() == 2.3);
    REQUIRE(doubb.get_value() == 3.3);
    std::swap(douba, doubb);
    REQUIRE(douba.get_value() == 3.3);
    REQUIRE(doubb.get_value() == 2.3);
}

TEST_CASE("unary node swap", "[parser]") {
    using enum vnd::TokenType;
    auto token1 = vnd::Token{MINUS, "-", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{IDENTIFIER, "d", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token3 = vnd::Token{STAR, "*", vnd::CodeSourceLocation{filename, t_line4, t_colum}};
    auto token4 = vnd::Token{IDENTIFIER, "s", vnd::CodeSourceLocation{filename, t_line4, t_colum4}};
    vnd::UnaryExpressionNode unara{"-", token1, MAKE_UNIQUE(vnd::VariableNode, "d", token2)};
    vnd::UnaryExpressionNode unarb{"*", token3, MAKE_UNIQUE(vnd::VariableNode, "s", token4)};
    REQUIRE(unara.getOp() == "-");
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unara.get_token() == token1);
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unarb.getOp() == "*");
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unarb.get_token() == token3);
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->get_token() == token4);
    std::swap(unara, unarb);
    REQUIRE(unara.getOp() == "*");
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unara.get_token() == token3);
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->get_token() == token4);
    REQUIRE(unarb.getOp() == "-");
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unarb.get_token() == token1);
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->get_token() == token2);
}

TEST_CASE("binary node swap", "[parser]") {
    using enum vnd::TokenType;
    auto token1 = vnd::Token{MINUS, "-", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    auto token2 = vnd::Token{IDENTIFIER, "d", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token3 = vnd::Token{IDENTIFIER, "a", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token4 = vnd::Token{STAR, "*", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    auto token5 = vnd::Token{IDENTIFIER, "s", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token6 = vnd::Token{IDENTIFIER, "b", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    vnd::BinaryExpressionNode unara{"-", token1, MAKE_UNIQUE(vnd::VariableNode, "d", token2), MAKE_UNIQUE(vnd::VariableNode, "a", token3)};
    vnd::BinaryExpressionNode unarb{"*", token4, MAKE_UNIQUE(vnd::VariableNode, "s", token5), MAKE_UNIQUE(vnd::VariableNode, "b", token6)};
    REQUIRE(unara.getOp() == "-");
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->getName() == "a");
    REQUIRE(unara.get_token() == token1);
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->get_token() == token3);
    REQUIRE(unarb.getOp() == "*");
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->getName() == "b");
    REQUIRE(unarb.get_token() == token4);
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->get_token() == token5);
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->get_token() == token6);
    std::swap(unara, unarb);
    REQUIRE(unara.getOp() == "*");
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->getName() == "b");
    REQUIRE(unara.get_token() == token4);
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->get_token() == token5);
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->get_token() == token6);
    REQUIRE(unarb.getOp() == "-");
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->getName() == "a");
    REQUIRE(unarb.get_token() == token1);
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->get_token() == token3);
}
TEST_CASE("Parser::convertToInt tests", "[parser]") {
    SECTION("Valid integer conversion") {
        REQUIRE(vnd::Parser::convertToInt("123") == 123);
        REQUIRE(vnd::Parser::convertToInt("-456") == -456);
    }

    SECTION("Invalid argument throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToInt("abc"), std::invalid_argument); }

    SECTION("Result out of range throws exception") {
        REQUIRE_THROWS_AS(vnd::Parser::convertToInt("2147483648"), std::out_of_range);  // Assuming int is 32-bit
    }

    SECTION("Trailing characters throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToInt("123a"), std::invalid_argument); }
}

TEST_CASE("Parser::convertToIntformExa tests") {
    SECTION("Valid hexadecimal conversion") {
        REQUIRE(vnd::Parser::convertToIntformExa("1A") == 26);     // 0x1A = 26 in decimal
        REQUIRE(vnd::Parser::convertToIntformExa("-FF") == -255);  // 0xFF = -255 in decimal
    }

    SECTION("Invalid argument throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToIntformExa("xyz"), std::invalid_argument); }

    SECTION("Result out of range throws exception") {
        REQUIRE_THROWS_AS(vnd::Parser::convertToIntformExa("80000000"), std::out_of_range);  // Assuming int is 32-bit
    }

    SECTION("Trailing characters throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToIntformExa("1A a"), std::invalid_argument); }
}

TEST_CASE("Parser::convertToIntformOct tests") {
    SECTION("Valid octal conversion") {
        REQUIRE(vnd::Parser::convertToIntformOct("77") == 63);   // 077 = 63 in decimal
        REQUIRE(vnd::Parser::convertToIntformOct("-10") == -8);  // -010 = -8 in decimal
    }

    SECTION("Invalid argument throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToIntformOct("xyz"), std::invalid_argument); }

    SECTION("Result out of range throws exception") {
        REQUIRE_THROWS_AS(vnd::Parser::convertToIntformOct("20000000000"), std::out_of_range);  // Assuming int is 32-bit
    }

    SECTION("Trailing characters throws exception") { REQUIRE_THROWS_AS(vnd::Parser::convertToIntformOct("77a"), std::invalid_argument); }
}

TEST_CASE("Parser emit integer number node form exadecimal", "[parser]") {
    vnd::Parser parser("#23", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->get_value() == 35);
}

TEST_CASE("Parser emit integer number node form exadecimal max int -1", "[parser]") {
    vnd::Parser parser("#7ffffffe", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->get_value() == 2147483646);
}

TEST_CASE("Parser emit integer number node form octal", "[parser]") {
    vnd::Parser parser("#o23", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->get_value() == 19);
}

TEST_CASE("Parser emit integer number node form octal max int -1", "[parser]") {
    vnd::Parser parser("#o17777777776", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->get_value() == 2147483646);
}

TEST_CASE("Parser emit integer number node print", "[parser]") {
    vnd::Parser parser("1", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->print() == "NUMBER_INTEGER(1)");
}

TEST_CASE("Parser emit integer number node compat print", "[parser]") {
    vnd::Parser parser("1", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<int>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "i");
#else
    REQUIRE(number->getTypeIDName() == "int");
#endif
    REQUIRE(number->comp_print() == "NUM_INT(1)");
}

TEST_CASE("Parser emit variable node", "[parser]") {
    vnd::Parser parser("y", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
}

TEST_CASE("Parser emit double number node double", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<double>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "d");
#else
    REQUIRE(number->getTypeIDName() == "double");
#endif
    REQUIRE(number->get_value() == 1.5);
}

TEST_CASE("Parser emit double number node double print", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<double>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "d");
#else
    REQUIRE(number->getTypeIDName() == "double");
#endif
    REQUIRE(number->get_value() == 1.5);
    REQUIRE(number->print() == "NUMBER_DOUBLE(1.5)");
}

TEST_CASE("Parser emit double number node double compat print", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<vnd::NumberNode<double>>();
    REQUIRE(number != nullptr);
#ifdef __linux__
    REQUIRE(number->getTypeIDName() == "d");
#else
    REQUIRE(number->getTypeIDName() == "double");
#endif
    REQUIRE(number->get_value() == 1.5);
    REQUIRE(number->comp_print() == "NUM_DBL(1.5)");
}

TEST_CASE("Parser emit variable node print", "[parser]") {
    vnd::Parser parser("y", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
    REQUIRE(variable->print() == "VARIABLE(y)");
}

TEST_CASE("Parser emit variable node compat print", "[parser]") {
    vnd::Parser parser("y", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
    REQUIRE(variable->comp_print() == "VAR(y)");
}

TEST_CASE("Parser emit unary expression node", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
}

TEST_CASE("Parser emit unary expression node print", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->print() == "UNARY_EXPRESION(op:\"-\" operand:VARIABLE(x))");
}

TEST_CASE("Parser emit unary expression node compat print", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->comp_print() == "UNE(op:\"-\" opr:VAR(x))");
}

TEST_CASE("Parser emit binary expression node", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();
    REQUIRE(binaryNode != nullptr);

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto *leftNumber = binaryNode->getLeft()->as<vnd::NumberNode<int>>();
    const auto *rightNumber = binaryNode->getRight()->as<vnd::NumberNode<int>>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
#ifdef __linux__
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#else
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#endif

    // Check the values of left and right operands
    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
}

TEST_CASE("Parser emit binary expression node print", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto *leftNumber = binaryNode->getLeft()->as<vnd::NumberNode<int>>();
    const auto *rightNumber = binaryNode->getRight()->as<vnd::NumberNode<int>>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
#ifdef __linux__
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#else
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#endif

    // Check the values of left and right operands
    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
    REQUIRE(binaryNode->print() == R"(BINARY_EXPRESION(op:"+" left:NUMBER_INTEGER(1), right:NUMBER_INTEGER(2)))");
}

TEST_CASE("Parser emit binary expression node compact print", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto *leftNumber = binaryNode->getLeft()->as<vnd::NumberNode<int>>();
    const auto *rightNumber = binaryNode->getRight()->as<vnd::NumberNode<int>>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
#ifdef __linux__
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#else
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#endif

    // Check the values of left and right operands
    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
    REQUIRE(binaryNode->comp_print() == R"(BINE(op:"+" l:NUM_INT(1), r:NUM_INT(2)))");
}

TEST_CASE("Parser pars complex expression", "[parser]") {
    // clang-format off
    vnd::Parser parser{"2 + 3 + (4.2 / 2) * 3 + y", "input.vn"};
    auto ast = parser.parse();
    REQUIRE(ast->print() == R"(BINARY_EXPRESION(op:"+" left:BINARY_EXPRESION(op:"+" left:BINARY_EXPRESION(op:"+" left:NUMBER_INTEGER(2), right:NUMBER_INTEGER(3)), right:BINARY_EXPRESION(op:"*" left:BINARY_EXPRESION(op:"/" left:NUMBER_DOUBLE(4.2), right:NUMBER_INTEGER(2)), right:NUMBER_INTEGER(3))), right:VARIABLE(y)))");
    REQUIRE(ast->comp_print() == R"(BINE(op:"+" l:BINE(op:"+" l:BINE(op:"+" l:NUM_INT(2), r:NUM_INT(3)), r:BINE(op:"*" l:BINE(op:"/" l:NUM_DBL(4.2), r:NUM_INT(2)), r:NUM_INT(3))), r:VAR(y)))");
    // clang-format on
}

TEST_CASE("Parser emit exception for mismacted  paren", "[parser]") {
    vnd::Parser tokenizer{"1 + 2 +( 2+3*3", filename};
    REQUIRE_THROWS_AS(tokenizer.parse(), vnd::ParserException);
}

TEST_CASE("Parser emit exception for uncomplete expression", "[parser]") {
    vnd::Parser tokenizer{"1 + 2 *", filename};
    REQUIRE_THROWS_AS(tokenizer.parse(), vnd::ParserException);
}

TEST_CASE("Parser emit exception for nonexistent unary operator", "[parser]") {
    vnd::Parser tokenizer{"*2", filename};
    REQUIRE_THROWS_AS(tokenizer.parse(), vnd::ParserException);
}

TEST_CASE("Parser emit i8 TypeNode node", "[parser]") {
    vnd::Parser parser("a: i8", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);
    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();
    REQUIRE(binaryNode != nullptr);
    REQUIRE(binaryNode->getOp() == ":");
    const auto *typeNode = binaryNode->getRight()->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->getVariableType() == vnd::TokenType::TYPE_I8);
    REQUIRE(typeNode->get_value() == "i8");
}

TEST_CASE("Parser emit identifier TypeNode node", "[parser]") {
    vnd::Parser parser("a: type", filename);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);
    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();
    REQUIRE(binaryNode != nullptr);
    REQUIRE(binaryNode->getOp() == ":");
    const auto *typeNode = binaryNode->getRight()->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->getVariableType() == vnd::TokenType::IDENTIFIER);
    REQUIRE(typeNode->get_value() == "type");
}

TEST_CASE("Parser emit exception for impossibile TypeNode node", "[parser]") {
    vnd::Parser parser("a: 1", filename);
    REQUIRE_THROWS_AS(parser.parse(), vnd::ParserException);
}

// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers)
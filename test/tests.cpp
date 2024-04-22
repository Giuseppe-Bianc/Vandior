// NOLINTBEGIN(*-include-cleaner)
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
static inline constexpr auto oper = vnd::TokenType::OPERATOR;
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
#ifdef _WIN32  // Windows
static inline constexpr std::string_view outFilename = R"(.\vnbuild\src\unknown.cpp)";
#else
static inline constexpr std::string_view outFilename = R"(./vnbuild/src/unknown.cpp)";
#endif

static inline constexpr long long int timerSleap = 12;
static inline constexpr long long int timerSleap2 = 5;
static inline constexpr std::size_t timerCicles = 1000000;
static inline constexpr long double timerResolution = 5.0L;
#define REQ_FORMAT(type, string) REQUIRE(FORMAT("{}", type) == (string));  // NOLINT(*-macro-usage)

TEST_CASE("std::filesystem::path formater", "[FMT]") { REQUIRE(FORMAT("{}", std::filesystem::path("../ssss")) == "../ssss"); }
TEST_CASE("glm::vec formater", "[FMT]") {
    REQUIRE(FORMAT("{}", glm::vec2{0.0F, 0.0F}) == "vec2(0, 0)");
    REQUIRE(FORMAT("{}", glm::dvec2{0.0, 0.0}) == "dvec2(0, 0)");
    REQUIRE(FORMAT("{}", glm::ldvec2{0.0, 0.0}) == "ldvec2(0, 0)");
    REQUIRE(FORMAT("{}", glm::vec3{0.0F, 0.0F, 0.0F}) == "vec3(0, 0, 0)");
    REQUIRE(FORMAT("{}", glm::dvec3{0.0, 0.0, 0.0}) == "dvec3(0, 0, 0)");
    REQUIRE(FORMAT("{}", glm::ldvec3{0.0, 0.0, 0.0F}) == "ldvec3(0, 0, 0)");
    REQUIRE(FORMAT("{}", glm::vec4{0.0F, 0.0, 0.0, 0.0}) == "vec4(0, 0, 0, 0)");
    REQUIRE(FORMAT("{}", glm::dvec4{0.0, 0.0, 0.0, 0.0}) == "dvec4(0, 0, 0, 0)");
    REQUIRE(FORMAT("{}", glm::ldvec4{0.0, 0.0, 0.0F, 0.0}) == "ldvec4(0, 0, 0, 0)");
}

TEST_CASE("glm::mat formater", "[FMT]") {
    REQUIRE(FORMAT("{}", glm::mat2{0.0F, 0.0F, 0.0F, 0.0F}) == "mat2x2((0,0), (0,0))");
    REQUIRE(FORMAT("{}", glm::dmat2{0.0, 0.0, 0.0, 0.0}) == "dmat2x2((0,0), (0,0))");
    REQUIRE(FORMAT("{}", glm::ldmat2{0.0, 0.0, 0.0, 0.0}) == "ldmat2x2((0,0), (0,0))");
    REQUIRE(FORMAT("{}", glm::mat3{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F}) == "mat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQUIRE(FORMAT("{}", glm::dmat3{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) == "dmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQUIRE(FORMAT("{}", glm::ldmat3{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) == "ldmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQUIRE(FORMAT("{}", glm::mat4{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F}) ==
            "mat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQUIRE(FORMAT("{}", glm::dmat4{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) ==
            "dmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQUIRE(FORMAT("{}", glm::ldmat4{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) ==
            "ldmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
}
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
    REQ_FORMAT(OPERATOR, "OPERATOR")
    REQ_FORMAT(MINUS_OPERATOR, "MINUS_OPERATOR")
    REQ_FORMAT(EQUAL_OPERATOR, "EQUAL_OPERATOR")
    REQ_FORMAT(DOT_OPERATOR, "DOT_OPERATOR")
    REQ_FORMAT(IDENTIFIER, "IDENTIFIER")
    REQ_FORMAT(CHAR, "CHAR")
    REQ_FORMAT(STRING, "STRING")
    REQ_FORMAT(K_MAIN, "K_MAIN")
    REQ_FORMAT(K_VAR, "K_VAR")
    REQ_FORMAT(K_FOR, "K_FOR")
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
    REQ_FORMAT(OPERATION_EQUAL, "OPERATION_EQUAL")
    REQ_FORMAT(BOOLEAN_OPERATOR, "BOOLEAN_OPERATOR")
    REQ_FORMAT(NOT_OPERATOR, "NOT_OPERATOR")
    REQ_FORMAT(LOGICAL_OPERATOR, "LOGICAL_OPERATOR")
    REQ_FORMAT(UNARY_OPERATOR, "UNARY_OPERATOR")
    REQ_FORMAT(COMMA, "COMMA")
    REQ_FORMAT(COLON, "COLON")
    REQ_FORMAT(COMMENT, "COMMENT")
    REQ_FORMAT(EOFT, "EOF")
    REQ_FORMAT(UNKNOWN, "UNKNOWN")
}

namespace {
    vnd::Transpiler createSimpleTranspiler(const std::vector<vnd::Token> &tokens) {
        vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
        for(const auto &token : tokens) { factory.checkToken(token); }
        factory.addInstruction();
        return vnd::Transpiler::create(factory.getInstructions());
    }

    vnd::Transpiler createComplexTranspiler(const std::vector<vnd::Token> &tokens) {
        vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
        auto line = tokens.at(0).getLine();
        for(const auto &token : tokens) {
            if(token.getLine() > line) {
                factory.addInstruction();
                line = token.getLine();
            }
            factory.checkToken(token);
        }
        factory.addInstruction();
        return vnd::Transpiler::create(factory.getInstructions());
    }
    void modifyToken(vnd::Token &token) {
        token.setType(vnd::TokenType::INTEGER);
        token.setValue("assss");
        token.setFileName(filename);
        token.setLine(1);
        token.setColumn(1);
    }
    std::string fileContent() {
        std::ifstream stream(outFilename.data());
        return {std::istreambuf_iterator<char>{stream}, {}};
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
    REQUIRE(FORMAT("{}", token) == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
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
#else
    REQUIRE(token.to_string() == R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
#endif
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
    REQUIRE(FORMAT("{}", token) == R"((type: INTEGER, value: 'assss', source location:(file:.\unknown.vn, line:1, column:1)))");
#else
    REQUIRE(FORMAT("{}", token) == R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
#endif
}

TEST_CASE("Token Comparison Equality", "[Token]") {
    vnd::Token token1(oper, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
    vnd::Token token2(oper, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
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
    vnd::Tokenizer tokenizer{"* / = , : < > ! | & + - ^ .", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 15);
    REQUIRE(tokens[0] == vnd::Token(oper, "*", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(oper, "/", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(EQUAL_OPERATOR, "=", vnd::CodeSourceLocation(filename, 1, 5)));
    REQUIRE(tokens[3] == vnd::Token(COMMA, ",", vnd::CodeSourceLocation(filename, 1, t_colum)));
    REQUIRE(tokens[4] == vnd::Token(COLON, ":", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[5] == vnd::Token(BOOLEAN_OPERATOR, "<", vnd::CodeSourceLocation(filename, 1, t_colum8)));
    REQUIRE(tokens[6] == vnd::Token(BOOLEAN_OPERATOR, ">", vnd::CodeSourceLocation(filename, 1, t_colum11)));
    REQUIRE(tokens[7] == vnd::Token(NOT_OPERATOR, "!", vnd::CodeSourceLocation(filename, 1, t_colum10)));
    REQUIRE(tokens[8] == vnd::Token(oper, "|", vnd::CodeSourceLocation(filename, 1, t_colum13)));
    REQUIRE(tokens[9] == vnd::Token(oper, "&", vnd::CodeSourceLocation(filename, 1, 19)));
    REQUIRE(tokens[10] == vnd::Token(oper, "+", vnd::CodeSourceLocation(filename, 1, 21)));
    REQUIRE(tokens[11] == vnd::Token(MINUS_OPERATOR, "-", vnd::CodeSourceLocation(filename, 1, 23)));
    REQUIRE(tokens[12] == vnd::Token(oper, "^", vnd::CodeSourceLocation(filename, 1, 25)));
    REQUIRE(tokens[13] == vnd::Token(DOT_OPERATOR, ".", vnd::CodeSourceLocation(filename, 1, 27)));
}

TEST_CASE("tokenizer emit operationEqual token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"+= -= *= /=", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(OPERATION_EQUAL, "+=", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(OPERATION_EQUAL, "-=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(OPERATION_EQUAL, "*=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(OPERATION_EQUAL, "/=", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit boolean operator token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"== >= <= !=", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(BOOLEAN_OPERATOR, "==", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(BOOLEAN_OPERATOR, ">=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(BOOLEAN_OPERATOR, "<=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(BOOLEAN_OPERATOR, "!=", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit logical operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"&& ||", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::LOGICAL_OPERATOR, "&&", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::LOGICAL_OPERATOR, "||", vnd::CodeSourceLocation(filename, 1, 4)));
}

TEST_CASE("tokenizer emit unary operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"++ --", filename};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::UNARY_OPERATOR, "++", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::UNARY_OPERATOR, "--", vnd::CodeSourceLocation(filename, 1, 4)));
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
}

TEST_CASE("corrected format for InstructionType", "[Instruction_type]") {
    using enum vnd::InstructionType;
    REQ_FORMAT(PARAMETER_EXPRESSION, "PARAMETER_EXPRESSION")
    REQ_FORMAT(OPERATION, "OPERATION")
    REQ_FORMAT(ASSIGNATION, "ASSIGNATION")
    REQ_FORMAT(EXPRESSION, "EXPRESSION")
    REQ_FORMAT(SQUARE_EXPRESSION, "SQUARE_EXPRESSION")
    REQ_FORMAT(RETURN_EXPRESSION, "RETURN_EXPRESSION")
    REQ_FORMAT(DECLARATION, "DECLARATION")
    REQ_FORMAT(INITIALIZATION, "INITIALIZATION")
    REQ_FORMAT(ARRAY_INIZIALIZATION, "ARRAY_INIZIALIZATION")
    REQ_FORMAT(MAIN, "MAIN")
    REQ_FORMAT(STRUCTURE, "STRUCTURE")
    REQ_FORMAT(ELSE, "ELSE")
    REQ_FORMAT(FOR_STRUCTURE, "FOR_STRUCTURE")
    REQ_FORMAT(BREAK, "BREAK")
    REQ_FORMAT(FOR_INITIALIZATION, "FOR_INITIALIZATION")
    REQ_FORMAT(FOR_CONDITION, "FOR_CONDITION")
    REQ_FORMAT(FOR_STEP, "FOR_STEP")
    REQ_FORMAT(DEFINITION, "DEFINITION")
    REQ_FORMAT(PARAMETER_DEFINITION, "PARAMETER_DEFINITION")
    REQ_FORMAT(RETURN_DEFINITION, "RETURN_DEFINITION")
    REQ_FORMAT(OPEN_SCOPE, "OPEN_SCOPE")
    REQ_FORMAT(CLOSE_SCOPE, "CLOSE_SCOPE")
    REQ_FORMAT(BLANK, "BLANK")
}

TEST_CASE("Instruction typeToString function", "[Instruction]") {
    vnd::Instruction instruction = vnd::Instruction::create(filename);
    std::vector<std::string> result = instruction.typeToString();
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == "BLANK");
}

TEST_CASE("Instruction checkToken function for valid tokens", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    vnd::Token identifierToken(vnd::TokenType::IDENTIFIER, "variable", vnd::CodeSourceLocation(filename, 1, 2));
    factory.checkToken(identifierToken);
    REQUIRE(factory.canTerminate() == true);
}

TEST_CASE("Instruction checkToken function for invalid tokens", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    vnd::Token invalidToken(vnd::TokenType::UNKNOWN, "invalid", vnd::CodeSourceLocation(filename, 1, 2));
    REQUIRE_THROWS_AS(factory.checkToken(invalidToken), vnd::InstructionException);
}

TEST_CASE("Corrected type of assignation instruction", "[Instruction]") {
    using enum vnd::TokenType;
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::BLANK);
    factory.checkToken(vnd::Token{IDENTIFIER, "a", vnd::CodeSourceLocation(filename, 1, 0)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::OPERATION);
    factory.checkToken(vnd::Token{EQUAL_OPERATOR, "", vnd::CodeSourceLocation(filename, 1, 1)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::ASSIGNATION);
    factory.checkToken(vnd::Token{OPEN_PARENTESIS, "", vnd::CodeSourceLocation(filename, 1, 2)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::EXPRESSION);
}

TEST_CASE("Corrected type of multy assignation instruction", "[Instruction]") {
    using enum vnd::TokenType;
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{K_VAR, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::DECLARATION);
    factory.checkToken(vnd::Token{COLON, "", vnd::CodeSourceLocation(filename, 0, 2)});
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, 3)});
    factory.checkToken(vnd::Token{OPEN_SQ_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 4)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::SQUARE_EXPRESSION);
    factory.checkToken(vnd::Token{CLOSE_SQ_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, t_colum2)});
    factory.checkToken(vnd::Token{EQUAL_OPERATOR, "", vnd::CodeSourceLocation(filename, 0, t_colum)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::INITIALIZATION);
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, t_colum3)});
    factory.checkToken(vnd::Token{OPEN_SQ_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, t_colum4)});
    factory.checkToken(vnd::Token{INTEGER, "", vnd::CodeSourceLocation(filename, 0, t_colum5)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::SQUARE_EXPRESSION);
    factory.checkToken(vnd::Token{CLOSE_SQ_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, t_colum6)});
}

TEST_CASE("Corrected type of parameter expression instruction", "[Instruction]") {
    using enum vnd::TokenType;
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{OPEN_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::PARAMETER_EXPRESSION);
}

TEST_CASE("Corrected type of parameter definition instruction", "[Instruction]") {
    using enum vnd::TokenType;
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{K_FUN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, 1)});
    factory.checkToken(vnd::Token{OPEN_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 2)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::PARAMETER_DEFINITION);
    factory.checkToken(vnd::Token{CLOSE_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 3)});
}

TEST_CASE("Corrected type of return expression instruction", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{vnd::TokenType::K_RETURN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{vnd::TokenType::BOOLEAN, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::RETURN_EXPRESSION);
}

TEST_CASE("Corrected type of main instruction", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{vnd::TokenType::K_MAIN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{vnd::TokenType::OPEN_CUR_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::MAIN);
}

TEST_CASE("Corrected type of for instruction", "[Instruction]") {
    using enum vnd::TokenType;
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{K_FOR, "", vnd::CodeSourceLocation(filename, 0, 0)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::FOR_STRUCTURE);
    factory.checkToken(vnd::Token{IDENTIFIER, "", vnd::CodeSourceLocation(filename, 0, 1)});
    factory.checkToken(vnd::Token{EQUAL_OPERATOR, "", vnd::CodeSourceLocation(filename, 0, 2)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::FOR_INITIALIZATION);
    factory.checkToken(vnd::Token{DOUBLE, "", vnd::CodeSourceLocation(filename, 0, 3)});
    factory.checkToken(vnd::Token{COMMA, "", vnd::CodeSourceLocation(filename, 0, 4)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::FOR_CONDITION);
    factory.checkToken(vnd::Token{DOUBLE, "", vnd::CodeSourceLocation(filename, 0, t_colum2)});
    factory.checkToken(vnd::Token{COMMA, "", vnd::CodeSourceLocation(filename, 0, t_colum)});
    REQUIRE(factory.getInstruction().getLastType() == vnd::InstructionType::FOR_STEP);
}
TEST_CASE("Instruction toString() Empty tokens", "[Instruction]") {
    vnd::Instruction instruction = vnd::Instruction::create(filename);
    REQUIRE(instruction.toString() == "");  // NOLINT(*-container-size-empty)
}

TEST_CASE("Instruction toString() Empty tokens FMT", "[Instruction]") {
    vnd::Instruction instruction = vnd::Instruction::create(filename);
    REQ_FORMAT(instruction, "")
}

TEST_CASE("Instruction toString()", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{vnd::TokenType::K_MAIN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{vnd::TokenType::OPEN_CUR_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().toString() == "0\t  ");
}

TEST_CASE("Instruction getTokens()", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{vnd::TokenType::K_MAIN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{vnd::TokenType::OPEN_CUR_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQUIRE(factory.getInstruction().getTokens().size() == 2);
}

TEST_CASE("Instruction toString() FMT", "[Instruction]") {
    vnd::InstructionFactory factory = vnd::InstructionFactory::create(filename);
    factory.checkToken(vnd::Token{vnd::TokenType::K_MAIN, "", vnd::CodeSourceLocation(filename, 0, 0)});
    factory.checkToken(vnd::Token{vnd::TokenType::OPEN_CUR_PARENTESIS, "", vnd::CodeSourceLocation(filename, 0, 1)});
    REQ_FORMAT(factory.getInstruction(), "0\t  ")
}

TEST_CASE("ExpressionFactory emit int type", "[factory]") {
    std::vector<vnd::Token> tokens = {{inte, "1", vnd::CodeSourceLocation(filename, 1, 0)},
                                      {oper, "+", vnd::CodeSourceLocation(filename, 1, 1)},
                                      {inte, "2", vnd::CodeSourceLocation(filename, 1, 2)}};
    std::shared_ptr<vnd::Scope> scope = vnd::Scope::createMain();
    auto iterator = tokens.begin();
    vnd::ExpressionFactory factory = vnd::ExpressionFactory::create(iterator, tokens.end(), scope, false, false);
    auto fpr = factory.parse({});
    REQUIRE(factory.size() == 1);
    REQUIRE(factory.getExpression().getType() == "u8");
}

TEST_CASE("ExpressionFactory emit float type", "[factory]") {
    std::vector<vnd::Token> tokens = {{doub, "1", vnd::CodeSourceLocation(filename, 1, 0)},
                                      {oper, "+", vnd::CodeSourceLocation(filename, 1, 1)},
                                      {inte, "2", vnd::CodeSourceLocation(filename, 1, 2)}};
    std::shared_ptr<vnd::Scope> scope = vnd::Scope::createMain();
    auto iterator = tokens.begin();
    vnd::ExpressionFactory factory = vnd::ExpressionFactory::create(iterator, tokens.end(), scope, false, false);
    auto fpr = factory.parse({});
    REQUIRE(factory.size() == 1);
    REQUIRE(factory.getExpression().getType() == "f64");
}

TEST_CASE("ExpressionFactory emit bool type", "[factory]") {
    using enum vnd::TokenType;
    std::vector<vnd::Token> tokens = {{OPEN_PARENTESIS, "(", vnd::CodeSourceLocation(filename, 1, 0)},
                                      {BOOLEAN, "true", vnd::CodeSourceLocation(filename, 1, 1)},
                                      {LOGICAL_OPERATOR, "||", vnd::CodeSourceLocation(filename, 1, 2)},
                                      {BOOLEAN, "false", vnd::CodeSourceLocation(filename, 1, t_colum2)},
                                      {CLOSE_PARENTESIS, ")", vnd::CodeSourceLocation(filename, 1, t_colum)}};
    std::shared_ptr<vnd::Scope> scope = vnd::Scope::createMain();
    auto iterator = tokens.begin();
    vnd::ExpressionFactory factory = vnd::ExpressionFactory::create(iterator, tokens.end(), scope, false, false);
    auto fpr = factory.parse({CLOSE_PARENTESIS});
    REQUIRE(factory.size() == 1);
    REQUIRE(factory.getExpression().getType() == "bool");
}

TEST_CASE("ExpressionFactory emit function type", "[factory]") {
    using enum vnd::TokenType;
    std::vector<vnd::Token> tokens = {{IDENTIFIER, "testPar", vnd::CodeSourceLocation(filename, 1, 0)},
                                      {OPEN_PARENTESIS, "(", vnd::CodeSourceLocation(filename, 1, 1)},
                                      {STRING, "Hello", vnd::CodeSourceLocation(filename, 1, 3)},
                                      {CLOSE_PARENTESIS, ")", vnd::CodeSourceLocation(filename, 1, t_colum4)}};
    std::shared_ptr<vnd::Scope> scope = vnd::Scope::createMain();
    auto iterator = tokens.begin();
    vnd::ExpressionFactory factory = vnd::ExpressionFactory::create(iterator, tokens.end(), scope, false, false);
    auto fpr = factory.parse({CLOSE_PARENTESIS});
    REQUIRE(factory.size() == 1);
    REQUIRE(factory.getExpression().getType() == "i64");
}
namespace {
    void createBuildSrcFolder(std::string_view fname) {
        auto fpath = fs::path(fname);
        fpath = fpath.parent_path();
        auto buildfres = vnd::FolderCreationResult::createFolder("vnbuild", fpath);
        if(buildfres.success()) {
            auto srcfres = vnd::FolderCreationResult::createFolder("src", buildfres.pathcref());
            // auto srcf = srcfres.pathcref();
        }
    }
}  // namespace
TEST_CASE("Transpiler transpile main instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{}", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\treturn 0;\n"
                    "}\n");
}  // namespace TEST_CASE("Transpiler transpile main instruction","[transpiler]")

TEST_CASE("Transpiler transpile declaration instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"var num, num1 : i32", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "i32 _num{}, _num1{};\n");
}

TEST_CASE("Transpiler transpile declaration underscore instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"var _num, _num1 : u32", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "u32 v_num{}, v_num1{};\n");
}

TEST_CASE("Transpiler transpile initialization instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"var num, num1 : i8 = 1", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "i8 _num = 1, _num1{};\n");
}

TEST_CASE("Transpiler transpile initialization underscore instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"var _num, _num1 : u64 = 1", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "u64 v_num = 1, v_num1{};\n");
}

TEST_CASE("Transpiler transpile const instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"const num : u16 = 1 + 333", filename};
    auto transpiler = createSimpleTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "const u16 _num = 334;\n\n");
}

TEST_CASE("Transpiler transpile operation instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main {\nprint(\"Test {}\", args[0])\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\t_print(string(\"Test {}\"), _args.at(0));\n"
                    "\treturn 0;\n"
                    "}\n");
}

TEST_CASE("Transpiler transpile assignation instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{\nvar num : i8\nnum = 1\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\ti8 _num{};\n"
                    "\t_num = 1;\n"
                    "\treturn 0;\n"
                    "}\n");
    ;
}

TEST_CASE("Transpiler transpile if instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{\nif(true){\n} else if(false){\n} else{}\n\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\tif(true) {\n"
                    "\t} else if(false) {\n"
                    "\t} else {}\n"
                    "\treturn 0;\n"
                    "}\n");
}

TEST_CASE("Transpiler transpile while and break instructions", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{\nwhile(true){\nbreak\n}\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\twhile(true) {\n"
                    "\t\tbreak;\n"
                    "\t}\n"
                    "\treturn 0;\n"
                    "}\n");
}

TEST_CASE("Transpiler transpile for instruction", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{\nfor var i : i8 = 0, 10, 1 {}\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\tFOR_LOOP(i8, _i, 0, 10, 1) {}\n"
                    "\treturn 0;\n"
                    "}\n");
}

TEST_CASE("Transpiler transpile open and close scope instructions", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"main{\n{\n}\n\n}", filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    createBuildSrcFolder(filename);
    transpiler.transpile(std::string{filename});
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "int main(int argc, char **argv) {\n"
                    "\tconst vnd::vector<string> _args = vnd::createArgs(argc, argv);\n"
                    "\t{\n"
                    "\t}\n"
                    "\treturn 0;\n"
                    "}\n");
}

TEST_CASE("Transpiler transpile function instructions", "[transpiler]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"fun funzione(num: u8): u8, bool {\n"
                             "\treturn num * 2, num % 2 == 0\n"
                             "}\n"
                             "fun procedura() {\n"
                             "\tprint(\"Procedura\")\n"
                             "}\n",
                             filename};
    auto transpiler = createComplexTranspiler(tokenizer.tokenize());
    transpiler.transpile(std::string{filename});
    createBuildSrcFolder(filename);
    REQUIRE(std::filesystem::exists(outFilename));
    std::string code = fileContent();
    REQUIRE(code == "#include <base/base.hpp>\n\n"
                    "std::tuple<u8, bool> _funzione(u8 _num) {\n"
                    "\treturn { _num * 2,  vnd::mod(_num, 2) == 0};\n"
                    "}\n"
                    "void _procedura() {\n"
                    "\t_print(string(\"Procedura\"));\n"
                    "}\n");
}
// NOLINTEND(*-include-cleaner)
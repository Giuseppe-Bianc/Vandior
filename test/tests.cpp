#include <catch2/catch_test_macros.hpp>


#include <Vandior/vandior.hpp> // NOLINT(*-include-cleaner)

TEST_CASE("default constructed token", "[token]"){
    Token token{}; // NOLINT(*-include-cleaner)
    REQUIRE(token.getType() == TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
}

TEST_CASE("default constructed token toString", "[token]"){
    Token token{}; // NOLINT(*-include-cleaner)
    REQUIRE(token.getType() == TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: , line: 0, column: 0)");
}

TEST_CASE("default constructed token set propriety", "[token]"){
    Token token{}; // NOLINT(*-include-cleaner)
    REQUIRE(token.getType() == TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    token.setType(TokenType::INTEGER);
    token.setValue("assss");
    token.setLine(1);
    token.setColumn(1);
    REQUIRE(token.getType() == TokenType::INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}

TEST_CASE("default constructed token set propriety tostring", "[token]"){
    Token token{}; // NOLINT(*-include-cleaner)
    REQUIRE(token.getType() == TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    token.setType(TokenType::INTEGER);
    token.setValue("assss");
    token.setLine(1);
    token.setColumn(1);
    REQUIRE(token.getType() == TokenType::INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
    REQUIRE(token.to_string() == "(type: INTEGER, value: assss, line: 1, column: 1)");
}

TEST_CASE("tokenizer emit identifier token", "[tokenizer]"){
    Tokenizer tokenizer{"a a_ a0"}; // NOLINT(*-include-cleaner)
    std::vector<Token> tokens = tokenizer.tokenize(); // NOLINT(*-include-cleaner)
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == Token(TokenType::IDENTIFIER, "a", 1, 1));
    REQUIRE(tokens[1] == Token(TokenType::IDENTIFIER, "a_", 1, 3));
    REQUIRE(tokens[2] == Token(TokenType::IDENTIFIER, "a0", 1, 6));
}

TEST_CASE("tokenizer emit integer token", "[tokenizer]"){
    Tokenizer tokenizer{"42 333 550 34000000"}; // NOLINT(*-include-cleaner)
    std::vector<Token> tokens = tokenizer.tokenize(); // NOLINT(*-include-cleaner)
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == Token(TokenType::INTEGER, "42", 1, 1));
    REQUIRE(tokens[1] == Token(TokenType::INTEGER, "333", 1, 4));
    REQUIRE(tokens[2] == Token(TokenType::INTEGER, "550", 1, 8));
    REQUIRE(tokens[3] == Token(TokenType::INTEGER, "34000000", 1, 12));
}


TEST_CASE("tokenizer emit double token", "[tokenizer]"){
    Tokenizer tokenizer{"1. 1.0 1e+1 1E+1 1.1e+1 1.1E+1 1e-1 1E-1 1.1e-1 1.1E-1"}; // NOLINT(*-include-cleaner)
    std::vector<Token> tokens = tokenizer.tokenize(); // NOLINT(*-include-cleaner)
    REQUIRE(tokens.size() == 11);
    REQUIRE(tokens[0] == Token(TokenType::DOUBLE, "1.", 1, 1));
    REQUIRE(tokens[1] == Token(TokenType::DOUBLE, "1.0", 1, 4));
    REQUIRE(tokens[2] == Token(TokenType::DOUBLE, "1e+1", 1, 8));
    REQUIRE(tokens[3] == Token(TokenType::DOUBLE, "1E+1", 1, 13));
    REQUIRE(tokens[4] == Token(TokenType::DOUBLE, "1.1e+1", 1, 18));
    REQUIRE(tokens[5] == Token(TokenType::DOUBLE, "1.1E+1", 1, 25));
    REQUIRE(tokens[6] == Token(TokenType::DOUBLE, "1e-1", 1, 32));
    REQUIRE(tokens[7] == Token(TokenType::DOUBLE, "1E-1", 1, 37));
    REQUIRE(tokens[8] == Token(TokenType::DOUBLE, "1.1e-1", 1, 42));
    REQUIRE(tokens[9] == Token(TokenType::DOUBLE, "1.1E-1", 1, 49));
}

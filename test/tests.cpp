#include <catch2/catch_test_macros.hpp>

#include <Vandior/vandior.hpp>

static inline constexpr std::size_t line = 5;
static inline constexpr std::size_t colum = 6;
static inline constexpr auto lfh = 1.0;
static inline constexpr auto lfh2 = lfh+0.5;
static inline constexpr auto rhs = 2.0;
static inline constexpr auto identf = vnd::TokenType::IDENTIFIER;
static inline constexpr auto inte = vnd::TokenType::INTEGER;
static inline constexpr auto doub = vnd::TokenType::DOUBLE;
static inline constexpr auto oper = vnd::TokenType::OPERATOR;
#define REQ_FORMAT(type, string)  REQUIRE(FORMAT("{}", type) == (string)); // NOLINT(*-macro-usage)
TEST_CASE("corrected format for Tokentype", "[token_type]") {
    using enum vnd::TokenType;
    REQ_FORMAT(INTEGER, "INTEGER");
    REQ_FORMAT(DOUBLE,"DOUBLE");
    REQ_FORMAT(BOOLEAN, "BOOLEAN")
    REQ_FORMAT(OPERATOR, "OPERATOR");
    REQ_FORMAT(MINUS_OPERATOR, "MINUS_OPERATOR");
    REQ_FORMAT(EQUAL_OPERATOR, "EQUAL_OPERATOR");
    REQ_FORMAT(DOT_OPERATOR, "DOT_OPERATOR");
    REQ_FORMAT(IDENTIFIER,"IDENTIFIER");
    REQ_FORMAT(CHAR, "CHAR");
    REQ_FORMAT(STRING, "STRING");
    REQ_FORMAT(K_MAIN,"K_MAIN");
    REQ_FORMAT(K_VAR, "K_VAR");
    REQ_FORMAT(K_STRUCTURE, "K_STRUCTURE");
    REQ_FORMAT(K_FOR, "K_FOR");
    REQ_FORMAT(K_FUN, "K_FUN");
    REQ_FORMAT(K_RETURN, "K_RETURN");
    REQ_FORMAT(OPEN_PARENTESIS, "OPEN_PARENTESIS");
    REQ_FORMAT(OPEN_SQ_PARENTESIS, "OPEN_SQ_PARENTESIS");
    REQ_FORMAT(OPEN_CUR_PARENTESIS, "OPEN_CUR_PARENTESIS");
    REQ_FORMAT(CLOSE_PARENTESIS, "CLOSE_PARENTESIS");
    REQ_FORMAT(CLOSE_SQ_PARENTESIS, "CLOSE_SQ_PARENTESIS");
    REQ_FORMAT(CLOSE_CUR_PARENTESIS, "CLOSE_CUR_PARENTESIS");
    REQ_FORMAT(OPERATION_EQUAL, "OPERATION_EQUAL");
    REQ_FORMAT(BOOLEAN_OPERATOR, "BOOLEAN_OPERATOR");
    REQ_FORMAT(NOT_OPERATOR, "NOT_OPERATOR");
    REQ_FORMAT(LOGICAL_OPERATOR, "LOGICAL_OPERATOR");
    REQ_FORMAT(UNARY_OPERATOR, "UNARY_OPERATOR");
    REQ_FORMAT(COMMA, "COMMA");
    REQ_FORMAT(COLON, "COLON");
    REQ_FORMAT(COMMENT, "COMMENT");
    REQ_FORMAT(EOFT,"EOF");
    REQ_FORMAT(UNKNOWN,"UNKNOWN");
}

TEST_CASE("default constructed token", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
}

TEST_CASE("default constructed token toString", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', line: 0, column: 0)");
}

TEST_CASE("default constructed token format", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(FORMAT("{}", token) == "(type: UNKNOWN, value: '', line: 0, column: 0)");
}

TEST_CASE("default constructed token set propriety", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    token.setType(vnd::TokenType::INTEGER);
    token.setValue("assss");
    token.setLine(1);
    token.setColumn(1);
    REQUIRE(token.getType() == vnd::TokenType::INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}

TEST_CASE("default constructed token set propriety tostring", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    token.setType(vnd::TokenType::INTEGER);
    token.setValue("assss");
    token.setLine(1);
    token.setColumn(1);
    REQUIRE(token.getType() == vnd::TokenType::INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
    REQUIRE(token.to_string() == "(type: INTEGER, value: 'assss', line: 1, column: 1)");
}

TEST_CASE("default constructed token set propriety format", "[token]"){
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    token.setType(vnd::TokenType::INTEGER);
    token.setValue("assss");
    token.setLine(1);
    token.setColumn(1);
    REQUIRE(token.getType() == vnd::TokenType::INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
    REQUIRE(FORMAT("{}", token) == "(type: INTEGER, value: 'assss', line: 1, column: 1)");
}

TEST_CASE("Token Comparison Equality", "[Token]") {
    vnd::Token token1(oper, "+", line,  colum);
    vnd::Token token2(oper, "+", line,  colum);
    REQUIRE(token1 == token2);
}

TEST_CASE("Token Comparison Inequality", "[Token]") {
    vnd::Token token1(identf, "variable", line,  colum);
    vnd::Token token2(identf, "variable2", line,  colum);
    REQUIRE(token1 != token2);
}

TEST_CASE("tokenizer emit identifier token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"a a_ a0 a000_"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", 1, 3));
    REQUIRE(tokens[2] == vnd::Token(identf, "a0", 1, 6));
    REQUIRE(tokens[3] == vnd::Token(identf, "a000_", 1, 9));
}

TEST_CASE("tokenizer emit identifier token new line", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"a a_\na0 a000_"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", 1, 3));
    REQUIRE(tokens[2] == vnd::Token(identf, "a0", 2, 1));
    REQUIRE(tokens[3] == vnd::Token(identf, "a000_", 2, 4));
}

TEST_CASE("tokenizer emit integer token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"42 333 550 34000000"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", 1, 4));
    REQUIRE(tokens[2] == vnd::Token(inte, "550", 1, 8));
    REQUIRE(tokens[3] == vnd::Token(inte, "34000000", 1, 12));
}

TEST_CASE("tokenizer emit integer token new line", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"42 333\n550 34000000"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", 1, 4));
    REQUIRE(tokens[2] == vnd::Token(inte, "550", 2, 1));
    REQUIRE(tokens[3] == vnd::Token(inte, "34000000", 2, 5));
}

TEST_CASE("tokenizer emit double token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"1. 1.0 1e+1 1E+1 1.1e+1 1.1E+1 1e-1 1E-1 1.1e-1 1.1E-1 .4e12"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 12);
    REQUIRE(tokens[0] == vnd::Token(doub, "1.", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(doub, "1.0", 1, 4));
    REQUIRE(tokens[2] == vnd::Token(doub, "1e+1", 1, 8));
    REQUIRE(tokens[3] == vnd::Token(doub, "1E+1", 1, 13));
    REQUIRE(tokens[4] == vnd::Token(doub, "1.1e+1", 1, 18));
    REQUIRE(tokens[5] == vnd::Token(doub, "1.1E+1", 1, 25));
    REQUIRE(tokens[6] == vnd::Token(doub, "1e-1", 1, 32));
    REQUIRE(tokens[7] == vnd::Token(doub, "1E-1", 1, 37));
    REQUIRE(tokens[8] == vnd::Token(doub, "1.1e-1", 1, 42));
    REQUIRE(tokens[9] == vnd::Token(doub, "1.1E-1", 1, 49));
    REQUIRE(tokens[10] == vnd::Token(doub, ".4e12", 1, 56));
}

TEST_CASE("tokenizer emit operator token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"* / = , : < > ! | & + - ^ ."};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 15);
    REQUIRE(tokens[0] == vnd::Token(oper, "*", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(oper, "/", 1, 3));
    REQUIRE(tokens[2] == vnd::Token(vnd::TokenType::EQUAL_OPERATOR, "=", 1, 5));
    REQUIRE(tokens[3] == vnd::Token(vnd::TokenType::COMMA, ",", 1, 6));
    REQUIRE(tokens[4] == vnd::Token(vnd::TokenType::COLON, ":", 1, 8));
    REQUIRE(tokens[5] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, "<", 1, 11));
    REQUIRE(tokens[6] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, ">", 1, 13));
    REQUIRE(tokens[7] == vnd::Token(vnd::TokenType::NOT_OPERATOR, "!", 1, 15));
    REQUIRE(tokens[8] == vnd::Token(oper, "|", 1, 17));
    REQUIRE(tokens[9] == vnd::Token(oper, "&", 1, 19));
    REQUIRE(tokens[10] == vnd::Token(oper, "+", 1, 21));
    REQUIRE(tokens[11] == vnd::Token(vnd::TokenType::MINUS_OPERATOR, "-", 1, 23));
    REQUIRE(tokens[12] == vnd::Token(oper, "^", 1, 25));
    REQUIRE(tokens[13] == vnd::Token(vnd::TokenType::DOT_OPERATOR, ".", 1, 27));
}

TEST_CASE("tokenizer emit operationEqual token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"+= -= *= /="};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPERATION_EQUAL, "+=", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::OPERATION_EQUAL, "-=", 1, 4));
    REQUIRE(tokens[2] == vnd::Token(vnd::TokenType::OPERATION_EQUAL, "*=", 1, 7));
    REQUIRE(tokens[3] == vnd::Token(vnd::TokenType::OPERATION_EQUAL, "/=", 1, 10));
}

TEST_CASE("tokenizer emit boolean operator token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"== >= <= !="};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, "==", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, ">=", 1, 4));
    REQUIRE(tokens[2] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, "<=", 1, 7));
    REQUIRE(tokens[3] == vnd::Token(vnd::TokenType::BOOLEAN_OPERATOR, "!=", 1, 10));
}

TEST_CASE("tokenizer emit logical operator token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"&& ||"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::LOGICAL_OPERATOR, "&&", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::LOGICAL_OPERATOR, "||", 1, 4));
}

TEST_CASE("tokenizer emit unary operator token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"++ --"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::UNARY_OPERATOR, "++", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::UNARY_OPERATOR, "--", 1, 4));
}

TEST_CASE("tokenizer emit parenthesis token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"( )"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_PARENTESIS, "(", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_PARENTESIS, ")", 1, 3));
}

TEST_CASE("tokenizer emit square parenthesis token", "[tokenizer]"){
    vnd::Tokenizer tokenizer{"[ ]"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_SQ_PARENTESIS, "[", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_SQ_PARENTESIS, "]", 1, 3));
}

TEST_CASE("vnd::Tokenizer emit square curly token", "[vnd::Tokenizer]"){
    vnd::Tokenizer tokenizer{"{ }"};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_CUR_PARENTESIS, "{", 1, 1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_CUR_PARENTESIS, "}", 1, 3));
}

TEST_CASE("vnd::Tokenizer emit char token", "[vnd::Tokenizer]"){
    constexpr std::string_view code2 = R"('a' '\\' '')";
    vnd::Tokenizer tokenizer{code2};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::CHAR, "a", 1, 2));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CHAR,  R"(\\)", 1, 6));
    REQUIRE(tokens[2] == vnd::Token(vnd::TokenType::CHAR, "", 1, 11));
}

TEST_CASE("vnd::Tokenizer emit string token", "[vnd::Tokenizer]"){
    constexpr std::string_view code2 = R"("a" "\\" "")";
    vnd::Tokenizer tokenizer{code2};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::STRING, "a", 1,1));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::STRING, R"(\\)", 1, 5));
    REQUIRE(tokens[2] == vnd::Token(vnd::TokenType::STRING, "", 1, 10));
}

TEST_CASE("tokenizer emit unknown token on non closed char token", "[tokenizer]"){
    constexpr std::string_view code2 = R"('a")";
    vnd::Tokenizer tokenizer{code2};
    std::vector<vnd::Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::UNKNOWN, R"(a")", 1, 2));
}
/*
TEST_CASE("tokenizer emit unknown token on non closed string token", "[tokenizer]"){
    constexpr std::string_view code2 = R"("a')";
    Tokenizer tokenizer{code2};
    std::vector<Token> tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == Token(TokenType::UNKNOWN, R"(a')", 1, 2));
}*/
TEST_CASE("corrected format for InstructionType", "[Instruction_type]") {
    using enum vnd::InstructionType;
    REQ_FORMAT(PARAMETER_EXPRESSION, "PARAMETER_EXPRESSION");
    REQ_FORMAT(OPERATION, "OPERATION");
    REQ_FORMAT(ASSIGNATION, "ASSIGNATION")
    REQ_FORMAT(EXPRESSION, "EXPRESSION");
    REQ_FORMAT(SQUARE_EXPRESSION, "SQUARE_EXPRESSION");
    REQ_FORMAT(RETURN_EXPRESSION, "RETURN_EXPRESSION");
    REQ_FORMAT(DECLARATION, "DECLARATION");
    REQ_FORMAT(INITIALIZATION, "INITIALIZATION");
    REQ_FORMAT(ARRAY_INIZIALIZATION, "ARRAY_INIZIALIZATION");
    REQ_FORMAT(MAIN, "MAIN");
    REQ_FORMAT(STRUCTURE, "STRUCTURE");
    REQ_FORMAT(FOR_STRUCTURE, "FOR_STRUCTURE");
    REQ_FORMAT(FOR_INITIALIZATION, "FOR_INITIALIZATION");
    REQ_FORMAT(FOR_CONDITION, "FOR_CONDITION");
    REQ_FORMAT(FOR_STEP, "FOR_STEP");
    REQ_FORMAT(DEFINITION, "DEFINITION");
    REQ_FORMAT(PARAMETER_DEFINITION, "PARAMETER_DEFINITION");
    REQ_FORMAT(RETURN_DEFINITION, "RETURN_DEFINITION");
    REQ_FORMAT(OPEN_SCOPE, "OPEN_SCOPE");
    REQ_FORMAT(CLOSE_SCOPE, "CLOSE_SCOPE");
    REQ_FORMAT(BLANK, "BLANK");
}

TEST_CASE("Parser emit number node", "[parser]"){
    vnd::Parser parser("1");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh);
}

TEST_CASE("Parser emit number node print", "[parser]"){
    vnd::Parser parser("1");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh);
    REQUIRE(number->print() == "NUMBER(1)");
}

TEST_CASE("Parser emit number node compat print", "[parser]"){
    vnd::Parser parser("1");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh);
    REQUIRE(number->comp_print() == "NUM(1)");
}

TEST_CASE("Parser emit variable node", "[parser]"){
    vnd::Parser parser("y");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto * variable = dynamic_cast<const VariableNode*>(ast.get());
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() ==  "y");
}

TEST_CASE("Parser emit number node double", "[parser]"){
    vnd::Parser parser("1.5");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh2);
}

TEST_CASE("Parser emit number node double print", "[parser]"){
    vnd::Parser parser("1.5");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh2);
    REQUIRE(number->print() == "NUMBER(1.5)");
}

TEST_CASE("Parser emit number node double compat print", "[parser]"){
    vnd::Parser parser("1.5");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto * number = dynamic_cast<const NumberNode*>(ast.get());
    REQUIRE(number != nullptr);
    REQUIRE(number->getValue()==lfh2);
    REQUIRE(number->comp_print() == "NUM(1.5)");
}


TEST_CASE("Parser emit variable node print", "[parser]"){
    vnd::Parser parser("y");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto * variable = dynamic_cast<const VariableNode*>(ast.get());
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() ==  "y");
    REQUIRE(variable->print()=="VARIABLE (y)");
}

TEST_CASE("Parser emit variable node compat print", "[parser]"){
    vnd::Parser parser("y");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto * variable = dynamic_cast<const VariableNode*>(ast.get());
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() ==  "y");
    REQUIRE(variable->comp_print()=="VAR(y)");
}

TEST_CASE("Parser emit unary expression node", "[parser]") {
    vnd::Parser parser("-x");
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto* unaryNode = dynamic_cast<const UnaryExpressionNode*>(ast.get());

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto& operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto* variableNode = dynamic_cast<const VariableNode*>(operand.get());
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
}

TEST_CASE("Parser emit unary expression node print", "[parser]") {
    vnd::Parser parser("-x");
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto* unaryNode = dynamic_cast<const UnaryExpressionNode*>(ast.get());

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto& operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto* variableNode = dynamic_cast<const VariableNode*>(operand.get());
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->print()== "UNARY_EXPRESION(op:\"-\" operand:VARIABLE (x))");
}

TEST_CASE("Parser emit unary expression node compat print", "[parser]") {
    vnd::Parser parser("-x");
    auto ast = parser.parse();

    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto* unaryNode = dynamic_cast<const UnaryExpressionNode*>(ast.get());

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto& operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto* variableNode = dynamic_cast<const VariableNode*>(operand.get());
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->comp_print()== "UNE(op:\"-\" opr:VAR(x))");
}



TEST_CASE("Parser emit binary expression node", "[parser]") {
    vnd::Parser parser("1 + 2");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto* binaryNode = dynamic_cast<const BinaryExpressionNode*>(ast.get());
    REQUIRE(binaryNode != nullptr);

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto * leftNumber = dynamic_cast<const NumberNode*>(binaryNode->getLeft().get());
    const auto * rightNumber = dynamic_cast<const NumberNode*>(binaryNode->getRight().get());

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);

    // Check the values of left and right operands
    REQUIRE(leftNumber->getValue() == lfh);
    REQUIRE(rightNumber->getValue() == rhs);
}


TEST_CASE("Parser emit binary expression node print", "[parser]") {
    vnd::Parser parser("1 + 2");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto* binaryNode = dynamic_cast<const BinaryExpressionNode*>(ast.get());

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto * leftNumber = dynamic_cast<const NumberNode*>(binaryNode->getLeft().get());
    const auto * rightNumber = dynamic_cast<const NumberNode*>(binaryNode->getRight().get());

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);

    // Check the values of left and right operands
    REQUIRE(leftNumber->getValue() == lfh);
    REQUIRE(rightNumber->getValue() == rhs);
    REQUIRE(binaryNode->print()== "BINARY_EXPRESION(op:\"+\" left:NUMBER(1), right:NUMBER(2))");
}

TEST_CASE("Parser emit binary expression node compact print", "[parser]") {
    vnd::Parser parser("1 + 2");
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto* binaryNode = dynamic_cast<const BinaryExpressionNode*>(ast.get());

    // Check the operation
    REQUIRE(binaryNode->getOp() == "+");

    // Check the left and right operands
    const auto * leftNumber = dynamic_cast<const NumberNode*>(binaryNode->getLeft().get());
    const auto * rightNumber = dynamic_cast<const NumberNode*>(binaryNode->getRight().get());

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);

    // Check the values of left and right operands
    REQUIRE(leftNumber->getValue() == lfh);
    REQUIRE(rightNumber->getValue() == rhs);
    REQUIRE(binaryNode->comp_print() == "BINE(op:\"+\" l:NUM(1), r:NUM(2))");
}
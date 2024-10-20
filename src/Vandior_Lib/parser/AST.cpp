/*
 * Created by gbian on 20/10/2024.
 */
// NOLINTBEGIN(*-include-cleaner, *-qualified-auto, *-no-recursion, *-easily-swappable-parameters)
#include "Vandior/parser/AST.hpp"

/** \cond */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26445 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)
void printParentNode(const vnd::ASTNode &node, const std::string &indentmark) {
    const auto *const nodeParent = node.get_parent();
    if(nodeParent != nullptr) {
        const auto nodeParentType = nodeParent->getType();
        const auto nodeParentTypeStr = comp_NodeType(nodeParentType);
        if(nodeParentType == NodeType::Index) {
            LINFO("{} Parent of Node ({})", indentmark, nodeParentTypeStr);
        } else {
            LINFO("{} Parent of Node ({}){}", indentmark, nodeParentTypeStr, nodeParent->get_token().compat_to_string());
        }
    } else {
        LINFO("{} Node ({}) has no parent", indentmark, comp_NodeType(node.getType()));
    }
}

// Helper functions for printing different node types
void printBinaryNode(const vnd::BinaryExpressionNode &node, const std::string &imarknode, const std::string &newindent) {
    LINFO(PRETTYPRINT_AST_FORMAT, imarknode, node.getOp());
    prettyPrint(*node.getLeft(), newindent, false, "L");
    prettyPrint(*node.getRight(), newindent, true, "R");
}

void printUnaryNode(const vnd::UnaryExpressionNode &node, const std::string &imarknode, const std::string &newindent) {
    LINFO(PRETTYPRINT_AST_FORMAT, imarknode, node.getOp());
    prettyPrint(*node.getOperand(), newindent, true, "OPR");
}

void printVariableNode(const vnd::VariableNode &node, const std::string &imarknode, const std::string &newindent) {
    LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, node.getName());

    if(node.is_call()) {
        if(const auto &callNode = node.get_call()) { prettyPrint(*callNode, newindent, true, "CALL"); }
    }
    if(const auto &indexNode = node.get_index()) { prettyPrint(*indexNode, newindent, true, "INDEX"); }
}
void printIndexNode(const vnd::IndexNode &node, const std::string &indentmark, const std::string &newindent) {
    LINFO("{}", indentmark, node.comp_print());
    if(const auto &elementsNode = node.get_elements()) { prettyPrint(*elementsNode, newindent, true, ""); }
    if(const auto &elementsIndexNode = node.get_index()) { prettyPrint(*elementsIndexNode, newindent, true, "INDEX"); }
    if(const auto &elementsArrayNode = node.get_array()) { prettyPrint(*elementsArrayNode, newindent, true, "ELEM"); }
}

void printArrayNode(const vnd::ArrayNode &node, const std::string &indentmark, const std::string &newindent) {
    LINFO("{} {}", indentmark, node.comp_print());
    if(node.get_elements()) { prettyPrint(*node.get_elements(), newindent, true, ""); }
}

// Main prettyPrint function refactored
void prettyPrint(const vnd::ASTNode &node, const std::string &indent, bool isLast, const std::string &lorf) {
    const auto &indentmark = FORMAT("{}{}{}", indent, isLast ? "+-" : "|-", lorf);
    const auto &newindent = FORMAT("{}{}", indent, isLast ? "  " : "| ");
    const auto &imarknode = FORMAT("{}({}", indentmark, comp_NodeType(node.getType()));

    // printParentNode(node, indentmark);
    // Determine the type of node and print information
    if(const auto *binaryNode = node.safe_as<vnd::BinaryExpressionNode>()) {
        printBinaryNode(*binaryNode, imarknode, newindent);
    } else if(const auto *unaryNode = node.safe_as<vnd::UnaryExpressionNode>()) {
        printUnaryNode(*unaryNode, imarknode, newindent);
    } else if(const auto *variableNode = node.safe_as<vnd::VariableNode>()) {
        printVariableNode(*variableNode, imarknode, newindent);
    } else if(const auto *intnumberNode = node.safe_as<VND_NUM_INT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(intnumberNode->getNumberType()), intnumberNode->get_value());
    } else if(const auto *cflnumberNode = node.safe_as<VND_NUM_CFLOAT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(cflnumberNode->getNumberType()), cflnumberNode->get_value());
    } else if(const auto *dflnumberNode = node.safe_as<VND_NUM_CDOUBLE>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(dflnumberNode->getNumberType()), dflnumberNode->get_value());
    } else if(const auto *flnumberNode = node.safe_as<VND_NUM_FLOAT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(flnumberNode->getNumberType()), flnumberNode->get_value());
    } else if(const auto *dblnumberNode = node.safe_as<VND_NUM_DOUBLE>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(dblnumberNode->getNumberType()), dblnumberNode->get_value());
    } else if(const auto *blliteralNode = node.safe_as<vnd::LiteralNode<bool>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, blliteralNode->get_value());
    } else if(const auto *chliteralNode = node.safe_as<vnd::LiteralNode<char>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, chliteralNode->get_value());
    } else if(const auto *svliteralNode = node.safe_as<vnd::LiteralNode<std::string_view>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, svliteralNode->get_value());
    } else if([[maybe_unused]] const auto *nullptrNode = node.safe_as<vnd::NullptrNode>()) {
        LINFO(PRETTYPRINT_AST_NULLPTR, imarknode);
    } else if(const auto *typeNode = node.safe_as<vnd::TypeNode>()) {
        LINFO("{}, {})", imarknode, typeNode->get_value());
        if(typeNode->get_index()) { prettyPrint(*typeNode->get_index(), newindent, true, "INDEX"); }
    } else if(const auto *indexNode = node.safe_as<vnd::IndexNode>()) {
        printIndexNode(*indexNode, indentmark, newindent);
    } else if(const auto *arrayNode = node.safe_as<vnd::ArrayNode>()) {
        printArrayNode(*arrayNode, indentmark, newindent);
    } else {
        LERROR("Unknown or not handled node type: {}", node.getType());
    }
}

/** \cond */
DISABLE_WARNINGS_POP()
/** \endcond */

// NOLINTEND(*-include-cleaner, *-qualified-auto, *-no-recursion, *-easily-swappable-parameters)

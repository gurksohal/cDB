//
// Created by G on 11/20/2024.
//

#ifndef CDB_PARSER_H
#define CDB_PARSER_H

#include <variant>

#include "../query/Constant.h"
#include "../query/Expression.h"
#include "../query/Predicate.h"
#include "../query/Term.h"
#include "CreateIndexData.h"
#include "CreateTableData.h"
#include "CreateViewData.h"
#include "DeletaData.h"
#include "InsertData.h"
#include "Lexer.h"
#include "ModifyData.h"

class Parser {
public:
    explicit Parser(std::string& s);
    auto field() -> std::string;
    auto constant() -> Constant;
    auto expression() -> Expression;
    auto term() -> Term;
    auto predicate() -> Predicate;
    auto query() -> QueryData;
    auto updateCmd()
        -> std::variant<InsertData, DeleteData, ModifyData, CreateTableData, CreateViewData, CreateIndexData>;
    auto remove() -> DeleteData;
    auto insert() -> InsertData;
    auto modify() -> ModifyData;
    auto createTable() -> CreateTableData;
    auto createView() -> CreateViewData;
    auto createIndex() -> CreateIndexData;

private:
    auto selectList() -> std::vector<std::string>;
    auto tableList() -> std::vector<std::string>;
    auto create() -> std::variant<InsertData, DeleteData, ModifyData, CreateTableData, CreateViewData, CreateIndexData>;
    auto fieldList() -> std::vector<std::string>;
    auto constList() -> std::vector<Constant>;
    auto fieldDefs() -> Schema;
    auto fieldDef() -> Schema;
    auto fieldType(std::string& fld_name) -> Schema;

    Lexer lex;
};

#endif  // CDB_PARSER_H

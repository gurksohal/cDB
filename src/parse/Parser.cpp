//
// Created by G on 11/20/2024.
//

#include "Parser.h"

Parser::Parser(std::string& s) : lex(s) {}

auto Parser::field() -> std::string {
    return lex.eatId();
}

auto Parser::constant() -> Constant {
    if (lex.matchStringConstant()) {
        return Constant(lex.eatStringConstant());
    }
    return Constant(lex.eatIntConstant());
}

auto Parser::expression() -> Expression {
    if (lex.matchId()) {
        return Expression(field());
    }
    return Expression(constant());
}

auto Parser::term() -> Term {
    Expression const lhs = expression();
    lex.eatDelim('=');
    Expression const rhs = expression();
    return {lhs, rhs};
}

auto Parser::predicate() -> Predicate {
    Predicate pred(term());
    while (lex.matchKeyword("and")) {
        lex.eatKeyword("and");
        Predicate next_pred(term());
        pred.conjoinWith(next_pred);
    }
    return pred;
}

auto Parser::query() -> QueryData {
    lex.eatKeyword("select");
    std::vector<std::string> const fields = selectList();
    lex.eatKeyword("from");
    std::vector<std::string> const tables = tableList();
    Predicate pred;
    if (lex.matchKeyword("where")) {
        lex.eatKeyword("where");
        pred = predicate();
    }
    return {fields, tables, pred};
}

auto Parser::updateCmd()
    -> std::variant<InsertData, DeleteData, ModifyData, CreateTableData, CreateViewData, CreateIndexData> {
    if (lex.matchKeyword("insert")) {
        return insert();
    }
    if (lex.matchKeyword("delete")) {
        return remove();
    }
    if (lex.matchKeyword("update")) {
        return modify();
    }
    return create();
}

auto Parser::remove() -> DeleteData {
    lex.eatKeyword("delete");
    lex.eatKeyword("from");
    std::string tbl_name = lex.eatId();
    Predicate pred;
    if (lex.matchKeyword("where")) {
        lex.eatKeyword("where");
        pred = predicate();
    }
    return {tbl_name, pred};
}

auto Parser::insert() -> InsertData {
    lex.eatKeyword("insert");
    lex.eatKeyword("into");
    std::string const tbl_name = lex.eatId();
    lex.eatDelim('(');
    std::vector<std::string> const fields = fieldList();
    lex.eatDelim(')');
    lex.eatKeyword("values");
    lex.eatDelim('(');
    std::vector<Constant> const values = constList();
    lex.eatDelim(')');
    return {tbl_name, fields, values};
}

auto Parser::modify() -> ModifyData {
    lex.eatKeyword("update");
    std::string const tbl_name = lex.eatId();
    lex.eatKeyword("set");
    std::string const fld_name = field();
    lex.eatDelim('=');
    Expression const new_val = expression();
    Predicate pred;
    if (lex.matchKeyword("where")) {
        lex.eatKeyword("where");
        pred = predicate();
    }
    return {tbl_name, fld_name, new_val, pred};
}

auto Parser::createTable() -> CreateTableData {
    lex.eatKeyword("table");
    std::string const tbl_name = lex.eatId();
    lex.eatDelim('(');
    Schema const schema = fieldDefs();
    lex.eatDelim(')');
    return {tbl_name, schema};
}

auto Parser::createView() -> CreateViewData {
    lex.eatKeyword("view");
    std::string const view_name = lex.eatId();
    lex.eatKeyword("as");
    QueryData const qd = query();
    return {view_name, qd};
}

auto Parser::createIndex() -> CreateIndexData {
    lex.eatKeyword("index");
    std::string const idx_name = lex.eatId();
    lex.eatKeyword("on");
    std::string const tbl_name = lex.eatId();
    lex.eatDelim('(');
    std::string const fld_name = field();
    lex.eatDelim(')');
    return {idx_name, tbl_name, fld_name};
}

auto Parser::selectList() -> std::vector<std::string> {
    std::vector<std::string> fields;
    fields.push_back(field());
    while (lex.matchDelim(',')) {
        lex.eatDelim(',');
        fields.push_back(field());
    }
    return fields;
}

auto Parser::tableList() -> std::vector<std::string> {
    std::vector<std::string> tables;
    tables.push_back(lex.eatId());
    while (lex.matchDelim(',')) {
        lex.eatDelim(',');
        tables.push_back(lex.eatId());
    }
    return tables;
}

auto Parser::create()
    -> std::variant<InsertData, DeleteData, ModifyData, CreateTableData, CreateViewData, CreateIndexData> {
    lex.eatKeyword("create");
    if (lex.matchKeyword("table")) {
        return createTable();
    }
    if (lex.matchKeyword("view")) {
        return createView();
    }
    return createIndex();
}

auto Parser::fieldList() -> std::vector<std::string> {
    std::vector<std::string> fields;
    fields.push_back(field());
    while (lex.matchDelim(',')) {
        lex.eatDelim(',');
        fields.push_back(field());
    }
    return fields;
}

auto Parser::constList() -> std::vector<Constant> {
    std::vector<Constant> values;
    values.push_back(constant());
    while (lex.matchDelim(',')) {
        lex.eatDelim(',');
        values.push_back(constant());
    }
    return values;
}

auto Parser::fieldDefs() -> Schema {
    Schema schema = fieldDef();
    while (lex.matchDelim(',')) {
        lex.eatDelim(',');
        Schema schema2 = fieldDef();
        schema.addAll(schema2);
    }
    return schema;
}

auto Parser::fieldDef() -> Schema {
    std::string fld_name = field();
    return fieldType(fld_name);
}

auto Parser::fieldType(std::string& fld_name) -> Schema {
    Schema schema;
    if (lex.matchKeyword("int")) {
        lex.eatKeyword("int");
        schema.addIntField(fld_name);
    } else {
        lex.eatKeyword("varchar");
        lex.eatDelim('(');
        int const str_len = lex.eatIntConstant();
        lex.eatDelim(')');
        schema.addStringField(fld_name, str_len);
    }
    return schema;
}

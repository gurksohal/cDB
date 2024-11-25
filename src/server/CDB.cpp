//
// Created by G on 11/24/2024.
//

#include "CDB.h"

#include <iostream>

#include "../plan/BasicQueryPlanner.h"
#include "../plan/BasicUpdatePlanner.h"

int const CDB::BUFFER_SIZE = 8;
int const CDB::BLOCK_SIZE = 400;
const char* CDB::LOG_FILE = "simpledb.log";

CDB::CDB(std::filesystem::path& dir_name, int block_size, int buff_size)
    : fm(std::make_unique<FileManager>(dir_name, block_size)),
      lm(std::make_unique<LogManager>(fm.get(), LOG_FILE)),
      bm(std::make_unique<BufferManager>(fm.get(), lm.get(), buff_size)) {}

CDB::CDB(std::filesystem::path& dir_name) : CDB(dir_name, BLOCK_SIZE, BUFFER_SIZE) {
    auto tx = nextTx();
    auto is_new = fm->isNew();
    if (is_new) {
        std::cout << "Create new database\n";
    } else {
        std::cout << "recovering existing database\n";
        tx.recover();
    }

    mdm = std::make_unique<MetadataMgr>(is_new, tx);
    std::shared_ptr<QueryPlanner> const qp = std::make_shared<BasicQueryPlanner>(mdm.get());
    std::shared_ptr<UpdatePlanner> const up = std::make_shared<BasicUpdatePlanner>(mdm.get());
    planner_m = std::make_unique<Planner>(qp, up);
    tx.commit();
    curr_tx = std::make_unique<Transaction>(nextTx());
}

auto CDB::nextTx() -> Transaction {
    return {fm.get(), lm.get(), bm.get()};
}

auto CDB::mdMgr() -> MetadataMgr& {
    return *mdm;
}

auto CDB::planner() -> Planner& {
    return *planner_m;
}

auto CDB::fileMgr() -> FileManager& {
    return *fm;
}

auto CDB::logMgr() -> LogManager& {
    return *lm;
}

auto CDB::bufferMgr() -> BufferManager& {
    return *bm;
}

auto CDB::execQuery(std::string& sql) -> void {
    try {
        auto pln = planner_m->createQueryPlan(sql, *curr_tx);
        printRows(pln);
    } catch (std::exception e) {  // NOLINT
        rollback();
        throw e;
    }
}

auto CDB::execUpdate(std::string& sql) -> int {
    try {
        auto res = planner_m->executeUpdate(sql, *curr_tx);
        commit();
        return res;
    } catch (std::exception e) {  // NOLINT
        rollback();
        throw e;
    }
}

auto CDB::rollback() -> void {
    curr_tx->rollback();
    curr_tx = std::make_unique<Transaction>(nextTx());
}

auto CDB::commit() -> void {
    curr_tx->commit();
    curr_tx = std::make_unique<Transaction>(nextTx());
}

auto CDB::printRows(const std::shared_ptr<Plan>& p) -> void {
    auto s = p->open();
    auto sch = p->schema();
    int const cols = static_cast<int>(sch.fields().size());
    std::string output;
    while (s->next()) {
        std::ostringstream oss;
        for (int i = 0; i < cols; i++) {
            auto name = sch.fields()[i];
            oss << s->getVal(name) << " | ";
        }
        std::string const t = oss.str();
        output += t.substr(0, t.size() - 3) + "\n";
    }

    std::cout << output;
    s->close();
    commit();
}

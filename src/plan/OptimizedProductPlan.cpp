//
// Created by G on 11/21/2024.
//

#include "OptimizedProductPlan.h"

#include "ProductPlan.h"

OptimizedProductPlan::OptimizedProductPlan(const std::shared_ptr<Plan>& p1, const std::shared_ptr<Plan>& p2) {
    std::shared_ptr<Plan> const prod1 = std::make_shared<ProductPlan>(p1, p2);
    std::shared_ptr<Plan> const prod2 = std::make_shared<ProductPlan>(p1, p2);
    int const b1 = prod1->blocksAccessed();
    int const b2 = prod2->blocksAccessed();
    best_plan = (b1 < b2) ? prod1 : prod2;
}

auto OptimizedProductPlan::open() -> std::shared_ptr<Scan> {
    return best_plan->open();
}

auto OptimizedProductPlan::blocksAccessed() -> int {
    return best_plan->blocksAccessed();
}

auto OptimizedProductPlan::recordsOutput() -> int {
    return best_plan->recordsOutput();
}

auto OptimizedProductPlan::distinctValues() -> int {
    return best_plan->distinctValues();
}

auto OptimizedProductPlan::schema() -> Schema {
    return best_plan->schema();
}

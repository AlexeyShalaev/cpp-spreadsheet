#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>

namespace {
    class Formula : public FormulaInterface {
        FormulaAST ast_;
    public:
// Реализуйте следующие методы:
        Formula(const std::string &expression) : ast_(ParseFormulaAST(expression)) {
        }

        [[nodiscard]] Value Evaluate(const SheetInterface &sheet) const override {
            try {
                auto ans = ast_.Execute(sheet);
                return ans;
            }
            catch (FormulaError &err) {
                return err;
            }
        }

        [[nodiscard]] std::string GetExpression() const override {
            std::stringstream str;
            ast_.PrintFormula(str);
            return str.str();
        }

        [[nodiscard]] std::vector<Position> GetReferencedCells() const override {
            std::vector<Position> ans;
            for (auto &&item: ast_.GetCells())
                ans.emplace_back(item);
            std::sort(ans.begin(), ans.end());
            ans.erase(std::unique(ans.begin(), ans.end()), ans.end());
            return ans;
        }

    };
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(const std::string &expression) {
    return std::make_unique<Formula>(expression);
}

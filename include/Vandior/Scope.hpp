 #pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace vnd {

    class Scope {
    public:
        static std::shared_ptr<Scope> create() noexcept;
        /*static std::shared_ptr<Scope> create(std::shared_ptr<Scope> _parent) noexcept;
        std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
        void checkVariable(const std::string &variable);
        bool checkType(const std::string &type) const;*/
    private:
        Scope() noexcept;
        //Scope(std::shared_ptr<Scope> _parent) noexcept;
        std::unordered_map<std::string, std::string> _identifiers;
        std::unordered_set<std::string> _types;
        std::shared_ptr<Scope> _parent;
    };

}  // namespace vnd*/
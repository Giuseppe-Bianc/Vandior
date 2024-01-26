 #pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace vnd {

    class Scope {
    public:
        static std::shared_ptr<Scope> create(std::shared_ptr<Scope> parent) noexcept;
        static std::shared_ptr<Scope> createMain() noexcept;
        std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
        void addType(std::string_view type) noexcept;
        //void checkVariable(const std::string &variable);
        //bool checkType(const std::string &type) const;
    private:
        Scope(std::shared_ptr<Scope> parent) noexcept;
        std::unordered_map<std::string_view, std::string_view> _identifiers;
        std::unordered_set<std::string_view> _types;
        std::shared_ptr<Scope> _parent;
    };

}  // namespace vnd
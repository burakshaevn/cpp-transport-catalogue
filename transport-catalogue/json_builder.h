#pragma once

#include "json.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace json {

    class Builder {
    private:
        class BaseContext;
        class DictValueContext;
        class DictItemContext;
        class ArrayItemContext;

    public:
        Builder();
        Node Build();
        DictValueContext Key(std::string key);
        BaseContext Value(Node::Value value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        BaseContext EndDict();
        BaseContext EndArray();

    private:
        Node root_{ nullptr };
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> key_{ std::nullopt };

        Node::Value& GetCurrentValue();
        const Node::Value& GetCurrentValue() const;

        void AssertNewObjectContext() const;
        void AddObject(Node::Value value, bool one_shot);

        class BaseContext {
        public:
            BaseContext(Builder& builder) : builder_(builder) {}
            Node Build() { return builder_.Build(); }
            DictValueContext Key(std::string key) { return builder_.Key(std::move(key)); }
            BaseContext Value(Node::Value value) { return builder_.Value(std::move(value)); }
            DictItemContext StartDict() { return builder_.StartDict(); }
            ArrayItemContext StartArray() { return builder_.StartArray(); }
            BaseContext EndDict() { return builder_.EndDict(); }
            BaseContext EndArray() { return builder_.EndArray(); }
        private:
            Builder& builder_;
        };

        class DictValueContext : public BaseContext {
        public:
            using BaseContext::BaseContext;
            DictItemContext Value(Node::Value value) { return  Value(std::move(value)); }
        };

        class DictItemContext : public BaseContext {
        public:
            using BaseContext::BaseContext;
        };

        class ArrayItemContext : public BaseContext {
        public:
            using BaseContext::BaseContext;
            ArrayItemContext Value(Node::Value value) { return Value(std::move(value)); }
        };
    };

}  // namespace json

#pragma once
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "LazySequence.hpp"
 
template<typename T>
using Deserializer = std::function<T(const std::string&)>;
 
template<typename T>
using Serializer = std::function<std::string(const T&)>;

template <typename T>
class ReadOnlyStream{
private:
    enum class Source { Sequence, Lazy, String, File };
 
    Sequence<T>*     seq_;
    LazySequence<T>* lazy_;
    ArraySequence<T> buffer_;
    size_t           pos_;
    Source           source_;
    std::string      path_;
    std::ifstream    file_;
    Deserializer<T>  deserializer_;
 
    void parse_string(const std::string& data) {
        std::string token;
        for (char c : data) {
            if (c == ' ' || c == '\n' || c == ',') {
                if (!token.empty()) {
                    buffer_.append(deserializer_(token));
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty())
            buffer_.append(deserializer_(token));
    }
 
    void load_from_file() {
        std::string line;
        while (std::getline(file_, line))
            if (!line.empty())
                buffer_.append(deserializer_(line));
    }

    public:

    ReadOnlyStream(Sequence<T>* seq)
        : seq_(seq), lazy_(nullptr), pos_(0), source_(Source::Sequence) {}
 
    ReadOnlyStream(LazySequence<T>* lazy)
        : seq_(nullptr), lazy_(lazy), pos_(0), source_(Source::Lazy) {}
 
    ReadOnlyStream(const std::string& data, Deserializer<T> deserializer)
        : seq_(nullptr), lazy_(nullptr), pos_(0),
          source_(Source::String), deserializer_(deserializer) {
        parse_string(data);
    }

    ReadOnlyStream(const std::string& path, Deserializer<T> deserializer, bool is_file)
        : seq_(nullptr), lazy_(nullptr), pos_(0),
          source_(Source::File), path_(path), deserializer_(deserializer) {}
 
    void open(){
        if (source_ == Source::File) {
            file_.open(path_);
            if (!file_.is_open())
                throw InvalidFilePath(path_);
            load_from_file();
            file_.close();
        }
    }

    void close() {
        if (file_.is_open())
            file_.close();
    }


    bool is_end_of_stream() const {
        switch (source_) {
            case Source::Sequence: return pos_ >= seq_->getLength();
            case Source::Lazy: return !lazy_->is_infinite() && pos_ >= lazy_->get_materialized_count();
            case Source::String: 
            case Source::File: return pos_ >= buffer_.getLength();
        }
        return true;
    }

    T read(){
        if (is_end_of_stream())
            throw
        switch (source_)
        {
        case Source::Sequence : return seq_->get(pos_++);
        case Source::Lazy : return  seq_->get(pos_++);
        case Source::String : return 
        case Source::File : return seq_->get(pos_++);
        }
    }

    ReadOnlyStream<T>& operator>>(T& value) {
        value = read();
        return *this;
    }

    size_t get_position() const {return pos_; }
};
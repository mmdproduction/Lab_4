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
    std::ifstream    file_; //TODO _FILE_
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
            throw EndOfStream();
        switch (source_)
        {
        case Source::Sequence : return seq_->get(pos_++);
        case Source::Lazy : return  seq_->get(pos_++);
        case Source::String :
        case Source::File : return seq_->get(pos_++);
        }
        throw EndOfStream();
    }

    ReadOnlyStream<T>& operator>>(T& value) {
        value = read();
        return *this;
    }

    size_t get_position() const {return pos_; }

};


template <typename T>
class WriteOnlyStream{
    private:

    enum class Sink {Sequence, File, Buffer};

    Sequence<T>*     seq_;
    std::string      buffer_;
    size_t           pos_;
    Sink             sink_;
    std::string      path_;
    std::ofstream    file_;
    Serializer<T>    serializer_;


    public:

     WriteOnlyStream(Sequence<T>* seq)
        : seq_(seq), pos_(0), sink_(Sink::Sequence) {}
 
    
    WriteOnlyStream(const std::string& path, Serializer<T> serializer)
        : seq_(nullptr), pos_(0), sink_(Sink::File),
          path_(path), serializer_(serializer) {}
 
    WriteOnlyStream(Serializer<T> serializer)
        : seq_(nullptr), pos_(0), sink_(Sink::Buffer),
          serializer_(serializer) {}

    void open() {
        if (sink_ == Sink::File) {
            file_.open(path_);
            if (!file_.is_open())
                throw InvalidFilePath(path_);
        }
    }
 
    void close() {
        if (file_.is_open())
            file_.close();
    }

    size_t write(const T& item) {
        switch (sink_) {
            case Sink::Sequence:
                seq_->append(item);
                break;
            case Sink::File:
                if (!file_.is_open())
                    throw InvalidFilePath(path_);
                file_ << serializer_(item) << "\n";
                break;
            case Sink::Buffer:
                buffer_ += serializer_(item) + "\n";
                break;
        }
        return ++pos_;
    }

    WriteOnlyStream<T>& operator<<(const T& item) {
        write(item);
        return *this;
    }

    size_t get_position() const {return pos_; }

    const std::string& get_buffer() const {
        if (sink_ != Sink::Buffer)
            throw std::runtime_error("WriteOnlyStream: not a buffer stream");
        return buffer_;
    }

};

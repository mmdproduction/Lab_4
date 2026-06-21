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
class IReadOnlyStream {
public:
    virtual ~IReadOnlyStream() = default;
    virtual T read() = 0;
    virtual bool is_end_of_stream() const = 0;
    virtual size_t get_position() const = 0;
};


template <typename T>
class IWriteOnlyStream {
public:
    virtual ~IWriteOnlyStream() = default;
    virtual void write(const T& item) = 0;
    virtual size_t get_position() const = 0;
    virtual void close() = 0;
};


template <typename T>
class SequenceReadOnlyStream : public IReadOnlyStream<T> {
    const Sequence<T>* seq_;
    size_t pos_;
public:
    explicit SequenceReadOnlyStream(const Sequence<T>* seq) : seq_(seq), pos_(0) {}

    T read() override {
        if (is_end_of_stream()) throw EndOfStream();
        return seq_->get(pos_++);
    }

    bool is_end_of_stream() const override {
        return pos_ >= seq_->getLength();
    }

    size_t get_position() const override { return pos_; }
};

template <typename T>
class LazyReadOnlyStream : public IReadOnlyStream<T> {
    LazySequence<T>* lazy_;
    size_t pos_;
public:
    explicit LazyReadOnlyStream(LazySequence<T>* lazy) : lazy_(lazy), pos_(0) {}

    T read() override {
        if (is_end_of_stream()) throw EndOfStream();
        return lazy_->get(pos_++); 
    }

    bool is_end_of_stream() const override {
        return !lazy_->is_infinite() && pos_ >= lazy_->get_materialized_count();
    }

    size_t get_position() const override { return pos_; }
};

template <typename T>
class BufferReadOnlyStream : public IReadableStream<T> {
    ArraySequence<T> buffer_;
    size_t pos_;
public:

    BufferReadOnlyStream(const std::string& data, Deserializer<T> deserializer) : pos_(0) {
        parse_string(data, deserializer);
    }

    private:
    void parse_string(const std::string& data, Deserializer<T> deserializer) {
        std::string token;
        for (char c : data) {
            if (c == ' ' || c == '\n' || c == ',') {
                if (!token.empty()) {
                    buffer_.append(deserializer(token));
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty())
            buffer_.append(deserializer(token));
    }

    public:
    T read() override {
        if (is_end_of_stream()) throw EndOfStream();
        return buffer_.get(pos_++);
    }

    bool is_end_of_stream() const override {
        return pos_ >= buffer_.getLength();
    }

    size_t get_position() const override { return pos_; }
};

template <typename T>
class FileReadonlyStream: public IReadOnlyStream{
    ArraySequence<T> buffer_;
    size_t pos_;
    Deserializer<T> deserializer_;

    void load_data(const std::string& path) {
        FILE* file_ = fopen(path.c_str(), "r");
        if (!file_) {
            throw InvalidFilePath(path);
        }

        char line[4096];
        while (fgets(line, sizeof(line), file_)) {
            std::string strLine(line);

            if (!strLine.empty() && strLine.back() == '\n') {
                strLine.pop_back();
            }
            if (!strLine.empty()) {
                buffer_.append(deserializer_(strLine));
            }
        }
        fclose(file_);
        file_ = nullptr;
    }
public:
    FileReadOnlyStream(const std::string& path, Deserializer<T> deserializer) : pos_(0), deserializer_(deserializer) {
        load_data(path);
    }

    private:

    public:
    T read() override {
        if (is_end_of_stream()) throw EndOfStream();
        return buffer_.get(pos_++);
    }

    bool is_end_of_stream() const override {
        return pos_ >= buffer_.getLength();
    }

    size_t get_position() const override { return pos_; }

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

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
    IReadOnlyStream<T>& operator>>(T& item) {
        item = read();
        return *this;
    }
};


template <typename T>
class IWriteOnlyStream {
public:
    virtual ~IWriteOnlyStream() = default;
    virtual void write(const T& item) = 0;
    virtual size_t get_position() const = 0;
    virtual void close() = 0;
    IWriteOnlyStream<T>& operator<<(const T& item) {
        write(item);
        return *this;
    }
};


template <typename T>
class SequenceReadOnlyStream : public IReadOnlyStream<T> {
    const Sequence<T>* seq_;
    size_t pos_;
public:
    SequenceReadOnlyStream(const Sequence<T>* seq) : seq_(seq), pos_(0) {}

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
    LazyReadOnlyStream(LazySequence<T>* lazy) : lazy_(lazy), pos_(0) {}

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
class BufferReadOnlyStream : public IReadOnlyStream<T> {
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
class FileReadOnlyStream: public IReadOnlyStream<T> {
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
class FileWriteOnlyStream : public IWriteOnlyStream<T> {
private:
    FILE* file_;
    Serializer<T> serializer_;
    size_t pos_;
    std::string path_;

public:
    FileWriteOnlyStream(const std::string& path, Serializer<T> serializer) 
        : file_(nullptr), serializer_(serializer), pos_(0), path_(path) 
    {
        open();
    }

    ~FileWriteOnlyStream() override {
        close();
    }

    void open() {
        if (file_) return; // Уже открыт
        
        file_ = fopen(path_.c_str(), "w");
        if (!file_) {
            throw InvalidFilePath(path_);
        }
    }

    void write(const T& item) override {
        if (!file_) {
            throw std::runtime_error("File stream is not open");
        }
        
        std::string serialized = serializer_(item);
        fprintf(file_, "%s\n", serialized.c_str());
        ++pos_;
    }

    size_t get_position() const override { return pos_; }

    void close() override {
        if (file_) {
            fclose(file_);
            file_ = nullptr;
        }
    }
};

template <typename T>
class SequenceWriteOnlyStream : public IWriteOnlyStream<T> {
    Sequence<T>* seq_;
    size_t pos_;
public:
    SequenceWriteOnlyStream(Sequence<T>* seq) : seq_(seq), pos_(0) {}

    void write(const T& item) override {
        seq_->append(item);
        ++pos_;
    }

    size_t get_position() const override { return pos_; }
    void close() override {}
};

template <typename T>
class BufferWriteOnlyStream : public IWriteOnlyStream<T> {
    std::string buffer_;
    Serializer<T> serializer_;
    size_t pos_;
public:
    BufferWriteOnlyStream(Serializer<T> serializer) 
        : serializer_(serializer), pos_(0) {}

    void write(const T& item) override {
        buffer_ += serializer_(item) + "\n";
        ++pos_;
    }

    size_t get_position() const override { return pos_; }
    void close() override {}

    const std::string& get_buffer() const { return buffer_; }
};

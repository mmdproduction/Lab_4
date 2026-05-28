#pragma once
#include<exception>
#include<string>
#include<format>
#include <source_location>


class Exception : public std::exception {
    protected:
    std::string message;
    public:
    explicit Exception(const char* func_name,  const char* file_name, uint32_t line, const std::string& msg = "Неизвестная ошибка"): message(std::format("{} Функция: {} Файл: {}:{}", msg, func_name, file_name, line )){}
    const char* what() const noexcept override {
        return message.c_str();
    }
};


class IndexOutOfRange : public Exception{
    public:
    IndexOutOfRange(int index, int size, const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), 
        std::format("Выход за границы! Индекс: {}, Размер: {}", index, size)){}

        explicit IndexOutOfRange(int index,  const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), std::format("Выход за границы! Индекс: {}",
        index)){}

        explicit IndexOutOfRange( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Неверный индекс!"){}
};

class InvalidPointer : public Exception{
    public:
    explicit InvalidPointer( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Ошибка: указатель не действителен!"){}
};

class ReadOnlyError : public Exception{
    public:
    explicit ReadOnlyError( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Данные только для чтения!"){}
};

class InvalidEnumerator : public Exception{
    public:
    explicit InvalidEnumerator( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Недопустимый элемент перечисления!"){}
};

class InvalidArgument : public Exception{
    public:
    explicit InvalidArgument( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Недопустимый аргумент!"){}
};

class EmptySequence : public Exception{
    public:
    explicit EmptySequence( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Пустая последовательность!"){}
};

class OptionalHasNotValue : public Exception{
    public:
    explicit OptionalHasNotValue( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Нет значения у объекта типа Optional!"){}
};

class InvalidNextValue : public Exception{
    public:
    explicit InvalidNextValue( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Недопустимое обращение к слудующему элементу!"){}
};

class InvalidFilePath : public Exception{
    public:
    InvalidFilePath(const std::string path, const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), 
        std::format("Невозможно открыть файл! Путь: {}!", path)){}
};

class EndOfStream : public Exception{
    public:
    explicit EndOfStream( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Конец потока чтения!"){}
};

class EmptyStream : public Exception{
    public:
    explicit EmptyStream( const std::source_location& loc = std::source_location::current())
        : Exception(loc.function_name(), loc.file_name(), loc.line(), "Пустой поток!"){}
};
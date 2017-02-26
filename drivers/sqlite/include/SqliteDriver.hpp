#ifndef SQLITEDRIVER_HPP
#define SQLITEDRIVER_HPP

#include <string>

struct sqlite3;
struct sqlite3_stmt;

class SqliteDriver
{
public:
    SqliteDriver();

    void open(const std::string& databaseFileName);

    bool isOpen();

    void close();

    bool isClosed();

    void prepare(const std::string& sqlStatement);

    void execute();

    bool nextRow();

    bool isNull(int columnIndex);

    int getInt(int columnIndex);

    int64_t getInt64(int columnIndex);

    double getDouble(int columnIndex);

    const unsigned char* getStringData(int columnIndex);

    const char* getRawString(int columnIndex);

    std::string getStdString(int columnIndex);

    std::size_t getSize(int columnIndex);

    const void* getBlob(int columnIndex);

    void bindNull(int columnIndex);

    void bindInt(int columnIndex, int value);

    void bindInt64(int columnIndex, int64_t value);

    void bindDouble(int columnIndex, double value);

    void bindRawString(int columnIndex, const char* str);

    void bindStdString(int columnIndex, const std::string& str);

    void bindBlob(int columnIndex, void* blob, std::size_t size);

    void finalize();

    ~SqliteDriver();

private:
    sqlite3* mHandle;
    sqlite3_stmt* mStatement;
};

#endif // SQLITEDRIVER_HPP

#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <string>

class DBConnector {
public:
    virtual ~DBConnector() {};
    virtual bool connect(const std::string& dbPath) = 0;
    virtual void disconnect() = 0;
    virtual void initializeSchema() = 0;
    // virtual void saveRoom();
};

#endif // DBCONNECTOR_H

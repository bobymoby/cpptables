#pragma once

#include "TableEntry.h"

class ErrorEntry : public TableEntry
{
private:
    std::string errorMsg;
public:
    ErrorEntry(const std::string& inputValue, const std::string& errorMsg);

    const std::string& getErrorMsg() const;

    EntryType getType() const override;
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    TableEntry* clone() const override;
};
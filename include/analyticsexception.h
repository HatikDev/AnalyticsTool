#ifndef ANALYTICS_EXCEPTION_H
#define ANALYTICS_EXCEPTION_H

#include <stdexcept>
#include <string>

class AnalyticsException : public std::exception {
public:
    AnalyticsException(const char* mesg);
    AnalyticsException(const std::string& mesg);
    virtual const char* what() const noexcept;

private:
    std::string m_message;
};

#endif // ANALYTICS_EXCEPTION_H
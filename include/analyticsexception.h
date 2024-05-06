#ifndef ANALYTICS_EXCEPTION_H
#define ANALYTICS_EXCEPTION_H

#include <stdexcept>
#include <string>

class AnalyticsException : public std::exception {
public:
    AnalyticsException(const char* mesg);
    const char* what() const;

private:
    std::string m_message;
};

#endif // ANALYTICS_EXCEPTION_H
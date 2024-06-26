#include "analyticsexception.h"

AnalyticsException::AnalyticsException(const char* mesg)
    : m_message{ mesg }
{
}

AnalyticsException::AnalyticsException(const std::string& mesg)
    : m_message{ mesg }
{
}

const char* AnalyticsException::what() const noexcept
{
    return m_message.c_str();
}

#include "analyticsexception.h"
#include "dataset.h"

#include <filesystem>

namespace fs = std::filesystem;

Dataset::Dataset()
{}

Dataset::Dataset(const std::string& path)
    : m_path{ path }
{
    load(path);
}

Dataset::Dataset(const Dataset& other)
    : m_path{ other.m_path }, m_currentIndex{ other.m_currentIndex }, m_files{ other.m_files }
{}

Dataset::Dataset(Dataset&& other)
    : m_path{ std::move(other.m_path) }, m_currentIndex{ other.m_currentIndex }, m_files{ std::move(other.m_files) }
{}

Dataset& Dataset::operator=(Dataset other)
{
    swap(other);

    return *this;
}

Dataset::~Dataset()
{}

std::string Dataset::path() const
{
    return m_path;
}

Picture Dataset::current() const
{
    std::string imagePath = m_path + "/images/" + m_files[m_currentIndex];
    std::string labelsPath = m_path + "/images_labels/" + m_files[m_currentIndex] + ".txt"; // TODO: think about file names

    return Picture(imagePath, labelsPath, QSize(360, 360)); // TODO: change size
}

std::string Dataset::currentName() const
{
    return m_files[m_currentIndex];
}

Picture Dataset::next()
{
    if (m_currentIndex + 1 >= m_files.size())
        throw AnalyticsException("Too big index");

    ++m_currentIndex;

    return current();
}

Picture Dataset::previous()
{
    if (m_currentIndex == 0)
        throw AnalyticsException("Too small index");

    --m_currentIndex;

    return current();
}

size_t Dataset::currentIndex() const
{
    return m_currentIndex;
}

size_t Dataset::count() const
{
    return m_files.size();
}

void Dataset::swap(Dataset& dataset)
{
    std::swap(m_path, dataset.m_path);
    std::swap(m_currentIndex, dataset.m_currentIndex);
    std::swap(m_files, dataset.m_files);
}

void Dataset::load(const std::string& path)
{
    for (const auto& entry : fs::directory_iterator(path + "/images/"))
        m_files.push_back(entry.path().stem().string());
}

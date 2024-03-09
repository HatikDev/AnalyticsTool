#include "analyticsexception.h"
#include "dataset.h"

#include <fstream>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

Dataset::Dataset()
{}

Dataset::Dataset(const std::string& path)
    : m_path{ path }
{
    load(path);
}

Dataset::Dataset(const Dataset& other)
    : m_path{ other.m_path }, m_currentIndex{ other.m_currentIndex }
    , m_names{ other.m_names }, m_classes{ other.m_classes }
{}

Dataset::Dataset(Dataset&& other)
    : m_path{ std::move(other.m_path) }, m_currentIndex{ other.m_currentIndex }
    , m_names{ std::move(other.m_names) }, m_classes{ std::move(other.m_classes) }
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
    return Picture(m_path, m_names[m_currentIndex], QSize(360, 360)); // TODO: change size
}

std::string Dataset::currentName() const
{
    return m_names[m_currentIndex];
}

const std::unordered_map<size_t, std::string>& Dataset::classes() const
{
    return m_classes;
}

Picture Dataset::next()
{
    if (m_currentIndex + 1 >= m_names.size())
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
    return m_names.size();
}

void Dataset::swap(Dataset& dataset)
{
    std::swap(m_path, dataset.m_path);
    std::swap(m_currentIndex, dataset.m_currentIndex);
    std::swap(m_names, dataset.m_names);
    std::swap(m_classes, dataset.m_classes);
}

void Dataset::load(const std::string& path)
{
    for (const auto& entry : fs::directory_iterator(path + "/images/"))
        m_names.push_back(entry.path().stem().string());

    // TODO: move to other method
    auto reserved = std::move(m_classes); // save current classes for recovering in exception
    m_classes = std::unordered_map<size_t, std::string>();

    std::ifstream classesFile(path + "/predefined_classes.txt");
    if (!classesFile.is_open())
        throw AnalyticsException("Failed to read predefined classes");

    std::string line;
    size_t classType;
    std::string classLabel;
    while (getline(classesFile, line)) {
        std::stringstream ss;
        ss << line;
        ss >> classType;
        getline(ss, classLabel);

        if (m_classes.find(classType) != m_classes.end()) {
            m_classes = reserved;
            throw AnalyticsException("There are 2 classes with 1 label - " + classType);
        }

        m_classes[classType] = classLabel;
    }
}

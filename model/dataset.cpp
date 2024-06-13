#include "analyticsexception.h"
#include "dataset.h"

#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

//Dataset::Dataset()
//{}
//
//Dataset::Dataset(const std::string& path)
//    : m_path{ path }
//{
//    load(path);
//}
//
//Dataset::Dataset(const Dataset& other)
//    : m_path{ other.m_path }, m_currentIndex{ other.m_currentIndex }
//    , m_names{ other.m_names }, m_classes{ other.m_classes }
//{}
//
//Dataset::Dataset(Dataset&& other)
//    : m_path{ std::move(other.m_path) }, m_currentIndex{ other.m_currentIndex }
//    , m_names{ std::move(other.m_names) }, m_classes{ std::move(other.m_classes) }
//{}
//
//Dataset& Dataset::operator=(Dataset other)
//{
//    swap(other);
//
//    return *this;
//}
//
//Dataset::~Dataset()
//{}
//
//std::string Dataset::path() const
//{
//    return m_path;
//}
//
//Picture Dataset::current() const
//{
//    // TODO: make constants
//    return Picture(m_path, m_names[m_currentIndex], QSize(2048, 2048));
//}
//
//std::string Dataset::currentName() const
//{
//    return m_names[m_currentIndex];
//}
//
//const std::unordered_map<size_t, std::string>& Dataset::classes() const
//{
//    return m_classes;
//}
//
//Picture Dataset::next()
//{
//    if (m_currentIndex + 1 >= m_names.size())
//        throw AnalyticsException("Too big index");
//
//    ++m_currentIndex;
//
//    return current();
//}
//
//Picture Dataset::previous()
//{
//    if (m_currentIndex == 0)
//        throw AnalyticsException("Too small index");
//
//    --m_currentIndex;
//
//    return current();
//}
//
//size_t Dataset::currentIndex() const
//{
//    return m_currentIndex;
//}
//
//size_t Dataset::count() const
//{
//    return m_names.size();
//}
//
//void Dataset::swap(Dataset& dataset)
//{
//    std::swap(m_path, dataset.m_path);
//    std::swap(m_currentIndex, dataset.m_currentIndex);
//    std::swap(m_names, dataset.m_names);
//    std::swap(m_classes, dataset.m_classes);
//}
//
//void Dataset::load(const std::string& path)
//{
//    checkCorrectness(path);
//
//    for (const auto& entry : fs::directory_iterator(path + "/images/"))
//        m_names.push_back(entry.path().stem().string());
//
//    // TODO: move to other method
//    auto reserved = std::move(m_classes); // save current classes for recovering in exception
//    m_classes = std::unordered_map<size_t, std::string>();
//
//    std::ifstream classesFile(path + "/predefined_classes.txt");
//    if (!classesFile.is_open())
//        throw AnalyticsException("Failed to read predefined classes");
//
//    std::string line;
//    size_t classType;
//    std::string classLabel;
//    while (getline(classesFile, line)) {
//        std::stringstream ss;
//        ss << line;
//        ss >> classType;
//        getline(ss, classLabel);
//
//        if (m_classes.find(classType) != m_classes.end()) {
//            m_classes = reserved;
//            throw AnalyticsException("There are 2 classes with 1 label - "
//                                    + std::to_string(classType));
//        }
//
//        m_classes[classType] = classLabel;
//    }
//}
//
//void Dataset::checkCorrectness(const fs::path& path) const
//{
//    fs::path imagesPath = path / "images";
//    if (!fs::exists(imagesPath) || !fs::is_directory(imagesPath))
//        throw AnalyticsException("Image directory is not exist");
//
//    fs::path imagesLabelsPath = path / "images_labels";
//    if (!fs::exists(imagesLabelsPath) || !fs::is_directory(imagesLabelsPath))
//        throw AnalyticsException("image_labels directory is not exist");
//
//    fs::path imagesResultsPath = path / "images_results";
//    if (!fs::exists(imagesResultsPath) || !fs::is_directory(imagesResultsPath))
//        throw AnalyticsException("image_results directory is not exist");
//
//    fs::path predefinedClassesPath = path / "predefined_classes.txt";
//    if (!fs::exists(predefinedClassesPath) || !fs::is_regular_file(predefinedClassesPath))
//        throw AnalyticsException("Image directory is not exist");
//}

Dataset::Dataset(std::filesystem::path datasetPath, size_t batchSize)
	: kBatchSize{ batchSize }, kDatasetPath{ datasetPath }
{
    checkDatasetFormatCorrectness();

    loadClasses();

    // TODO: remove it
    for (const auto& entry : fs::directory_iterator(kDatasetPath.string() + "/images/"))
        m_names.push_back(entry.path().stem().string());

    load(0);
}

size_t Dataset::batchSize() const
{
	return kBatchSize;
}

size_t Dataset::currentIndex() const
{
	return m_currentIndex;
}

std::shared_ptr<IDataObject> Dataset::data()
{
    if (m_currentIndex < m_batchStart || m_currentIndex > m_batchStart + kBatchSize)
    {
		load(m_currentIndex / kBatchSize);
        m_batchStart = m_currentIndex / kBatchSize;
    }

	return m_batch[m_currentIndex - m_batchStart];
}

void Dataset::setIndex(size_t index)
{
    m_currentIndex = index;
}

void Dataset::next()
{
    ++m_currentIndex;
}

void Dataset::previous()
{
    --m_currentIndex;
}

void Dataset::loadClasses()
{
    std::ifstream classesFile(kDatasetPath.string() + "/predefined_classes.txt");
    if (!classesFile.is_open())
        throw AnalyticsException("Failed to read predefined classes");
    
    std::string line;
    size_t classType;
    std::string classLabel;

    std::unordered_map<size_t, std::string> temp;

    while (getline(classesFile, line)) {
        std::stringstream ss;
        ss << line;
        ss >> classType;
        getline(ss, classLabel);
    
        if (temp.find(classType) != temp.end()) {
            throw AnalyticsException("There are 2 classes with 1 label - "
                                    + std::to_string(classType));
        }
    
        temp[classType] = classLabel;
    }

    std::swap(m_classes, temp); // Is it noexcept???
}

void Dataset::load(size_t startIndex)
{
    // temporary implementation

    // add checks

    auto start = m_names.begin() + startIndex;
    auto end = start + kBatchSize;

    std::vector<std::shared_ptr<IDataObject>> temp;
    temp.reserve(kBatchSize);

    for (auto it = start; it != end; ++it)
        temp.push_back(std::make_shared<BloodCellDataObjectV1>(kDatasetPath, *it));

    std::swap(temp, m_batch);

    // end of temporary realization
}

void Dataset::checkDatasetFormatCorrectness() const
{
	fs::path imagesPath = kDatasetPath / "images";
    if (!fs::exists(imagesPath) || !fs::is_directory(imagesPath))
        throw AnalyticsException("Image directory is not exist");

    fs::path imagesLabelsPath = kDatasetPath / "images_labels";
    if (!fs::exists(imagesLabelsPath) || !fs::is_directory(imagesLabelsPath))
        throw AnalyticsException("image_labels directory is not exist");

    fs::path imagesResultsPath = kDatasetPath / "images_results";
    if (!fs::exists(imagesResultsPath) || !fs::is_directory(imagesResultsPath))
        throw AnalyticsException("image_results directory is not exist");

    fs::path predefinedClassesPath = kDatasetPath / "predefined_classes.txt";
    if (!fs::exists(predefinedClassesPath) || !fs::is_regular_file(predefinedClassesPath))
        throw AnalyticsException("Image directory is not exist");
}

BloodCellDataObjectV1::BloodCellDataObjectV1(std::filesystem::path datasetPath, std::filesystem::path imgPath)
    : IDataObject{}, m_imgPath{ imgPath }
{
    // implementation of loading data
}

BloodCellDataObjectV1::~BloodCellDataObjectV1()
{
}

size_t BloodCellDataObjectV1::objectClass() const
{
    return m_class;
}

void BloodCellDataObjectV1::data() const
{
    // now it's just empty ;)
}

std::string BloodCellDataObjectV1::name() const
{
    return std::string();
}

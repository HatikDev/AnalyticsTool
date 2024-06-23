#ifndef DATASET_H
#define DATASET_H

#include "gui/rectangle.h"
#include "analyticsexception.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

struct Rect {
	size_t type;
	std::string name;
	float centerX;
	float centerY;
	float w;
	float h;
};

struct BloodCellObj {
	std::filesystem::path imgPath;
	std::vector<Rect> rects;
};

// TODO: add move-semantic
template <typename T>
class IDataObject {
public:
	IDataObject(const std::string& name, const T& obj)
		: m_name{ name }, m_obj{ obj } {}
	virtual ~IDataObject() {}

	virtual T& data() {
		return m_obj;
	}

	virtual std::string name() const {
		return m_name;
	}

private:
	std::string m_name;

	T m_obj;
};

class BloodCellDataObject : public IDataObject<BloodCellObj> {
public:
	BloodCellDataObject(const std::string& name, const BloodCellObj& obj)
		: IDataObject<BloodCellObj>(name, obj) {}
	~BloodCellDataObject() override {}

private:
};

template <typename ObjType>
class IDataset {
public:
	//    Dataset();
	//    Dataset(const std::string& path);
	//    Dataset(const Dataset& other);
	//    Dataset(Dataset&& other);
	//    Dataset& operator=(Dataset other);
	//    ~Dataset();
	//
	//    std::string path() const;
	//
	//    Picture current() const;
	//
	//    std::string currentName() const;
	//
	//    const std::unordered_map<size_t, std::string>& classes() const;
	//
	//    Picture next();
	//
	//    Picture previous();
	//
	//    size_t currentIndex() const;
	//
	//    size_t count() const;
	//
	//private:
	//    std::string m_path;
	//
	//    size_t m_currentIndex = 0;
	//
	//    std::vector<std::string> m_names;
	//
	//    std::unordered_map<size_t, std::string> m_classes;
	//
	//    void swap(Dataset& dataset);
	//
	//    void load(const std::string& path);
	//
	//    void checkCorrectness(const std::filesystem::path& path) const;

public:
	IDataset(std::filesystem::path datasetPath, size_t batchSize = 10)
		: kBatchSize{ batchSize }, kDatasetPath{ datasetPath }
	{}

	IDataset(const IDataset& dataset) = delete;
	IDataset(IDataset&& dataset) = delete;
	IDataset& operator=(const IDataset& dataset) = delete;
	IDataset& operator=(IDataset&& dataset) = delete;
	virtual ~IDataset() {}

	size_t batchSize() const
	{
		return kBatchSize;
	}

	std::vector<std::shared_ptr<IDataObject<ObjType>>> batch() const
	{
		return m_batch;
	}

	size_t currentIndex() const
	{
		return m_currentIndex;
	}

	std::unordered_map<size_t, std::string>& classes()
	{
		return m_classes;
	}

	std::shared_ptr<IDataObject<ObjType>> data()
	{
		return m_batch.at(m_currentIndex % kBatchSize);
	}

	void setIndex(size_t index)
	{
		m_currentIndex = index;
	}

	size_t batchStart() const
	{
		return m_batchStart;
	}

	void next(bool save = true)
	{
		if (m_currentIndex + 1 >= size())
			throw AnalyticsException("Invalid index");

		if (m_currentIndex + 1 >= m_batchStart + kBatchSize)
		{
			load(m_batchStart + kBatchSize);
			m_batchStart += kBatchSize;
		}

		++m_currentIndex;
	}

	void previous(bool save = true)
	{
		if (m_currentIndex == 0)
			throw AnalyticsException("Invalid index");

		if (m_currentIndex - 1 < m_batchStart)
		{
			load(m_batchStart - kBatchSize);
			m_batchStart -= kBatchSize;
		}

		--m_currentIndex;
	}

	bool hasPrevious() const
	{
		return size() > 0 && m_currentIndex > 0;
	}

	bool hasNext() const
	{
		return size() > 0 && m_currentIndex + 1 <= size() - 1;
	}

	size_t size() const
	{
		return m_names.size();
	}

	virtual void saveCurrent(const ObjType& objs) = 0;

protected:
	const size_t kBatchSize;

	const std::filesystem::path kDatasetPath;

	size_t m_currentIndex = 0;

	size_t m_batchStart = 0;

	std::vector<std::shared_ptr<IDataObject<ObjType>>> m_batch;

	std::unordered_map<size_t, std::string> m_classes;

	std::vector<std::string> m_names; // remove it

	virtual void load(size_t startIndex) = 0;

	virtual void loadClasses() = 0;

	// Maybe it depends from smth??
	virtual void checkDatasetFormatCorrectness() const = 0;
};

class BloodDataset final : public IDataset<BloodCellObj> {
public:
	BloodDataset(std::filesystem::path datasetPath, size_t batchSize = 10)
		: IDataset<BloodCellObj>(datasetPath, batchSize)
	{
		checkDatasetFormatCorrectness();

		loadClasses();

		// TODO: remove this
		for (const auto& entry : std::filesystem::directory_iterator(kDatasetPath.string() + "/images/"))
			m_names.push_back(entry.path().stem().string());

		load(0);
	}
	~BloodDataset() override {}

	void saveCurrent(const BloodCellObj& obj) override
	{
		// TODO: add exception safety
		auto resultPath = kDatasetPath / "images_labels" / m_batch[m_currentIndex % kBatchSize]->data().imgPath.filename();
		resultPath.replace_extension(".txt");

		std::ofstream file(resultPath);
		if (!file.is_open())
			throw AnalyticsException("Failed to open file " + resultPath.string() + " to disk");

		for (auto& rect : obj.rects)
			file << rect.type << " " << rect.centerX << " " << rect.centerY << " " << rect.w << " " << rect.h << std::endl;

		file.close();

		auto& cachedObj = m_batch.at(m_currentIndex % kBatchSize).get()->data();
		cachedObj.rects = obj.rects;
	}

private:
	void load(size_t startIndex) override
	{
		// temporary implementation

		// add checks

		auto start = m_names.begin() + startIndex;
		auto end = start + std::min<size_t>(kBatchSize, std::distance(start, m_names.end()));

		std::vector<std::shared_ptr<IDataObject<BloodCellObj>>> temp;
		temp.reserve(kBatchSize);

		for (auto it = start; it != end; ++it) {
			// add loading rects
			std::filesystem::path labelsPath = (kDatasetPath / "images_labels" / *it).string() + ".txt";
			auto rects = loadRects(labelsPath);

			BloodCellObj obj = { kDatasetPath / "images" / *it, rects };

			temp.push_back(std::make_shared<BloodCellDataObject>(kDatasetPath.string(), obj));
		}

		std::swap(temp, m_batch);

		// end of temporary realization
	}

	void loadClasses() override
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

	std::vector<Rect> loadRects(std::filesystem::path labelsPath) const
	{
		std::ifstream file(labelsPath);
		if (!file.is_open())
			throw AnalyticsException(std::string("Failed to open file" + labelsPath.string()).c_str());

		std::vector<Rect> rects;

		std::string line;
		std::string type;
		float width, height;
		float centerX, centerY;

		size_t counter = 0;
		while (getline(file, line)) {
			std::stringstream ss;
			ss << line;
			ss >> type >> centerX >> centerY >> width >> height;

			// TODO: change type by predefined class
			auto name = m_classes.find(std::stoi(type))->second;
			rects.push_back({ std::stoul(type), name, centerX, centerY, width, height });
		}

		return rects;
	}

	// Maybe it depends from smth??
	void checkDatasetFormatCorrectness() const override
	{
		namespace fs = std::filesystem;

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
};

#endif // DATASET_H

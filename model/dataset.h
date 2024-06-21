#ifndef DATASET_H
#define DATASET_H

//#include "picture.h"
#include "analyticsexception.h"

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

	virtual T data() const {
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
		return m_batch.at(m_currentIndex);
	}

	void setIndex(size_t index)
	{
		m_currentIndex = index;
	}

	size_t batchStart() const
	{
		return m_batchStart;
	}

	void next()
	{
		++m_currentIndex;
	}

	void previous()
	{
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

	size_t size() const {
		return m_names.size();
	}

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

private:
	void load(size_t startIndex) override
	{
		// temporary implementation

		// add checks

		auto start = m_names.begin() + startIndex;
		auto end = start + kBatchSize;

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
		uint8_t type;
		float width, height;
		float centerX, centerY;

		//auto& classes = m_classes; // Model::instanse().dataset().classes();
		size_t counter = 0;
		while (getline(file, line)) {
			std::stringstream ss;
			ss << line;
			ss >> type >> centerX >> centerY >> width >> height;

			type -= '0';

			// TODO: we need to multiply coordinates on image size
			//float x0 = centerX - width / 2;
			//float y0 = centerY - height / 2;
			//float x1 = x0 + width;
			//float y1 = y0 + height;

			// TODO: change type by predefined class
			auto name = m_classes.find(type)->second;
			//std::string className = classNameIt == classes.end() ? std::to_string(type) : classNameIt->second;
			rects.push_back({ type, name, centerX, centerY, width, height });
			//m_rects.push_back(std::make_shared<Rectangle>(className, counter++, QPointF(x0, y0), type));
			//m_rects.back()->setEndPoint(QPointF(x1, y1));

			//connect(m_rects.get(), &Rectangle::rectSelected, this, &PaintScene::rectSelectionChanged);
			//connect(m_rects.get(), &Rectangle::rectDeselected, this, &PaintScene::rectSelectionChanged);
			// TODO: add disconnection
			//emit rectAdded(m_rects.back());
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

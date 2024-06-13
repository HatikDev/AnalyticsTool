#ifndef DATASET_H
#define DATASET_H

#include "picture.h"

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

class IDataObject {
public:
	IDataObject() {};
	virtual ~IDataObject() {}

	virtual size_t objectClass() const = 0;

	virtual void data() const = 0;

	virtual std::string name() const = 0;
};

class BloodCellDataObjectV1 : public IDataObject {
public:
	BloodCellDataObjectV1(std::filesystem::path datasetPath, std::filesystem::path imgPath);
	~BloodCellDataObjectV1() override;

	size_t objectClass() const override;

	void data() const override;

	std::string name() const override;

private:
	size_t m_class;

	std::filesystem::path m_imgPath;
};

class Dataset {
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
	Dataset(std::filesystem::path datasetPath, size_t batchSize = 10);
	Dataset(const Dataset& dataset) = delete;
	Dataset(Dataset&& dataset) = delete;
	Dataset& operator=(const Dataset& dataset) = delete;
	Dataset& operator=(Dataset&& dataset) = delete;

	size_t batchSize() const;

	size_t currentIndex() const;

	std::shared_ptr<IDataObject> data();

	void setIndex(size_t index);

	void next();

	void previous();

private:
	const size_t kBatchSize;

	const std::filesystem::path kDatasetPath;

	size_t m_currentIndex = 0;

	size_t m_batchStart = 0;

	std::vector<std::shared_ptr<IDataObject>> m_batch;

	std::unordered_map<size_t, std::string> m_classes;

	std::vector<std::string> m_names; // remove it

	void load(size_t startIndex);

	void loadClasses();

	// Maybe it depends from smth??
	void checkDatasetFormatCorrectness() const;
};

#endif // DATASET_H

#ifndef DATASET_H
#define DATASET_H

#include "picture.h"

#include <string>
#include <vector>

class Dataset {
public:
    Dataset();
    Dataset(const std::string& path);
    Dataset(const Dataset& other);
    Dataset(Dataset&& other);
    Dataset& operator=(Dataset other);
    ~Dataset();

    std::string path() const;

    Picture current() const;

    std::string currentName() const;

    Picture next();

    Picture previous();

    size_t currentIndex() const;

    size_t count() const;

private:
    std::string m_path;

    size_t m_currentIndex = 0;

    std::vector<std::string> m_names;

    void swap(Dataset& dataset);

    void load(const std::string& path);
};

#endif // DATASET_H
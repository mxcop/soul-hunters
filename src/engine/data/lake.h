/**
 * @file lake.h
 * @author Max (mxcop)
 * @brief Memory pool.
 * @date 2023-07-28
 */
#pragma once
#include <functional>

/**
 * @brief Memory pool implementation.
 */
template <typename T>
class Lake {
    T *data;
    bool *status;

    size_t capacity;
    size_t size;

public:
    Lake(size_t capacity = 16) {
        data = static_cast<T*>(std::malloc(capacity * sizeof T));
        status = new bool[capacity]{0};
        this->capacity = capacity;
        size = 0;
    }

    ~Lake() {
        delete[] data;
        delete[] status;
    }

    /**
     * @brief Add a new element into the pool.
     *
     * @param element The element to add.
     * @return The index of the added element.
     */
    size_t add(T element) {
        /* Check if there's an inactive element we can override */
        for (size_t i = 0; i < size; i++)
        {
            if (status[i]) continue;

            data[i] = element;
            status[i] = true;
            return i;
        }
        
        /* Reallocate the data with double the size */
        if (size >= capacity) {
            reserve(capacity * 2);
        }

        /* Insert the new element */
        data[size] = element;
        status[size] = true;

        size++;
        return size - 1;
    }

    /**
     * @brief Get an element by index.
     * 
     * @param idx The index of the element.
     * @return A reference to the element.
     */
    T& get(size_t idx) {
        if (idx < size)
            return data[idx];
    }

    /**
     * @brief Deactivate an element in the pool.
     * 
     * @param idx The index of the element to deactivate.
     * @return True if the element was deactivated.
     */
    bool remove(size_t idx) {
        if (idx >= size)
            return false;

        status[idx] = false;
        return true;
    }

    /**
     * @brief Iterate over all active elements in the pool.
     */
    void iterate(std::function<void(T&)> iterator) {
        for (size_t i = 0; i < size; i++)
        {
            if (status[i] == false) continue;

            iterator(data[i]);
        }
    }

    /**
     * @brief Set the capacity of the pool.
     */
    void reserve(size_t capacity) {
        T* temp = static_cast<T*>(std::malloc(capacity * sizeof T));
        bool* temp_s = new bool[capacity] {0};

        /* Copy the data */
        if (this->capacity < capacity) {
            std::memcpy(temp, data, sizeof data);
            std::memcpy(temp_s, status, sizeof status);
        } else {
            std::memcpy(temp, data, sizeof temp);
            std::memcpy(temp_s, status, sizeof temp_s);
        }
        this->capacity = capacity;
        
        /* Deallocate the old data */
        delete[] data;
        delete[] status;

        data = temp;
        status = temp_s;
    }

    size_t get_size() { return size; }
    size_t get_capacity() { return capacity; }
};
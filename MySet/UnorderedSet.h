// Author: Muhamed Agic

#pragma once

#include <iostream>
#include <string>
#include <typeinfo>


namespace MyContainers {

    // To be even more robust, you can restrict UnorderedSet<T> to types that are EqualityComparable and MoveConstructible using concepts or static_assert.
	template <typename T>
	class UnorderedSet {
	public:

        UnorderedSet() :
            m_data(std::make_unique<T[]>(m_capacity))
        {}


        explicit UnorderedSet(int capacity) :
            m_capacity(capacity),
            m_data(std::make_unique<T[]>(m_capacity))
        {}


        UnorderedSet(const UnorderedSet<T>& other) :
            m_capacity(other.m_capacity),
            m_count(other.m_count),
            m_data(std::make_unique<T[]>(m_capacity)) {
            other.copy_to(*this);
        }


        UnorderedSet(UnorderedSet<T>&& other)  noexcept :
                m_capacity(other.m_capacity),
                m_count(other.m_count),
                m_data(std::move(other.m_data)) {
            other.m_capacity = 0;
            other.m_count = 0;
        }

        UnorderedSet(std::initializer_list<T> ilist) :
        m_capacity(ilist.size()),
        m_count(0),
        m_data(std::make_unique<T[]>(ilist.size())) {
            for (const auto& val : ilist) {
                add(val);
            }
        }


        template<typename... Args>
        bool emplace(Args&&... args) {
            T value(std::forward<Args>(args)...);
            return add(value);
        }


        UnorderedSet<T>& operator = (const UnorderedSet<T>& other) {
            if (this == &other) {
                return *this;
            }

            other.copy_to(*this);

            if (*this != other) {
                throw std::exception("Something went wrong in assigning");
            }

            return *this;
        }


        UnorderedSet<T>& operator = (UnorderedSet<T>&& other)  noexcept {
            if (this == &other) {
                return *this;
            }

            m_capacity = other.m_capacity;
            m_count = other.m_count;
            m_data = std::move(other.m_data);

            other.m_capacity = 0;
            other.m_count = 0;

            return *this;
        }


        bool operator == (const UnorderedSet<T>& other) const {
            // when are sets equal:
            // Their size is the same
            // They contain exactly the same elements
            // Order does not matter
            if (m_count != other.m_count) {
                return false;
            }
            for (std::size_t i = 0; i < m_count; i++) {
                if (!contains(other.m_data[i])) {   // item from other not present in this
                    return false;
                }
            }
            return true;
        }


        T& operator [] (std::size_t index) {
            if (index >= m_count) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }


        const T& operator [] (std::size_t index) const {
            if (index >= m_count) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }


        T* begin() {
            return m_data.get();
        }


        T* end() {
            return m_data.get() + m_count;
        }


        const T* begin() const {
            return m_data.get();
        }


        const T* end() const {
            return m_data.get() + m_count;
        }


        bool operator != (const UnorderedSet<T>& other) const {
            return !(*this == other);
        }


        [[nodiscard]] std::size_t size() const {
            return m_count;
        }


        [[nodiscard]] std::size_t capacity() const {
            return m_capacity;
        }


        const T& at(std::size_t index) const {
            if (index < m_count) {
                return m_data[index];
            }
            throw std::out_of_range("The given index is out of range");
        }


        template<typename U>
        bool add(U&& s) {
            if (contains(s)) {
                return false;
            }

            if (needs_expansion()) {
                expand();
            }

            m_data[m_count] = std::forward<U>(s);
            ++m_count;
            return true;
        }


        bool contains(const T& s) const {
            for (std::size_t i = 0; i < m_count; i++) {
                if (m_data[i] == s) {
                    return true;
                }
            }
            return false;
        }


        bool remove(const T& s) {
            for (std::size_t i = 0; i < m_count; i++) {
                if (m_data[i] == s) {
                    m_data[i] = std::move(m_data[m_count - 1]); // move last element to this spot
                    --m_count;
                    return true;
                }
            }
            return false;
        }


        void clear() {
            m_count = 0;
        }


        void swap(UnorderedSet& other) noexcept {
            std::swap(m_data, other.m_data);
            std::swap(m_capacity, other.m_capacity);
            std::swap(m_count, other.m_count);
        }


        T* find(const T& element) {
            for (std::size_t i = 0; i < m_count; i++) {
                if (m_data[i] == element) {
                    return m_data.get() + i;
                }
            }
            return m_data.get() + m_count; // end
        }


        const T* find(const T& element) const {
            for (std::size_t i = 0; i < m_count; i++) {
                if (m_data[i] == element) {
                    return m_data.get() + i;
                }
            }
            return m_data.get() + m_count; // end
        }


        void reserve(std::size_t n) {
            if (n < m_capacity) {
                return;
            }

            auto new_data = std::make_unique<T[]>(n);
            for (std::size_t i = 0; i < m_count; i++) {
                new_data[i] = std::move(m_data[i]);
            }

            m_data = std::move(new_data);
            m_capacity = n;
        }


        bool shrink_to_fit() {
            if (m_count < m_capacity) {
                auto new_data = std::make_unique<T[]>(m_count);
                for (std::size_t i = 0; i < m_count; ++i) {
                    new_data[i] = std::move(m_data[i]);
                }
                m_data = std::move(new_data);
                m_capacity = m_count;
                return true;
            }
            return false;
        }


    private:
		std::size_t m_capacity = 10;	// nr of allocated elements
		std::size_t m_count = 0;		// nr of elements
		std::unique_ptr<T[]> m_data;	// pointer to data


        bool copy_to(UnorderedSet<T>& other) const {
            // copies data from this to the other
            while (other.m_capacity < m_count) {
                other.expand();
            }

            for (std::size_t i = 0; i < m_count; i++) {
                other.m_data[i] = m_data[i];
            }

            other.m_count = m_count;
            return (*this == other);
        }


        [[nodiscard]] bool needs_expansion() const {
            return m_count == m_capacity;
        }


        bool expand() {
            std::size_t new_capacity = m_capacity * 2; // double capacity for now
            auto new_data = std::make_unique<T[]>(new_capacity);

            for (std::size_t i = 0; i < m_count; i++) {
                new_data[i] = std::move(m_data[i]);
            }

            m_data = std::move(new_data);
            m_capacity = new_capacity;

            return true;
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const UnorderedSet<T>& mySet) {
        os << "Address: " << &mySet << std::endl;
        os << "Data start address: " << mySet.begin() << std::endl;
        os << "Set of type: " << typeid(T).name() << std::endl;
        os << "Count: " << mySet.size() << std::endl;
        os << "Capacity: " << mySet.capacity() << std::endl;
        os << "Contents: {";
        for (std::size_t i = 0; i < mySet.size(); ++i) {
            os << mySet.at(i);
            if (i < mySet.size() - 1) {
                os << ", ";
            }
        }
        os << "}\n";
        return os;
    }

} // Namespace MyContainers
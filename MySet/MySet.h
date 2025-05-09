// Author: Muhamed Agic

#pragma once

#include <iostream>
#include <string>

namespace MyContainers {

	template <typename T>
	class MySet {
	public:

        MySet() :
            m_data(new T[m_capacity])
        {}


        explicit MySet(int capacity) :
            m_capacity(capacity),
            m_data(new T[m_capacity])
        {}


        MySet(const MySet<T>& other) :
            m_data(new T[m_capacity]) {
            other.copy_to(*this);
        }


        MySet(MySet<T>&& other)  noexcept :
                m_capacity(other.m_capacity),
                m_count(other.m_count),
                m_data(other.m_data) {
            other.m_data = nullptr;
        }


        ~MySet() {
            std::cout << "Destructing MySet instance: " << this << std::endl;
            delete[] m_data;
            m_capacity = 0;
            m_count = 0;
        }


        MySet<T>& operator = (const MySet<T>& other) {
            if (this == &other) {   // check of die aan zichzelf assigned
                return *this;
            }

            if (this->m_data != nullptr) {
                delete[] this->m_data;
                this->m_data = new T[other.m_capacity];
            }

            other.copy_to(*this);

            if (*this != other) {
                throw std::exception("Something went wrong in assigning");
            }

            return *this;
        }


        MySet<T>& operator = (MySet<T>&& other)  noexcept {
            if (this == &other) {   // check of die aan zichzelf assigned
                return *this;
            }

            delete[] this->m_data;
            this->m_capacity = other.m_capacity;
            this->m_count = other.m_count;
            this->m_data = other.m_data;

            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_count = 0;

            return *this;
        }


        bool operator == (const MySet<T>& other) const {
            // twijfel of capacity ook aan elkaar gelijk moet zijn
            // ik ga nu voor: elementen moeten exact hetzelfde zijn in dezelfde volgorde
            if (this->m_count != other.m_count) {
                return false;   // niet gelijk als een set niet even veel elementen heeft
            }
            for (std::size_t i = 0; i < this->m_count; i++) {
                if (this->m_data[i] != other.m_data[i]) { // is er iets niet aan elkaar gelijk
                    return false;
                }
            }
            return true;
        }


        bool operator != (const MySet<T>& other) const {
            return !(*this == other);
        }


        std::size_t size() const {
            return this->m_count;
        }


        std::size_t capacity() const {
            return this->m_capacity;
        }


        T at(std::size_t index) const {
            if (index <= this->m_count) {
                return this->m_data[index];
            }
            // mooier op te lossen denk ik met std::optional, maar die zit niet in C++-11, voor nu throwen
            throw std::out_of_range("The given index is out of range");
        }


        bool add(const T& s) {
            if (contains(s)) {
                return false;   // mag niet worden ge-add
            }

            if (needs_expansion()) {
                expand();
            }

            this->m_data[this->m_count] = std::move(s);
            ++this->m_count;

            return contains(s);
        }


        bool contains(const T& s) const {
            // Alles 1x langs gaan
            for (std::size_t i = 0; i < this->m_count; i++) {
                if (m_data[i] == s) {
                    return true;
                }
            }
            return false;
        }


        bool remove(const T& s) {
            for (std::size_t i = 0; i <= this->m_count; i++) {
                if (this->m_data[i] == s) { // als we s hebben gevonden
                    shift(i);               // alles een plekje opschuiven om gat te vullen
                    --this->m_count;
                    return true;
                }
            }
            return false;   // s niet gevonden
        }


	private:
		std::size_t m_capacity = 10;	// nr of allocated elements
		std::size_t m_count = 0;		// nr of elements
		T* m_data = nullptr;	// pointer to data


        void shift(std::size_t index) {
            while(index < this->m_count) { // alles een plekje naar links schuiven, vul memory gat
                this->m_data[index] = std::move(this->m_data[index + 1]);
                ++index;
            }
        }


        bool copy_to(MySet<T>& other) const {
            // kopieert data van this naar de ander
            while (other.m_capacity < this->m_count) { // deze set heeft meer elementen dan de ander kan opvangen
                other.expand();
            }
            for (std::size_t i = 0; i <= this->m_count; i++) {
                other.m_data[i] = this->m_data[i];
            }
            other.m_count = this->m_count;
            other.m_capacity = this->m_capacity;

            return (*this == other);
        }


        bool needs_expansion() const {
            return this->m_count == this->m_capacity; // dan zit ie vol
        }


        bool expand() {
            std::size_t new_capacity = this->m_capacity * 2; // even simpel capaciteit verdubbelen
            T* new_data = nullptr;

            try {
                new_data = new T[new_capacity];
            }
            catch (const std::exception& e) {
                std::cout << "Could not resize MySet from " << this->m_capacity << " to " << new_capacity << std::endl;
                std::cout << "Exception: " << e.what() << std::endl;
                return false;
            }

            for (std::size_t i = 0; i < this->m_count; i++) {
                new_data[i] = std::move(this->m_data[i]);
            }

            // oude data verwijderen en members updaten
            delete[] this->m_data;
            this->m_data = new_data;
            this->m_capacity = new_capacity;

            return true;
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const MySet<T>& mySet) {
        os << "Address: " << &mySet << std::endl;
        os << "Count: " << mySet.size() << std::endl;
        os << "Capacity: " << mySet.capacity() << std::endl;
        for (std::size_t i = 0; i < mySet.size(); ++i) {
            os << "Element " << i << ": " << mySet.at(i) << std::endl;
        }
        os << std::endl;
        return os;
    }

} // Namespace MyContainers
#include "MySet.h"

MyContainers::MySetV2<std::string>::MySetV2() :
    m_data(new std::string[m_capacity])
{}


MyContainers::MySetV2<std::string>::MySetV2(int capacity) :
    m_capacity(capacity), 
    m_data(new std::string[m_capacity]) 
{}


MyContainers::MySetV2<std::string>::MySetV2(const MySetV2& other) :
    m_data(new std::string[m_capacity]) {
    other.copy_to(*this);
}


MyContainers::MySetV2<std::string>::MySetV2(MySetV2&& other) :
    m_capacity(other.m_capacity),
    m_count(other.m_count),
    m_data(other.m_data) {
    other.m_data = nullptr;
}


MyContainers::MySetV2<std::string>::~MySetV2() {
    std::cout << "Destructing MySetV2 instance: " << this << std::endl;
    delete[] m_data;
    m_capacity = 0;
    m_count = 0;
}


std::ostream& MyContainers::operator << (std::ostream& ostream, const MyContainers::MySetV2<std::string>& mySet) {
    std::cout << "Address: " << &mySet << std::endl;
    std::cout << "Count: " << mySet.size() << std::endl;
    std::cout << "Capacity: " << mySet.capacity() << std::endl;
    for (int i = 0; i < mySet.size(); i++) {
        std::cout << "Element " << i << ": " << mySet.at(i) << std::endl;
    }
    std::cout << std::endl;
    return ostream;
}


MyContainers::MySetV2<std::string>& MyContainers::MySetV2<std::string>::operator = (const MySetV2& other) {
    if (this == &other) {   // check of die aan zichzelf assigned
        return *this;   
    }

    if (this->m_data != nullptr) {
        delete[] this->m_data;
        this->m_data = new std::string[other.m_capacity];
    }

    other.copy_to(*this);

    if (*this != other) {
        throw std::exception("Something went wrong in assigning");
    }

    return *this;
}


MyContainers::MySetV2<std::string>& MyContainers::MySetV2<std::string>::operator = (MySetV2&& other) {
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


bool MyContainers::MySetV2<std::string>::operator == (const MySetV2& other) const {
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


bool MyContainers::MySetV2<std::string>::operator != (const MySetV2& other) const {
    return !(*this == other);
}


std::size_t MyContainers::MySetV2<std::string>::size() const {
    return this->m_count; 
}


std::size_t MyContainers::MySetV2<std::string>::capacity() const {
    return this->m_capacity;
}


std::string MyContainers::MySetV2<std::string>::at(std::size_t index) const {
    if (index <= this->m_count) {
        return this->m_data[index];
    }
    // mooier op te lossen denk ik met std::optional, maar die zit niet in C++-11, voor nu throwen
    throw std::out_of_range("The given index is out of range");
}


bool MyContainers::MySetV2<std::string>::needs_expansion() const {
    return this->m_count == this->m_capacity; // dan zit ie vol
}


bool MyContainers::MySetV2<std::string>::expand() {
    std::size_t new_capacity = this->m_capacity * 2; // even simpel capaciteit verdubbelen
    std::string* new_data = nullptr;

    try {
        new_data = new std::string[new_capacity];
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


bool MyContainers::MySetV2<std::string>::add(const std::string& s) {
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


bool MyContainers::MySetV2<std::string>::contains(const std::string& s) const {
	// Alles 1x langs gaan
	for (std::size_t i = 0; i < this->m_count; i++) {
		if (m_data[i] == s) {
			return true;
		}
	}
	return false;
}


bool MyContainers::MySetV2<std::string>::remove(const std::string& s) {
    for (std::size_t i = 0; i <= this->m_count; i++) {
        if (this->m_data[i] == s) { // als we s hebben gevonden
            shift(i);               // alles een plekje opschuiven om gat te vullen
            --this->m_count;
            return true;
        }
    }
    return false;   // s niet gevonden
}


void MyContainers::MySetV2<std::string>::shift(std::size_t index) {
    while(index < this->m_count) { // alles een plekje naar links schuiven, vul memory gat
        this->m_data[index] = std::move(this->m_data[index + 1]);
        ++index;
    }
}


bool MyContainers::MySetV2<std::string>::copy_to(MyContainers::MySetV2<std::string>& other) const {
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

// Author: Muhamed Agic

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace MyContainers {

	template <typename T>
	class MySet;

	// full template specialization, we doen het voor strings
	template <>
	class MySet <std::string> {
	public:

		bool add(const std::string& s);
		bool contains(const std::string& s) const;
		bool remove(const std::string& s);


	private:
		// ik weet niet of dit mag?:
		// "It is up to you how you want to deal with the internal storage, but it is not allowed to just write a wrapper around std::set."
		// Maar dit is een std vector, dit leek iets te makkelijk
		std::vector<std::string> m_data;
	};


	// dit is denk ik de bedoeling
	template <typename T>
	class MySetV2;

	template <>
	class MySetV2 <std::string> {
	public:

		MySetV2();
		MySetV2(int capacity);
		MySetV2(const MySetV2& other);
		MySetV2(MySetV2&& other);
		~MySetV2();

		MySetV2& operator = (const MySetV2& other);
		MySetV2& operator = (MySetV2&& other);
		bool operator == (const MySetV2& other) const;
		bool operator != (const MySetV2& other) const;

		bool add(const std::string& s);
		bool contains(const std::string& s) const;
		bool remove(const std::string& s);

		std::size_t size() const;
		std::size_t capacity() const;
		std::string at(std::size_t index) const;

		friend std::ostream& operator << (std::ostream& ostream, const MySetV2<std::string>& mySet);

	private:
		std::size_t m_capacity = 10;	// nr of allocated elements
		std::size_t m_count = 0;		// nr of elements
		std::string* m_data = nullptr;	// pointer to data

		bool copy_to(MySetV2& other) const;
		bool needs_expansion() const;
		bool expand();
		void shift(std::size_t index);
	};

}
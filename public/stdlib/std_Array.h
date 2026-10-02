#pragma once

#include <initializer_list>

#include "std_dll_types.h"


namespace krystallic
{
    namespace Stdlib
    {
        template <typename T, uint capacity> class Array
        {
        private:
            T m_data[capacity];

        public:
            Array() {}

            Array(std::initializer_list<T>& lst)
            {
                //int n = 0;
                //for (std::initializer_list<T>::iterator i = lst.begin(); i != lst.end(); i++)
                //	m_data[n++] = *i;
            }

            ~Array() {}

            int Capacity() { return capacity; }

            T& operator[](int i) { return m_data[i]; }

            const T& operator[](int i) const { return m_data[i]; }

            T* Data() { return m_data; }

            const T* Data() const { return m_data; }
        };


        template <typename T, uint row, uint col> class Array2D
        {
        public:
			typedef Array<Array<T, row>, col> type;
        };
    } // namespace Stdlib
} // namespace krystallic
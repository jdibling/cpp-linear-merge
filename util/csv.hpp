//
// Created by john on 4/13/15.
//

#ifndef QC_CSV_HPP
#define QC_CSV_HPP

#include <string>
#include <sstream>

namespace pico {
namespace util {
namespace csv {

    class Row {
        std::vector<std::string> mData;
    public:
        using const_iterator = decltype (mData)::const_iterator;
        using reference = decltype (mData)::reference;

        const std::string operator[](size_t index) const {
            return mData[index];
        }

        size_t size() const {
            return mData.size();
        }

        const_iterator begin () const {
            return mData.begin ();
        }

        const_iterator end () const {
            return mData.end ();
        }

        template<typename Str>
        void ReadLine(Str &str) {
            std::string line;
            std::getline(str, line);

            std::stringstream lineStream(line);

            mData.clear();
            for (std::string cell; std::getline(lineStream, cell, ',');) {
                mData.push_back(cell);
            }
        }

    private:
    };

}
}
}

template<typename Str>
Str &operator>>(Str &str, pico::util::csv::Row &data)
{
    data.ReadLine(str);
    return str;
}



#endif //QC_CSV_HPP

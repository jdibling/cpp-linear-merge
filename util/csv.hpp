//
// Created by john on 4/13/15.
//

#ifndef QC_CSV_HPP
#define QC_CSV_HPP
namespace pico {
namespace util {
namespace csv {

    class Row {
    public:
        const std::string operator[](size_t index) const {
            return mData[index];
        }

        size_t size() const {
            return mData.size();
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
        std::vector <std::string> mData;
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

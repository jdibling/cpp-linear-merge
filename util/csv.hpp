//
// Created by john on 4/13/15.
//

#ifndef QC_CSV_HPP
#define QC_CSV_HPP
namespace pico {
namespace util {
namespace csv {

    class Row {
        typedef std::vector<std::string> Cont;
        Cont mData;
        std::string mRaw;
    public:
        typedef Cont::iterator iterator;
        typedef Cont::const_iterator const_iterator;

        typedef Cont::reference reference;

        const std::string operator[](size_t index) const {
            return mData[index];
        }

        size_t size() const {
            return mData.size();
        }

        Cont::iterator begin () { return mData.begin (); }

        Cont::const_iterator begin () const { return mData.begin (); }

        Cont::iterator end () { return mData.end (); }

        Cont::const_iterator end () const { return mData.end (); }

        template<typename Str>
        void ReadLine(Str &str) {
            mRaw.clear ();
            std::getline (str, mRaw);

            std::stringstream lineStream (mRaw);

            mData.clear();
            for (std::string cell; std::getline(lineStream, cell, ',');) {
                mData.push_back(cell);
            }
        }

        const std::string &Raw () const { return mRaw; }

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

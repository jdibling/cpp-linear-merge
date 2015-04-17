//
// Created by john on 4/13/15.
//
#include <iterator>
#include <algorithm>

#include <boost/iterator/indirect_iterator.hpp>

#include "../std/memory.hpp"
#include "../std/utility.hpp"

#ifndef QC_HEADER_HPP
#define QC_HEADER_HPP

template <typename Val> Val Convert (const std::string& str)
{
    std::stringstream ss;
    ss << str;
    Val ret;
    ss >> ret;
    return ret;
}


class Cell {
public:
    virtual ~Cell() = 0;
    virtual std::string Repr() const = 0;

    virtual bool operator== (const Cell &other) const = 0;
};

inline Cell::~Cell()
{

}

class TextCell
:
    public Cell
{
public:

    TextCell (const std::string& data)
    :
        mData (trim(data))
    {
    }

    std::string Repr() const {return mData;}

    bool operator== (const Cell &other) const {
        if (typeid (*this) != typeid (other)) {
            return false;
        }
        const TextCell &that = static_cast <const TextCell &> (other);
        return mData == that.mData;
    }

private:
    std::string mData;
};

class UIntCell
:
        public Cell
{
public:
    UIntCell (const std::string& data)
    :
        mData (Convert <uint64_t> (trim(data)))
    {

    }

    std::string Repr() const
    {
        std::stringstream ss;
        ss << mData;
        return ss.str();
    }

    bool operator== (const Cell &other) const {
        if (typeid (*this) != typeid (other)) {
            return false;
        }
        const UIntCell &that = static_cast <const UIntCell &> (other);
        return mData == that.mData;
    }

    bool operator== (Cell &other) {
        if (typeid (*this) != typeid (other)) {
            return false;
        }
        const UIntCell &that = static_cast <const UIntCell &> (other);
        return mData == that.mData;
    }
private:
    uint64_t mData;
};

/*
class InputRow
{
public:
    typedef std::unique_ptr <Cell> Elem;
    typedef std::vector <Elem> Cont;

    Cont mCells;
    inline Cont::const_iterator begin() const
    {
        return mCells.begin();
    }
    inline Cont::const_iterator end() const
    {
        return mCells.end();
    }
    inline Elem& operator[] (size_t idx)
    {
        return mCells[idx];
    }
    inline const Elem& operator[] (size_t idx) const
    {
        return mCells[idx];
    }
};
*/

typedef std::unique_ptr<Cell> RowCell;
typedef std::vector<RowCell> InputRow;
typedef std::vector <InputRow> InputRows;

template<typename Str>
Str &operator<< (Str &str, const Cell &cell) {
    str << cell.Repr ();
    return str;
}

template<typename Str>
Str &operator<< (Str &str, const InputRow &row) {
    std::stringstream ss;
    for (auto &cell: row) {
        ss << *cell << ",";
    }

    str << ss.str ();
    return str;
}
class Column
{
public:
    enum class Importance {
        Unimportant,
        Key
    };

    Column (const std::string &name, Importance importance = Importance::Unimportant)
    :
      mName (name),
      mImportance (importance)
    {
    }
    virtual std::unique_ptr <Cell> CreateCell(const std::string& data) const = 0;
    virtual ~Column() = 0;

    const std::string mName;
    const Importance mImportance;
};

inline Column::~Column()
{

}

class TextColumn
:
    public Column
{
public:
    using Column::Column;
    ~TextColumn()
    {
    }

    std::unique_ptr <Cell> CreateCell(const std::string& data) const
    {
        return pstd::make_unique <TextCell> (data);
    }
};

class UIntColumn
:
        public Column
{
public:
    using Column::Column;
    ~UIntColumn()
    {

    }

    std::unique_ptr <Cell> CreateCell(const std::string& data) const
    {
        return pstd::make_unique <UIntCell> (data);
    }
};

typedef std::vector <std::unique_ptr <Column>> Columns;

enum class TriState {
    False,
    True,
    Undefined
};

bool Equ (const InputRow &lhs, const InputRow &rhs, const Columns &cols) {
//    const auto &leftSeq = lhs[1]->Repr ();
//    const auto &rightSeq = rhs[1]->Repr ();

    TriState state = TriState::Undefined;

    for (size_t idx = 0; state != TriState::False && idx < cols.size (); ++idx) {
        const Column &col = *cols[idx];
        if (col.mImportance == Column::Importance::Key) {
            const Cell &lhc = *lhs[idx];
            const Cell &rhc = *rhs[idx];
            switch (lhc == rhc) {
                case true:
                    state = TriState::True;
                break;
                case false:
                    state = TriState::False;
                break;
            }
        }
    }
    return state != TriState::False;
}

Columns GetInputFileHeader()
{
    Columns cols;
    cols.push_back (pstd::make_unique <UIntColumn> ("RecvTime"));
    cols.push_back(pstd::make_unique <TextColumn> ("SeqNum"));
    cols.push_back(pstd::make_unique <TextColumn> ("Ticker"));
    cols.push_back(pstd::make_unique <TextColumn> ("BidSide"));
    cols.push_back (pstd::make_unique<TextColumn> ("BidPrice", Column::Importance::Key));
    cols.push_back(pstd::make_unique <TextColumn> ("BidSizeRaw"));
    cols.push_back (pstd::make_unique<TextColumn> ("BidSize", Column::Importance::Key));
    cols.push_back(pstd::make_unique <TextColumn> ("BidTime"));
    cols.push_back(pstd::make_unique <TextColumn> ("BidMarket"));
    cols.push_back(pstd::make_unique <TextColumn> ("AskSide"));
    cols.push_back (pstd::make_unique<TextColumn> ("AskPrice", Column::Importance::Key));
    cols.push_back(pstd::make_unique <TextColumn> ("AskSizeRaw"));
    cols.push_back (pstd::make_unique<TextColumn> ("AskSize", Column::Importance::Key));
    cols.push_back(pstd::make_unique <TextColumn> ("AskTime"));
    cols.push_back(pstd::make_unique <TextColumn> ("AskMarket"));

    return cols;

}

#endif //QC_HEADER_HPP

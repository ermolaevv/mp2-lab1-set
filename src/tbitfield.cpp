// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw "Len is negative";
    BitLen = len;
    MemLen = len / (sizeof(TELEM) * 8);
    if (len % (sizeof(TELEM) * 8) != 0)
        MemLen++;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    if (pMem != nullptr)
        delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen)
        throw "Wrong index";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen)
        throw "Wrong index";

    return 1 << (n % (sizeof(TELEM) * 8));
} 

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen)
        throw "Wrong index";

    const size_t index = GetMemIndex(n);
    pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen)
        throw "Wrong index";


    const size_t index = GetMemIndex(n);
    pMem[index] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen)
        throw "Wrong index";
    const size_t index = GetMemIndex(n);
    return pMem[index] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this == bf)
        return *this;
    
    if (pMem != nullptr && BitLen != bf.BitLen)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
            
        delete[] pMem;
        pMem = new TELEM[MemLen];
    }
    
    for (size_t i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (size_t i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField tmp(max(BitLen, bf.BitLen));

    for (size_t i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];
    for (size_t i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] |= bf.pMem[i];

    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField tmp(max(BitLen, bf.BitLen));

    for (size_t i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];
    for (size_t i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] &= bf.pMem[i];

    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);

    for (size_t i = 0; i < tmp.BitLen; i++)
    {
        if (tmp.GetBit(i))
        {
            tmp.ClrBit(i);
        }
        else
        {
            tmp.SetBit(i);
        }
    }

    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string tmp;
    istr >> tmp;

    if (tmp.size() != bf.GetLength())
        throw "Wrong size";

    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == '0')
            bf.ClrBit(i);
        else if (tmp[i] == '1')
            bf.SetBit(i);
        else
            throw "Incorrect string";

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i))
            ostr << 1;
        else
            ostr << 0;
    }
    return ostr;
}

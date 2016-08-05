#ifndef KLETH_ASSET_ASSET_H
#define KLETH_ASSET_ASSET_H

#include "prefix.h"

namespace kletch {

template <class T>
class Asset
{
public:
    explicit Asset(string const& name, T const* data);
    Asset(Asset<T> const& other) = delete;
    ~Asset();

    T const& operator * () const;
    bool has_data() const;
    void release();

private:
    string const m_name;
    T const* m_data;
};

template <class T> inline
Asset<T>::Asset(string const& name, T const* data) : m_name(name), m_data(data) { }

template <class T> inline
Asset<T>::~Asset() { release(); }

template <class T> inline
T const& Asset<T>::operator * () const
{
    if (!has_data())
        throw std::runtime_error("Asset invalid or released");
    return *m_data;
}

template <class T> inline
bool Asset<T>:: has_data() const { return m_data != nullptr; }

template <class T> inline
void Asset<T>::release()
{
    delete m_data;
    m_data = nullptr;
}

} // namespace kletch

#endif // KLETH_ASSET_ASSET_H

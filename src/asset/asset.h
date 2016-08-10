#ifndef KLETH_ASSET_ASSET_H
#define KLETH_ASSET_ASSET_H

#include "prefix.h"
#include "asset_header.h"

namespace kletch {

template <class T>
class Asset
{
public:
    explicit Asset(AssetHeader const& header, T const* data);
    Asset(Asset<T>&& other);
    Asset(Asset<T> const& other) = delete;
    ~Asset();

    string const& name() const { return m_name; }
    string const& source_path() const { return m_source_path; }

    T const& operator * () const;
    bool has_data() const;
    void release();

private:
    string const m_name;
    string const m_source_path;
    T const* m_data;
};

template <class T> inline
Asset<T>::Asset(AssetHeader const& header, T const* data)
    : m_name(header.name), m_source_path(header.source_path), m_data(data)
{ }

template <class T> inline
Asset<T>::Asset(Asset<T>&& other) :
    m_name(std::move(other.m_name)), m_source_path(std::move(other.m_source_path)),
    m_data(std::move(other.m_data))
{ }

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

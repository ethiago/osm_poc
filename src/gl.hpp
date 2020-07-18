#pragma once

#include <GL/osmesa.h>

#include <list>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace osmpoc
{
inline std::list<GLenum> getErrors()
{
    std::list<GLenum> errors;
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        errors.push_back(err);
    }
    return errors;
}

inline GLint getValue(GLint pname)
{
    GLint value;
    OSMesaGetIntegerv(pname, &value);
    return value;
}

using buffer_type = unsigned char;

struct size_type
{
    std::size_t width;
    std::size_t height;
};

enum class supported_formats
{
    RGBA
};

inline GLenum toOGL(supported_formats format)
{
    switch (format)
    {
    case supported_formats::RGBA:
        return OSMESA_RGBA;   
    }

    throw std::invalid_argument("");
}

inline unsigned char num_channels(supported_formats format)
{
    switch (format)
    {
    case supported_formats::RGBA:
        return 4;
    }

    throw std::invalid_argument("");
}

template<supported_formats _format>
struct Context
{
    Context()
        : _context(OSMesaCreateContextExt(toOGL(_format), 0, 0, 0, nullptr))
    {
        if(!_context)
            throw std::bad_alloc();
    }

    ~Context()
    {
        if(_context)
            OSMesaDestroyContext(_context);
    }

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    OSMesaContext get()
    { return _context; }

private:
    OSMesaContext _context{nullptr};
};

template<supported_formats _format>
struct AttachedBuffer
{ 
    AttachedBuffer() = default;

    AttachedBuffer(std::vector<buffer_type> buffer, size_type size)
     : _buffer(std::move(buffer))
     , _size(size)
    {}

    AttachedBuffer(const AttachedBuffer&) = delete;
    AttachedBuffer(AttachedBuffer&&) = default;

    AttachedBuffer& operator=(const AttachedBuffer&) = delete;
    AttachedBuffer& operator=(AttachedBuffer&&) = default;

    const buffer_type* data() const
    { return _buffer.data(); }

    const size_type& size() const
    { return _size; }

private:
    std::vector<buffer_type> _buffer;
    size_type _size;
};

template<supported_formats format>
inline AttachedBuffer<format> attach_buffer(Context<format>& context, std::size_t width, std::size_t height)
{ 
    auto maxWidth = getValue(OSMESA_MAX_WIDTH);
    auto maxHeight = getValue(OSMESA_MAX_HEIGHT);

    if(width >  std::size_t(maxWidth) || height > std::size_t(maxHeight))
        throw std::invalid_argument(
            "Buffer size has execeded limit. Max values are: " +
             std::to_string(maxWidth) + "x" + std::to_string(maxHeight)
        );

    auto channels = num_channels(format);
    std::vector<buffer_type> buffer(channels*width*height);

    if(!OSMesaMakeCurrent(context.get(), buffer.data(), GL_UNSIGNED_BYTE, width, height))
        throw std::runtime_error("Can't attach the buffer.");

    return { std::move(buffer), {width, height} };
}

template<supported_formats format>
struct pixel_view;

template<>
struct pixel_view<supported_formats::RGBA>
{   
    pixel_view(const buffer_type* bufferFirst)
        :  _bufferFirst(bufferFirst)
    {}

    const buffer_type& r() const
    { return _bufferFirst[0]; }

    const buffer_type& g() const
    { return _bufferFirst[1]; }

    const buffer_type& b() const
    { return _bufferFirst[2]; }

    const buffer_type& a() const
    { return _bufferFirst[3]; }

private:
    const buffer_type* _bufferFirst;
};

template<supported_formats format>
pixel_view<format> getPixel(const AttachedBuffer<format>& buffer, std::size_t row, std::size_t column);

template<>
pixel_view<supported_formats::RGBA> 
getPixel(
    const AttachedBuffer<supported_formats::RGBA>& buffer,
    std::size_t row,
    std::size_t column)
{
    return { buffer.data() +  (row*buffer.size().height + column)*num_channels(supported_formats::RGBA)  };
}

}

#pragma once

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/io/png/write.hpp>

#include "gl.hpp"

namespace osmpoc{

namespace detail
{
    template<supported_formats format>
    inline boost::gil::rgb8_pixel_t make_pixel(const pixel_view<format>& pixel)
    {
        return {
            pixel.r(),
            pixel.g(),
            pixel.b()
        };
    }
}

template<supported_formats format>
inline auto write(std::string filename, const AttachedBuffer<format>& buffer)
{
    using namespace boost::gil;

    rgb8_image_t img(buffer.size().width, buffer.size().height);
    auto view_v = view(img);

    for (int y=0; y<view_v.height(); ++y)
        for (int x=0; x<view_v.width(); ++x)
            view_v(x,y) =  detail::make_pixel(getPixel(buffer, y,x));

    return write_view(filename, view_v, png_tag());
}

}
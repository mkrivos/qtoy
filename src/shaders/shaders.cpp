#include "shaders.h"

#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include <gli/texture2d.hpp>
#include <gli/sampler2d.hpp>

#include <Poco/Timestamp.h>
#include <blend2d.h>


using namespace glm;

vec2 iResolution;

inline float sdf_circle(vec2 center, float radius, vec2 point)
{
    return length(point - center) - radius;
}

vec4 color_blend(vec4 dst, vec4 src)
{
    return dst * (1.0f - src.a) + src * src.a;
}

vec4 composite(vec4 back, vec4 front)
{
    return mix(back, front, front.a);
}

float sdf_rect(vec2 center, vec2 half_size, vec2 point)
{
    point = abs(point - center);

    float x_dist = point.x - half_size.x;
    float y_dist = point.y - half_size.y;
    float c_dist = length(point-half_size);

    return x_dist > 0 && y_dist > 0 ? c_dist : max(x_dist, y_dist);
}

float sdf_rect2(vec2 topleft, vec2 size, vec4 radii, vec2 pos)
{
    pos -= topleft; // normalize as if rect starts at 0,0
    pos -= size;    // not sure why but it's needed ..
    // from https://iquilezles.org/articles/distfunctions2d/
    radii.xy() = (pos.x > 0.0) ? radii.xy() : radii.zw();
    radii.x = (pos.y > 0.0) ? radii.x : radii.y;
    vec2 q = abs(pos) - size + radii.x;
    return min(max(q.x, q.y), 0.0f) + length(max(q, 0.0f)) - radii.x;
}

float sdf_border(float d, float thickness)
{
    return d <= 0.0 ? 1.0 - smoothstep(thickness - 0.4f, thickness + 0.4f, abs(d)) : 0.0;
}

float sdf_border(float center, float half_width, float dist)
{
    return abs(dist - center) - half_width;
}

float sdf_fill(float d, float softness)
{
    d = 1.0f - d;
    return smoothstep(1.0f - softness, 1.0f + softness, d);
}

float fill_factor(float dist, float softness)
{
    return smoothstep(-softness, softness, -dist);
}

vec4 apply_factor(vec4 color, float f)
{
    return vec4(color.rgb(), color.a * f);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

inline void mainImage(const ShaderParameters& params, vec4& color, vec2 coord, gli::sampler2d<float>& sampler)
{
    int function = params.function;

    if (function == 0)
    {
        vec4 back(1, 0, 0, 0);
        vec4 front(0, 1, 0, 0.5);
        color = mix(back, front, front.a);
    }
    else if (function == 1)
    {
        vec4 background(0.8, 0.9, 0.8, 0);

        float dist = sdf_circle(vec2(40, 40), 35.0, coord);
        float f = fill_factor(dist, 1.0f);
        vec4 fill(0.8f, 0.1f, 0.0f, 1.0f);
        color = composite(background, apply_factor(fill, f));
    }
    else if (function == 2)
    {
        vec4 background(0.8, 0.9, 0.8, 1);
        color = background;

        vec2 center = vec2(40, 40);
        float radius = 35;

        vec4 fg_color(1.0f, 0.0f, 0.0f, 1.0f);
        vec4 sh_color(0.0f, 0.0f, 0.0f, 0.5f);
        vec2 sh_offset = vec2(1, -2);
        float sh_softness = 1.0;

        {
            float dist = sdf_circle(center + sh_offset, radius, coord);
            float f = fill_factor(dist, sh_softness);
            color = composite(color, apply_factor(sh_color, f));
        }
        {
            float dist = sdf_circle(center, radius, coord);
            float f = fill_factor(dist, 1.2);
            color = composite(color, apply_factor(fg_color, f));
        }
    }
    else if (function == 3)
    {
        vec4 background(0.6, 0.66, 0.6, 1);
        color = background;

        vec2 center = vec2(40.5, 40.2);
        vec2 half_size(28, 14);
        vec4 fg_color(0.3f, 0.2f, 1.0f, 1.0f);

        {
            float dist = sdf_rect(center, half_size, coord);
            dist = sdf_border(0.5, 0.5, dist);
            float f = fill_factor(dist, 0.5);
            color = composite(color, apply_factor(fg_color, f));
        }
    }
    else if (function == 4)
    {
        // default fancy stuff
        vec2 uv = coord / iResolution.xy();
        color = vec4(0.5, 0.5, 0.5, 1);

        coord.y = iResolution.y - coord.y; // y starts at the top!
        coord /= 2.0;

        vec2 origin = vec2(-3.0);
        vec2 size = vec2(50.0, 50.0);
        vec4 radii = vec4(10.0);

        float dist = sdf_rect2(origin, size, radii, coord);
        float dist_shadow = sdf_rect2(origin, size, radii, coord - vec2(2.0));

        float f_border = sdf_border(dist, 4.0);
        float f_fill = sdf_fill(dist, 0.5);

        float f_shadow = sdf_fill(dist_shadow, 10.0);

        vec4 border = vec4(0.0, 1.0, 0.0, 1.0); // green
        vec4 fill = vec4(1.0, 0.0, 0.0, 1.0); // red

        vec4 shadow = vec4(0.0, 0.0, 0.0, 0.5); // dark

        color = color_blend(color, vec4(shadow * f_shadow));
        color = color_blend(color, vec4(fill * f_fill));
        color = color_blend(color, vec4(border * f_border));
    }
}

BLImage generate(int ch, int w, int h)
{
    return {};
}

void computeFrame(const ShaderParameters& params)
{
    int width = params.resolution.x;
    int height = params.resolution.y;
    int count = width * height;

    gli::texture2d texture{
        gli::FORMAT_R8_UNORM_PACK8,
        gli::texture2d::extent_type(32,32)
    };
    gli::sampler2d<float> sampler(texture, gli::wrap::WRAP_CLAMP_TO_EDGE, gli::filter::FILTER_LINEAR, gli::filter::FILTER_LINEAR);
/*
    BLImageData dataOut;
    bmp.getData(&dataOut);

    memcpy(
        texture.data(),
        dataOut.pixelData,
        texture.size()
    );

    gli::texture2d texture2{
        gli::FORMAT_R8_UNORM_PACK8,
        gli::texture2d::extent_type(params.serifSize.x, params.serifSize.y)
    };
    gli::sampler2d<float> sampler2(texture2, gli::wrap::WRAP_CLAMP_TO_EDGE, gli::filter::FILTER_LINEAR, gli::filter::FILTER_LINEAR);
    memcpy(texture2.data(), params.serif, params.serifSize.x*params.serifSize.y);
*/
    // Uniforms
    iResolution = params.resolution;

    Poco::Timestamp stamp;

    #pragma omp parallel for schedule(static) if (params.parallelRun)
    for (int i = 0; i < count; i++)
    {
        auto& image = params.image[i];
        mainImage(params, image, vec2(i % width, i / width), sampler);
        // Force 100% alpha
        image.a = 1;
    }

    std::cout <<  stamp.elapsed()/1000 << std::endl;
}

#version 330 core

in vec2 viewportUv;

out vec4 outColor;

uniform sampler2D backgroundTexture;
uniform sampler2D overlayTexture;

/*
    Coordinates use a top-left viewport origin:

    overlayTopLeft.x     = left
    overlayTopLeft.y     = top

    overlayBottomRight.x = right
    overlayBottomRight.y = bottom

    Every component ranges from 0.0 to 1.0.
*/
uniform vec2 overlayTopLeft;
uniform vec2 overlayBottomRight;

/*
    Additional overall opacity for the overlay:
    0.0 = invisible
    1.0 = use the texture's original alpha
*/
uniform float overlayOpacity;

void main()
{
    /*
        viewportUv uses the OpenGL convention:
        y=0 at the bottom and y=1 at the top.
    */
    vec4 backgroundColor =
        texture(backgroundTexture, viewportUv);

    /*
        Convert the viewport coordinate to a top-left-origin coordinate
        so it can be compared with overlayTopLeft/overlayBottomRight.
    */
    vec2 viewportTopLeftUv = vec2(
        viewportUv.x,
        1.0 - viewportUv.y);

    bool insideOverlay =
        viewportTopLeftUv.x >= overlayTopLeft.x &&
        viewportTopLeftUv.y >= overlayTopLeft.y &&
        viewportTopLeftUv.x <= overlayBottomRight.x &&
        viewportTopLeftUv.y <= overlayBottomRight.y;

    if (!insideOverlay)
    {
        outColor = backgroundColor;
        return;
    }

    vec2 rectangleSize =
        overlayBottomRight - overlayTopLeft;

    /*
        Protect against a zero-sized rectangle.
    */
    rectangleSize = max(
        rectangleSize,
        vec2(0.000001));

    /*
        Coordinates relative to the overlay rectangle, using a
        top-left origin.
    */
    vec2 overlayTopLeftUv =
        (viewportTopLeftUv - overlayTopLeft) /
        rectangleSize;

    /*
        Convert to the normal OpenGL texture-coordinate convention.
    */
    vec2 overlayUv = vec2(
        overlayTopLeftUv.x,
        1.0 - overlayTopLeftUv.y);

    vec4 overlayColor =
        texture(overlayTexture, overlayUv);

    float sourceAlpha = clamp(
        overlayColor.a * overlayOpacity,
        0.0,
        1.0);

    /*
        Straight-alpha source-over compositing.
    */
    float resultAlpha =
        sourceAlpha +
        backgroundColor.a * (1.0 - sourceAlpha);

    vec3 resultRgb = vec3(0.0);

    if (resultAlpha > 0.000001)
    {
        resultRgb =
            (
                overlayColor.rgb * sourceAlpha +
                backgroundColor.rgb *
                backgroundColor.a *
                (1.0 - sourceAlpha)
            ) / resultAlpha;
    }

    outColor = vec4(resultRgb, resultAlpha);
}
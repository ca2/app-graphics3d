#version 450

layout(location = 0) in vec2 fragmentLocalPosition;

layout(push_constant) uniform CirclePushConstants
{
   vec4 uniformFragmentColor;
   float radius;
   float thickness;
} circle;

layout(location = 0) out vec4 fragmentColor;

void main()
{
   float distanceFromCenter =
      length(fragmentLocalPosition);

   float signedDistance =
      abs(distanceFromCenter - circle.radius)
      - circle.thickness * 0.5;

   vec2 distanceGradient =
      vec2(
         dFdx(signedDistance),
         dFdy(signedDistance));

   float pixelWidth =
      max(length(distanceGradient), 0.0001);

   float coverage =
      clamp(
         0.5 - signedDistance / pixelWidth,
         0.0,
         1.0);

   fragmentColor =
      circle.uniformFragmentColor * coverage;
}
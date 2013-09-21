#version 150

uniform vec2 LensCenter;
uniform vec2 ScreenCenter;
uniform vec2 Scale;
uniform vec2 ScaleIn;
uniform vec4 HmdWarpParam;

uniform sampler2D tex;
in vec2 fraguv;
out vec4 finalColor;

vec2 HmdWarp(vec2 in01) {
   vec2  theta = (in01 - LensCenter) * ScaleIn; // Scales to [-1, 1]
   float rSq = theta.x * theta.x + theta.y * theta.y;
   vec2  rvector = theta * (HmdWarpParam.x +
							HmdWarpParam.y * rSq + 
							HmdWarpParam.z * rSq * rSq +
							HmdWarpParam.w * rSq * rSq * rSq);
   return LensCenter + Scale * rvector;
}

void main() {
   vec2 tc = HmdWarp(fraguv);
   if (!all(equal(clamp(tc, ScreenCenter-vec2(0.25,0.5),
							ScreenCenter+vec2(0.25,0.5)), tc)))
       finalColor = vec4(0);
   else
       finalColor = texture(tex, tc);
}
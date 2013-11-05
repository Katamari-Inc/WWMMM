uniform float t;
uniform sampler2D color;

attribute vec3 center;
attribute float speed;
attribute float offset;
attribute vec3 axis;

float cubicOut(float t) {
	return 1. - pow(1. - t, 3.);
}

vec3 rotate(vec3 v, float a) {
	float halfAngle = a * .5;
	float s = sin(halfAngle);
	vec4 q = normalize(vec4(axis.x * s, axis.y * s, axis.z * s, cos(halfAngle)));
	float ix =  q.w * v.x + q.y * v.z - q.z * v.y;
	float iy =  q.w * v.y + q.z * v.x - q.x * v.z;
	float iz =  q.w * v.z + q.x * v.y - q.y * v.x;
	float iw = -q.x * v.x - q.y * v.y - q.z * v.z;
	float x = ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y;
	float y = iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z;
	float z = iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x;
	return vec3(x, y, z);
}

void main() {
	float b = cubicOut(clamp(t / 3., 0., 1.));
	float c = smoothstep(0., 1., (7. - speed * 2. - t) / 3.);
	gl_Position = gl_ModelViewProjectionMatrix * vec4(rotate(gl_Vertex.xyz * b * c, (1. - c) * 3.) + center * b, 1.);
	// gl_Position = gl_ModelViewProjectionMatrix * vec4(rotate(gl_Vertex.xyz + center, (1. - c) * 3.), 1.0);
	// gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0);
	gl_FrontColor = texture2D(color, vec2(mod(t / 8. + offset, 1.), .5));
	gl_FrontColor.a = c;
}

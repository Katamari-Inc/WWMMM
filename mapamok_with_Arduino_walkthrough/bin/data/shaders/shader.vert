varying vec3 ec_pos;

void main() {

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    ec_pos = (gl_ModelViewMatrix * gl_Vertex).xyz;
}

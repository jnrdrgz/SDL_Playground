#shader vertex
#version 330 core
layout(location = 0) in vec2 posAttr;
void main(){
	gl_Position = vec4(posAttr, 0.0, 1.0); 
};

#shader fragment
#version 330 core
out vec4 col;
void main() {
	col = vec4(1.0, 0.0, 0.0, 1.0)
}
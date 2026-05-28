#version 330 core
            layout (location = 0) in vec4 vertex;
            out vec2 TexCoords;
            out vec4 Color;
            uniform mat4 projection;
            uniform vec3 textColor;
            void main() {
                gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0);
                TexCoords = vertex.zw;
                Color = vec4(textColor, 1.0);
            }
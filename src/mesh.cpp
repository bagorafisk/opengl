#include <mesh.h>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    VAO1.Bind();

    VBO VBO(vertices);

    EBO EBO(indices);

    VAO1.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    VAO1.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    VAO1.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
    VAO1.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

    VAO1.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
    shader.use();
    VAO1.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for(unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].type;
        if(type == "diffuse") {
            num = std::to_string(numDiffuse++);
        }
        else if(type == "specular") {
            num = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
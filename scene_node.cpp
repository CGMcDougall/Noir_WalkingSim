#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"

namespace game {

SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture){

    // Set name of scene node
    name_ = name;

    // Set geometry
    if (geometry->GetType() == PointSet){
        mode_ = GL_POINTS;
    } else if (geometry->GetType() == Mesh){
        mode_ = GL_TRIANGLES;
    } else {
        throw(std::invalid_argument(std::string("Invalid type of geometry")));
    }

    array_buffer_ = geometry->GetArrayBuffer();
    element_array_buffer_ = geometry->GetElementArrayBuffer();
    size_ = geometry->GetSize();

    // Set material (shader program)
    if (material->GetType() != Material){
        throw(std::invalid_argument(std::string("Invalid type of material")));
    }

    material_ = material->GetResource();

    // Set texture
    if (texture){
        texture_ = texture->GetResource();
    } else {
        texture_ = 0;
    }

    // Other attributes
    scale_ = glm::vec3(1.0, 1.0, 1.0);
}


SceneNode::~SceneNode(){
}


const std::string SceneNode::GetName(void) const {

    return name_;
}


glm::vec3 SceneNode::GetPosition(void) const {

    return position_;
}


glm::quat SceneNode::GetOrientation(void) const {

    return orientation_;
}


glm::vec3 SceneNode::GetScale(void) const {

    return scale_;
}


void SceneNode::SetPosition(glm::vec3 position){

    position_ = position;
}


void SceneNode::SetOrientation(glm::quat orientation){

    orientation_ = orientation;
}


void SceneNode::SetScale(glm::vec3 scale){

    scale_ = scale;
}


void SceneNode::Translate(glm::vec3 trans){

    position_ += trans;
}


void SceneNode::Rotate(glm::quat rot){

    orientation_ *= rot;
    orientation_ = glm::normalize(orientation_);
}


void SceneNode::Scale(glm::vec3 scale){

    scale_ *= scale;
}


GLenum SceneNode::GetMode(void) const {

    return mode_;
}


GLuint SceneNode::GetArrayBuffer(void) const {

    return array_buffer_;
}


GLuint SceneNode::GetElementArrayBuffer(void) const {

    return element_array_buffer_;
}


GLsizei SceneNode::GetSize(void) const {

    return size_;
}


GLuint SceneNode::GetMaterial(void) const {

    return material_;
}


void SceneNode::Draw(Camera *camera){

    // Select proper material (shader program)
    glUseProgram(material_);

    // Set geometry to draw
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

    // Set globals for camera
    camera->SetupShader(material_);

    // Set world matrix and other shader input variables
    SetupShader(material_);

    // Draw geometry
    if (mode_ == GL_POINTS){
        glDrawArrays(mode_, 0, size_);
    } else {
        glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
    }
}


void SceneNode::Update(void){

    // Do nothing for this generic type of scene node
}


void SceneNode::SetupShader(GLuint program){

    // Set attributes for shaders
    GLint vertex_att = glGetAttribLocation(program, "vertex");
    glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint normal_att = glGetAttribLocation(program, "normal");
    glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(normal_att);

    GLint color_att = glGetAttribLocation(program, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(program, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (9*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);


    /*GLint lightSources = glGetAttribLocation(program, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray();*/

    //glm::vec3 lightSources[5] = { glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,0) ,glm::vec3(0,0,0) ,glm::vec3(0,0,0) };


    //glUniform3fv(originsLoc, origins.size(), reinterpret_cast<GLfloat*>(trackObstacle->obstaclesPopupNormals.data()));

    for (int i = 0; i < 5; i++) {

        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "lightSources[" + index + "]";
        //std::cout << name << std::endl;

        const char* c_str = name.c_str();

        GLint sources = glGetUniformLocation(program, c_str);
        //std::cout << "Got to Here? " << i << std::endl;
        if (i >= lightSource.size())break;
        //std::cout << "still Here? " << i << std::endl;
        glm::vec3 li = lightSource.at(i);
        glUniform3f(sources, li.x, li.y, li.z);
    }
    
    float lampP[3] = {0,0,0};
    for (int i = 0; i < lampLightPos.size(); i++) {
        lampP[i*3] = lampLightPos.at(i).x;
        lampP[i*3+1] = lampLightPos.at(i).y;
        lampP[i*3+2] = lampLightPos.at(i).z;
    }
    //std::cout << lampLightPos.at(0).x << std::endl;
    GLint lampLightPositions = glGetUniformLocation(program, "lampLightPos");
    glUniform3fv(lampLightPositions, lampLightPos.size(), lampP);


    // World transformation
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
    glm::mat4 rotation = glm::mat4_cast(orientation_);
    glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);

    glm::vec3 toJoint = joint_ - position_;
    glm::mat4 joint = glm::translate(glm::mat4(1.0),toJoint) * rotation * scaling;

    glm::mat4 orbit = glm::translate(glm::mat4(1.0), -toJoint) * glm::mat4_cast(orbit_) * glm::translate(glm::mat4(1.0), toJoint);
    //orbit = -joint * glm::mat4_cast(orbit_) * joint;

    glm::mat4 transf = translation *orbit* rotation * scaling;

    GLint world_mat = glGetUniformLocation(program, "world_mat");
    glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

    // Normal matrix
    glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
    GLint normal_mat = glGetUniformLocation(program, "normal_mat");
    glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    // Texture
    if (texture_){
        GLint tex = glGetUniformLocation(program, "texture_map");
        glUniform1i(tex, 0); // Assign the first texture to the map
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
        // Define texture interpolation
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    double current_time = glfwGetTime();
    glUniform1f(timer_var, (float) current_time);
}



void SceneNode::setLightSources(std::vector<glm::vec3> li) {
    lightSource.clear();
    for(int i = 0; i < li.size(); i++) {
        lightSource.push_back(li.at(i));
    }
    //std::cout << lightSource.size() << " this is what size should be" << std::endl;
}


void SceneNode::setDirectionalLightSources(std::vector<glm::vec3> li) {
    lampLightPos.clear();
    for (int i = 0; i < li.size(); i++) {
        lampLightPos.push_back(li.at(i));
    }
}

} // namespace game;

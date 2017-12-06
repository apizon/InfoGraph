#include "./../../include/lighting/DirectionalLightRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

DirectionalLightRenderable::DirectionalLightRenderable(ShaderProgramPtr shaderProgram, DirectionalLightPtr light, const glm::vec3 &position) :
    HierarchicalRenderable(shaderProgram), m_light(light), m_position(position),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    std::vector<glm::vec3> tmp_x, tmp_n;
    unsigned int strips=20, slices=20;
    glm::mat4 transformation(1.0);

    transformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,1.0));
    getUnitCone(tmp_x, tmp_n, strips, slices);
    for(size_t i=0; i<tmp_x.size(); ++i) m_positions.push_back(glm::vec3(transformation*glm::vec4(tmp_x[i],1.0)));
    m_normals.insert(m_normals.end(), tmp_n.begin(), tmp_n.end());

    transformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-1.0))*glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
    getUnitCylinder(tmp_x, tmp_n, strips);
    for(size_t i=0; i<tmp_x.size(); ++i) m_positions.push_back(glm::vec3(transformation*glm::vec4(tmp_x[i],1.0)));
    m_normals.insert(m_normals.end(), tmp_n.begin(), tmp_n.end());

    m_colors.resize(m_positions.size(), glm::vec4(light->diffuse(),1.0));

    glm::vec3 init_direction(0.0,0.0,1.0);
    glm::quat quat = glm::rotation(init_direction, m_light->direction());
    transformation = glm::translate(glm::mat4(1.0), m_position)*glm::toMat4(quat);
    setParentTransform(transformation);

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void DirectionalLightRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void DirectionalLightRenderable::do_animate(float /*time*/) {

}

glm::vec3 DirectionalLightRenderable::position() const
{
    return m_position;
}

void DirectionalLightRenderable::setPosition(const glm::vec3 &position)
{
    m_position = position;
}


DirectionalLightRenderable::~DirectionalLightRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

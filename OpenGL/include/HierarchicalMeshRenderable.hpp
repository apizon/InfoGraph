#ifndef HIERARCHICAL_MESH_RENDERABLE_HPP
#define HIERARCHICAL_MESH_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

class HierarchicalMeshRenderable : public HierarchicalRenderable
{
public:
	~HierarchicalMeshRenderable();
	HierarchicalMeshRenderable(ShaderProgramPtr program, const std::string& filename);
	HierarchicalMeshRenderable(ShaderProgramPtr shaderProgram, const std::vector< glm::vec3 >& positions, const std::vector<unsigned int>& indices,
		const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);

private:
	void do_draw();
	void do_animate(float time);

protected:
	std::vector< glm::vec3 > m_positions;
	std::vector< glm::vec3 > m_normals;
	std::vector< glm::vec4 > m_colors;
	std::vector< unsigned int > m_indices;

	unsigned int m_pBuffer;
	unsigned int m_cBuffer;
	unsigned int m_nBuffer;
	unsigned int m_iBuffer;
};

typedef std::shared_ptr<HierarchicalMeshRenderable> HierarchicalMeshRenderablePtr;

#endif
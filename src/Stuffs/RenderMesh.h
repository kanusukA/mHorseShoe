#pragma once

//#include <Stuffs/ColliderMesh.h>

#include <Stuffs/ShaderObject.h>


class RenderMesh : public RenderMeshResource {

private:
	GDBuilderContext* builderCxt;

public:

	// Buffers
	std::vector<MonsterBuffer> transformBuffers{};
	std::vector<MonsterBuffer> fragBuffers{};

	std::shared_ptr<Shader> shader;


	RenderMesh(GDBuilderContext* builderCxt_p, std::string name_p, std::filesystem::path meshPath, std::shared_ptr<Shader> shader_p ) : 
		RenderMeshResource(ResourceHandler::GetInstance(), name_p , meshPath) {
		builderCxt = builderCxt_p;

		shader = shader_p;

	}

	// Expose functions to the underlying variables
	void loadRenderMesh() { 
		builderCxt->loadRenderMesh(this); 
	};

	void allocateBufferInfo(std::vector<std::vector<MonsterBuffer>>& buffers) override {
		if (buffers.size() != this->allocatingBufferSizes.size())
		{
			throw std::runtime_error("IMPROPER BUFFER ALLOCATION");
		}
		transformBuffers = std::move(buffers.at(0));
		fragBuffers = std::move(buffers.at(1));
	}

	void updateDescriptorWrite(vk::raii::Device* device) override {
		shader->_updateDescriptorWrites(device, transformBuffers, fragBuffers);
	}

	ShaderResource* getShader() override { return shader.get(); }

	

	~RenderMesh() {
		// MESHES ARE DELETED BY THE RESOURCE AS UNLOADING AND RELOADING IS WASTEFUL. IF A MESH IS NO LONGER NEEDED, IT SHOULD BE DELETED MANUALLY.
	}

};
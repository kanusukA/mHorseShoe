#pragma once

#include <Stuffs/ShaderObject.h>


class RenderMesh : public RenderMeshResource {

private:
	

public:

	GDBuilderContext* builderCxt;

	bool isMeshLoaded = false;

	MeshData renderMeshData;

	std::shared_ptr<Shader> shader;

	RenderMesh(GDBuilderContext* builderCxt_p, std::string name_p, std::filesystem::path meshPath, std::shared_ptr<Shader> shader_p ) : 
		RenderMeshResource(ResourceHandler::GetInstance(), name_p , meshPath, shader_p.get()) {
		builderCxt = builderCxt_p;

		shader = shader_p;

		fragParameters = shader->getFragParameters();

	}

	// Expose functions to the underlying variables
	virtual void loadRenderMesh() {

		builderCxt->loadRenderMesh(this);
		checkPushConsts();
		isMeshLoaded = true;


	};
	// TODO MAKE IT GENERALIZED
	void checkPushConsts() {
		if (!shader->fragPushConsts.empty())
		{
			pushConstSize = shader->pushConstSize;
			containsPushConstants = true;
		}
	}

	const void* getPushConst() override {
		float time = Glock::GetInstance()->getShaderTime();
		return &time;
		
	}

	void _loadVertexIndexBuffers() override {
		vertexBuffer = builderCxt->monster->createGetVertexBuffer(mesh.front().vertices).first;
		indexBuffer = builderCxt->monster->createGetIndexBuffer(mesh.front().indices).first;
	}

	void updateDescriptorWrite(vk::raii::Device* device) override {
		shader->_updateDescriptorWrites(device, &descritorSets, transformBuffers, {}, fragBuffers); // vertex UBO is empty by default!
	}

	ShaderResource* getShader() override { return shader.get(); }

	

	~RenderMesh() {
		// MESHES ARE DELETED BY THE RESOURCE AS UNLOADING AND RELOADING IS WASTEFUL. IF A MESH IS NO LONGER NEEDED, IT SHOULD BE DELETED MANUALLY.
	}

};